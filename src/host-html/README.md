##Building the Html Host

The html host has a CMake file under the CMake folder. To build this host you will need to have installed emscripten
by following the tutorial at https://github.com/kripken/emscripten/wiki/Tutorial. This includes installing llvm and
its clang frontend.

Once you are happy with your emscripten setup, have configured your .emscripten file, and ensured it is all working,
you will need to edit the CMAKE file `cmake/CMakeLists.txt` and change the value of the `EMSCRIPTEN_TOOLS` variable 
to point to the path of your emscripten install.

You can then use the `cmake` command to generate makefiles and then execute `make host-html-template`.

> Ignore the warnings about duplicate variables, things defined by absolute path and $_strong0.
> You should now have a folder in your output path called host-template. This can be copied to your project source and 
> be the basis of your html host.

Follow the directions in the readme in the host-template folder for instructions on bundling your assets for deployment 
and testing.

