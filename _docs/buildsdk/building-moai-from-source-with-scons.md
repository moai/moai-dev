---
title: "Building Moai From Source With Scons"
order: 30
out_of_date: true
---

Moai SDK now supports running games with [Native Client](http://en.wikipedia.org/wiki/Google_Native_Client) in Google's Chrome browser, as announced [recently](http://getmoai.com/2011/10/big-news-moai-for-chrome/) on the Moai blog.

Prerequisites
-------------

- Moai SDK 0.9 (Revision 8) or later.

- Chrome 15 or later.

- The [Native Client SDK](http://code.google.com/chrome/nativeclient/docs/download.html).

- The [FMOD SDK](http://www.fmod.org/index.php/download) for sound support. NOTE: FMOD use requires a license. More information about licensing is available [here](http://www.fmod.org/index.php/sales). If you do not want sound in your game, you will need to remove the FMOD library dependencies from the build.

Building for Native Client
--------------------------

Before building Moai for Native Client, it is necessary to tell the build scripts where the Native Client SDK is located on your computer. To do so, set the NACL\_SDK\_ROOT environment variable in your shell. For example:

NACL\_SDK\_ROOT=/Chrome/nacl\_sdk

Likewise, the build needs to know where the FMOD SDK is installed as well. To do so, set the FMOD\_SDK\_ROOT environment variable in your shell. For example:

FMOD\_SDK\_ROOT=/Chrome/fmodapi43805nacl

To build, go to scons/ and run the build.sh (Mac) or build.bat (Windows) script (NOTE: Windows build requires cygwin). The script takes two optional command line parameters:

-clean (clean, rather than build)

-p platform-level (specifies the Native Client platform level to build for - the default is pepper\_16)

You will also have to ensure that FMOD has provided libraries for the platform level that you have specified or the linking phase of the build will fail. Look in FMOD\_SDK\_ROOT/api/lib/ and ensure that a subdirectory exists for the Native Client platform level you intend to build with. If no official FMOD libraries for a given platform level were provided in the SDK, you can attempt to use another version of the FMOD libraries by creating a soft-link (or shortcut in Windows) that links the desired platform level with an existing platform level library supplied by FMOD. For example:

cd \$FMOD\_SDK\_ROOT/api/lib

ln -s pepper\_15 pepper\_16

This will create a node called pepper\_16 that simply points to pepper\_15 and will allow you to build and link for platform level pepper\_16 even if no FMOD libraries for pepper\_16 are present in the FMOD SDK. If you do not want sound support in your game, you can eliminate the FMOD dependency, but to do so you will have to manually remove the build dependencies in build.scons.

When the build completes, you will need to copy the moai\_x86\_32.exe, moai\_x86\_64.exe and moai.nmf to your extension directory.

Running a Moai Native Client Host
---------------------------------

See [Building Moai Games for Google Chrome](../building/building-moai-games-for-google-chrome.html).

The basic workflow is:

- Run the build script (only for C++ changes).

- Launch Chrome.

- Clear browsing data.

- Run the extension.

For questions, please see this [forum thread](http://getmoai.com/forums/moai-sdk-developer-support/google-chrome-native-client/).
