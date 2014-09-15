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

OUTPUT_DIR			= INVOKE_DIR .. 'libmoai/'

MY_ARM_MODE			= 'arm'
MY_ARM_ARCH			= 'armeabi-v7a'
MY_APP_PLATFORM		= 'android-10'

CONFIGS				= {}

DISABLED			= {}
DISABLE_ALL			= false

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if escape == 'D' or escape == 'disable-all' then
		DISABLE_ALL = true
	end
	
	if param then
	
		if escape == 'c' or escape == 'config' then
			table.insert ( CONFIGS, MOAIFileSystem.getAbsoluteFilePath ( INVOKE_DIR .. param ))
		end

		if escape == 'd' or escape == 'disable' then
			DISABLED [ param ] = true
		end

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

JNI_DIR					= OUTPUT_DIR .. 'jni/'
TEMP_FILENAME			= OUTPUT_DIR .. '.tmp'

print ( 'MOAI_SDK_HOME', MOAI_SDK_HOME )
print ( 'SCRIPT_DIR', SCRIPT_DIR )
print ( 'INVOKE_DIR', INVOKE_DIR )
print ( 'OUTPUT_DIR', OUTPUT_DIR )

GLOBALS						= {}
MODULES						= {}
PLUGINS						= {}

ALL_LIBRARIES				= {}
STATIC_LIBRARIES			= {} -- set of all static libraries
WHOLE_STATIC_LIBRARIES		= {}

--==============================================================
-- util
--==============================================================

local addStaticLibraries
local concat
local getGlobalsString
local getLibrariesString
local getModulesString
local getPluginsStringFunc
local isEnabled
local processConfigFile
local usage
local writeList
local writeModule

----------------------------------------------------------------
addStaticLibraries = function ( libraries )

	if type ( libraries ) == 'string' then
		local t = util.tokenize ( libraries )
		for i, v in ipairs ( t ) do
			ALL_LIBRARIES [ v ] = true
		end
	end

	if type ( libraries ) == 'table' then
		for i, v in ipairs ( libraries ) do
			addStaticLibraries ( v )
		end
	end
end

----------------------------------------------------------------
cleanTempFiles = function ()
	
	MOAIFileSystem.deleteFile ( GLOBALS_TMP_FILENAME )
	MOAIFileSystem.deleteFile ( MODULES_TMP_FILENAME )
end

----------------------------------------------------------------
concat = function ( list )

	if type ( list ) == 'string' then
		return list
	end

	if type ( list ) == 'table' then
		return table.concat ( list, ' ' )
	end
end

----------------------------------------------------------------
getGlobalsString = function ()

	local file = io.open ( TEMP_FILENAME, 'w' )
	local first = true
	
	for k, v in pairs ( GLOBALS ) do
		if not first then file:write ( '\n\t' ) end
		file:write ( string.format ( '%s\t:= %s', k, v ))
		first = false
	end
	file:close ()

	local str = util.readFileAll ( TEMP_FILENAME )
	MOAIFileSystem.deleteFile ( TEMP_FILENAME )
	
	return str
end

----------------------------------------------------------------
getLibrariesString = function ( libraries )

	local str

	for i, v in ipairs ( libraries ) do
		if ALL_LIBRARIES [ v ] then
			str = ( str and ( str .. ' ' ) or '' ) .. v
		end
	end
	
	return str or ''
end

----------------------------------------------------------------
getModulesString = function ()

	local file = io.open ( TEMP_FILENAME, 'w' )
	local first = true
	
	for name, module in pairs ( MODULES ) do
		if not first then file:write ( '\n\t' ) end
		writeModule ( file, name, module )
		first = false
	end
	file:close ()
	
	local str = util.readFileAll ( TEMP_FILENAME )
	MOAIFileSystem.deleteFile ( TEMP_FILENAME )
	
	return str
end

----------------------------------------------------------------
getPluginsString = function ( key, format, spacer )

	local file = io.open ( TEMP_FILENAME, 'w' )
	local first = true
	
	for name, plugin in pairs ( PLUGINS ) do
		if isEnabled ( name ) then
		
			local value = plugin [ key ]
		
			if type ( value ) == 'string' then
				if not first then file:write ( spacer ) end
				file:write ( string.format ( format, value ))
				first = false
			end
		
			if type ( value ) == 'table' then
			
				for i, v in ipairs ( value ) do
					if not first then file:write ( spacer ) end
					file:write ( string.format ( format, v ))
					first = false
				end
			end
		end
	end
	file:close ()
	
	local str = util.readFileAll ( TEMP_FILENAME )
	MOAIFileSystem.deleteFile ( TEMP_FILENAME )
	
	return str
end

----------------------------------------------------------------
isEnabled = function ( name )
	return not ( DISABLE_ALL or DISABLED [ name ])
end

