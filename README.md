# How to use this project
---
## Install dependancy
### Install log4cplus (Mandotary)
- For x86 linux (download [REL_1_2_0-RC4](https://github.com/log4cplus/log4cplus))

```
# mkdir /usr/local/log4cplusx86
# ./configure --prefix=/usr/local/log4cplusx86 --enable-static
# make
# make install

> Note: if not specify --prefix, it will install to /usr/local by default
```

- For arm linux
```
# mkdir /usr/local/log4cplusarm
# export PATH=$PATH:/omap/eldk/usr/bin/
# ./configure --prefix=/usr/local/log4cplusarm --enable-static --host=arm-linux CXX=arm-linux-g++ CC=arm-linux-gcc AR=arm-linux-ar
# make
# make install
```

### Install gtest (Optional, required by unit test)
- For x86 linux
1. install cmake if not yet. (download [cmake-3.5.2.tar.gz](https://cmake.org/files/v3.5/cmake-3.5.2.tar.gz))
```
# tar zxvf cmake-3.5.2.tar.gz
# cd cmake-3.5.2
# ./configure
# gmake
# make install 
```
2. compile gtest

```
# cd test/lib/gtest
# ./build.sh

```

- For arm linux
> Not support yet!!

### Install boost (Optional, required by /test/http/ut)
> Note. boost must be installed bo compile and run ut

- Download [boost_1_64_0.tar.gz](https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz)
- 
- Install for x86 linux
```
# mkdir /usr/local/boostx86
# ./bootstrap.sh --prefix=/usr/local/boostx86
# ./b2 install
```

- Install for arm linux
> Not support yet!!


### Install postgresql lib (Optional, required by dao/)
> If not installed, please make sure "export DB_SUPPORT=FALSE" is set in build.sh. 
- TBD

### Install lksctp-tools (Optional, required by SCTP socket)
> SCTP is disabled by default. If you want to support SCTP, please make sure "export SCTP_SUPPORT=TRUE" is set in build.sh

- For x86 linux

1. download lksctp-tools from http://lksctp.sourceforge.net/ and compile or install the rpm as below

```
# yum install lksctp-tools
# yum install lksctp-tools-devel
# rpm -qa | grep sctp
lksctp-tools-1.0.10-7.el6.x86_64
lksctp-tools-devel-1.0.10-7.el6.x86_64

Note: must install lksctp-tools-devel, or sctp.h may be not found!
```

- For arm linux
> Note support yet!!

---
## Compile
- For x86 linux

```
# ./build.sh
```

> Note: dao/ is not compiled by default. Please mofify build.sh to enable compiling it.

- For arm linux

```
# ./build_arm.sh
```

> Note: dao/, sctp/ and test/ are not compiled by default. Please modify build_arm.sh to enable any of them. And NOTE that boost and gtest are required by test/, postgresql lib are required by dao/, lksctp-tools is required by sctp/

---
## Test
- Run sample test
> e.g.  Demo how to use TCP socket in net/

```
# sample/net/obj/tcpserver
```
- Run unit test
> e.g.  Test code in cm/
```
# test/cm/ut/obj/testCM
```