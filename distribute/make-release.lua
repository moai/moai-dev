-- 3rdparty
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../3rdparty' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/3rdparty' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'glut-3.7.6-bin',            to .. 'glut-3.7.6' )

-- bin/android
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../ant/libmoai/libs' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/bin/android' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )

-- bin/chrome
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../scons')
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/bin/chrome' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'moai_x86_32.nexe',		to .. 'moai_x86_32.nexe' )
MOAIFileSystem.copy ( from .. 'moai_x86_64.nexe',		to .. 'moai_x86_64.nexe' )
MOAIFileSystem.copy ( from .. 'moai.nmf',				to .. 'moai.nmf' )

-- bin/ios
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '/tmp/moai-dev-xcode-libmoai/ios/Release')
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/bin/ios' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )

-- bin/osx
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '/tmp/moai-dev-xcode-osx/osx/MoaiSample/moai/macosx/Release' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/bin/osx' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )

-- bin/win32
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../vs2008/bin/Win32/Release' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/bin/win32' )
MOAIFileSystem.affirmPath ( to .. 'vs2008' )
MOAIFileSystem.affirmPath ( to .. 'vs2010' )
if MOAIFileSystem.checkPathExists ( from ) then
	MOAIFileSystem.copy ( from .. 'moai.exe',				to .. 'moai.exe' )
	MOAIFileSystem.copy ( from .. 'moai-test.exe',			to .. 'moai-test.exe' )
	MOAIFileSystem.copy ( from .. 'glut32.dll',				to .. 'glut32.dll' )
	MOAIFileSystem.copy ( from .. 'moaicore.lib',			to .. 'vs2008/moaicore.lib' )
	MOAIFileSystem.copy ( from .. 'moaiext-luaext.lib',		to .. 'vs2008/moaiext-luaext.lib' )
	MOAIFileSystem.copy ( from .. 'moaiext-untz.lib',		to .. 'vs2008/moaiext-untz.lib' )
end
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../vs2010/bin/Win32/Release' )
if MOAIFileSystem.checkPathExists ( from ) then
	MOAIFileSystem.copy ( from .. 'moaicore.lib',			to .. 'vs2010/moaicore.lib' )
	MOAIFileSystem.copy ( from .. 'moaiext-luaext.lib',		to .. 'vs2010/moaiext-luaext.lib' )
	MOAIFileSystem.copy ( from .. 'moaiext-untz.lib',		to .. 'vs2010/moaiext-untz.lib' )
end

-- docs
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../docs' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/docs' )
MOAIFileSystem.copy ( from, to )
from = MOAIFileSystem.getAbsoluteDirectoryPath ( 'doxygen/html-lua' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/docs/html-lua' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )

-- hosts/ant
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../ant/untitled-host' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/ant' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )
--MOAIFileSystem.deleteDirectory ( to .. 'external', true )
--MOAIFileSystem.deleteDirectory ( to .. 'src', true )
--MOAIFileSystem.copy ( from .. 'src/app',				to .. 'src/com/getmoai/samples' )
--MOAIFileSystem.copy ( from .. 'src/moai/Moai.java',		to .. 'src/com/ziplinegames/moai/Moai.java' )
--MOAIFileSystem.copy ( from .. 'src/moai/MoaiLog.java',	to .. 'src/com/ziplinegames/moai/MoaiLog.java' )
--MOAIFileSystem.copy ( from .. '../init.lua',			to .. '../init.lua' )
--MOAIFileSystem.copy ( from .. 'src/moai/crittercism',	to .. '../moai/crittercism' )
--MOAIFileSystem.copy ( from .. 'src/moai/facebook',		to .. '../moai/facebook' )
--MOAIFileSystem.copy ( from .. 'src/moai/google-billing',to .. '../moai/google-billing' )
--MOAIFileSystem.copy ( from .. 'src/moai/google-push',	to .. '../moai/google-push' )
--MOAIFileSystem.copy ( from .. 'src/moai/tapjoy',		to .. '../moai/tapjoy' )
--MOAIFileSystem.copy ( from .. 'external',				to .. '../external' )
--MOAIFileSystem.copy ( from .. '../../res',				to .. '../../res' )
--MOAIFileSystem.copy ( from .. '../../README.txt',		to .. '../../README.txt' )
--MOAIFileSystem.copy ( from .. '../../run-host.bat',		to .. '../../run-host.bat' )
--MOAIFileSystem.copy ( from .. '../../run-host.sh',		to .. '../../run-host.sh' )
--MOAIFileSystem.copy ( from .. '../../settings-global.sh',	to .. '../../settings-global.sh' )
--MOAIFileSystem.copy ( from .. '../../settings-local.sh', 	to .. '../../settings-local.sh' )

