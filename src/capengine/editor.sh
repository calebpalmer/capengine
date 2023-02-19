#!/bin/bash

curdir=$(pwd)
#export PATH=$curdir/capengine:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$curdir

#gdb -batch -ex "run" -ex "bt" -e "turtlegame" 2>&1 | grep -v ^"No stack."$
gdb -batch -ex "run $@" -ex "bt" -e "editor" 2>&1 | grep -v ^"No stack."$
