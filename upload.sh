#!/bin/sh

tar cvf test.tar untar.sh build.sh Makefile cm/ ipc/ log/ cpp/ dao/ mq/ net/ test/ 
scp test.tar root@120.24.67.50:/noname/demo
