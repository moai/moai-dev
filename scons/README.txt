Building for Chrome Native Client requires the Native Client SDK, which can be
downloaded from the following URL:

https://developers.google.com/native-client/sdk/download

Download and unzip the SDK and run "./naclsdk update", which will download the
most recent updates for the SDK and Native Client platform levels. You can move
the SDK anywhere on your computer, but you have to tell the build where it's 
located. To do so, set the NACL_SDK_ROOT environment variable in your shell to 
the location of the root of the SDK. For example:

export NACL_SDK_ROOT=/Chrome/nacl_sdk

Moai for Native Client also requires FMOD for sound support. FMOD for Native
Client can be downloaded from the following URL:

http://www.fmod.org/index.php/download

PLEASE NOTE that FMOD requires a license - information about licensing FMOD is
available at the following URL:

http://www.fmod.org/index.php/sales

Download and unzip the FMOD SDK and move it anywhere on your computer. To let
the build know where the SDK is located, set the FMOD_CHROME_SDK_ROOT 
environment variable in your shell to the location of the root of the SDK. For 
example:

export FMOD_CHROME_SDK_ROOT=/Chrome/fmodapi43805nacl

To build Moai for Native client, run the "build.sh" or "build.bat" script. 
Note that if you are building on Windows, you will need to install cygwin and 
ensure that the cygwin binaries are in your executable path.

The build script takes two optional command line parameters:

-clean (clean, rather than build)

-p platform-level (specifies the Native Client platform level to build for - 
	the default is pepper_17)

You will also have to ensure that FMOD has provided libraries for the platform
level that you have specified or the linking phase of the build will fail. Look
in FMOD_CHROME_SDK_ROOT/api/lib and ensure that a subdirectory exists for the 
Native Client platform level you intend to build with. If no official FMOD 
libraries for a given platform level were provided in the SDK, you can attempt 
to use another version of the FMOD libraries by creating a soft-link (or shortcut 
in Windows) that links the desired platform level with an existing platform level
library supplied by FMOD. For example:

cd $FMOD_CHROME_SDK_ROOT/api/lib
ln -s pepper_17 pepper_16

This will create a node called pepper_16 that simply points to pepper_17 and
will allow you to build and link for platform level pepper_16 even if no FMOD
libraries for pepper_16 are present in the FMOD SDK.

