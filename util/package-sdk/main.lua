--==============================================================
-- args
--==============================================================

OUTPUT_DIR			= INVOKE_DIR .. 'moai-sdk/'

COPY_FILES			= {}
CLEAN_DIRS			= {}
DEV_PLATFORM		= nil

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then

		if escape == 'p' or escape == 'platform' then
			DEV_PLATFORM = param
		end

		if escape == 'o' or escape == 'out' then
			if not util.isAbsPath ( param ) then
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
	util.mergeTables ( COPY_FILES, config.MAC )
	util.mergeTables ( CLEAN_DIRS, config.CLEAN_DIRS )
end

--==============================================================
-- main
--==============================================================

local moaiexec = function ( cmd, ... )
	local result = os.execute ( string.format ( cmd, ... ))
	if not result == 0 then os.exit ( result ) end
	return result
end

processConfigFile ( 'config.lua' )

MOAIFileSystem.deleteDirectory ( OUTPUT_DIR, true )
MOAIFileSystem.copy ( 'moai-sdk', OUTPUT_DIR )

moaiexec ( './prepare-sdk-osx.sh' )

for k, v in pairs ( COPY_FILES ) do
	v = v == true and k or v
	print ( 'COPYING:', k, v )
	MOAIFileSystem.copy ( MOAI_SDK_HOME .. k, OUTPUT_DIR .. v )
end

for i, v in ipairs ( CLEAN_DIRS ) do
	print ( 'CLEANING:', v )
	MOAIFileSystem.deleteDirectory ( OUTPUT_DIR .. v, true )
end

moaiexec ( 'moaiutil make-lua-docs -o "%sdocs/sdk-lua-reference"', OUTPUT_DIR )
