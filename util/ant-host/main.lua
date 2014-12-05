--==============================================================
-- args
--==============================================================

OUTPUT_DIR				= INVOKE_DIR .. 'host/'
MOAI_JAVA_NAMESPACE		= 'com.ziplinegames.moai'

CONFIGS					= {}

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then
	
		if escape == 'c' or escape == 'config' then
			table.insert ( CONFIGS, MOAIFileSystem.getAbsoluteFilePath ( INVOKE_DIR .. param ))
		end

		if escape == 'o' or escape == 'out' then
			if ( param [ 1 ] ~= '/' ) and ( param [ 1 ] ~= '\\' ) then
				param = INVOKE_DIR .. param
			end 
			
			OUTPUT_DIR = MOAIFileSystem.getAbsoluteDirectoryPath ( param )
		end
	end
end

ANT_DIR							= OUTPUT_DIR .. 'ant/'
MOAI_PROJECT_PATH				= ANT_DIR .. 'project/'

MODULES							= {}

MODULE_APP_DECLARATIONS			= ''
MODULE_MANIFEST_PERMISSIONS		= ''
MODULE_PROJECT_INCLUDES			= ''

COPY							= {}

----------------------------------------------------------------
local config = {}

config.PROJECT_NAME					= 'MoaiSample'
config.PACKAGE_NAME					= 'com.ziplinegames.moaisample'
config.PLATFORM_NAME				= 'android'

config.LUA_WORKING_DIR				= 'bundle/assets/lua'
config.LUA_MAIN						= 'main.lua'

config.MANIFEST_DEBUGGABLE			= 'false'

config.ANDROID_PLATFORM_TARGET		= 'android-17'

--==============================================================
-- util
--==============================================================

local importBin
local importLib
local importSrc
local processConfigFile

----------------------------------------------------------------
local importBin = function ( path )

	if not path then return end
	local filename = util.getFilenameFromPath ( path )
	MOAIFileSystem.copy (  path, MOAI_PROJECT_PATH .. 'libs/armeabi-v7a/' .. filename )
end

----------------------------------------------------------------
local importLib = function ( path )
	
	if not path then return end
	
	local srcPath = path .. 'src/'
	if MOAIFileSystem.checkPathExists ( srcPath ) then
		MOAIFileSystem.copy (  srcPath, MOAI_PROJECT_PATH .. 'src/' )
	end

	local libPath = path .. 'lib/'
	if MOAIFileSystem.checkPathExists ( libPath ) then
		for i, filename in ipairs ( util.listFiles ( libPath, 'jar' )) do
			MOAIFileSystem.copy (  libPath .. filename, MOAI_PROJECT_PATH .. 'libs/' .. filename )
		end
	end
	
	local projectPath = path .. 'project/'
	if MOAIFileSystem.checkPathExists ( projectPath ) then
		for i, pathname in ipairs ( util.listDirectories ( projectPath )) do
			MOAIFileSystem.copy (  projectPath .. pathname, ANT_DIR .. pathname )
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
end

----------------------------------------------------------------
local importSrc = function ( path, namespace )

	if not path then return end

	local projectSrcFolder	= string.format ( '%ssrc/%s/', MOAI_PROJECT_PATH, string.gsub ( namespace, '%.', '/' ))

	local files = util.listFiles ( path, 'java' )
	for i, filename in ipairs ( files ) do
		MOAIFileSystem.copy (  path .. filename, projectSrcFolder .. filename )
	end
end

----------------------------------------------------------------
processConfigFile = function ( filename )

	filename = MOAIFileSystem.getAbsoluteFilePath ( filename )
	if not MOAIFileSystem.checkFileExists ( filename ) then return end
	local configPath = util.getFolderFromPath ( filename )

	local configFile = { MOAI_SDK_HOME = MOAI_SDK_HOME }
	util.dofileWithEnvironment ( filename, configFile )

	for k, v in pairs ( configFile ) do
		config [ k ] = v
	end

	local resolvePath = function ( path )
		return path and ( string.find ( path, '^/' ) and path or configPath .. path )
	end

	if configFile.MODULES then
		for name, mod in pairs ( configFile.MODULES ) do

			local src = resolvePath ( mod.src )
			local lib = resolvePath ( mod.lib )
			local bin = resolvePath ( mod.bin )

			MODULES [ name ] = {
				namespace = mod.namespace,
				src = src and MOAIFileSystem.getAbsoluteDirectoryPath ( src ),
				lib = lib and MOAIFileSystem.getAbsoluteDirectoryPath ( lib ),
				bin = bin and MOAIFileSystem.getAbsoluteFilePath ( bin ),
			}
		end
	end

	if configFile.COPY then
		for dst, src in pairs ( configFile.COPY ) do
			COPY [ MOAI_PROJECT_PATH .. dst ] = resolvePath ( src )
		end
	end

	if configFile.KEYSTORE_PATH then
		config.KEYSTORE_PATH = resolvePath ( configFile.KEYSTORE_PATH )
	end
