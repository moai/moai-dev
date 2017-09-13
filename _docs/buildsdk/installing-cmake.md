---
title: Installing CMake
order: -900
help_wanted: true
---

The easiest way to get cmake working is to download the zip/tar.gz version and then add the bin folder to your path.

### 1. Download CMake 

Download CMake from http://www.cmake.org/download

  * Windows - [http://www.cmake.org/files/v3.2/cmake-3.2.2-win32-x86.zip](http://www.cmake.org/files/v3.2/cmake-3.2.2-win32-x86.zip)
  * OSX - [http://www.cmake.org/files/v3.2/cmake-3.2.2-Darwin-x86_64.tar.gz](http://www.cmake.org/files/v3.2/cmake-3.2.2-Darwin-x86_64.tar.gz)
  * Linux - [http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-x86_64.tar.gz](http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-x86_64.tar.gz)
    
### 2. Extract to a folder somewhere. 

 Extract the downloaded package. 
  
  <div class="bs-callout bs-callout-default"><h4>Tip</h4>
 It can help to extract to a place where you can keep all your dependencies for MOAI building (`/moai-deps/cmake`) such as the android NDK, the emscripten SDK etc
  </div>
   
   * Linux : `tar xvf cmake-3.2.2-Linux-x86_64.tar.gz`
   * OSX : `tar xvf cmake-3.2.2-Darwin-x86_64.tar.gz`
   * Windows: `right click -> extract all`
   
### 3. Ensure that CMake is on the path
  
  To launch cmake during build it will need to be on your path.
  
  This can be done by either:
  
   * __AdHoc__ (will need to be run every time you start a new prompt)
     
     * Linux and OSX : `export PATH=<cmake-bin-path>:$PATH`
     * Windows: set PATH=<cmake-bin-path>;%PATH%
     
     
   * __Permanently__ (might affect other apps or conflict with existing tools)
   
     * Linux : add to your `~/.profile` or `~/.bash_profle` or `~/.bashrc` check with your linux vendor using same syntax as adhoc above
     * OSX: create or add to `~/.bash_profile` using same syntax as adhoc above
  
     
   * __Moai Environment Script__
   
     * Windows: edit `<SDK_ROOT>/bin/env-win.bat` and change the `set CMAKE_PATH=...` line to point to your cmake bin path.
     * OSX: edit `<SDK_ROOT>`/bin/env-osx.sh` and change the `CMAKE_PATH=...` line to point to your cmake bin path
     * Linux: edit `<SDK_ROOT>`/bin/env-linux.sh` and change the `CMAKE_PATH=...` line to point to your cmake bin path
     
  <div class="bs-callout bs-callout-warning"><h4>Caution</h4>The env scripts are very rough and could be improved, they currently don't have cmake support in them. Pull requests welcome
  </div>
  
### 4. Ready to roll

Just ensure that cmake is on the path before you call the `bin/build-xxxx` scripts and you should be right to go. Don't forget most of the build scripts have other dependencies.
   