# Welcome to Moai!

Thanks for checking out the Moai source code. Here's the scoop.

## Download
Please clones the source in the following way.

    git clone git@github.com:moai/moai-dev.git
    git submodule init
    git submodule update

## Setup
Tools needed to build the MOAI SDK.

* [Android SDK](http://developer.android.com/sdk/index.html)
* [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)
* [Ant 1.9](http://ant.apache.org/)
* [CMake 2.8.10](http://www.cmake.org/)
* [Visual Studio 2010](http://www.visualstudio.com/) (Windows Only)
* [Cygwin](http://www.cygwin.com/) (Windows Only)
* [XCode 4 or 5](https://itunes.apple.com/app/xcode/id497799835?mt=12) (Mac Only)

## Build

### Windows Host
Please run the "./bin/build-cmake-win.bat".

	# Build
	cd moai-dev
	.¥bin¥build-cmake-win.bat
	
### OSX Host

	# Build
	cd moai-dev
	./bin/build-cmake-osx
	
	# Run
	release/osx/x64/bin/moai main.lua

### Linux Host

	# Build
	cd moai-dev
	./bin/build-linux_x64
	
	# Run
	release/linux/x64/bin//moai main.lua

### iOS Host

	cd moai-dev
	./bin/build-cmake-ios.sh <lua_dir>

### Android Host

	# Build
	cd moai-dev
	./bin/build-android
	
	# Edit Setting
	cd release/android/host
	vi settings-global.sh
	vi settings-local.sh
	
	# Run
	./run-host.sh

### HTML Host
TODO:Please describe someone.

## Sample code
Go to the samples folder, which contains dozens of samples that can be run using one the hosts located in ant (Android), xcode (OSX and iOS), vs2008 or vs2010 (Windows). One of our priorities is to build richer samples for Moai developers, so if you'd like to see a particular code sample, let us know by posting in the [Moai forums](http://getmoai.com/forums/).

## API Documentation
TODO:API documentation is older

[API Documentation](http://getmoai.com/docs/).

## FAQ
[Moai FAQ](http://getmoai.com/marketing/moai-faqs.html)

## Wiki

[Moai Wiki](http://getmoai.com/wiki/)
[Moai Wiki(github)](https://github.com/moai/moai-dev/wiki)

## License
This SDK uses the CPAL license (http://www.opensource.org/licenses/cpal_1.0). Refer to the [Made With Moai page](http://getmoai.com/marketing/made-with-moai.html) for attribution screens to include with games made with Moai.

Happy game-making with Moai. Check out the [Moai Wiki](http://getmoai.com/wiki/), ping us through the [forums](http://getmoai.com/forums/) or email us at support@getmoai.com if you need help. The Moai team is here to help.

## Project structure

* 3rdparty - All of the 3rd party libraries we use.
* ant - Android source and reference projects.
* cmake - 
* distribute - The files and set up used to create the SDK installers and other distributable packages.
* docs - Moai documentation, release notes and attribution images.
* samples - Lua samples and development test/experiment scripts.
* scons - Google Chrome source.
* src - Moai source code and headers.
* test - Lua unit and stress tests for Moai.
* vs2008 - Visual Studio 2008 source and reference projects.
* vs2010 - Visual Studio 2010 source and reference projects.
* xcode - Mac OSX and iOS source and reference projects. 
