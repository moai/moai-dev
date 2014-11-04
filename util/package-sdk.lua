--==============================================================
-- setup
--==============================================================

MOAI_SDK_HOME	= MOAIFileSystem.getAbsoluteDirectoryPath ( '../' ) -- default path to Moai SDK relative to script dir
SCRIPT_DIR		= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
INVOKE_DIR		= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])

MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )

require ( 'util' )

--==============================================================
-- args
--==============================================================

OUTPUT_DIR			= INVOKE_DIR .. 'moai-sdk/'

COPY_FILES			= {}
DEV_PLATFORM		= nil

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then

		if escape == 'p' or escape == 'platform' then
			DEV_PLATFORM = param
		end

		if escape == 'o' or escape == 'out' then
			if ( param [ 1 ] ~= '/' ) and ( param [ 1 ] ~= '\\' ) then
				param = INVOKE_DIR .. param
			end 
			
			OUTPUT_DIR = MOAIFileSystem.getAbsoluteDirectoryPath ( param )
		end
	end
end

--==============================================================
-- util
--==============================================================

local processConfigFile

----------------------------------------------------------------
processConfigFile = function ( filename )

	filename = MOAIFileSystem.getAbsoluteFilePath ( filename )
	if not MOAIFileSystem.checkFileExists ( filename ) then return end

	local config = {}
	util.dofileWithEnvironment ( filename, config )

	util.mergeTables ( COPY_FILES, config.COMMON )
	util.mergeTables ( COPY_FILES, ( DEV_PLATFORM == 'mac' ) and config.MAC )
end

--==============================================================
-- main
--==============================================================

processConfigFile ( MOAI_SDK_HOME .. 'util/package-sdk/config.lua' )

MOAIFileSystem.deleteDirectory ( OUTPUT_DIR, true )
MOAIFileSystem.affirmPath ( OUTPUT_DIR )

for k, v in pairs ( COPY_FILES ) do
	v = v == true and k or v
	print ( 'COPYING:', k, v )
	MOAIFileSystem.copy ( MOAI_SDK_HOME .. k, OUTPUT_DIR .. v )
end
