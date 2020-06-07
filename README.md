# huffman-compress
A compression and decompression program

## Description
This application was written long ago to study file compression. It is not optimized to obtain a good compression ratio, nor for performance. It is uploaded to github for historical reference/interest purposes and will not be maintained.

## Installation
There are no dependencies besides a working copy of the GNU C++ compiler. Just type `make` in the main directory of this repository and the applications `compress` and `uncompress` will be compiled.
There is no installation script provided, you have to copy the resulting executables manually to a folder in your path.

## Usage
Usage is straightforward, `compress` takes the name of a file to compress as parameter, the resulting file will have the same name with the extension .compressed attached. The original file will be kept. `uncompress` takes a compressed file as parameter and reconstructs the original uncompressed version.

**NOTE:** There are no checks implemented, you can throw any file with a .compressed extension at `uncompress` but if the file was not created by `compress` the result is unpredictable. Output files will be overwritten without warning and there is no warranty whatsoever that `uncompress` restores the original file.

