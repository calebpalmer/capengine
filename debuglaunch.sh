#!/bin/bash
gdb -batch -ex run -ex bt -ex quit build/bin/flappypei "$@"
