#ifndef __NODE_H__
#define __NODE_H__

#include <iostream>
#include <map>
    using namespace std;
#include "BitStream.h"

class Node
{
private:
    long myFrequency;
    unsigned char myChar;
    Node * myLeftNode;
    Node * myRightNode;
    
    void Print(const int level, const BitStream & bitStream) const;
    void FillTranslationTable(map<unsigned char, BitStream> & table,
                              const BitStream & bitStream) const;
    
public:
    // constructor as Node
    Node(Node * leftNode, Node * rightNode);
    // constructor as Leaf
    Node(const unsigned char ch, const long frequency);
    // constructor from BitStream object
    Node(BitStream & bitStream);
    Node(const Node & node);
    ~Node();
    
    long GetFrequency() const;
    unsigned char GetChar() const;
    bool IsLeaf() const;
    
    unsigned char Load(BitStream & bitStream) const;
    void Save(BitStream & bitStream) const;
    
    Node & operator=(const Node & node);
    int operator==(const Node & node) const;
    int operator<(const Node & node) const;
    int operator>(const Node & node) const;
    
    void Print() const;
    void FillTranslationTable(map<unsigned char, BitStream> & table) const;
    
    unsigned long GetCompressedSize(const long depth) const
    {
        if (IsLeaf())
        {
            return myFrequency * depth;
        }
        else
        {
            return myLeftNode->GetCompressedSize(depth + 1) +
                   myRightNode->GetCompressedSize(depth + 1);
        }
    }
};

#endif

