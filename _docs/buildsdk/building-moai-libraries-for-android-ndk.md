---
title: "Building Moai Libraries For Android"
order: 0
out_of_date: true
---

Building the Moai source code for Android requires the Android Native Development Kit (NDK), which can be downloaded [here](http://developer.android.com/sdk/ndk/index.html). If you're using Windows, you will also need to install Cygwin, which is required by the Android NDK. Android may abandon Cygwin for future releases of the NDK, but for now it is still required and can be downloaded [here](http://cygwin.com/install.html).

Unzip the contents of the NDK package to a location with no spaces in its path - spaces can negatively impact the scripts you'll be running - and make sure that the NDK folder is in your path. Cygwin's installer is more complex and features a large number of modules that can be selectively installed. Note that the Moai build process relies on several executables which are not installed by default, so to be safe, you should install the entire Cygwin developer folder. There is a README.txt file located in 'ant/libmoai' that describes all of the additional things that you need to do to get things up and running under Cygwin on Windows - if you run into issues, check there for additional instructions. If the build process is breaking because a certain executable cannot be found, you can probably find information about getting the missing executable by searching online.

Once the NDK and Cygwin are installed, you can run the 'build' script located in the ant/libmoai/ folder to build Moai for Android - no command-line parameters are required, but many are available to control optional components and the target environment. Once the build is complete (it takes a while), you should have a new Moai library in 'ant/libmoai/libs/armeabi' and/or 'ant/libmoai/libs/armeabi-v7a'.

See [Building Moai Games For Android Devices](../building/building-moai-games-for-android-devices.html) for details on creating and running a Moai Android host.

NOTE: More details coming soon...
