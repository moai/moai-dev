# Building Libmoai on Windows

The following are the instructions for building the libs for all windows compatible hosts. These instructions are for the static libs only. For building the Hosts see the next section [Building Moai Hosts on Windows](#building-moai-hosts-on-windows).

## Building Visual Studio 2013 Lib (using cmake)

### Prerequisites
 - [CMake 2.8.11+](http://www.cmake.org/files/v3.1/cmake-3.1.0-win32-x86.exe)
 - [Visual Studio 2013 Community Edition](http://www.visualstudio.com/en-us/news/vs2013-community-vs.aspx)

### Build
Edit the bin\env-win.bat file and change the CMAKE_PATH to point to your cmake install (you can ignore the other settings for now). 

From the SDK folder, run `bin\env-win.bat` to set the paths

From the SDK folder, run `bin\build-windows.bat vs2013`. This will create a `lib\windows\vs2013\Distribute` folder inside your SDK folder which contains the includes and the libs needed to use libmoai in a custom host. It also includes a bin folder with the compiled bundled SDL host. This can be used to run any of the samples. This also adds moai.exe to the SDK_PATH/util folder in order for moaiutil scripts to run.

## Building Android Lib (using cmake)

### Prerequisites
  - [CMake 2.8.11+](http://www.cmake.org/files/v3.1/cmake-3.1.0-win32-x86.exe) 
  - [Android NDK r9d](http://dl.google.com/android/ndk/android-ndk-r9d-windows-x86.zip)

### Build  
Edit the `bin\env-win.bat` file and change the `CMAKE_PATH` to point to your cmake install and the `NDK_PATH` to point to your extracted NDK (you can ignore the other settings for now). 

From the SDK folder, run `bin\env-win.bat` to set the paths

From the SDK folder, run `bin\build-android.bat`. This will create a `lib\android` folder in your SDK folder which contains the libs needed to create a custom android host. It has builds for arm, armv7, and x86.

## Building HTML Lib

### Prerequisites
- [CMake 2.8.11+](http://www.cmake.org/files/v3.1/cmake-3.1.0-win32-x86.exe) 
- [Mingw64 TDM](http://tdm-gcc.tdragon.net/)
- [Emscripten portable-sdk 1.25](https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-1.25.0-portable-64bit.zip)
  - after extracting from the sdk folder run 
    - `emsdk update` 
    - then `emsdk install latest` 
    - then `emsdk activate latest`

### Build
Edit the `bin\env-win.bat` file and set the values of `CMAKE_PATH`, `EMSDK_PATH`,`MINGW_PATH`.

From the SDK folder, run `bin\env-win.bat` to set the paths

From the SDK folder, run `bin\build-html.bat`. This will create the `lib\html\moaijs.js` file which contains libmoai as a emscripten compiled js file.

# Building Moai Hosts on Windows

A Moai host is a project that links against the moai libraries. There is usually one host per project and setting up a host may involve pointing it to your lua source, editing the author and product names, changing the icon, and linking against different host modules. 

To generate a host for your lua project, you use `moaiutil.bat` found in the util folder. `moaiutil.bat` requires a recent copy of moai.exe either in the util path or on your system path.

Hosts will be generated under the `SDK_ROOT\hosts` folder by default but this can be changed by passing `-o [output dir]` .

Hosts will also take a copy of the compiled libs they need from `SDK_ROOT\lib\[platform]` folder. The lib source can be changed with `-l [lib dir]`. Instead of a copy, a symlink to the lib can be created instead with `-s`. This only works if the moaiutil script is run in an administrator cmd window. This is useful if you are updating your libs and want all of your projects to get the update (instead of manually copying the libs to each project after building).

Each host includes a `readme.md` file in the root of the host output that describes what steps are needed to customize and build the host.

### Visual Studio 2013 Host

Generates a vs2013 solution that contains the sdl host sources and is preconfigured with a resource file for versioning and branding and an application and taskbar icon.

`moaiutil host-windows-vs2013`  generates a host under `SDK_ROOT\hosts\windows\vs2013`

`moaiutil host-windows-vs2013 -s -l c:\moai\1.5-stable\lib\windows\vs2013 -o c:\projects\mygame\hosts\vs2013` generates a host using a symlink to the libs in `c:\moai\1.5-stable\lib\windows\vs2013` and places it at `c:\projects\mygame\hosts\vs2013`

### Android Studio Host

Generates a android studio (Google's new default IDE for android projects) compatible gradle project.

`moaiutil host-android-gradle` generates a host under `SDK_ROOT\hosts\android-studio`

### HTML/JS Host

Generates a html site template containing a moai player that will play your moai lua in the browser.

`moaiutil host-html` generates a host under `SDK_ROOT\hosts\html`


# Building all Windows compatible Libs and Hosts

If you set all the variables in the top section of the `bin\env-win.bat` file, then after running `bin\env-win.bat` you can run `bin\build-all-windows.bat` to build all moai libs (vs2013, android, html) and all hosts (vs2013,android-gradle,html) with one command. This might be handy after pulling in some mods to your SDK folder.
