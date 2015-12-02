#!/bin/sh

tar cvf test.tar untar.sh build.sh cm/ ipc/ log/ test/
scp test.tar root@120.24.67.50:/noname/demo
