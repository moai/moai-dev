---
title: Introduction
order: -1000
---
### The MOAI Libraries

This section is about building the MOAI libraries from source.

A MOAI game is built using 3 core pieces:

  1. Your Lua code and Assets
  2. The host project and host code (usually one per platform per game)
  3. The MOAI libraries (usually shared between games, can be downloaded as a binary releases)

### Why build from scratch

Sometimes you may want to build the library from source instead of  downloading the binary sdk.

You might want to do this to:

  - Confirm the security and safety of your binaries (you may not trust the downloaded precompiled binaries)
  - To build the libraries with different compiler settings and optimisations.
  - To enhance, or fix the MOAI runtime in order to contribute back to the community.
  - To customize the MOAI runtime to suit your particular game.
  
### Building MOAI Libraries

Each supported platform has its own build script in the bin folder. Each of these scripts will build the static libraries for that platform and place them in `<SDK_ROOT>/lib/<platform>/lib` the include files will be placed in `<SDK_ROOT>/lib/<platform>/include`. These libraries and headers are used by the host projects to produce your game's  distributables.

Before the majority of these scripts can run, you will need to install cmake (version 3.x). See our [CMAKE install guide](installing-cmake.html) for info on getting setup.

Each of the platforms has it own requirements other than cmake and are outlined in the build instructions for each platform.

### Per Platform Build Instructions

Plaform guides for building Moai Libraries:

  * [Android](building-moai-libraries-for-android-ndk.html)
  * iOS
  * Windows
  * Linux
  * OSX
  * Windows Phone
  * HTML/JS
  
