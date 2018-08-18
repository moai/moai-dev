#!/bin/bash


  export MOAI_SDK_HOME=${MOAI_SDK_HOME-$(cd $(dirname "${BASH_SOURCE[0]}")/../; pwd)}
  #--User config
  if [ -e "$(dirname ${BASH_SOURCE[0]})/env-local.sh" ]; then
     source $(dirname "${BASH_SOURCE[0]}")/env-local.sh
  fi




#--check for reqs
error=false

which cmake || (echo "could not detect cmake. apt-get install cmake" && error = true)


#--install
if [ "$error" == "false" ]; then

echo "MOAI_SDK_HOME = $MOAI_SDK_HOME"

if [ ! -z "$NDK_PATH" ]; then
   echo "Setting NDK path..."
   export ANDROID_NDK=$NDK_PATH
 else
   echo "No NDK_PATH specified, Android will not be buildable"
fi


if [ ! -z "$ANDROID_SDK_HOME" ]; then
   echo "Setting Android SDK path..."
   export ANDROID_SDK_HOME=$ANDROID_SDK_HOME
 else
   echo "No ANDROID_SDK_HOME specified, Android hosts will need path set in local.properties to be buildable"
fi

if [ -z "$EMSDK_PATH" ]; then
   echo "No EMSDK_PATH specified, JS libs will not be buildable"
fi

echo "Environment setup complete"
fi
