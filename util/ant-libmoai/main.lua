--==============================================================
-- args
--==============================================================

OUTPUT_DIR				= INVOKE_DIR .. 'libmoai/'

LIB_NAME				= 'moai'
MY_ARM_MODE				= 'arm'
MY_ARM_ARCH				= 'armeabi-v7a'
MY_APP_PLATFORM			= 'android-10'

CONFIGS					= {}

DISABLED				= {}
DISABLE_ALL				= false

MOAI_JAVA_NAMESPACE		= 'com.ziplinegames.moai'

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

GLOBALS							= {}
MODULES							= {}
PLUGINS							= {}
TARGETS							= {}

MODULE_APP_DECLARATIONS			= ''
MODULE_MANIFEST_PERMISSIONS		= ''
MODULE_PROJECT_INCLUDES			= ''

--DEFAULT_LIBRARIES				= {}
STATIC_LIBRARIES				= {} -- set of all static libraries
WHOLE_STATIC_LIBRARIES			= {}

--==============================================================
-- util
--==============================================================

local addLibraries
local concat
local getGlobalsString
local getLibrariesString
local getModulesString
local getPluginsStringFunc
local importJava
local isEnabled
local makeTarget
local processConfigFile
local usage
local writeList
local writeModule

----------------------------------------------------------------
addLibraries = function ( mask, libraries )

	if type ( libraries ) == 'string' then
		local t = util.tokenize ( libraries )
		for i, v in ipairs ( t ) do
			mask [ v ] = true
		end
	end

	if type ( libraries ) == 'table' then
		for i, v in ipairs ( libraries ) do
			addLibraries ( mask, v )
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
getLibrariesString = function ( libraries, mask )

	local str

	for i, v in ipairs ( libraries ) do
		if mask [ v ] then
			str = ( str and ( str .. ' ' ) or '' ) .. v
		end
	end

	return str or ''
end

----------------------------------------------------------------
getModulesString = function ()

	local file = io.open ( TEMP_FILENAME, 'w' )
	local first = true
	
	for name, module in util.pairsByKeys ( MODULES ) do
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

	spacer = spacer or ''

	local file = io.open ( TEMP_FILENAME, 'w' )
	local first = true
	
	for name, module in pairs ( MODULES ) do
		
		local value = module.PLUGIN and module.PLUGIN [ key ]
	
		if value then

			file:write ( '\n' )
			file:write ( spacer )
			file:write ( string.format ( '#ifdef %s\n', module.PREPROCESSOR_FLAG ))

			for i, v in util.iterateSingleOrArray ( value ) do
				file:write ( spacer )
				file:write ( string.format ( format, v ))
			end

			file:write ( spacer )
			file:write ( string.format ( '#endif\n' ))
		end
	end

	file:close ()
	
	local str = util.readFileAll ( TEMP_FILENAME )
	MOAIFileSystem.deleteFile ( TEMP_FILENAME )
	
	return str
end

----------------------------------------------------------------
local importJava = function ( path, namespace )
	
	if not path then return end
	path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
	
	local srcPath = path .. 'src/'
	if MOAIFileSystem.checkPathExists ( srcPath ) then
		MOAIFileSystem.copy (  srcPath, OUTPUT_DIR .. 'src/' )
	end

	local libPath = path .. 'lib/'
	if MOAIFileSystem.checkPathExists ( libPath ) then
		for i, filename in ipairs ( util.listFiles ( libPath, 'jar' )) do
			MOAIFileSystem.copy (  libPath .. filename, OUTPUT_DIR .. 'libs/' .. filename )
		end
	end
	
	local projectPath = path .. 'project/'
	if MOAIFileSystem.checkPathExists ( projectPath ) then
		for i, pathname in ipairs ( util.listDirectories ( projectPath )) do
			MOAIFileSystem.copy (  projectPath .. pathname, OUTPUT_DIR .. 'ant/' .. pathname )
			MODULE_PROJECT_INCLUDES = MODULE_PROJECT_INCLUDES .. string.format ( 'android.library.reference.1=../%s/\n', pathname )
		end
	end
	
	local appDeclarationsPath = path .. 'manifest_declarations.xml'
	
	if MOAIFileSystem.checkFileExists ( appDeclarationsPath ) then
		local fp = io.open ( appDeclarationsPath, "r" )
		MODULE_APP_DECLARATIONS = MODULE_APP_DECLARATIONS .. '\n' .. fp:read ( "*all" )
		fp:close ()
	end
	
	local manifestPermissionsPath = path .. 'manifest_permissions.xml'
	if MOAIFileSystem.checkFileExists ( manifestPermissionsPath ) then
		local fp = io.open ( manifestPermissionsPath, "r" )
		MODULE_MANIFEST_PERMISSIONS = MODULE_MANIFEST_PERMISSIONS .. '\n' .. fp:read ( "*all" )
		fp:close ()
	end

	local projectSrcFolder	= string.format ( '%ssrc/%s/', OUTPUT_DIR, string.gsub ( namespace, '%.', '/' ))

	local files = util.listFiles ( path, 'java' )
	for i, filename in ipairs ( files ) do
		MOAIFileSystem.copy ( path .. filename, projectSrcFolder .. filename )
	end
