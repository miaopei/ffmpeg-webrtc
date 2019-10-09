#!/usr/bin/env sh

set -euv

# Updated on April 2019 to reflect changes in GN
# See:
# https://github.com/ninja-build/ninja
# https://gn.googlesource.com/gn/

# We need a recent git
export PATH=/depot/git-2.8.3/bin:$PATH

# Directory where all the stuff will be built
rm -rf build_tools
mkdir build_tools
cd build_tools

# We install all to $HOME/bin
mkdir -p $HOME/bin

# Build ninja
git clone https://github.com/ninja-build/ninja.git
cd ninja
./configure.py --bootstrap
cp -f ninja $HOME/bin/
cd ..

# Setup tools for GN
export PATH=/depot/binutils-2.30/bin:/depot/gcc-6.2.0/bin:$HOME/bin:$PATH
export CC=gcc
export CXX=g++
export LDFLAGS=-lrt

# Build GN
git clone https://gn.googlesource.com/gn
cd gn
python build/gen.py
ninja -C out
# out/gn_unittests
cp -f out/gn $HOME/bin
