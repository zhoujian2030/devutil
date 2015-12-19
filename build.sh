#!/bin/sh

export PROJBASE=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
echo "Current directory: $PROJBASE"

COMMAND=$1

if [[ $COMMAND = "clean" ]]
then
    make -C log clean
    make -C ipc clean
    make -C cpp clean
    make -C test/ipc clean
    make -C test/log clean
    make -C test/str clean
else
    make -C log
    make -C ipc
    make -C cpp
    make -C test/ipc
    make -C test/log
    make -C test/str
fi