-- hosts/src
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../src/hosts' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/src' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'FolderWatcher-mac.h',	to .. 'FolderWatcher-mac.h' )
MOAIFileSystem.copy ( from .. 'FolderWatcher-mac.mm',	to .. 'FolderWatcher-mac.mm' )
MOAIFileSystem.copy ( from .. 'FolderWatcher-win.cpp',	to .. 'FolderWatcher-win.cpp' )
MOAIFileSystem.copy ( from .. 'FolderWatcher-win.h',	to .. 'FolderWatcher-win.h' )
MOAIFileSystem.copy ( from .. 'GlutHost.cpp',			to .. 'GlutHost.cpp' )
MOAIFileSystem.copy ( from .. 'GlutHost.h',				to .. 'GlutHost.h' )
MOAIFileSystem.copy ( from .. 'GlutHostMain.cpp',		to .. 'GlutHostMain.cpp' )
MOAIFileSystem.copy ( from .. 'ParticlePresets.cpp',	to .. 'ParticlePresets.cpp' )
MOAIFileSystem.copy ( from .. 'ParticlePresets.h',		to .. 'ParticlePresets.h' )

-- hosts/xcode/ios
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../xcode/ios' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/xcode/ios' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'Classes',				to .. 'Classes' )
MOAIFileSystem.copy ( from .. 'moai.xcodeproj',			to .. 'moai.xcodeproj' )
--MOAIFileSystem.deleteDirectory ( to .. 'moai.xcodeproj/xcshareddata', true )
MOAIFileSystem.copy ( from .. 'Entitlements.plist',		to .. 'Entitlements.plist' )
MOAIFileSystem.copy ( from .. 'Icon.png',				to .. 'Icon.png' )
MOAIFileSystem.copy ( from .. 'Icon@2x.png',			to .. 'Icon@2x.png' )
MOAIFileSystem.copy ( from .. 'Icon-72.png',			to .. 'Icon-72.png' )
MOAIFileSystem.copy ( from .. 'Icon-Small.png',			to .. 'Icon-Small.png' )
MOAIFileSystem.copy ( from .. 'Icon-Small@2x.png',		to .. 'Icon-Small@2x.png' )
MOAIFileSystem.copy ( from .. 'Icon-Small-50.png',		to .. 'Icon-Small-50.png' )
MOAIFileSystem.copy ( from .. 'Info.plist',				to .. 'Info.plist' )
MOAIFileSystem.copy ( from .. 'main.mm',				to .. 'main.mm' )
MOAIFileSystem.copy ( from .. 'package.sh',				to .. 'package.sh' )

-- hosts/xcode/ios/Libraries/Crittercism
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/xcode/ios/Libraries/Crittercism' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )
MOAIFileSystem.copy ( from .. 'libCrittercismCrashOnly_v3_3_3.a', to .. 'libCrittercismCrashOnly_v3_3_3.a' )

-- hosts/xcode/ios/Libraries/Facebook
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../3rdparty/facebookiOS-3.0.6.b' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/xcode/ios/Libraries/Facebook' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'FacebookSDKResources.bundle',		to .. 'FacebookSDKResources.bundle' )
MOAIFileSystem.copy ( from .. 'FBUserSettingsViewResources.bundle',	to .. 'FBUserSettingsViewResources.bundle' )
MOAIFileSystem.copy ( from .. 'libfacebook_ios_sdk.a',				to .. 'libfacebook_ios_sdk.a' )

