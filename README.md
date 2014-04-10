# Welcome to Moai!

Thanks for checking out the Moai source code. Here's the scoop.

Linux CI: [![Build Status](https://travis-ci.org/moaiforge/moai-sdk.png?branch=master)](https://travis-ci.org/moaiforge/moai-sdk)
OSX CI: [![Build Status](https://travis-ci.org/moaiforge/moai-sdk.png?branch=travis-osx)](https://travis-ci.org/moaiforge/moai-sdk)
## Introduction

Moai SDK is an open source game framework for quickly developing great mobile games.

You can work in Lua or C++, and Moai SDK runs on multiple platforms including iOS, Android, Chrome, Windows, Mac and Linux.

## Download
Please clones the source in the following way.

    git clone git@github.com:moai/moai-dev.git
    git submodule init
    git submodule update

## Setup
Tools needed to build the MOAI SDK.

* [Android SDK](http://developer.android.com/sdk/index.html)
* [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html) (32bit Only)
* [Java SE 6](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [Ant 1.9](http://ant.apache.org/)
* [CMake 2.8.11](http://www.cmake.org/)
* [Ruby 1.9](http://www.ruby-lang.org/)
* [Visual Studio 2010](http://www.visualstudio.com/) (Windows only)
* [XCode 4 or 5](https://itunes.apple.com/app/xcode/id497799835?mt=12) (Mac only)
* [MinGW64 TCC](http://tdm-gcc.tdragon.net/) (Windows only, needed for android luajit support or can build mingw version of moai)
* [Emscripten](https://github.com/kripken/emscripten)
 
The following tools are required to build the API documentation.

* [Bundler](http://gembundler.com/)
* [Doxygen](http://www.doxygen.org/)

Please set the following environment variables.

	# Mac OSX
	export MOAI_BIN=<install_path>/moai-dev/release/osx/host-sdl/bin
	export MOAI_CONFIG=<install_path>/moai-dev/samples/config

	# Linux
	export MOAI_BIN=<install_path>/moai-dev/release/linux/host-sdl/x64/bin
	export MOAI_CONFIG=<install_path>/moai-dev/samples/config

	# Windows
	set MOAI_BIN=<install_path>¥moai-dev¥release¥win32¥host-sdl¥bin
	set MOAI_CONFIG=<install_path>¥moai-dev¥samples¥config

## Build Hosts
Please build according to the following procedure.

### Windows Host

	cd moai-dev
	.\bin\build-win.bat
	
### OSX Host

	# Build
	cd moai-dev
	./bin/build-osx-sdl.sh
	
	# Run
	cd <sample_directory>
	$MOAI_BIN/moai main.lua

### iOS Host

	cd moai-dev
	./bin/build-ios.sh <lua_dir>

### Linux Host

	# Build
	cd moai-dev
	./bin/build-linux-sdl.sh
	
	# Run
	$MOAI_BIN/moai main.lua

### Android Host

	# Build
	export ANDROID_NDK=<path_to_android_ndk>
	cd moai-dev
	./bin/build-android.sh
	
	# Edit Setting
	cd release/android/host
	vi settings-global.sh
	vi settings-local.sh
	
	# Run
	./run-host.sh

### HTML Host
    
    #build
    export EMSCRIPTEN_HOME=<path_to_emscripten_1.7.1>
    cd moai-dev
    ./bin/build-html.sh

    #run
    <http-serve> cmake/build/host-template/www

    #see src\host-html\readme.md for more info

## Create IDE Projects
 
### Visual Studio
Execute `create-projects-win.bat` in `moai-dev\bin`.
By default, this creates a Visual Studio 2012 solution in `moai-dev\cmake\projects\vs2012`. To generate project files for a different version of Visual Studio, specify your desired version (`vs2008`, `vs2010`, `vs2012`, or `vs2013`) as a command-line argument.

### iOS
    ./bin/create-projects-ios.sh
    open cmake/projects/moai-ios/moai.xcproj

### OSX
    ./bin/create-projects-osx.sh
	open cmake/projects/moai-osx/moai.xcproj

## Sample code
Go to the samples folder, which contains dozens of samples that can be run using one the hosts located in ant (Android), xcode (OSX and iOS), or Visual Studio (Windows).

One of our priorities is to build richer samples for Moai developers, so if you'd like to see a particular code sample, let us know by posting in the [Moai forums](http://getmoai.com/forums/).

## License

This SDK uses the [CPAL license](http://www.opensource.org/licenses/cpal_1.0). Refer to the  for attribution screens to include with games made with Moai.

## Forks

* [patrickmeehan/moai-dev](https://github.com/patrickmeehan/moai-dev)
* [moaiforge/moai-sdk](https://github.com/moaiforge/moai-sdk) 
* [moai-community/moai-dev](https://github.com/moai-community/moai-dev)

## Links

### Moai Pages

* [Moai New Web Site](http://moaiwebsite.github.io/)
* [Moai FAQ](http://getmoai.com/marketing/moai-faqs.html)
* [Moai SDK Framework Reference](http://getmoai.com/docs/)
* [Moai forums](http://getmoai.com/forums/)
* [Moai Wiki(getmoai)](http://getmoai.com/wiki/)
* [Moai Wiki(github)](https://github.com/moai/moai-dev/wiki)
* [Moai API Documentation](http://moaiforge.github.io/moai-sdk/api/latest/).

### Tutorials
* [GameFromScratch MOAI Tutorials](http://www.gamefromscratch.com/page/Moai-tutorial-series-The-adventures-of-an-intrepid-programmer-in-the-lands-of-Moai.aspx)
* [moaisnippets](http://www.moaisnippets.info/)

### Libraries
* [Rapanui](https://github.com/ymobe/rapanui)
* [Hanappe](https://github.com/makotok/Hanappe)
* [MOAI GUI](https://github.com/derickd/moaigui)
* [Shilke2D](https://github.com/Shrike78/Shilke2D)
* [Spine MOAI](https://github.com/tangerinagames/spine-moai)

### Tools
* [moaicli](http://halfnelson.github.io/moaicli/)
* [MoaiUtils](https://github.com/DanielSWolf/MoaiUtils/blob/master/README.md)
