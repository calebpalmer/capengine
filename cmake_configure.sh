#!/bin/bash
set -e
set -x

BUILDDIR=$1
INSTALLDIR=$BUILDDIR/install
BUILDTYPE=$2

if [[ "$BUILDTYPE" == "Debug" ]]; then
    CONANPROFILE=conan_debug.profile
    PRESET=conan-debug
else
    CONANPROFILE=conan_release.profile
    PRESET=conan-release
fi

mkdir -p $BUILDDIR

./conan_install.sh $CONANPROFILE

. conan/${BUILDTYPE}/generators/conanbuild.sh

CC=gcc CXX=g++ cmake -B $BUILDDIR \
       -DCMAKE_TOOLCHAIN_FILE=conan/${BUILDTYPE}/generators/conan_toolchain.cmake \
      -GNinja \
      -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
      -DCMAKE_BUILD_TYPE=$BUILDTYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      -DBOOST_STACKTRACE_USE_ADDR2LINE=true
