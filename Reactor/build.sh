#!/bin/bash

set -x

SOURCE_CODE=`pwd`
mkdir -p ../build \
	&& cd ../build\
  && cmake \
		-DCMAKE_INSTALL_PREFIX=./install \
		-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
		-DCMAKE_EXPROT_COMPILE_COMMANDS=ON \
		${SOURCE_CODE} \
	&& make $*

