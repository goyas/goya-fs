#!/bin/bash

BASE=$(dirname `readlink -f $0`)
#echo $BASE
cd $BASE

DEPS=$BASE/thirdparty
DEPS_SOURCE=$DEPS
DEPS_PREFIX=$DEPS/install
export PATH=${DEPS_PREFIX}/bin:$PATH
mkdir -p ${DEPS_PREFIX} ${DEPS_SOURCE}

# 1 boost
pushd $DEPS
if [ ! -f "${DEPS_PREFIX}/lib/libboost_system.a" ] \
  || [ ! -d "${DEPS_PREFIX}/include/boost" ]; then
  rm -rf boost_1_57_0
  wget https://raw.githubusercontent.com/lylei9/boost_1_57_0/master/boost_1_57_0.tar.gz
  tar zxvf boost_1_57_0.tar.gz
  cd boost_1_57_0
  ./bootstrap.sh --with-libraries=all --with-toolset=gcc
  ./b2 toolset=gcc
  ./b2 install --prefix=${DEPS_PREFIX}/
  sudo ldconfig
  cd -
  rm boost_1_57_0.tar.gz
fi
popd

# 2 protobuf
pushd $DEPS
if [ ! -f "${DEPS_PREFIX}/lib/libprotobuf.a" ] \
  || [ ! -d "${DEPS_PREFIX}/include/google/protobuf" ]; then
  rm -rf protobuf-3.9.1
  wget https://github.com/protocolbuffers/protobuf/releases/download/v3.9.1/protobuf-cpp-3.9.1.tar.gz
  tar zxvf protobuf-cpp-3.9.1.tar.gz
  cd protobuf-3.9.1
  autoreconf -ivf
  ./configure --prefix=${DEPS_PREFIX}/
  make -j4 && make install
  cd -
  rm protobuf-cpp-3.9.1.tar.gz
fi
popd

# 3 leveldb
pushd $DEPS
if [ ! -f "${DEPS_PREFIX}/lib/libleveldb.a" ] \
  || [ ! -d "${DEPS_PREFIX}/include/leveldb" ]; then
  rm -rf leveldb
  git clone https://github.com/r00289486/leveldb.git 
  cd leveldb
  echo "PREFIX=${DEPS_PREFIX}" > config.mk
  make -j4 && make install
  cd -
fi
popd

# 4 goya-rpc
pushd $DEPS
if [ ! -f "${DEPS_PREFIX}/lib/libgoya-rpc.a" ] \
  || [ ! -d "${DEPS_PREFIX}/include/goya/rpc" ]; then
  rm -rf goya-rpc 
  git clone --depth=1 https://github.com/goyas/goya-rpc.git
  cd goya-rpc
  deps_prefix_sed=$(echo $DEPS_PREFIX | sed -e 's/\//\\\//g')
  echo $deps_prefix_sed
  sed -i "s/\${PROJECT_BINARY_DIR}/${deps_prefix_sed}/" config.cmake
  ./onekeybuild.sh
fi
popd

if [ ! -d build ]; then
  mkdir -p build
fi

pushd $BASE/build
cmake .. && make 

