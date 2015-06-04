
## Getting Started ##

The moai binary sdk is shipped as a set of precompiled libraries, a moai desktop binary and a set of project templates for each platform. 

The provided moai desktop binary `<sdk-path>/util/moai` will allow you to run many of the provided samples in the samples folder, just set a `MOAI_BIN` environment variable to point to `<sdk-path>\util` folder where the run.bat|sh file in each sample folder can find your desktop binary.


You should try to  structure your moai projects so that there is a source directory that contains your lua src and assets that are to be deployed. This would be a subfolder under your project directory which might contain other things such as templates, build scripts, config etc. e.g For a test project called TestProj, I would create a folder called `TestProj`, then under that another folder  `TestProj/src` where I would keep all my lua and assets.

## Building Hosts ##

While you can use the moai desktop binary provided to run your moai lua code, you will eventually want to build your own host. Building your own host is required to run and deploy your project on other platforms (android, ios, html) or to customize the desktop version with your own icon and customisations or in preparation for submission to an app store.

Managing and building these per platform hosts can be a headache and repeating the process for each project can get annoying. Thankfully this sdk provides a set of utility scripts to help with building and creating moai projects called `moaiutil`.

To use moaiutil you should first add `<sdk-path>\util` to your `PATH` environment variable (if you haven't already done so).

You should also ensure you have the requirements installed for the platforms you wish to support. See [Requirements](Requirements) section below.

### Creating Host Config ###

moaiutil uses a lua based config file `hostconfig.lua` to create host projects with the correct settings and correct lua src location. To create this config file for the first time on a project, ensure you are one level up from your lua src and run 
```bash
moaiutil host init
```
This will create the hostconfig.lua which contains default settings for each platform, and can be edited to setup the name, source location, icon etc for the project you wish to build.

### Creating a host project for a platform ###

Once there is a hostconfig.lua, you can create (or recreate) a host project for a particular platform by running 
```bash
moaiutil host create <host-name>
```

where `host-name` is the name of the host you want to create (normally named after the target plaform). For a list of supported host-names run 
```bash
moaiutil host list
```

The created host will be in a subfolder called `hosts\<host-name>` in your current project. 

### Customising the created host project ###

To customize the created host, you can just edit the files in `hosts\<host-name>` using either the ide (xcode, visual studio, android studio) for the project, or just with a text editor. 

A better option (when applicable) is to just update the `hostconfig.lua` with new values and run `moaiutil host create <host-name>` again. This will remove your old `hosts/<host-name>` folder and create a new host with new settings. The advantage of this method is that you can recreate your host projects after updating the moai sdk to get all the latest fixes or customisations you have made to the sdk.

### Building the created host projects ###

To build the created hosts you can launch the created project in the relevant ide (xcode, visual studio, android studio) or you can run 
```bash
moaiutil host build <host-name>
```
or 
```bash
moaiutil host run <host-name>
```

These scripts will create the host if it doesn't exist, then build it using the build.sh|bat script in the host directory. The run command will also call the run.bat|sh script in the host directory (if available) to launch the build application.
 
## Requirements ##
  
### Android
  
 * Oracle JDK 1.7  
   * http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html
   * set a `JAVA_HOME` environment variable that points to the jdk
   
   
 * Android Studio 1.0+ 
   * http://developer.android.com/tools/studio/index.html
   
   
 * Android NDK r9d 
   * Windows - http://dl.google.com/android/ndk/android-ndk-r9d-windows-x86.zip)
   * Mac - http://dl.google.com/android/ndk/android-ndk-r9d-darwin-x86_64.tar.bz2)
   * Linux - http://dl.google.com/android/ndk/android-ndk-r9d-linux-x86_64.tar.bz2)
   * Set an `ANDROID_NDK` environment variable that points to the unzipped path.
   * Don't try to use a later one, it may be incompatible with the pre-built binaries.
   
 
### Windows
 
 * Visual Studio 2013 (community edition or higher) 
   * http://www.visualstudio.com/products/visual-studio-community-vs
   
   
### Linux

 * CMake 3.0.0+
 
 
### iOS and OSX
 
 * XCode 6+
  
  

 
 
 