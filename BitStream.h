#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <string>
    using namespace std;

class BitStream
{
private:
    long myCapacity;
    long mySize;
    bool * myCode;
    long myFlushAfterKb;
    string myFileName;
    bool myFileReadMode;
    long myFilePosition;
    long myCurrentReadPosition;
    bool myEOF;
    
    bool AssureAvailableBits(const long minNeeded);
    void ResizeCapacity(const long minNeeded);
    void Flush(bool force);

public:
    BitStream();
    BitStream(const long capacityKb, const char * fileName = "", bool isReadMode = true);
    BitStream(const BitStream & bitStream);
    ~BitStream();
    
    void Add(const bool value);
    void Add(const unsigned char value);
    void Add(const unsigned long value);
    void Add(const BitStream & bitStream);
    
    bool IsEOF() const { return myEOF; }
    bool GetBool();
    unsigned char GetChar();
    unsigned long GetLong();
    
    long GetSize() const;
    bool * GetCode() const;
    
    BitStream & operator=(const BitStream & bitStream);
};

ostream & operator<<(ostream & os, const BitStream & bitStream);

#endif