end

--==============================================================
-- main
--==============================================================

--[[
COPY = {
	{ dst = MOAI_PROJECT_PATH .. 'assets/lua',		src = LUA_SRC_PATH },
	{ dst = MOAI_PROJECT_PATH .. 'res/',			src = string.format ( '%sres-%s', TEMPLATE_PATH, TARGET )},
}
]]--

processConfigFile ( 'config.lua' )

for i, config in ipairs ( CONFIGS ) do
	print ( 'config', config )
	processConfigFile ( config )
end

MOAIFileSystem.deleteDirectory ( ANT_DIR, true )
MOAIFileSystem.affirmPath ( ANT_DIR )

MOAIFileSystem.copy ( 'project', MOAI_PROJECT_PATH )

if config.KEYSTORE_PATH and config.KEYSTORE_NAME then
	MOAIFileSystem.copy ( config.KEYSTORE_PATH .. config.KEYSTORE_NAME, MOAI_PROJECT_PATH .. config.KEYSTORE_NAME )
end

for name, mod in pairs ( MODULES ) do
	importSrc ( mod.src, mod.namespace or MOAI_JAVA_NAMESPACE )
	importLib ( mod.lib )
	importBin ( mod.bin )
end

for dst, src in pairs ( COPY ) do
	MOAIFileSystem.copy ( src, dst )
end

util.replaceInFiles ({
	
	[ MOAI_PROJECT_PATH .. 'AndroidManifest.xml' ] = {
		[ '@EXTERNAL_APPLICATION_ENTRIES@' ]	= MODULE_APP_DECLARATIONS,
		[ '@EXTERNAL_MANIFEST_PERMISSIONS@' ] 	= MODULE_MANIFEST_PERMISSIONS,
	},
	
	[ MOAI_PROJECT_PATH .. 'project.properties' ] = {
		[ '@EXTERNAL_PROJECT_INCLUDES@' ] 		= MODULE_PROJECT_INCLUDES,
	},
})

util.replaceInFiles ({
	
	[ MOAI_PROJECT_PATH .. '.project' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
	},
	
	[ MOAI_PROJECT_PATH .. 'build.xml' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
	},
	
	[ MOAI_PROJECT_PATH .. 'AndroidManifest.xml' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
		[ '@PACKAGE@' ]							= config.PACKAGE_NAME,
		[ '@VERSION_CODE@' ]					= '1',
		[ '@VERSION_NAME@' ]					= '1.0',
		[ '@DEBUGGABLE@' ]						= config.MANIFEST_DEBUGGABLE,
		[ '@EXTERNAL_APPLICATION_ENTRIES@' ]	= MODULE_APP_DECLARATIONS,
		[ '@EXTERNAL_MANIFEST_PERMISSIONS@' ] 	= MODULE_MANIFEST_PERMISSIONS,
	},
	
	[ MOAI_PROJECT_PATH .. 'ant.properties' ] = {
		[ '@KEY_STORE@' ]						= config.KEYSTORE_NAME,
		[ '@KEY_ALIAS@' ]						= config.KEYSTORE_ALIAS,
		[ '@KEY_STORE_PASSWORD@' ]				= config.KEYSTORE_PASSWORD,
		[ '@KEY_ALIAS_PASSWORD@' ]				= config.KEYSTORE_ALIAS_PASSWORD,
	},
	
	[ util.wrap ( util.iterateFilesAbsPath, MOAI_PROJECT_PATH, '^local.properties$' )] = {
		[ '@SDK_ROOT@' ]						= '$ANDROID_HOME',
	},
	
	[ MOAI_PROJECT_PATH .. 'project.properties' ] = {
		[ '@EXTERNAL_PROJECT_INCLUDES@' ] 		= MODULE_PROJECT_INCLUDES,
	},

	[ util.wrap ( util.iterateFilesAbsPath, MOAI_PROJECT_PATH .. 'src', '.java$' )] = {
		[ '@PACKAGE@' ]							= config.PACKAGE_NAME,
		[ '@PLATFORM_NAME@' ]					= string.upper ( config.PLATFORM_NAME ),
		[ '@RUN_SCRIPTS@' ]						= config.LUA_MAIN,
		[ '@WORKING_DIR@' ]						= config.LUA_WORKING_DIR,
	},
})

-- TODO: these should come from a table of projects since we might have any number based on the config
os.execute ( string.format ( 'android update project --target %s --path %s',
	tostring ( config.ANDROID_PLATFORM_TARGET ),
	MOAI_PROJECT_PATH
))

os.execute ( string.format ( 'android update project --target %s --path %s',
	tostring ( config.ANDROID_PLATFORM_TARGET ),
	ANT_DIR .. 'facebook'
))
