---
title: "Moai Project Setup"
order: -750
---

This article presents an overview of Moai's project structure and describes some typical ways to use the Moai framework from a development environment and project organization standpoint. To proceed, you'll need a good Lua text editor and an IDE. Most Moai developers uses Xcode, Visual Studio and Eclipse, but it is certainly possible to use an alternate IDE.

An application consists of a compiled executable along with a set of data resources (usually graphics and sound). The exact terminology and method for bundling the executable with its resources varies from platform to platform, but the basic concept is the same.

A Moai executable has two parts: the Moai framework and the host. The Moai framework is the body of C++ code officially maintained and supported. The host is a custom application that (at minimum) sets up a platform specific environment for the framework to run in, including an OpenGL surface to render to. The host also exposes any available input devices to the framework. The host is usually written in the language of choice for a given platform: Objective-C on iPhone, C, C++ or C\# on Windows, C or C++ on Linux and Java on Android.

In the Moai source tree, sample implementations are provided for iPhone, Android and GLUT. GLUT is a minimalist, open source, cross platform UI framework for writing OpenGL sample programs. As useful as it is for writing samples, GLUT is probably not the correct choice for writing a commercial desktop game. You should instead write a native host using a cross platform library such as [Qt](http://qt.nokia.com/products/)

The Moai framework, when initialized by the host, will create a Lua runtime and can be passed Lua scripts. Thanks to Moai's Lua bindings, these scripts can create and manipulate any of the Moai objects a game designer might care about.

As mentioned above, an application contains both the executable (Moai framework and host) and data resources. Because Lua scripts can be loaded and executed at runtime (without the need for recompilation), the Lua scripts that make up a game's logic are also data resources – just like images and sounds.

Loading Lua scripts at runtime is a great help during development, and may even be suitable for finished games under certain circumstances. That said, these scripts will exist in the game's resource folder as plaintext, readable by anyone. In some cases this won't matter, but in other cases you may wish to obfuscate these resources or bundle them into the project's executable itself; some options for doing this are discussed in another article.

There are four basic approaches to using Moai, depending on the user's skill set and goals:

**Game Designer (Lua only):** In this case, you doesn't care about anything under the hood. You can use Moai as an 'off the shelf' solution that will allow you to begin scripting and playing games immediately without any need for Objective-C, C++ or Java.

**Game Designer and Application Developer (Lua and a custom host):** This is an approach more suitable for commercial games. While it's certainly possible to use Moai with one of the supplied sample hosts, there may be platform specific features required for a particular game that the sample hosts do not provide. For example, video capture or access to a camera roll are features that you might want to add to a host and then expose through Lua. While new features like these might be added to Moai in the future, individual users also have the freedom to roll up their sleeves and add such functionality without having to dig into the guts of Moai.

**Game Designer and Game Engine Developer (Lua, C++ and a custom host):** Experienced game developers may recognize Moai's utility as a game library but also wish to extend it with their own objects and algorithms. This can be done using Moai's Lua to C++ binding system, or by injecting a custom binding system into the Lua runtime; it doesn't matter to Moai. This approach is recommended as an optimization; most of the studios using Moai now either implement their whole games in Lua or prototype in Lua and then move performance critical sections of their code to C++ after carefully profiling on the device – and taking all other measures to optimize Lua first.

**Game Engine Developer (C++ only):** This option is mentioned here only because it's possible. Even without Lua, Moai is a richly featured 2D game development framework. All of the Moai objects can be instantiated and manipulated directly in C++ without ever binding to the Lua runtime. That said, this isn't the intended use of Moai and so there are some challenges to doing this. The first is that almost all of the Moai documentation efforts are focused around the Lua binding, as most developers will be using Lua. The second is that many of the Moai objects do not offer parity between the Lua API and the C++ API – the Lua API calls are implemented as private static class methods and are not suitable for calling without a valid Lua runtime. If you are determined to use Moai directly from C++, this is easy enough to correct, but will require digging into and refactoring portions of the C++ APIs. (Anyone who decides to do this is encouraged to contribute these changes to the project).

The Development Environment
---------------------------

Developing with Moai requires a decent Lua text editor and an IDE that can build for your platform of choice. How much each of these is used depends on your approach.

**Lua only:** For this approach the only requirement is a Moai executable (or binaries and sample IDE projects if targeting mobile). On Windows and OSX you can use the pre-built host that comes with the Moai SDK. A Linux host will be available in the near future as well. As of this writing, on iPhone and Android you will still have to open an IDE project, add your script and content resources and build your application. This process is explained in more detail later in a separate article.

**Lua and a custom host:** This approach involves building with an IDE and linking to MOAI as a static library. Static libraries for each platform along with sample host projects are included in the Moai SDK.

**C++:** To use Moai as a C++ library, you could link to one of the pre-built binaries and simply include the full SDK header files from the open source code tree, but you're better off having access to the full Moai source and build. If for some reason you can't use one of the existing IDE projects, you can certainly port the code base to the IDE of your choice. If you do this, you may wish to pre-build (or set up your own build) for the 3rd party libraries Moai relies on.

Developers coming to Moai from an authoring environment like Flash, Game Maker or Corona may find themselves looking for a Lua IDE and supporting tools such as a Lua debugger and a Moai 'player' or device simulator. While there aren't official tools for these functions integrated into the Moai platform, there are two integrated Lua IDEs for Moai under development, with built in simulation and debugging capabilities. In addition, a standalone application based on Mono will appear as a sister project under an open source license. In the meantime, working without these higher level tools may turn out to be less onerous than you might think; remember that there are a number of commercial games already being developed for Moai by designers using nothing more complex than Notepad++.
