#!/bin/bash
PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig/ conan install . \
						   --build missing \
						   --profile debug.profile
