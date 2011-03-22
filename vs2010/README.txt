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