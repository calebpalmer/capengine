#!/bin/bash
mkdir -p .conan
pushd .conan

#mkdir -p packages
PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig/ conan install .. \
	       --build missing \
	       -s compiler.libcxx=libstdc++11

popd
