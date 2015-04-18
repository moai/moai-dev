---
title: "Building Moai From Source With Visual Studio"
order: 20
---

The Visual Studio solutions contain projects to build a suite of executables, the Moai core library and extensions, and sub-projects for the open source 3rd party libraries Moai depends on.

Open the solution for your version of Visual Studio and take a moment to look at the project structure. The open source projects have been assembled and included into the solution. This saves users the inconvenience of having to wrangle the build tools for each of these projects independently: everything you need to build them is right here at your fingertips.

![]({{ site.baseurl }}/img/docs/51r.png)

In the 'Libraries' folder you'll see the various libraries that make up Moai itself. The 'uslscore' and 'uslext' are utility libraries on which Moai is partially based. Don't worry about those for now. The 'moaiext-luaext' project is a set of Aku interfaces for initializing some of the additional Lua modules bundled with Moai. The 'moaiext-untz' project contains the Unntz extension.

In the 'Products' folder, you'll see 'moaicore' which contains the entire Moai framework and Lua bindings.

The 'Utilities' folder contains projects to build the various utility apps that ship with Moai.

To build the GLUT host and its dependencies, just right click on the 'Utilities/moai' project and choose 'build' from the context menu, which will compile and link several hundred files, resulting in a shiny new build of the GLUT host and the Moai libraries.

If you want to build 'moai-untz' you will have to install DirectX. The 'moai-untz' project makes no assumptions about where you've installed DirectX, so you will have to either add the search paths for DirectX includes and libraries to the project itself or add them globally to your install of Visual Studio. The latter method is recommended as this will not alter the project structure. A readme file is included in the vs20xx folder explaining exactly how to set up the DirectX paths. Please note that 'moai-untz' also builds a product called 'moai' and will overwrite (or be overwritten by) the product of 'moai'
