#!/bin/bash

echo '################################################'
echo 'Generating build system for stlc in "build/"...'
echo '################################################'
echo ''

cmake -B build/ -S .

echo ''
echo '##########################################'
echo 'Building object code into "libstlc.so"...'
echo '##########################################'
echo ''

cd build
make install

echo ''
echo '##########################################################'
echo 'Linking "./build/libstlc.so" to "/usr/lib/libstlc.so"...'
echo '##########################################################'
echo ''

sudo ln -s ./build/libstlc.so /usr/lib/libstlc.so

echo ''
echo '#####################################################'
echo 'Linking header files to "/usr/include/C++/9/stlc"...'
echo '#####################################################'
echo ''

if [ -d "/usr/include/C++/9/stlc" ]; then
    echo '"/usr/include/C++/9/stlc" directory exists!'
else
    echo '"/usr/include/C++/9/stlc" directory does not exists!'
    echo 'Making directory "/usr/include/C++/9/stlc"...'
    sudo mkdir /usr/include/C++/9/stlc
fi

sudo ln -s include/** /usr/include/C++/9/stlc