#!/bin/bash

BUILDDIR=$1
INSTALLDIR=$BUILDDIR/install
BUILDTYPE=$2

export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cmake -S. -B$BUILDDIR \
      -GNinja \
      -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
      -DCMAKE_BUILD_TYPE=$BUILDTYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1
