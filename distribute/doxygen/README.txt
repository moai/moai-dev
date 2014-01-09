This file explains how to generate the HTML documentation for the Moai SDK from source. This is useful if you've made changes to Moai's C++ source code and would like an updated Lua class reference.

Windows
- Install Perl (ActivePerl is available for Windows).
- Install Graphviz. Doxygen uses Graphviz to create the class diagrams.
- Install Doxygen.
- Make sure your PATH environment variable contains the bin directories of Perl, Graphviz, and Doxygen.
- Run make-docs-lua.bat