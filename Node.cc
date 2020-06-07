#include <iostream>
    using namespace std;
#include "Node.h"

Node::Node(Node * leftNode, Node * rightNode) :
    myChar('\0'),
    myLeftNode(leftNode),
    myRightNode(rightNode)
{
    myFrequency = leftNode->GetFrequency() + rightNode->GetFrequency();
}

Node::Node(const unsigned char ch, const long frequency) :
    myFrequency(frequency),
    myChar(ch),
    myLeftNode(NULL),
    myRightNode(NULL)
{
}

Node::Node(BitStream & bitStream) :
    myChar('\0'),
    myFrequency(0),
    myLeftNode(NULL),
    myRightNode(NULL)
{
    if (!bitStream.IsEOF())
    {
        bool isLeaf = bitStream.GetBool();
        if (isLeaf)
        {
            myChar = bitStream.GetChar();
        }
        else
        {
            myLeftNode = new Node(bitStream);
            myRightNode = new Node(bitStream);
        }
    }
}

Node::Node(const Node & node)
{
    myFrequency = node.myFrequency;
    myChar = node.myChar;
    if (node.myLeftNode)
        myLeftNode = new Node(*(node.myLeftNode));
    else
        myLeftNode = NULL;
    if (node.myRightNode)
        myRightNode = new Node(*(node.myRightNode));
    else
        myRightNode = NULL;
}

Node::~Node()
{
    delete myLeftNode;
    delete myRightNode;
}

long Node::GetFrequency() const
{
    return myFrequency;
}

unsigned char Node::GetChar() const
{
    return myChar;
}

bool Node::IsLeaf() const
{
    return (!myLeftNode && !myRightNode);
}

Node & Node::operator=(const Node & node)
{
    myFrequency = node.myFrequency;
    myChar = node.myChar;
    if (node.myLeftNode)
        myLeftNode = new Node(*(node.myLeftNode));
    else
        myLeftNode = NULL;
    if (node.myRightNode)
        myRightNode = new Node(*(node.myRightNode));
    else
        myRightNode = NULL;
    return *this;
}

int Node::operator==(const Node & node) const
{
    int retval = 0;
    if (myFrequency == node.myFrequency && myChar == node.myChar)
    {
        retval = 1;
    }
    
    return retval;
}

int Node::operator<(const Node & node) const
{
    int retval = 0;
    if (myFrequency < node.myFrequency ||
        (myFrequency == node.myFrequency && myChar < node.myChar))
    {
        retval = 1;
    }
    
    return retval;
}

int Node::operator>(const Node & node) const
{
    int retval = 0;
    if (myFrequency > node.myFrequency ||
        (myFrequency == node.myFrequency && myChar > node.myChar))
    {
        retval = 1;
    }
    
    return retval;
}

void Node::Print(const int level, const BitStream & bitStream) const
{
    for (int i = 0; i < level * 2; i++)
        cout << " ";
    
    if (IsLeaf())
    {
        if (myChar < 32 || myChar > 127)
        {
            cout << "leaf: char=(" << (int)myChar
                 << "), frequency=" << myFrequency
                 << ", code=" << bitStream << endl;
        }
        else
        {
            cout << "leaf: char='" << myChar
                 << "', frequency=" << myFrequency
                 << ", code=" << bitStream << endl;
        }
    }
    else
    {
        cout << "node: frequency=" << myFrequency << endl;
        BitStream left = bitStream;
        left.Add(true);
        myLeftNode->Print(level + 1, left);
        BitStream right = bitStream;
        right.Add(false);
        myRightNode->Print(level + 1, right);
    }
}

void Node::Print() const
{
    Print(0, BitStream());
}

void Node::FillTranslationTable(map<unsigned char, BitStream> & table,
                                const BitStream & bitStream) const
{
    if (IsLeaf())
    {
        table[myChar] = bitStream;
    }
    else
    {
        BitStream left = bitStream;
        left.Add(true);
        myLeftNode->FillTranslationTable(table, left);
        BitStream right = bitStream;
        right.Add(false);
        myRightNode->FillTranslationTable(table, right);
    }
}

void Node::FillTranslationTable(map<unsigned char, BitStream> & table) const
{
    FillTranslationTable(table, BitStream(32));
}

unsigned char Node::Load(BitStream & bitStream) const
{
    if (IsLeaf())
    {
        return myChar;
    }
    else
    {
        bool isLeftNode = bitStream.GetBool();
        if (isLeftNode)
        {
            return myLeftNode->Load(bitStream);
        }
        else
        {
            return myRightNode->Load(bitStream);
        }
    }
}

void Node::Save(BitStream & bitStream) const
{
    if (IsLeaf())
    {
        bitStream.Add(true);
        bitStream.Add(myChar);
    }
    else
    {
        bitStream.Add(false);
        myLeftNode->Save(bitStream);
        myRightNode->Save(bitStream);
    }
}

