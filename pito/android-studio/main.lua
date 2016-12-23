--==============================================================
-- args
--==============================================================

OUTPUT_DIR				= INVOKE_DIR

LIB_NAME				= 'moai'
MY_ARM_MODE				= 'arm'
MY_ARM_ARCH				= 'armeabi-v7a'
MY_APP_PLATFORM			= 'android-10'

CONFIGS					= {}

DISABLED				= {}
DISABLE_ALL				= false

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then

		if escape == 'm' or escape == 'moai' then
			MOAI_SDK_HOME =  MOAIFileSystem.getAbsoluteDirectoryPath ( INVOKE_DIR .. param )
		end

		if escape == 'o' or escape == 'out' then
			if ( param [ 1 ] ~= '/' ) and ( param [ 1 ] ~= '\\' ) then
				param = INVOKE_DIR .. param
			end 
			
			OUTPUT_DIR = MOAIFileSystem.getAbsoluteDirectoryPath ( param )
		end
	end
end

print ( 'MOAI_SDK_HOME', MOAI_SDK_HOME )
print ( 'INVOKE_DIR', INVOKE_DIR )
print ( 'OUTPUT_DIR', OUTPUT_DIR )

--==============================================================
-- main
--==============================================================

PROJECT_FOLDER = OUTPUT_DIR .. 'Moai/'
MOAI_SDK_HOME = MOAIFileSystem.getRelativePath ( MOAI_SDK_HOME, PROJECT_FOLDER ) .. '../'

print ( 'MOAI_SDK_HOME', MOAI_SDK_HOME )

MOAIFileSystem.copy ( 'Moai/', PROJECT_FOLDER )

util.replaceInFiles ({
	[ util.wrap ( util.iterateFilesAbsPath, PROJECT_FOLDER, '.gradle$' )] = {
		[ '@MOAI_SDK_HOME@' ]		= MOAI_SDK_HOME,
	},
})

