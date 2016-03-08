#!/bin/bash
ndkdir=$( awk -F"=" '/^ndk.dir=/{print $2}' < local.properties )
if [ ! $? -eq 0 ]; then
  echo "could not find the 'ndk.dir' property in your local.properties file"
	exit 1;
fi


if [ ! -e "$ndkdir" ]; then
  echo "path set for 'ndk.dir' in local.properties was not found ($ndkdir)" 
	exit 1;
fi

bash ./gradlew assembleDebug