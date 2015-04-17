---
title: "Moai SDK Desktop Basics"
order: -700
---

Moai SDK
--------

The Moai SDK contains binaries, samples and projects for all the supported platforms. The next sections describe how to use the binaries on Windows and OSX, as well as how to build for Android and iPhone.

The SDK has everything you need to start prototyping games using the GLUT host or building a custom host using Visual Studio 2008 or 2010. You can also build and run apps on Android using Eclipse and the Android SDK/NDK.

To get started, download the SDK zip file and unpack it. Open the 'samples' folder. The samples are a set of extremely basic code snippets that demonstrate some of the most frequently used objects in the Moai framework.

In the samples folder, open 'anim/anim-basic' and execute 'run.bat' or 'run.sh'. You should see a console window open followed by a graphics window that shows a spinning cat head. The run script files are simply a convenience to save you the trouble of launching the default Moai host and passing in a Lua file.

![]({{ site.baseurl }}/img/docs/3-1.png)

Now that Moai is working, you can set up a project to experiment with. Start by creating a new folder somewhere in your system; in your documents folder or on the desktop, for example. Copy the entire contents of the 'anim-basic' folder into your new folder. When you're done you should have a copy of the run scripts, main.lua, and cathead.png in your new folder. By default, the run scripts use a relative path to the Moai executables in the SDK. They also pass a relative reference to a sample 'config.lua' file as the first parameter. Since MOAI executes all the Lua files passed to it in order, this demonstrates how to set up configuration files which run before the main game files. You'll probably want to edit your run scripts and update them to continue pointing at the Moai executables and the 'config.lua' file in your SDK (or simply create your own config setup, or omit it entirely). You should also consider adding the binaries to your system path so you can easily access them from anywhere. Once you've configured this, execute the run script again. If you see the cat head spin, then everything is set up and working correctly.

Building with Visual Studio
---------------------------

The code and project that builds the GLUT host you've been using to spin cat heads is included in the Moai SDK. If you know C or C+++, you can use this as the starting point for writing a custom host or a device simulator.

First, locate the Moai host solution directory for the version of Visual Studio you have installed. Open the 'hosts' folder and choose either the 'vs2008' or the 'vs2010' subfolder.

The Visual Studio solution contains two projects: 'moai' and 'moai-untz'. Each is configured to reference Moai as a static linked library. The 'moai' project builds the Moai GLUT sample host without any sound support. It should build and run as-is without any additional configuration. The 'moai-untz' project is packaged separately as it requires DirectX for sound support, which is not distributed with Moai. To get DirectX, download and install the latest [DirectX SDK](http://msdn.microsoft.com/en-us/directx/aa937788) from Microsoft. Once this SDK is installed, you will need to add the search paths for the DirectX include and library folders to either the 'moai-untz' project or to Visual Studio's global search paths. A readme file is included in the vs20xx folder explaining exactly how to set up the DirectX paths.

It is assumed that anyone doing this knows how to use Visual Studio and has their own goals for a custom Moai host, so this article won't go into further detail on this process.

Setting up on Ubuntu [Experimental]
-----------------------------------

Developing on Linux is still experimental with MOAI but is possible using GLUT.

```
   sudo apt-get install cmake freeglut3 freeglut3-dev binutils-gold libxmu-dev libxi-dev dos2unix libasound2-dev
```
`   git clone -b Version-1.3-Build-98-cmake `[`https://github.com/shadowmint/moai-dev.git`](https://github.com/shadowmint/moai-dev.git)\
```
   cd moai-dev
   mkdir build
   cd build
   cmake ../cmake/
   make
```
