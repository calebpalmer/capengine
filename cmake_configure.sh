#!/bin/bash
set -e
set -x

BUILDDIR=$1
INSTALLDIR=$BUILDDIR/install
BUILDTYPE=$2

if [[ "$BUILDTYPE" == "Debug" ]]; then
    CONANPROFILE=debug.profile
    PRESET=conan-debug
else
    CONANPROFILE=release.profile
    PRESET=conan-release
fi

./conan_install.sh $CONANPROFILE

mkdir -p $BUILDDIR

CC=gcc CXX=g++ cmake -B $BUILDDIR \
       --preset $PRESET \
      -GNinja \
      -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
      -DCMAKE_BUILD_TYPE=$BUILDTYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      -DBOOST_STACKTRACE_USE_ADDR2LINE=true