----------------------------------------------------------------
processConfigFile = function ( filename )

	filename = MOAIFileSystem.getAbsoluteFilePath ( filename )
	if not MOAIFileSystem.checkFileExists ( filename ) then return end

	local config = {}
	util.dofileWithEnvironment ( filename, config )

	STATIC_LIBRARIES = util.joinTables ( config.STATIC_LIBRARIES, STATIC_LIBRARIES )
	WHOLE_STATIC_LIBRARIES = util.joinTables ( config.WHOLE_STATIC_LIBRARIES, WHOLE_STATIC_LIBRARIES )

	if config.CONFIG_NAME then
		local configPath = util.getFolderFromPath ( filename )
		GLOBALS [ config.CONFIG_NAME ] = MOAIFileSystem.getRelativePath ( configPath, JNI_DIR )
	end

	if config.SETTINGS then
		MY_ARM_MODE = config.SETTINGS.MY_ARM_MODE or MY_ARM_MODE
		MY_ARM_ARCH = config.SETTINGS.MY_ARM_ARCH or MY_ARM_ARCH
		MY_APP_PLATFORM = config.SETTINGS.MY_APP_PLATFORM or MY_APP_PLATFORM
	end

	if config.MODULES then
		for k, v in pairs ( config.MODULES ) do
			MODULES [ k ] = v
			if isEnabled ( k ) then
				addStaticLibraries ( v.STATIC_LIBRARIES )
			end
		end
	end

	if config.PLUGINS then
		for k, v in pairs ( config.PLUGINS ) do
			PLUGINS [ k ] = v
		end
	end

	if config.GLOBALS then
		for k, v in pairs ( config.GLOBALS ) do
			GLOBALS [ k ] = v
		end
	end
	
	if config.EXTERNAL_LIBRARIES then
		addStaticLibraries ( config.EXTERNAL_LIBRARIES )
	end
end

----------------------------------------------------------------
usage = function ()

	print ( 'usage:' )

	os.exit ( 1 )
end

----------------------------------------------------------------
writeList = function ( file, prefix, list, multiline )
	
	if multiline and type ( list ) == 'table' and #list > 0 then
		for i, v in ipairs ( list ) do
			file:write ( string.format ( '%s%s\n', prefix, v ))
		end
	else
		local listString = concat ( list )
		if listString then
			file:write ( string.format ( '%s%s\n', prefix, listString ))
		end
	end
end

----------------------------------------------------------------
writeModule = function ( file, name, module )

	file:write ( '#--------------------------------------------------------------#\n' )
	file:write ( string.format ( '\t# %s\n\n', name ))

	if isEnabled ( name ) then
		
		if module.MODULE_DEFINE then
			writeList ( file, '\tMY_LOCAL_CFLAGS += ', string.format ( '-D%s=1', module.MODULE_DEFINE ))
		end
		
		writeList ( file, '\tMY_LOCAL_CFLAGS += ',				module.LOCAL_CFLAGS )
		writeList ( file, '\tMY_HEADER_SEARCH_PATHS += ',		module.HEADER_SEARCH_PATHS, true )
		writeList ( file, '\tMY_INCLUDES += ',					module.INCLUDES, true )
		writeList ( file, '\tMY_LOCAL_STATIC_LIBRARIES += ',	module.LOCAL_STATIC_LIBRARIES )
	else
		
		if module.MODULE_DEFINE then
			writeList ( file, '\tMY_LOCAL_CFLAGS += ', string.format ( '-D%s=0', module.MODULE_DEFINE ))
		else
			file:write ( '\t--disabled\n' )
		end
	end
end

--==============================================================
-- main
--==============================================================

MOAIFileSystem.affirmPath ( JNI_DIR )

MOAIFileSystem.copy ( 'ant-libmoai/Android.mk', JNI_DIR .. 'Android.mk' )
MOAIFileSystem.copy ( 'ant-libmoai/Application.mk', JNI_DIR .. 'Application.mk' )
MOAIFileSystem.copy ( 'ant-libmoai/src/', JNI_DIR .. 'src/' )
MOAIFileSystem.copy ( MOAI_SDK_HOME .. 'src/host-modules/aku_plugins.cpp.in', JNI_DIR .. 'src/aku_plugins.cpp' )

processConfigFile ( MOAI_SDK_HOME .. 'ant/libmoai/modules.lua' )

for i, config in ipairs ( CONFIGS ) do
	print ( 'config', config )
	processConfigFile ( config )
end

util.replaceInFile ( JNI_DIR .. 'Android.mk', {
	[ '@MOAI_SDK_HOME@' ]				= MOAIFileSystem.getRelativePath ( MOAI_SDK_HOME, JNI_DIR ),
	[ '@MY_ARM_MODE@' ]					= MY_ARM_MODE,
	[ '@MY_ARM_ARCH@' ]					= MY_ARM_ARCH,
	[ '@GLOBALS@' ] 					= getGlobalsString (),
	[ '@MODULES@' ] 					= getModulesString (),
	[ '@STATIC_LIBRARIES@' ] 			= getLibrariesString ( STATIC_LIBRARIES ),
	[ '@WHOLE_STATIC_LIBRARIES@' ] 		= getLibrariesString ( WHOLE_STATIC_LIBRARIES ),
})

util.replaceInFile ( JNI_DIR .. 'Application.mk', {
	[ '@MY_ARM_ARCH@' ]				= MY_ARM_ARCH,
	[ '@MY_APP_PLATFORM@' ] 		= MY_APP_PLATFORM,
})

util.replaceInFile ( JNI_DIR .. 'src/aku_plugins.cpp', {
	[ '@AKU_PLUGINS_HEADERS@' ]					= getPluginsString ( 'INCLUDES', '#include %s', '\n' ),
	[ '@AKU_PLUGINS_APP_FINALIZE@' ]			= getPluginsString ( 'PREFIX', '%sAppFinalize ();', '\n\t' ),
	[ '@AKU_PLUGINS_APP_INITIALIZE@' ]			= getPluginsString ( 'PREFIX', '%sAppInitialize ();', '\n\t' ),
	[ '@AKU_PLUGINS_CONTEXT_INITIALIZE@' ] 		= getPluginsString ( 'PREFIX', '%sContextInitialize ();', '\n\t' ),
	[ '@AKU_PLUGINS_PAUSE@' ] 					= getPluginsString ( 'PREFIX', '%sPause ( pause );', '\n\t' ),
	[ '@AKU_PLUGINS_UPDATE@' ] 					= getPluginsString ( 'PREFIX', '%sUpdate ();', '\n\t' ),
})
