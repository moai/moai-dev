COMMON = {
	
	[ '3rdparty/LICENSE' ]						= true,

	[ '3rdparty/sdl2-2.0.0/COPYING.txt' ]		= true,
	[ '3rdparty/sdl2-2.0.0/CREDITS.txt' ]		= true,
	[ '3rdparty/sdl2-2.0.0/include' ]			= true,

	[ 'src/host-modules' ]						= true,
	[ 'src/host-sdl' ]							= true,

	[ 'src/lua-headers/moai_lua.h' ]			= true,
	[ 'src/lua-headers/moai_test_mgr_lua.h' ]	= true,

	[ 'src/moai-audio-sampler/host.h' ]			= true,
	[ 'src/moai-box2d/host.h' ]					= true,
	[ 'src/moai-core/host.h' ]					= true,
	[ 'src/moai-crypto/host.h' ]				= true,
	[ 'src/moai-http-client/host.h' ]			= true,
	[ 'src/moai-http-server/host.h' ]			= true,
	[ 'src/moai-luaext/host.h' ]				= true,
	[ 'src/moai-sdl/host.h' ]					= true,
	[ 'src/moai-sim/host.h' ]					= true,
	[ 'src/moai-sim/MOAIKeyCodeEnum.h' ]		= true,
	[ 'src/moai-untz/host.h' ]					= true,
	[ 'src/moai-util/host.h' ]					= true,

	[ 'src/zl-common' ]							= true,

	[ 'samples' ]								= true,
}

MAC = {

	[ 'src/host-ios' ]								= true,
	
	[ 'src/moai-apple/host.h' ]						= true,

	[ 'src/moai-ios/host.h' ]						= true,
	[ 'src/moai-ios-billing/host.h' ]				= true,
	[ 'src/moai-ios-facebook/host.h' ]				= true,
	[ 'src/moai-ios-gamecenter/host.h' ]			= true,
	[ 'src/moai-ios-motion/host.h' ]				= true,
	[ 'src/moai-ios-movie/host.h' ]					= true,

	[ 'bin/osx' ]									= true,

	[ 'lib/ios' ]									= true,
	[ 'lib/osx' ]									= true,

	[ 'ant/libmoai/obj/local/armeabi-v7a' ]			= 'lib/android/armeabi-v7a',

	[ 'xcode/resources' ]							= true,

	[ 'xcode/ios-static/MoaiSample.xcodeproj' ]						= 'xcode/ios/MoaiSample.xcodeproj',
	[ 'xcode/ios-static/MoaiSample.xcodeproj/project.pbxproj' ]		= 'xcode/ios/MoaiSample.xcodeproj/project.pbxproj',
	[ 'xcode/ios-static/MoaiSample.xcodeproj/xcshareddata' ]		= 'xcode/ios/MoaiSample.xcodeproj/xcshareddata',
	[ 'xcode/ios-static/Info.plist' ]								= 'xcode/ios/Info.plist',
	[ 'xcode/ios-static/main.lua' ]									= 'xcode/ios/main.lua',

	[ 'xcode/osx-static/Moai.xcodeproj' ]							= 'xcode/osx/Moai.xcodeproj',
	[ 'xcode/osx-static/Moai.xcodeproj/project.pbxproj' ]			= 'xcode/osx/Moai.xcodeproj/project.pbxproj',
	[ 'xcode/osx-static/Moai.xcodeproj/xcshareddata' ]				= 'xcode/osx/Moai.xcodeproj/xcshareddata',
}

CLEAN_DIRS = {
	'lib/android/armeabi-v7a/objs'
}
