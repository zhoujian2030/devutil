#!/bin/sh

export PROJBASE=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
echo "Current directory: $PROJBASE"

make -C log clean
make -C ipc clean
make -C test/ipc clean
make -C log
make -C ipc
make -C test/ipc
