----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local STATE_FILENAME		= 'count.lua'
local MARKED_FILENAME		= 'marked.lua'
local SAMPLES_DIRECTORY		= MOAI_SDK_HOME .. '/samples/'

MODE_BUILD = 'build'
MODE_CLEAN = 'clean'

gTargets = {
	[ 'ios' ]				= { build = 'build-ios.sh',				clean = 'clean-ios.sh' },
	[ 'ios-device' ]		= { build = 'build-ios-device.sh',		clean = 'clean-ios-device.sh' },
	[ 'ios-simulator' ]		= { build = 'build-ios-simulator.sh',	clean = 'clean-ios-simulator.sh' },
	[ 'osx' ]				= { build = 'build-osx.sh',				clean = 'clean-osx.sh' },
}

gMode = MODE_BUILD
gTargetsToBuild = {}

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do

	if escape == 'c' or escape == 'clean' then
		gMode = MODE_CLEAN
	end

	if param and gTargets [ param ] then
		table.insert ( gTargetsToBuild, param )
	end
end

for i, target in ipairs ( gTargetsToBuild ) do

	local command = gTargets [ target ]
	command = command and command [ gMode ]

	util.moaiexec ( './%s', command )
end
