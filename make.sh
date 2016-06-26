#!/bin/bash

if [ $# -eq 0 ]; then
    make tags && make
    exit
fi

if [ "$1" == "clean" ]; then
    make clean
fi

if [ "$1" == "rebuild" ]; then
    make clean && make tags && make
fi

