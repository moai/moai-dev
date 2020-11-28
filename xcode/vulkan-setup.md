This is a quick-and-dirty dev setup for the command line build of Moai with Vulkan.

I haven't found a way to set a path for the Vulkan loader at runtime, and the loader uses environment variables to find its junk. We could set those from an xcode scheme, but there are some issues with that. instead, we set them into preprocessor variables as absolute paths and then export them to the environment from inside of moai. which is gross, but makes it a bit easier to configure locally. for production, obviously, find another way.

- download the Vulkan SDK for mac (this includes MoltenVK).
- extract it to moai-dev/xcode/vulkan/vulkansdk-macos-<version>.
- if the version has changed, you'll.
- "vulkan.framework" has to be in the target's "link binary..." build phase.
- if the vulkan SDK version has changed, you may need to re-add the framework to xcode.
- copy moai-dev/xcode/libmoai/moaiconfig.xcconfig.example to moai-dev/xcode/libmoai/moaiconfig.xcconfig.
- update the environment variables to absolute paths.
- make sure the loader is somewhere moai can find it. since we're building for the command line, copy the vulkan.framework to the "Frameworks" build folder.
