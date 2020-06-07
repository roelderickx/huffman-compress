#include <iostream>
#include <fstream>
#include <list>
    using namespace std;
#include "IsLessPointerPredicate.h"
#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(const char * fileName) :
    myFileName(fileName),
    myTopNode(NULL)
{
}

HuffmanTree::~HuffmanTree()
{
    delete myTopNode;
    myTranslationTable.clear();
}

bool HuffmanTree::BuildTree()
{
    bool retval = true;
    
    long frequency[256];
    for (int i = 0; i < 256; i++)
    {
        frequency[i] = 0;
    }
    
    char ch;
    ifstream fin(myFileName.c_str());
    if (fin)
    {
        while (fin.get(ch))
        {
            frequency[(unsigned char)ch]++;
        }
        fin.close();
    }
    
    list<Node*> leaves;
    int numElements = 0;
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            Node * leaf = new Node(i, frequency[i]);
            leaves.push_back(leaf);
            numElements++;
        }
    }
    
    if (!leaves.empty())
    {
        leaves.sort(IsLessPointerPredicate<Node>());
        
        while (numElements > 1)
        {
            Node * first = leaves.front();
            leaves.pop_front();
            Node * second = leaves.front();
            leaves.pop_front();
            Node * node = new Node(first, second);
            leaves.push_back(node);
            leaves.sort(IsLessPointerPredicate<Node>());
            
            numElements--;
        }
        
        delete myTopNode;
        myTopNode = *(leaves.begin());
    
        leaves.clear();
        myTranslationTable.clear();
        
        if (myTopNode)
        {
            myTopNode->FillTranslationTable(myTranslationTable);
        }
        else
        {
            retval = false;
        }
    }
    else
    {
        delete myTopNode;
        myTopNode = NULL;
        myTranslationTable.clear();
        
        retval = false;
    }
    
    return retval;
}

void HuffmanTree::Compress()
{
    if (myTopNode)
    {
        string outFileName = myFileName + ".compressed";
        BitStream bs(10, outFileName.c_str(), false);
        
        char ch;
        ifstream fin(myFileName.c_str(), std::ifstream::ate | std::ifstream::binary);
        if (fin)
        {
            unsigned long filesize = fin.tellg();
            bs.Add(filesize);
            fin.seekg(0, ios_base::beg);

            myTopNode->Save(bs);
            
            while (fin.get(ch))
            {
                bs.Add(myTranslationTable[(unsigned char)ch]);
            }
            fin.close();
        }
    }
    else
    {
        cout << "HuffmanTree is empty" << endl;
    }
}

bool HuffmanTree::Decompress()
{
    BitStream bs(10, myFileName.c_str(), true);
    unsigned long filesize = bs.GetLong();
    if (bs.IsEOF() || filesize == 0)
    {
        return false;
    }
    myTopNode = new Node(bs);
    
    size_t compressed_pos = myFileName.find(".compressed");
    if (compressed_pos == std::string::npos)
        return false;
    string outFileName = myFileName.substr(0, compressed_pos);
    
    ofstream fout(outFileName.c_str(), ios::trunc);
    if (fout)
    {
        for (int i = 0; i < filesize; i++)
        {
            unsigned char ch = myTopNode->Load(bs);
            fout << ch;
        }
        fout.close();

        return true;
    }
    else
    {
        return false;
    }
}

void HuffmanTree::PrintTree() const
{
    if (myTopNode)
    {
        myTopNode->Print();
    }
    else
    {
        cout << "HuffmanTree is empty" << endl;
    }
}

