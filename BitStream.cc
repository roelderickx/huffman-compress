#include <iostream>
#include <fstream>
    using namespace std;
#include "BitStream.h"

BitStream::BitStream() :
    myCapacity(0),
    mySize(0),
    myCode(NULL),
    myFlushAfterKb(0),
    myFileName(""),
    myFileReadMode(true),
    myFilePosition(0),
    myCurrentReadPosition(0),
    myEOF(false)
{
}

BitStream::BitStream(const long capacityKb, const char * fileName, bool isReadMode) :
    myCapacity(capacityKb * 8192 + 100),
    mySize(0),
    myFlushAfterKb(capacityKb),
    myFileName(fileName),
    myFileReadMode(isReadMode),
    myFilePosition(0),
    myCurrentReadPosition(0),
    myEOF(false)
{
    myCode = new bool[myCapacity];
}

BitStream::BitStream(const BitStream & bitStream) :
    myCapacity(bitStream.myCapacity),
    mySize(bitStream.mySize),
    myFlushAfterKb(bitStream.myFlushAfterKb),
    myFileName(bitStream.myFileName),
    myFileReadMode(bitStream.myFileReadMode),
    myFilePosition(bitStream.myFilePosition),
    myCurrentReadPosition(bitStream.myCurrentReadPosition),
    myEOF(bitStream.myEOF)
{
    myCode = new bool[bitStream.myCapacity];
    for (long i = 0; i < bitStream.mySize; i++)
    {
        myCode[i] = bitStream.myCode[i];
    }
}

BitStream::~BitStream()
{
    Flush(true);
    delete [] myCode;
}

void BitStream::ResizeCapacity(const long minNeeded)
{
    if (mySize > myCapacity - minNeeded)
    {
        long neededCapacity = myCapacity + minNeeded;
        if (0 == myCapacity)
        {
            myCapacity = 1;
        }
        else
        {
            myCapacity *= 2;
        }
        if (myCapacity < neededCapacity)
        {
            myCapacity = neededCapacity;
        }
        bool * tmp = new bool[myCapacity];
        for (long i = 0; i < mySize; i++)
        {
            tmp[i] = myCode[i];
        }
        delete [] myCode;
        myCode = tmp;
    }
}

bool BitStream::AssureAvailableBits(const long minNeeded)
{
    if (myCurrentReadPosition + minNeeded > mySize)
    {
        // copy last unread bits to the beginning
        for (int i = myCurrentReadPosition; i < mySize; i++)
        {
            myCode[myCurrentReadPosition - i] = myCode[i];
        }
        mySize -= myCurrentReadPosition;
        myCurrentReadPosition = 0;
                
        // read next chunk from file
        long bytesRead = 0;
        ifstream fin(myFileName.c_str());
        if (fin)
        {
            char ch;
            fin.seekg(myFilePosition);
            while (bytesRead < myFlushAfterKb * 1024 && fin.get(ch))
            {
                Add((unsigned char)ch);
                bytesRead++;
            }
            fin.close();
            
            myFilePosition += bytesRead;
        }
        
        myEOF = (bytesRead == 0);
        return !myEOF;
    }
    else
    {
        return true;
    }
}

void BitStream::Flush(bool force)
{
    if (myFileReadMode)
    {
        return;
    }
    if ((!force && myFlushAfterKb > 0 && mySize >= myFlushAfterKb * 8192)
        ||
        force && myFlushAfterKb > 0)
    {
        long bitsToFlush = myFlushAfterKb * 8192;
        if (force)
        {
            if (mySize % 8 != 0)
            {
                long oldSize = mySize;
                mySize += 8 - (mySize % 8);
                for (int i = oldSize; i < mySize; i++)
                {
                    myCode[i] = 0;
                }
            }
            bitsToFlush = mySize;
        }
        
        char ch;
        ofstream fout(myFileName.c_str(), myFilePosition == 0 ? ios::trunc : ios::app);
        if (fout)
        {
            for (int i = 0; i < bitsToFlush; i++)
            {
                ch <<= 1;
                ch |= (myCode[i] ? 1 : 0);
                if (i % 8 == 7)
                {
                    fout << ch;
                }
            }
            fout.close();
            
            myFilePosition += bitsToFlush / 8;
            
            if (!force)
            {
                for (int i = myFlushAfterKb * 8192; i < mySize; i++)
                {
                    myCode[i - myFlushAfterKb * 8192] = myCode;
                }
                mySize -= myFlushAfterKb * 8192;
            }
            else
            {
                mySize = 0;
            }
        }
    }
}

