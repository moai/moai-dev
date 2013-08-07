##Building the Html Host

To build this host you will need to have installed emscripten
by following the tutorial at https://github.com/kripken/emscripten/wiki/Tutorial. This includes installing llvm and
its clang frontend.

Once you are happy with your emscripten setup, have configured your .emscripten file, and ensured it is all working
you can run from the root of the sdk
```
EMSCRIPTEN_HOME=~/em/emscripten bin/build-html
```
replace the `~/em/emscripten` with the path to your emscripten install

> Ignore the warnings about duplicate variables, things defined by absolute path and $_strong0.
> You should now have a folder in `cmake/build/host-html` called host-template. This can be copied to your project source and 
> be the basis of your html host.

Follow the directions in the readme in the host-template folder for instructions on bundling your assets for deployment 
and testing.

##Limitations of the HTML host

The html host has some limitations:

 * It can't do threads (it can do coroutines though). This shouldn't be a big problem at the moment
 * It currently doesn't set the document directory. Instead it will allow you to write anywhere to the filesystem and each change will be saved to local storage and restored upon page reload.
 * Texture loading is not optimized but fast enough. Most people do this during the load phase anyway
 * Untz has been wrapped around html audio api and as a result, only sound formats supported by the browser running the host will work
   see http://en.wikipedia.org/wiki/HTML5_Audio
 * IE does not seem to have webgl support yet. There is a plugin available however that enables it.
 * iOS has no webgl, android chrome does (enable it in chrome://settings)
 * Textures might be optimized in the future so try to use canvas compatible formats (png,jpg)
 * Current host does not have lua_ext (no sockets, sql, curl etc).
 
