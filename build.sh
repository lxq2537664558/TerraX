#!/bin/sh

mkdir -p build \
&& cd build \
&& cmake .. \
&& make $*

#$* what is the meaning of it?