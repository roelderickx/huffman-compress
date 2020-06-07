#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

#include <map>
    using namespace std;
#include "Node.h"

class HuffmanTree
{
private:
    string myFileName;
    Node * myTopNode;
    map<unsigned char, BitStream> myTranslationTable;

public:
    HuffmanTree(const char * fileName);
    ~HuffmanTree();

    bool BuildTree();
    
    void Compress();
    bool Decompress();

    void PrintTree() const;
    
    long GetCompressedSize() const
    {
        if (myTopNode)
        {
            return myTopNode->GetCompressedSize(0);
        }
        else
        {
            cout << "HuffmanTree is empty" << endl;
        }
    }
};

#endif

