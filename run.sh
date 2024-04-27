#!/bin/sh
# run test
if [ ${1} = 't' ]
then
    echo testing
    make clean
    make USER_SET_MACRO=-DTESTING test
    build/test
else
    echo not testing
    make
    build/rx
fi
