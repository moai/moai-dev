---
title: "Building Moai Games For Android Devices"
---

Creating an Android Host
------------------------

If you're using the binary Moai SDK, your Android host is already created for you and is located in the hosts/ant/ folder of the downloaded SDK. Otherwise, if you're working from source, you'll need to create a host. To do so, go the the ant/ folder in your Moai source repository and run the 'make-host' command. If you run the script with no command-line parameters, you'll see that many are available, but only one is required - "-p <package>". This parameter determines the Android package name for the host being created (e.g, com.getmoai.samples), but you can always change it later so long as you follow the necessary Android/Java guidelines. Once your host is created, you'll find it in the ant/untitled-host/ folder. Be aware that if you have not yet built the Moai library (in ant/libmoai/), make-host will warn you that the host is incomplete, in which case, you should go to the ant/libmoai/ folder and run the 'build' command, then return and re-run 'make-host' (See [ Building Moai From Source With Ant (Android)](../buildsdk/building-moai-libraries-for-android-ndk.html) for details). If you have built the Moai library at least once, 'make-host' will use that pre-compiled library unless you have made a configuration change warranting a re-compile, in which case it will perform a build.

NOTE: EVERY time the 'make-host' script is run, the contents of the ant/untitled-host/ folder is clobbered, so if you intend to make changes to the host (source- or config-level), you should copy or move the folder elsewhere on your computer or make the changes in the appropriate location in the ant/host-source/ folder to ensure that they are not lost the next time a host is created.

Running the Android Host
------------------------

To run the Android host, run the 'run-host' script inside the host folder. The first time you run the script, it will create a 'settings-local.sh' file for you and warn you that the file needs to be modified. Specifically, you need to specify the location of the Android SDK and the lua script(s) that you want to include in the host - by default, it will run a script from the available samples. You'll also notice a 'settings-global.sh' file that specifies host settings that are not environment-specific, such as the application name, version, icon resources, etc. that you may also want to modify eventually, unless you plan to do so manually later.

When you successfully run the 'run-host' script in your Android host, it will ingest the values in your settings files and create a legitimate Android project in the build/ folder. The root project resides in the build/project/ folder and any dependent libraries (Facebook, Tapjoy, etc.) are in sibling directories. The contents of build/project/ is a fully-fledged Android project and may be imported into Eclipse or other Android IDEs, though some additional IDE-specific steps may be necessary.

NOTE: EVERY time the 'run-host' script is run, the contents of the build/ folder is clobbered, so if you intend to make changes to the host (source-level), they should be made in the appropriate location under the host-source/ folder to ensure that they are respected the next time the host is run.

After creating the Android project, 'run-host' will compile it, generating an APK and attempt to run it on the default Android device, which can be an emulator or physical device - use 'adb devices' to view the currently recognized devices. At this point, the choice is up to you. You can either continue to use the provided scripts to create and/or run your hosts, or you can choose to import the generated Android project in the build/ folder into your favorite Android IDE and continue building your Moai game there.

NOTE: More details coming soon...
