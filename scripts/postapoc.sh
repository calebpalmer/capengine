#!/bin/bash
export CP_ASSETFOLDER=build/postapoc_resources

if [[ "$1" == "--debug" ]]; then
    gdb --args build/bin/postapoc
else
    build/bin/postapoc
fi


