# Welcome to Moai!

Thanks for checking out the Moai source code. Here's the scoop.

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
* [Java SE](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [Ant 1.9](http://ant.apache.org/)
* [CMake 2.8.11](http://www.cmake.org/)
* [Ruby](http://www.ruby-lang.org/)
* [Visual Studio 2010](http://www.visualstudio.com/) (Windows only)
* [XCode 4 or 5](https://itunes.apple.com/app/xcode/id497799835?mt=12) (Mac only)
* [MinGW64 TCC](http://tdm-gcc.tdragon.net/) (Windows only, needed for android luajit support or can build mingw version of moai)
 
The following tools are required to build the API documentation.

* [Bundler](http://gembundler.com/)
* [Doxygen](http://www.doxygen.org/)
* [Graphviz](http://www.graphviz.org/)

Please set the following environment variables.

## Build Hosts
Please build according to the following procedure.

### Windows Host

	cd moai-dev
	.¥bin¥build-win.bat
	
### OSX Host

	# Build
	cd moai-dev
	./bin/build-osx.sh
	
	# Run
	release/osx/x64/bin/moai main.lua

### iOS Host

	cd moai-dev
	./bin/build-ios.sh <lua_dir>

### Linux Host

	# Build
	cd moai-dev
	./bin/build-linux_x64.sh
	
	# Run
	release/linux/x64/bin/moai main.lua

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
 
### vs2012
    ./bin/create-projects-win.bat
    start cmake/projects/vs2012/moai.sln

### iOS
    ./bin/create-projects-ios.sh
    open cmake/projects/moai-ios/moai.xcproj

### OSX
    ./bin/create-projects-osx.sh
	open cmake/projects/moai-osx/moai.xcproj

## Sample code
Go to the samples folder, which contains dozens of samples that can be run using one the hosts located in ant (Android), xcode (OSX and iOS), vs2008 or vs2010 (Windows).

One of our priorities is to build richer samples for Moai developers, so if you'd like to see a particular code sample, let us know by posting in the [Moai forums](http://getmoai.com/forums/).

## License

This SDK uses the [CPAL license](http://www.opensource.org/licenses/cpal_1.0). Refer to the  for attribution screens to include with games made with Moai.

## Links

### Moai Pages

* [Moai New Web Site](http://moaiwebsite.github.io/)
* [Moai FAQ](http://getmoai.com/marketing/moai-faqs.html)
* [Moai SDK Framework Reference](http://getmoai.com/docs/)
* [Moai forums](http://getmoai.com/forums/)
* [Moai Wiki(getmoai)](http://getmoai.com/wiki/)
* [Moai Wiki(github)](https://github.com/moai/moai-dev/wiki)
* [Moai API Documentation](http://getmoai.com/docs/).

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
