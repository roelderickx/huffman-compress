#!/bin/bash

function run_test() {
    rm -f $1.compressed $1.decompressed

    ../compress $1
    mv $1 $1.orig
    ../uncompress $1.compressed
    mv $1 $1.decompressed
    mv $1.orig $1
    
    diff $1 $1.decompressed
}

../compress nonexistant.txt
../uncompress nonexistant.txt.compressed

run_test test1.txt
run_test test2.txt

