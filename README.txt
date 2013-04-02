Welcome to Moai!

Thanks for checking out the Moai source code. Here's the scoop.

Sample code:
Go to the samples folder, which contains dozens of samples that can be run using one the hosts located in ant (Android), xcode (OSX and iOS), vs2008 or vs2010 (Windows). One of our priorities is to build richer samples for Moai developers, so if you'd like to see a particular code sample, let us know by posting in the Moai forums at http://getmoai.com/forums/.

Object model documentation:
You can view the object model documentation at http://getmoai.com/docs/.

FAQ:
Answers to frequently asked questions are available in the Moai FAQ at http://getmoai.com/marketing/moai-faqs.html.

Wiki:
You can find detailed documentation, instructions and other helpful information in the Moai Wiki at http://getmoai.com/wiki/.

Moai Cloud:
You already have a Moai Cloud account which allows you to upload Lua scripts to be hosted and run as web services via the Moai Cloud Dashboard at http://dashboard.moaicloud.com/. If you have support questions, post them to the Moai Cloud Developer Support Forum at http://getmoai.com/forums/moai-cloud-developer-support.html or check out the Moai Wiki at http://getmoai.com/wiki/.

License:
This SDK uses the CPAL license (http://www.opensource.org/licenses/cpal_1.0). Refer to the Made With Moai page at http://getmoai.com/marketing/made-with-moai.html for attribution screens to include with games made with Moai.

Happy game-making with Moai. Check out the Moai Wiki at http://getmoai.com/wiki/, ping us through the forums at http://getmoai.com/forums/ or email us at support@getmoai.com if you need help. The Moai team is here to help.

Project structure: 

3rdparty - All of the 3rd party libraries we use.
ant - Android source and reference projects.
distribute - The files and set up used to create the SDK installers and other distributable packages.
docs - Moai documentation, release notes and attribution images.
samples - Lua samples and development test/experiment scripts.
scons - Google Chrome source.
src - Moai source code and headers.
test - Lua unit and stress tests for Moai.
vs2008 - Visual Studio 2008 source and reference projects.
vs2010 - Visual Studio 2010 source and reference projects.
xcode - Mac OSX and iOS source and reference projects. 

Developing on Windows:

Start with the vs2008 or 2010 reference solutions. Make sure a 32-bit architecture is selected (64-bit is not yet officially supported). The 'moai' project builds moai.exe, a command line Moai host based on GLUT.

Lua samples are located in the samples folder. To run, build moai.exe and set the MOAI_BIN and MOAI_CONFIG environment variables:

MOAI_BIN = <path to Moai>/vs20XX/bin/Win32/<Debug|Release>/
MOAI_CONFIG = <path to Moai>/config/

If these two environment variables are set up correctly, you can run each sample simply by double clicking the run.bat file found in each sample folder.

Developing on Mac:

Open xcode/osx/MoaiSample.xcodeproj. Make sure a 32-bit architecture is selected (64-bit is not yet officially supported). The 'moai' project builds moai, a command line Moai host based on GLUT.

To try out the samples, configure Xcode's debug working directory and command line parameters. In Xcode 3, right click the executable and choose 'Get Info.' In Xcode 4, select 'Edit Scheme' from the Schemes drop down. You will find the settings on the 'Info' and 'Arguments' tab the scheme editor.

Set the working directory to the directory of the sample you want to try, relative to the Xcode project (for example '../../samples/anim/anim-basic'). Set the command line parameters to '../../config/config.lua main.lua' (config.lua sets up some richer error messages so it's a good idea to always pass it to Moai, main.lua runs the sample).

Developing for iOS:

Open xcode/ios/MoaiSample.xcodeproj. This sample project includes the Lua and image found in samples/anim/anim-basic. The Moai runtime is implemented in an OpenGL view. Look at AppDelegate.mm, MoaiView.mm and OpenGLView.mm to see how it is used.

As Moai is written in C++, we've generally preferred Objective-C++ for our work. To make dealing with Apple's reference counting easier, we use a smart pointer called RefPtr <>.

Developing for Android:

Run ant/make-host.sh. This will create a new Android host template in the folder ant/untitled-host. Edit the local and global settings files in ant/untitled-host to setup the environment and choose a sample to run - by default it is setup to run ../../samples/anim/anim-basic. 

Use the ant/untitled-host/run-host.sh script in the generated directory to create the Android host once it has been configured. This will create the ant/untitled-host/build directory which contains a full Android project that can be imported into Eclipse and then launches the application onto the default Android emulator or device.



Git fork info:

This fork was created by Isaac D. Barrett (IkeBart) to add some additional functionality to the MOAITextBox class.  More specifically, it adds a method to find the optimal font size for a given rectangle and string.