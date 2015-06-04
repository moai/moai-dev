COMMON = {
	['lib'] = true,
	['util'] = true,
	['samples'] = true,
	['src/host-modules'] = true,
	
	
	--android host files are common to all
	
	['host-templates/android'] = true,
	['host-templates/html'] = true,
	['src/moai-android/moai.cpp'] = true,
	['src/moai-android/MoaiActivity.java'] = true,
	['src/moai-android/MoaiView.java'] = true,
	
	
	--sdl host files are common to all
	['src/host-sdl'] = true,
}


MAC = {
	--ios
	['src/host-ios'] = true,
	['host-templates/ios'] = true,
	['host-templates/osx'] = true,
	['lib/osx/bin/moai'] = 'util/moai'
}

WINDOWS = {
	['host-templates/windows/vs2013'] = true,
  ['lib/windows/vs2013/Distribute/bin/moai.exe'] = 'util/moai.exe'
}

LINUX = {
	['host-templates/linux'] = true,
	['lib/linux/bin/moai'] = 'util/moai'
}


CLEAN_DIRS = {
	
}