void BitStream::Add(const bool value)
{
    ResizeCapacity(1);
    myCode[mySize] = value;
    mySize++;
    Flush(false);
}

void BitStream::Add(const unsigned char value)
{
    unsigned char tmp = value;
    ResizeCapacity(8);
    for (int i = 7; i >= 0; --i)
    {
        myCode[mySize + i] = tmp & 1;
        tmp >>= 1;
    }
    mySize += 8;
    Flush(false);
}

// NOTE: value cannot be greater than 2^32
void BitStream::Add(const unsigned long value)
{
    unsigned long tmp = value;
    ResizeCapacity(32);
    for (int i = 31; i >= 0; --i)
    {
        myCode[mySize + i] = tmp & 1;
        tmp >>= 1;
    }
    mySize += 32;
    Flush(false);
}

void BitStream::Add(const BitStream & bitStream)
{
    ResizeCapacity(bitStream.GetSize());
    for (long i = 0; i < bitStream.GetSize(); i++)
    {
        myCode[mySize + i] = bitStream.myCode[i];
    }
    mySize += bitStream.GetSize();
    Flush(false);
}

bool BitStream::GetBool()
{
    if (AssureAvailableBits(1))
    {
        bool retval = myCode[myCurrentReadPosition];
        myCurrentReadPosition++;
        return retval;
    }
    else
    {
        return false;
    }
}

unsigned char BitStream::GetChar()
{
    if (AssureAvailableBits(8))
    {
        unsigned char retval = 0;
        for (int i = 0; i < 8; i++)
        {
            retval <<= 1;
            retval |= myCode[myCurrentReadPosition + i] ? 1 : 0;
        }
        myCurrentReadPosition += 8;
        return retval;
    }
    else
    {
        return 0;
    }
}

unsigned long BitStream::GetLong()
{
    if (AssureAvailableBits(32))
    {
        unsigned long retval = 0;
        for (int i = 0; i < 32; i++)
        {
            retval <<= 1;
            retval |= myCode[myCurrentReadPosition + i] ? 1 : 0;
        }
        myCurrentReadPosition += 32;
        return retval;
    }
    else
    {
        return 0;
    }
}

long BitStream::GetSize() const
{
    return mySize;
}

bool * BitStream::GetCode() const
{
    return myCode;
}

BitStream & BitStream::operator=(const BitStream & bitStream)
{
    myCapacity = bitStream.myCapacity;
    mySize = bitStream.mySize;
    myCode = new bool[bitStream.myCapacity];
    myFlushAfterKb = bitStream.myFlushAfterKb;
    myFileName = bitStream.myFileName;
    myFileReadMode = bitStream.myFileReadMode;
    myFilePosition = bitStream.myFilePosition;
    myCurrentReadPosition = bitStream.myCurrentReadPosition;
    myEOF = bitStream.myEOF;
    for (long i = 0; i < bitStream.mySize; i++)
    {
        myCode[i] = bitStream.myCode[i];
    }
    return *this;
}

ostream & operator<<(ostream & os, const BitStream & bitStream)
{
    for (long i = 0; i < bitStream.GetSize(); i++)
    {
        os << (bitStream.GetCode()[i] ? "1" : "0");
    }
    
    return os;
}

