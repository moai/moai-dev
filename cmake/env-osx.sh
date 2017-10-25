#!/bin/bash

if [ ! "$0" == "-bash" ]; then
  ruby -e "exit '$0'.end_with? '.sh'"
  if [ $? -ne 0 ]; then
  echo ERROR: This file is meant to be be sourced by running with: source ./bin/env-osx.sh
  echo was run with: $0
	exit 1
fi
fi

  export MOAI_SDK_HOME=${MOAI_SDK_HOME-$(cd $(dirname "${BASH_SOURCE[0]}")/../; pwd)}
  #--User config
  if [ -e "$(dirname ${BASH_SOURCE[0]})/env-local.sh" ]; then
     source $(dirname "${BASH_SOURCE[0]}")/env-local.sh
  fi


#--check for reqs
error=false

#android sdk likes 1.7 of javac
#javac -version 2>&1 | grep "javac 1\.[78]" > /dev/null || { echo "JDK was not at 1.7"; error=true; }


#--install

echo "MOAI_SDK_HOME = $MOAI_SDK_HOME"

if [ ! -z "$NDK_PATH" ]; then
   echo "Setting NDK path..."
   export ANDROID_NDK=$NDK_PATH
 else
   echo "No NDK_PATH specified, Android will not be buildable"
fi

if [  -z "$EMSDK_PATH" ]; then
   echo "No EMSDK_PATH specified, JS libs will not be buildable"
fi

echo "Environment setup complete"

