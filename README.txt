Project structure: 

3rdparty - All of the 3rd party libraries we use.
android - The Android reference projects.
distribute - The files and set up used to create the SDK installers and other distributable packages
samples - Lua and tools samples as well as some development test/experiment scripts.
src - Moai source code and headers.
test - Lua stress tests for Moai. These were written to crash Moai. We'll be fixing Moai to handle more user errors gracefully (instead of crash) as the project develops.
vs2008 - Visual Studio 2008 reference projects.
vs2010 - Visual Studio 2010 reference projects.
xcode - Mac OSX and iPhone projects. 

Developing on Windows:

Start with the vs2008 or 2010 reference solutions. Make sure a 32-bit architecture is selected (64-bit not yet officially supported). The 'moai' project builds moai.exe, a command line Moai host based on GLUT.

Lua samples are located in samples/basics. To run, build moai.exe. Add the MOAI_BIN and MOAI_CONFIG environment variables:

MOAI_BIN = <path to Moai>/vs20XX/bin/Win32/<Debug|Release>/
MOAI_CONFIG = <path to Moai>/config/

If these two environment variables are set up correctly, you can run each sample simply by double clicking the run.bat file found in each folder.

Developing on Mac:

Open xcode/osx/MoaiSample.xcodeproj. Make sure a 32-bit architecture is selected (64-bit not yet officially supported). Build moai as a command line utility.

To try out the samples, configure Xcode's debug working directory and command line parameters. Set the working directory to the directory of the sample you want to try, relative to the Xcode project (for example '../../samples/basics/anim-basic'). Set the command line parameters to '../../config/config.lua main.lua'.

config.lua sets up some richer error messages; it's a good idea to always pass it to Moai. main.lua runs the sample.

In Xcode 3, right click the executable and choose 'Get Info.' In Xcode 4, select 'Edit Scheme' from the Schemes drop down. You will find the settings on the 'Info' and 'Arguments' tab the scheme editor.

Developing for iOS:

Use xcode/ios/MoaiSample.xcodeproj. This sample project includes the Lua and image found in samples/basics/anim-basic. The Moai runtime is implemented in an OpenGL view. Look at AppDelegate to see how it is used.

As Moai is written in C++, we've generally preferred Objective-C++ for our work. To make dealing with Apple's reference counting easier, we use a smart pointer called RefPtr <>. If you're planning to extend MoaiView and don't mind using Objective-C++, try it out for yourself.

Developing on Linux:

 $ cmake .
 $ make

Installing the dependencies on Ubuntu:

 $ sudo apt-get install cmake libcurl4-openssl-dev libfreetype6-dev \
     liblua5.1-0-dev libpng12-dev freeglut3-dev libxi-dev libxmu-dev