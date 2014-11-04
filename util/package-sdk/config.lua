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
}

MAC = {

	[ 'bin/ios' ]							= true,
	[ 'bin/osx' ]							= true,

	[ 'src/host-sdl' ]						= true,
	[ 'src/moai-ios/host.h' ]				= true,

	[ 'xcode/osx-static/Moai.xcodeproj' ]						= 'xcode/osx/Moai.xcodeproj',
	[ 'xcode/osx-static/Moai.xcodeproj/project.pbxproj' ]		= 'xcode/osx/Moai.xcodeproj/project.pbxproj',
	[ 'xcode/osx-static/Moai.xcodeproj/xcshareddata' ]			= 'xcode/osx/Moai.xcodeproj/xcshareddata',
}