-- hosts/xcode/ios/Libraries/TapjoyConnect
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../3rdparty/tapjoyiOS-8.1.9/TapjoyConnect/Components' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/back_default.png',		to .. 'back_default.png' )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/button_highlight.png',	to .. 'button_highlight.png' )
MOAIFileSystem.copy ( from .. 'TJCCore/Views/close_button.png',		to .. 'close_button.png' )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/done_default.png',		to .. 'done_default.png' )
MOAIFileSystem.copy ( from .. 'TJCFeaturedApp/featured_app.sql',	to .. 'featured_app.sql' )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/tapjoy_icon.png',		to .. 'tapjoy_icon.png' )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/tapjoy_logo.png',		to .. 'tapjoy_logo.png' )
MOAIFileSystem.copy ( from .. 'TJCCore/Views/TJCLoadingView.xib',	to .. 'TJCLoadingView.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Views/TJCVGItemButton.xib', to .. 'TJCVGItemButton.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Views/TJCVGItemDetail.xib', to .. 'TJCVGItemDetail.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Views/TJCVGPageView.xib', to .. 'TJCVGPageView.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Views/TJCVGTableViewCell.xib', to .. 'TJCVGTableViewCell.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Views/TJCVGView.xib', to .. 'TJCVGView.xib' )
MOAIFileSystem.copy ( from .. 'TJCVideoAds/TJCVideoView.xib',		to .. 'TJCVideoView.xib' )
MOAIFileSystem.copy ( from .. 'TJCVirtualGoods/TJCVG_Resources',	to )

-- include/aku
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../src/aku' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/include/aku' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'AKU.h',					to .. 'AKU.h' )
MOAIFileSystem.copy ( from .. 'AKU-adcolony.h',			to .. 'AKU-adcolony.h' )
MOAIFileSystem.copy ( from .. 'AKU-audiosampler.h',		to .. 'AKU-audiosampler.h' )
MOAIFileSystem.copy ( from .. 'AKU-chartboost.h',		to .. 'AKU-chartboost.h' )
MOAIFileSystem.copy ( from .. 'AKU-debugger.h',			to .. 'AKU-debugger.h' )
MOAIFileSystem.copy ( from .. 'AKU-fmod-designer.h',	to .. 'AKU-fmod-designer.h' )
MOAIFileSystem.copy ( from .. 'AKU-fmod-ex.h',			to .. 'AKU-fmod-ex.h' )
MOAIFileSystem.copy ( from .. 'AKU-iphone.h',			to .. 'AKU-iphone.h' )
MOAIFileSystem.copy ( from .. 'AKU-luaext.h',			to .. 'AKU-luaext.h' )
MOAIFileSystem.copy ( from .. 'AKU-particles.h',		to .. 'AKU-particles.h' )
MOAIFileSystem.copy ( from .. 'AKU-untz.h',				to .. 'AKU-untz.h' )
MOAIFileSystem.copy ( from .. 'pch.h',					to .. 'pch.h' )

-- include/lua-headers
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../src/lua-headers' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/include/lua-headers' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'moai_lua.h',				to .. 'moai_lua.h' )

-- include/lua-modules
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../src/lua-modules' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/include/lua-modules' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from .. 'moai',					to .. 'moai' )

-- samples
-- not psd, test, replace-run-bat-files.bat
from = MOAIFileSystem.getAbsoluteDirectoryPath ( '../samples' )
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/samples' )
MOAIFileSystem.affirmPath ( to )
MOAIFileSystem.copy ( from, to )
MOAIFileSystem.deleteDirectory ( to .. 'psd', true )
MOAIFileSystem.deleteDirectory ( to .. 'test', true )
MOAIFileSystem.deleteFile ( to .. 'replace-run-bat-files.bat' )

-- release-notes.txt, version.txt
MOAIFileSystem.copy ( '../docs/release-notes.txt',	'moai-sdk/release-notes.txt' )
MOAIFileSystem.copy ( '../version.txt',				'moai-sdk/version.txt' )

-- delete moai target
to = MOAIFileSystem.getAbsoluteDirectoryPath ( 'moai-sdk/hosts/xcode/ios' )
MOAIFileSystem.deleteFile ( to .. 'moai-target' )