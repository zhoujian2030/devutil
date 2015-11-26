#!/bin/sh

export PROJBASE=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
echo "Current directory: $PROJBASE"

make -C ipc clean
make -C test/ipc clean
make -C ipc
make -C test/ipc
