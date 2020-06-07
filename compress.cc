#include <iostream>
	using namespace std;
#include "HuffmanTree.h"

int main(int argc, char ** argv)
{
	if (2 == argc)
	{
		HuffmanTree ht(*(argv+1));
		
		if (ht.BuildTree())
		{
			// ht.PrintTree();
			// cout << "compressed data is " << ht.GetCompressedSize()
			// 	 << " bits long." << endl;
			
			ht.Compress();
		}
		else
		{
			cout << *(argv+1) << " could not be opened for reading." << endl;
		}
	}
	else
	{
		cout << "Usage: " << *argv << " [filename]" << endl;
	}
	
	return 0;
}
