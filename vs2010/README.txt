----------------------------------------------------------------------------------------------------------------------------
To build moaiext-untz:

Visual Studio 2010
1) Install the latest Direct X SDK
2) Click on the menu command View->Other Windows->Property Manager
3) In Property Manager, open box2d->Debug | Win32->Microsoft.Cpp.Win32.user
4) In the resulting pop-up window, click Common Properties->VC++ Directories
5) Edit the value of Include Directories to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
6) Edit the value of Library Directories to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86

----------------------------------------------------------------------------------------------------------------------------
To build moaiext-fmod:

Visual Studio 2010
1) Install the latest Direct X SDK
2) Click on the menu command View->Other Windows->Property Manager
3) In Property Manager, open box2d->Debug | Win32->Microsoft.Cpp.Win32.user
4) In the resulting pop-up window, click Common Properties->VC++ Directories
5) Edit the value of Include Directories to include C:\Program Files\FMOD SoundSystem\FMOD Programmers API Win32\api\inc
6) Edit the value of Library Directories to include C:\Program Files\FMOD SoundSystem\FMOD Programmers API Win32\api\lib

----------------------------------------------------------------------------------------------------------------------------
March 1, 2011

There is apparently a bug in vs2010 requiring a patch:

http://connect.microsoft.com/VisualStudio/feedback/details/638534/unresolved-externals-when-build-a-vc-project-with-chained-static-lib-dependencies

"When we are trying to build exe or dll projects that reference a static lib project that references another static lib project, building the solution will fail to link the exe project with unresolved externals that should be resolved by linking the lowest level dependency.
For example, ExeProject has a project reference on LibA. LibA has a project reference on LibB. Linking will fail with missing symbols from LibB.
Rebuilding the solution will succeed."

And here's the patch:

Open %ProgramsFile%\MSBuild\Microsoft.cpp\v4.0\Microsoft.CPPBuild.Targets. Look for the line below:

<Target Name="GetResolvedLinkLibs" Returns="@(LibFullPath)" DependsOnTargets="$(CommonBuildOnlyTargets)">

Add ResolvedLinkLib to the DependsOnTargets like this:

<Target Name="GetResolvedLinkLibs" Returns="@(LibFullPath)" DependsOnTargets="$(CommonBuildOnlyTargets);ResolvedLinkLib">