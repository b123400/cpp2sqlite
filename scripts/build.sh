#!/bin/bash

pushd ../
git pull        # dangerous: it could update this build.sh file
git submodule init
git submodule update
popd

./download.sh

SRC_DIR=$(pwd)/..
BLD_DIR="$SRC_DIR/build"
BIN_DIR=/usr/local/bin/

BIN_XLNT=/usr/local

rm -rf $BLD_DIR
mkdir -p $BLD_DIR
cd $BLD_DIR

cmake -G"Unix Makefiles" \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=$BIN_DIR \
    -D XLNT_DIR=$BIN_XLNT \
    $SRC_DIR

make -j9 all
#sudo make install

cd $BLD_DIR  # it should be $BIN_DIR otherwise there is no point in doing make install
time ./cpp2sqlite --lang=fr --inDir $SRC_DIR/input&
time ./cpp2sqlite --inDir $SRC_DIR/input&
./interaction --inDir $SRC_DIR/input