end

----------------------------------------------------------------
isEnabled = function ( name )
	return name and not ( DISABLE_ALL or DISABLED [ name ])
end

----------------------------------------------------------------
makeTarget = function ( target )

	local targetMakefile = JNI_DIR .. target.NAME .. '.mk'
	MOAIFileSystem.copy ( 'MoaiTarget.mk', targetMakefile )

	local modules = {}
	local libraries = {}
	local preprecessorFlags = {}

	--addLibraries ( libraries, DEFAULT_LIBRARIES )

	-- build a set of modules to include
	for i, moduleName in ipairs ( target.MODULES or {}) do
		local module = MODULES [ moduleName ]
		if module then
			modules [ moduleName ] = module
			addLibraries ( libraries, module.STATIC_LIBRARIES )
		end
	end

	-- flags for modules included in this target
	for k, module in pairs ( MODULES ) do
		if module.PREPROCESSOR_FLAG then
			preprecessorFlags [ module.PREPROCESSOR_FLAG ] = modules [ k ] and 1 or 0
		end
	end

	-- build the preprocessor string
	local preprocessorString = '\n'
	for k, v in util.pairsByKeys ( preprecessorFlags ) do
		preprocessorString = preprocessorString .. string.format ( '\tMY_LOCAL_CFLAGS += -D%s=%d\n', k, v )
	end

	util.replaceInFile ( targetMakefile, {
		[ '@LIB_NAME@' ]					= target.NAME,
		[ '@AKU_PREPROCESSOR@' ]			= preprocessorString,
		[ '@STATIC_LIBRARIES@' ] 			= getLibrariesString ( STATIC_LIBRARIES, libraries ),
		[ '@WHOLE_STATIC_LIBRARIES@' ] 		= getLibrariesString ( WHOLE_STATIC_LIBRARIES, libraries ),
	})
end

----------------------------------------------------------------
processConfigFile = function ( filename )

	print ( 'CONFIG', filename )

	filename = MOAIFileSystem.getAbsoluteFilePath ( filename )
	if not MOAIFileSystem.checkFileExists ( filename ) then return end

	local config = { MOAI_SDK_HOME = MOAI_SDK_HOME }
	util.dofileWithEnvironment ( filename, config )

	--DEFAULT_LIBRARIES			= util.joinTables ( config.DEFAULT_LIBRARIES, DEFAULT_LIBRARIES )
	STATIC_LIBRARIES			= util.joinTables ( config.STATIC_LINK_ORDER, STATIC_LIBRARIES )
	WHOLE_STATIC_LIBRARIES		= util.joinTables ( config.WHOLE_STATIC_LIBRARIES, WHOLE_STATIC_LIBRARIES )

	--if config.CONFIG_NAME then
	--	local configPath = util.getFolderFromPath ( filename )
	--	GLOBALS [ config.CONFIG_NAME ] = MOAIFileSystem.getRelativePath ( configPath, JNI_DIR )
	--end

	if config.SETTINGS then
		LIB_NAME = config.SETTINGS.LIB_NAME or LIB_NAME
		MY_ARM_MODE = config.SETTINGS.MY_ARM_MODE or MY_ARM_MODE
		MY_ARM_ARCH = config.SETTINGS.MY_ARM_ARCH or MY_ARM_ARCH
		MY_APP_PLATFORM = config.SETTINGS.MY_APP_PLATFORM or MY_APP_PLATFORM
	end

	if config.MODULES then
		for k, v in pairs ( config.MODULES ) do
			MODULES [ k ] = v
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
	
	if config.TARGETS then
		for k, v in pairs ( config.TARGETS ) do
			TARGETS [ k ] = v
		end
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

	writeList ( file, '\tMY_HEADER_SEARCH_PATHS += ',		module.HEADER_SEARCH_PATHS, true )
	writeList ( file, '\tMY_INCLUDES += ',					module.MAKE, true )
