COMMON = {
	[ 'src/moai-audio-sampler/host.h' ]		= true,
	[ 'src/moai-box2d/host.h' ]				= true,
	[ 'src/moai-core/host.h' ]				= true,
	[ 'src/moai-crypto/host.h' ]			= true,
	[ 'src/moai-http-client/host.h' ]		= true,
	[ 'src/moai-http-server/host.h' ]		= true,
	[ 'src/moai-luaext/host.h' ]			= true,
	[ 'src/moai-sim/host.h' ]				= true,
	[ 'src/moai-untz/host.h' ]				= true,
	[ 'src/moai-util/host.h' ]				= true,

	[ 'samples' ]							= true,
}

MAC = {

	[ 'bin/osx' ]									= true,

	[ 'lib/ios' ]									= true,
	[ 'lib/osx' ]									= true,

	[ 'ant/libmoai/obj/local/armeabi-v7a' ]			= 'lib/android/armeabi-v7a',

	[ 'src/host-sdl' ]								= true,
	[ 'src/moai-ios/host.h' ]						= true,

	[ 'xcode/ios-static/MoaiSample.xcodeproj' ]						= 'xcode/ios/MoaiSample.xcodeproj',
	[ 'xcode/ios-static/MoaiSample.xcodeproj/project.pbxproj' ]		= 'xcode/ios/MoaiSample.xcodeproj/project.pbxproj',
	[ 'xcode/ios-static/MoaiSample.xcodeproj/xcshareddata' ]		= 'xcode/ios/MoaiSample.xcodeproj/xcshareddata',

	[ 'xcode/osx-static/Moai.xcodeproj' ]							= 'xcode/osx/Moai.xcodeproj',
	[ 'xcode/osx-static/Moai.xcodeproj/project.pbxproj' ]			= 'xcode/osx/Moai.xcodeproj/project.pbxproj',
	[ 'xcode/osx-static/Moai.xcodeproj/xcshareddata' ]				= 'xcode/osx/Moai.xcodeproj/xcshareddata',
}

CLEAN_DIRS = {
	'lib/android/armeabi-v7a/objs'
}
