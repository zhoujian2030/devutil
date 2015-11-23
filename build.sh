#!/bin/sh

export PROJBASE=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
echo "Current directory: $PROJBASE"

cd ipc
make
