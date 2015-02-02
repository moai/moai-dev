# HTML5 Host

## Building the Moai SDK

### On Linux

To build this host you will need to have installed Emscripten by following the [Emscripten tutorial](http://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html). This includes installing llvm and its clang frontend.

Once you are happy with your emscripten setup, have configured your .emscripten file, and ensured it is all working
you can run from the root of the Moai SDK
```
EMSCRIPTEN_HOME=~/em/emscripten bin/build-html
```
(Replace the `~/em/emscripten` with the path to your emscripten install.)

Ignore the warnings about duplicate variables, things defined by absolute path and `$_strong0`.

### On Windows

 * Install [CMake](http://www.cmake.org/) 2.8.11 or later. Add CMake's bin directory to your PATH variable.
 * Install [TDM-GCC](http://tdm-gcc.tdragon.net/) (that includes mingw32-make).
 * Install [Emscripten](http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html#sdk-download-and-install). **Make sure the installation path does not contain spaces!**
 * Execute `build-html.bat` in the `bin` directory of your Moai repository.

## Building the Host

You should now have a folder `lib/html` which contains the generated file `moaijs.js`. Read the `README.md` file in `host-templates/html` to find out how to incorporate this file into the host template.

## Limitations of the HTML5 Host

The HTML5 host has some limitations:

 * It can't do threads (it can do coroutines though). This shouldn't be a big problem at the moment
 * It currently doesn't set the document directory. Instead it will allow you to write anywhere to the filesystem and each change will be saved to local storage and restored upon page reload.
 * Texture loading is not optimized but fast enough. Most people do this during the load phase anyway
 * Untz has been wrapped around html audio api and as a result, only sound formats supported by the browser running the host will work
   see http://en.wikipedia.org/wiki/HTML5_Audio
 * IE does not seem to have webgl support yet. There is a plugin available however that enables it.
 * iOS has no webgl, android chrome does (enable it in chrome://settings)
 * Textures might be optimized in the future so try to use canvas compatible formats (png,jpg)
 * Current host does not have lua_ext (no sockets, sql, curl etc).
 
