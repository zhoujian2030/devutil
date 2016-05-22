#!/bin/sh

tar cvf test.tar untar.sh build.sh Makefile cm/ ipc/ cpp/ dao/ net/ ads/ test/ 
scp test.tar root@120.24.67.50:/noname/demo
