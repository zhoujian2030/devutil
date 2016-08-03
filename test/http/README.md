# Compile and install boost in Centos
1. download [boost_1_61_0.tar.bz2](https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2/download)

2. extract the package
- tar --bzip2 -xf boost_1_61_0.tar.bz2
- cd boost_1_61_0

3. build and install
- ./bootstrap.sh --prefix=/usr/local
- ./b2 install

4. compile and link application code
- with dynamic boost lib: g++ -I/usr/local/include example.cpp -o example -L/usr/local/lib -lboost_system
- with static boost lib: g++ -I/usr/local/include example.cpp -o example /usr/local/lib/libboost_system.a

5. [Reference Link](http://www.boost.org/doc/libs/1_61_0/more/getting_started/unix-variants.html#easy-build-and-install)