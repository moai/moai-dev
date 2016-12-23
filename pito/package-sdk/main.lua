--==============================================================
-- args
--==============================================================

VERSION				= dofile ( '../sdk-version/version.lua' )

assert ( VERSION )
assert ( VERSION.MAJOR )
assert ( VERSION.MINOR )
assert ( VERSION.REVISION )

VERSION_STR			=	VERSION.MINOR == 0 and
						VERSION.REVISION == 0 and
						string.format ( '%d.%d', VERSION.MAJOR, VERSION.MINOR ) or
						string.format ( '%d.%d.%d', VERSION.MAJOR, VERSION.MINOR, VERSION.REVISION )

PACKAGE_NAME		= string.format ( 'moaisdk-%s-%s', util.osname (), VERSION_STR )
OUTPUT_DIR			= INVOKE_DIR
PACKAGE_DIR			= OUTPUT_DIR .. PACKAGE_NAME .. '/'

COPY_FILES			= {}
CLEAN_DIRS			= {}
DEV_PLATFORM		= nil

print ( VERSION_STR )
print ( PACKAGE_NAME )

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
			PACKAGE_DIR = OUTPUT_DIR .. PACKAGE_NAME .. '/'
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

processConfigFile ( 'config.lua' )

MOAIFileSystem.deleteDirectory ( PACKAGE_DIR, true )
MOAIFileSystem.copy ( 'moai-sdk', PACKAGE_DIR )

util.moaiexec ( './prepare-sdk-osx.sh' )

for k, v in pairs ( COPY_FILES ) do
	v = v == true and k or v
	MOAIFileSystem.copy ( MOAI_SDK_HOME .. k, PACKAGE_DIR .. v )
end

for i, v in ipairs ( CLEAN_DIRS ) do
	print ( 'CLEANING:', v )
	MOAIFileSystem.deleteDirectory ( PACKAGE_DIR .. v, true )
end

util.moaiexec ( 'pito make-lua-docs -o "%sdocs/sdk-lua-reference"', PACKAGE_DIR )
util.moaiexec ( './test-build.sh %s', PACKAGE_DIR )

--MOAIFileSystem.copy ( PACKAGE_DIR .. 'bin/osx/moai', MOAI_SDK_HOME .. 'bin/osx/moai' )

util.zip ( string.format ( '%s%s.zip', OUTPUT_DIR, PACKAGE_NAME ), PACKAGE_DIR )
