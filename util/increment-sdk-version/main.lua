require ( 'util' )

for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then
		if escape == 'r' or escape == 'Release' then
			RELEASE_TYPE=param
		end
	end
end

if RELEASE_TYPE == nil then
	print('Please enter Release type')
end

str = util.loadFileAsString ( MOAI_SDK_HOME .. 'src/config-default/moai_version_minor.h' )

type="MOAI_SDK_VERSION_MINOR %d"
minor= string.match(string.sub(str, string.find(str, type)),"%d+")

type2="MOAI_SDK_VERSION_REVISION %d"
revision= string.match(string.sub(str, string.find(str, type2)),"%d+")

if RELEASE_TYPE == 'release' then
	minor = minor+ 1
	revision = 0
end

if RELEASE_TYPE == 'revision' then
	revision = revision + 1
end

util.replaceInFile ( MOAI_SDK_HOME .. 'src/config-default/moai_version_minor.h', {
	[ string.sub(str,string.find(str, type)) ]				= string.format("MOAI_SDK_VERSION_MINOR %i",minor),
})

util.replaceInFile ( MOAI_SDK_HOME .. 'src/config-default/moai_version_minor.h', {
	[ string.sub(str,string.find(str, type2)) ]				= string.format("MOAI_SDK_VERSION_REVISION %i",revision),
})