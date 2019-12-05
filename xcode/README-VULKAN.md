Add moai-dev/xcode/frameworks/
Add a symbolic link to the OSX Vulkan SDK called "Vulkan"
You may need to re-add the Vulkan framework for linking

set VULKAN_SDK in /.MacOSX/environment.plist
then, from the command line:
defaults write com.apple.dt.Xcode UseSanitizedBuildSystemEnvironment -bool NO

useful:
https://github.com/hschmidt/EnvPane