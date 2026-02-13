#!/bin/bash
CONANPROFILE=$1

conan install . \
      --build missing \
      --profile:all $1
