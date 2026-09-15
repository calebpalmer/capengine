#!/bin/bash
export CP_ASSETFOLDER=build/breakout_resources

if [[ "$1" == "--debug" ]]; then
    gdb --args build/bin/breakout
else
    build/bin/breakout
fi


