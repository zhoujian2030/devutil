#!/bin/sh

echo "build in platform" $PLATFORM

if [ $# == 1 ]; then
    libname="$1"
    echo "lib name is" $libname
else
    libname=libhttp
fi

if [ "$PLATFORM" == "arm" ]; then
    cp lib/${libname}.a lib/${libname}_arm.a
fi

