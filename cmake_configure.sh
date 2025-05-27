#!/bin/bash
set -e
set -x

./conan_install.sh

BUILDDIR=$1
INSTALLDIR=$BUILDDIR/install
BUILDTYPE=$2
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p $BUILDDIR
pushd $BUILDDIR

CC=gcc CXX=g++ cmake -S${SCRIPT_DIR} -B. \
      -GNinja \
      -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
      -DCMAKE_BUILD_TYPE=$BUILDTYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      -DCMAKE_TOOLCHAIN_FILE="build/Debug/generators/conan_toolchain.cmake"

popd
