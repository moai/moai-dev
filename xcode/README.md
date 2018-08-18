* __libmoai__ - Project to build all moai static libraries (ios and osx). The 'build all' target copies them to the 'moai-dev/lib/<os>/' folder.
* __ios__ - References the __libmoai__ project. Builds the iOS host.
* __ios-static__ - References the static libraries. Builds the iOS host. Just used to test static library linking and to ship with the binary SDK release.
* __osx__ - References the __libmoai__ project. Builds the OSX host.
* __osx-static__ - References the static libraries. Builds the OSX host. Just used to test static library linking and to ship with the binary SDK release.
* __moai.xcworkspace__ - Contains all osx/ios projects. Used by the command line build scripts invoked by the 'pito build' command. (Scripts are located at moai-dev/pito/build).
* __resources__ - Common resources for the iOS projects.