end

--==============================================================
-- main
--==============================================================

MOAIFileSystem.deleteDirectory ( OUTPUT_DIR, true )
MOAIFileSystem.affirmPath ( JNI_DIR )

MOAIFileSystem.copy ( 'README.txt', OUTPUT_DIR .. 'README.txt' )
MOAIFileSystem.copy ( 'Android.mk', JNI_DIR .. 'Android.mk' )
MOAIFileSystem.copy ( 'Application.mk', JNI_DIR .. 'Application.mk' )
MOAIFileSystem.copy ( 'src/', JNI_DIR .. 'src/' )
MOAIFileSystem.copy ( MOAI_SDK_HOME .. 'src/host-modules/aku_plugins.cpp.in', JNI_DIR .. 'src/aku_plugins.cpp' )

processConfigFile ( 'config.lua' )
processConfigFile ( INVOKE_DIR .. 'config.lua' )

for i, config in ipairs ( CONFIGS ) do
	print ( 'config', config )
	processConfigFile ( config )
end

util.replaceInFile ( JNI_DIR .. 'Android.mk', {
	[ '@MOAI_SDK_HOME@' ]				= MOAIFileSystem.getRelativePath ( MOAI_SDK_HOME, JNI_DIR ),
	[ '@MY_ARM_MODE@' ]					= MY_ARM_MODE,
	[ '@MY_ARM_ARCH@' ]					= MY_ARM_ARCH,
	[ '@GLOBALS@' ] 					= '', --getGlobalsString (),
	[ '@MODULES@' ] 					= getModulesString (),
})

util.replaceInFile ( JNI_DIR .. 'Application.mk', {
	[ '@MY_ARM_ARCH@' ]					= MY_ARM_ARCH,
	[ '@MY_APP_PLATFORM@' ] 			= MY_APP_PLATFORM,
})

for k, module in pairs ( MODULES ) do
	for i, path in ipairs ( module.JAVA or {} ) do
		importJava ( path, module.NAMESPACE or MOAI_JAVA_NAMESPACE )
	end
end

local file = io.open ( JNI_DIR .. 'libraries.mk', 'w' )
for k, target in pairs ( TARGETS ) do
	if target.NAME and isEnabled ( target.NAME ) then
		makeTarget ( target )
		file:write ( string.format ( 'include %s.mk\n', target.NAME ))
	end
end
file:close ()

util.replaceInFile ( JNI_DIR .. 'src/aku_plugins.cpp', {
	[ '@AKU_PLUGINS_HEADERS@' ]					= getPluginsString ( 'INCLUDES', '\t#include %s\n' ),
	[ '@AKU_PLUGINS_APP_FINALIZE@' ]			= getPluginsString ( 'PREFIX', '\t%sAppFinalize ();\n', '\t' ),
	[ '@AKU_PLUGINS_APP_INITIALIZE@' ]			= getPluginsString ( 'PREFIX', '\t%sAppInitialize ();\n', '\t' ),
	[ '@AKU_PLUGINS_CONTEXT_INITIALIZE@' ] 		= getPluginsString ( 'PREFIX', '\t%sContextInitialize ();\n', '\t' ),
	[ '@AKU_PLUGINS_PAUSE@' ] 					= getPluginsString ( 'PREFIX', '\t%sPause ( pause );\n', '\t' ),
	[ '@AKU_PLUGINS_UPDATE@' ] 					= getPluginsString ( 'PREFIX', '\t%sUpdate ();\n', '\t' ),
})
