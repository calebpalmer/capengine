#!/bin/bash
export CP_ASSETFOLDER=build/rps_resources

if [[ "$1" == "--debug" ]]; then
    gdb --args build/bin/rps
else
    build/bin/rps
fi


