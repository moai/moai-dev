---
title: "Setup Moai SDK on Windows using Sublime2"
---

At the end of this guide you'll be able to test your Moai SDK scripts within seconds on desktop as well as deploying them to your Android device for testing purposes. For deploying to Android we use a community developed tool called MoaiNao. It essentially is a Moai SDK player that allows you to quickly test your scripts without having to compile the Android project yourself.

[Sublime Text 2](http://www.sublimetext.com/2) offers a great and very efficient work environment for Moai SDK. Most of the techniques described here can easily be adapted to any other editor or simply using batch files directly.

Please note, this guide has been written with Windows 7 in mind. However adapting this to Mac and other versions of Windows shouldn't be a problem. It also uses the environment variables on Windows. If you do not like that you can easily change some of the steps to accommodate for that.

Big thanks to thebullno1 who developed MoaiNao.

Download and install Sublime2
-----------------------------

Head over to the [sublime website](http://www.sublimetext.com/2) and download the editor. Have a look around the editor and play with the different color schemes under Preferences -\> Color Schemes. You can also turn on folder view as a sidebar under View -\> Sidebar -\> Show Sidebar.

Download the Android SDK
------------------------

*(If you already have Android SDK installed you can skip this step)*

Download the Android SDK from here: [1](http://developer.android.com/sdk/index.html). It's easiest if you download the installer, but any of the SDK versions will work fine. Install the SDK to a folder of your choice. For simplicity's sake let's just install it to "C:\\android-sdk". After the installation process, it'll automatically bring up the SDK Manager. You need to install at least the Android SDK tools and the platform tools.

Once that is done, we need to add the Android SDK platform tools to our PATH environment variable. In Windows 7 this is done by going to Control Panel -\> System -\> Advanced System Settings. At the bottom of the window that pops up click "Environment Variables". Under "System" (the bottom list) find the "Path" variable and edit it. Add this to the end of the string (don't replace it):

```
;C:\android-sdk\platform-tools
```

Download the Moai SDK
---------------------

Login [here](https://dashboard.moaicloud.com/signup.php) and download the SDK. Again this is not an installer, just a zip file. Extract the zip file to your location of choice. For this guide let's use "C:\\moai-sdk".

Let's quickly examine the SDK folder structure:

```
/3dparty (contains glut libs)
/bin (this folder contains all the hosts, i.e what will run your lua scripts)
/docs (the API reference guide)
/hosts 
/include (lua includes)
/samples (the sdk samples)
```

Before we go any further, you should go into the samples folder and run some of the samples. This will work without any extra setup. Just go into any sample's folder and double click the run.bat file and it'll execute the sample. Familiarize yourself with all the different functionalities the SDK has to offer. There are a lot of samples!

Next we need to add the SDK to the "Path" environment variable. Follow the instructions from step 2 and add the following to the end of the string:

```
;C:\moai-sdk\bin\win32
```

Install 7zip
------------

This is needed for the Android deployment and can be downloaded from [here](http://www.7-zip.org/). Just run the installer. By default it'll install to "C:\\Program Files (x86)\\7-Zip".

Go ahead and also add this folder to the "Path" enviroment variable.

```
;C:\Program Files (x86)\7-Zip
```

Download the project template
-----------------------------

[This zip](http://www.thegreystudios.com/moai/project-template.zip) file gives you a clean basic project that you can use to work off. It has everything ready setup for deploying to Android (using MoaiNao). Of course you can also use this to test and develop on desktop.

Extract the zip file to a folder of your choice. Have a quick look at the extracted folder structure. You should have a folder structure like this:

```
/main
../assets (some test asset)
../main.lua
../config.lua
/tools(various batch files and txt files in here)
```

Get the project running on Desktop
----------------------------------

Now the fun can begin. Open Sublime2 and go to File -\> Open Folder. Navigate to the location you just extracted the template to and open up the root folder (the folder that has main & tools in it).

This will open up the project template in Sublime2. You should have the above folder structure visible on the left now (make sure you have the sidebar visible, View -\> Sidebar -\> Show Sidebar).

The next thing we need to do is define a build system for Moai SDK. You only have to do this the first time. This will be saved with your Sublime2 install so all subsequent projects you create will have it already.

Go to Tools -\> Build System -\> New Build System .. This will open up a new file. Replace the text with the following:

```
{
       "cmd": ["${project_path:${folder}}/tools/run.bat"]
}
```

Press save and specify an appropriate name for the build, i.e Moai-Desktop. Verify that the new build system has been selected by going to Tools -\> Build System -\> (x) Moai-Desktop.

Now navigate to main.lua via the sidebar and open it by double clicking. You can now either start this by going to Tools -\> Build or simply use Ctrl+B. This will start the script running in the windows host. If everything has worked you should see the moai logo spinning in front of you.

Get the project running on Android
----------------------------------

First, download MoaiNoa from here: [<http://www.host-a.net/u/bullno1/moainao.zip>](http://www.host-a.net/u/bullno1/moainao.zip)

The only thing we care about in this guide is the MoaiNao-debug.apk. Move the apk over to your Android device (via adb, or dropbox, gdrive) and install the apk.

Next, we need to connect the device to the PC via adb (Android Developer Bridge). You can do this via USB or wifi. If you have never done it before you can find some more information here: [<http://developer.android.com/guide/developing/device.html#setting-up>](http://developer.android.com/guide/developing/device.html#setting-up)

If you have a rooted device a fast and convenient choice is adbWireless which allows a wifi connection to adb.

Assuming you have a connection setup we're going to create another build system specifically for Android. Same process as for Desktop, Tools -\> Build System -\> New Build System

Replace the text with this:

```
{
       "cmd": ["${project_path:${folder}}/tools/deploy-to-device.bat"]
}
```

Press save and specify a name for the build system, i.e. Moai-Android. Verify that it's the current build system via Tools -\> Build System -\> (x) Moai-Android.

Navigate to main.lua and press Ctrl+B or Tools-\>Build. This should now automatically deploy your script to Android and run it on your device.
