---
title: Crash Course on CMAKE
out_of_date: true
---

Moai's CMake build system
======================

Crash Course on CMAKE
-------------------

#### The problem

What do vcxproj, xcodeproj, Makefile all have in common? They all tell the build system which files combine with which options to produce which outputs.

What they don't have in common is syntax, or portability or easy diff merges in a distributed development environment. If you have a project that builds on systems using all of the above formats, then every time you need to add a file as a dependency of an output, you will need to:
 * boot up windows and visual studio (once for each version you support)
 * boot up a macOSX box and xcode, (you have one of them too right?)
 * fire up vim and edit a makefile (you know that syntax too of course)

Wouldn't it be good if we could just specify the files and settings needed for each output and have it generate a makefile, xcodeproj and a vcxproj file for each major version of visual studio?

#### A Possible Solution

[CMake](http://www.cmake.org/) is one tool that claims to do this for us and prior to MOAI 1.5, it was already in use for the linux and blackberry builds, so it seemed like a logical choice to use CMake as a way to keep the project files in sync for other platforms.

#### The Basics

The CMake build system is a set of `CMakeList.txt` files in the folders under `cmake/` in the moai sdk. Each file defines:
 * a project
 * the files that make up that project
 * compiler settings for that project and any sub projects
 * variables for that project and any sub projects
 * the outputs of the project (executable, static lib or shared lib)

it uses these files to generate the correct makefiles, projects etc required to produce the specified outputs. CMake is a makefile/project generator the building of the final moai executable is still done by make, msbuild, xcodebuild etc.

#### A CMakeLists.txt File

Here is an example from the build system:

```CMake
cmake_minimum_required ( VERSION 2.8.5 )

project ( contrib )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )

set ( CONTRIB_INCLUDES
  "${MOAI_ROOT}/3rdparty/"
)

set ( CONTRIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.c
)

include_directories(${CONTRIB_INCLUDES})
add_library( contrib STATIC ${CONTRIB_SOURCES})
target_include_directories (contrib PUBLIC ${CONTRIB_INCLUDES})
set_target_properties( contrib PROPERTIES FOLDER Third-Party )
```

Not too scary, lets break it down:

```CMake
cmake_minimum_required ( VERSION 2.8.5 )
```
this line tells us the minimum required version of cmake needed to parse this file.

```CMake
project ( contrib )
```
this acts to group the files, settings and targets (outputs) together. In our build system it is usually named after the source folder it represents, in this case `contrib`.

```CMake
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )
```
this sets up a variable called CMAKE_CURRENT_SOURCE_DIR for later use. You have probably have noticed that all cmake commands are like function calls and follow the `<directive> ( <param> [<param>*] )` format. The `set` directive updates or creates a variable with the `<param>` as the value.

the `${MOAI_ROOT}` is a variable that is set in a parent project *(the root project in fact)* and contains the path to our checked out moai sdk. The variables name is MOAI_ROOT and we wrap it in `${}` so that it will output its value. `${MOAI_ROOT}/3rdparty/contrib/` expands to `/home/dave/moai-sdk/3rdparty/contrib` on my box.

```CMake
set ( CONTRIB_INCLUDES
  "${MOAI_ROOT}/3rdparty/"
)

set ( CONTRIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.c
)
```

this sets the `CONTRIBS_INCLUDES` variable to the string `${MOAI_ROOT}/3rdparty/`. and sets up the `CONTRIB_SOURCES` variable with our source files. Note that cmake separates parameters by spaces, and will grab multiple parameters like the source files above and treat them like a list.

Why set the variables (that's all they are, nothing special about them), well most projects in moai use the pattern
```CMake
set (xxxx_INCLUDES <include dirs>)
set (xxxx_SOURCES <source files>)
```
to collect all of the source files and all the include directories into a couple of variables to make the part where we define the output/target cleaner and easier to read.

```CMake
include_directories(${CONTRIB_INCLUDES})
```
Adds the directories in our `CONTRIB_INCLUDES` variable to the list of search paths for header files (-I for gcc)

```CMake
add_library( contrib STATIC ${CONTRIB_SOURCES})
```
Tells cmake to generate a static library from files listed in the `CONTRIB_SOURCES` variable. It will compile all source files and generate a `libcontrib.a` file (CMake shoves the lib in front by default). 

It is important to note here that the first parameter is the name of our target, we use this and not the project name (which is just for grouping settings) when we need to link to this library/target or update its settings.

```CMake
target_include_directories (contrib PUBLIC ${CONTRIB_INCLUDES})
```
This is a handy feature that says: any other targets that need to link with contrib, should have the directories in the `CONTRIB_INCLUDES` variable added to their search path. This means that if my code relies on libcontrib.a and has a header file located in deep in contrib's source directory, the header path for my project will be updated to include that path just by me specifying the dependency on contrib.

So anyone linking against libcontrib gets `CONTRIB_INCLUDES` header paths too.

```CMake
set_target_properties( contrib PROPERTIES FOLDER Third-Party )
```
This is not essential to the building of moai, but it helps the generated IDE projects (if used) to be more organised. It will place the contrib target inside a solution folder called Third-Party.

And thats it!.

We have everything cmake needs to generate a makefile, vcxproj, xcodeproj, CDT, etc for the contrib project.

#### Using the CMakeLists File

Is it usable like this? SURE!

if you go to the folder where this file is located and type 
```bash
cmake -DMOAI_ROOT=/home/david/moai-dev .
```

It will detect which build environment is best for your system (xcode, makefile or visualstudio) and generate a project to build contrib.a which you can then open in the ide or build using make

the `-DMOAI_ROOT=/home/david/moai-dev` is a way of defining a variable on the commandline for use in the script. It is the same as using `set ( MOAI_ROOT /home/david/moai-dev)` in the `CMakeLists.txt`. We need it because: 
```CMake
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )
```
expects it to exist so it can find our source.

**What about linking to it from another CMakeLists.txt?**

Easy! First CMake will need to know about the target. This is done by including the directory in which the CMakeLists.txt file is located, into your script. For example, if we were a `CMakeLists.txt` in `/` and contrib was located in `/contrib` then we could:

```CMake
   add_subdirectory(contrib)
```
This would add contrib to the list of project files generated, **but** would *not* define a relationship or dependency to our current target. To do that we need another command:

```CMake
target_link_libraries(my_executable contrib)
```
This will make sure `contrib` is built before `my_executable` and then included in the linking phase of my_executable. It will also ensure that the c files defined for my_executable have the path to the contrib headers included in their search paths.

#### What about settings?

The other core feature of CMake that is in heavy use throughout the moai project is the `CMAKE_C_FLAGS` and `CMAKE_CXX_FLAGS` variables. These are just normal variables except that their contents will be added after the compiler command so that:
```CMake
set (CMAKE_C_FLAGS "-m32")
```
when compiled for linux, would generate makefiles for this project (and all sub projects included via add_subdirectory) which compiled with the commandline `gcc -m32`.

If you want to add to the list of flags in a sub project for example, you would:
```CMake
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAnotherSetting=1")
```
this would generate the compiler command `gcc -m32 -DAnotherSetting=1` for the sub project, but keep in mind that it only takes effect for this project and sub projects, the parent project would still just generate `gcc -m32`

#### How it applies to MOAI 

The whole MOAI build system is built of these simple cmake commands. The `bin/build-linux-sdl.sh` just calls `cmake` with a bunch of variable definitions and points to the `CMakeLists.txt` file under `/cmake` which slurps up CMakeLists for each moai module and 3rdparty library using a bunch of `add_subdirectory()` commands and joins them all together with their dependencies using `target_link_libraries()`

Libraries and source files are included or excluded as dependancies based on the supplied command line variables such as `-DMOAI_UNTZ` and C defines are added to the command lines using `CMAKE_C_FLAGS` and its CPP equiv `CMAKE_CXX_FLAGS`

CMake has conditional functions that execute one set of definitions or another depending on the result which helps with handling platform differences eg (from the sdl host):
```CMake
if ( BUILD_WINDOWS )
  target_link_libraries ( moai 
    SDL2-static
    ${OPENGL_LIBRARIES}
    winmm
  )
endif (BUILD_WINDOWS)

if ( BUILD_LINUX )
  target_link_libraries ( moai 
    pthread 
    rt
  )
endif ( BUILD_LINUX )
```

which links the moai executable target against different libraries depending on platform (in this case sdl and wimm for windows and pthread and rt for linux). `BUILD_LINUX` and `BUILD_WINDOWS` are just variables we have included on the cmake command line in our build-xxx.sh files  `-DBUILD_LINUX=TRUE`

The CMakeLists files for moai are laid out in a folder structure that loosely follows the structure of the src. If you were to add a new third party library to moai called `libmoney` you would put the src files in `3rdParty/money` and create a `CMakeLists.txt` file under `/cmake/third-party/money`. You would add a `add_subdirectory(money)` call in the `CMakeLists.txt` located in `/cmake/third-party` and make sure to link it to the target you want by adding it to a `target_link_libraries(xxxx money)` call.

#### Go Forth and CMake Things

This was only a breif introduction, but should be enough to give you a general idea of how it all fits together and allow you to read and get the gist of the `CMakeLists.txt` files in the moai `cmake` folder. 

There is plenty in there to copy and paste from so when in doubt, find a similar library or use case and copy that.

Further cmake reading can be found at:  http://www.cmake.org/cmake/help/v2.8.12/cmake.html

where you should be able to find out what any of the unexplained commands in the cmake files do.










