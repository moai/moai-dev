--==============================================================
-- args
--==============================================================

VERSION = dofile ( 'version.lua' )

	prepare		= nil
	update		= nil

----------------------------------------------------------------
prepare = function ()

	assert ( VERSION.REVISION )

	util.moaiexec ( 'git rev-parse HEAD > commit.tmp' )
	COMMIT = string.sub ( util.loadFileAsString ( 'commit.tmp' ), 1, 40 )
	MOAIFileSystem.deleteFile ( 'commit.tmp' )

	MOAIFileSystem.copy ( 'moai_version.h.in', MOAI_SDK_HOME .. 'src/moai-core/moai_version.h' )
	util.replaceInFile ( MOAI_SDK_HOME .. 'src/moai-core/moai_version.h', {
		[ '@VERSION_MAJOR@' ]			= VERSION.MAJOR,
		[ '@VERSION_MINOR@' ]			= VERSION.MINOR,
		[ '@VERSION_REVISION@' ]		= VERSION.REVISION,
	})

	MOAIFileSystem.copy ( 'moai_version_author.h.in', MOAI_SDK_HOME .. 'src/config/moai_version_author.h' )
	util.replaceInFile ( MOAI_SDK_HOME .. 'src/config/moai_version_author.h', {
		[ '@COMMIT@' ]					= COMMIT,
	})
end

----------------------------------------------------------------
update = function ()

	MOAIFileSystem.copy ( 'version.lua.in', 'version.lua' )
	util.replaceInFile ( 'version.lua', {
		[ '@VERSION_MAJOR@' ]			= VERSION.MAJOR,
		[ '@VERSION_MINOR@' ]			= VERSION.MINOR,
		[ '@VERSION_REVISION@' ]		= VERSION.REVISION or 'nil',
	})
end

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do

	if escape == 'm' or escape == 'minor' then

		print ( 'INCREMENT' )
		VERSION.MINOR = VERSION.MINOR and ( VERSION.MINOR + 1 ) or 0
		VERSION.REVISION = nil
		update ()
	end

	if escape == 'M' or escape == 'major' then

		print ( 'INCREMENT' )
		VERSION.MAJOR = VERSION.MAJOR and ( VERSION.MAJOR + 1 ) or 0
		VERSION.MINOR = 0
		VERSION.REVISION = nil
		update ()
	end

	if escape == 'i' or escape == 'increment' then

		print ( 'INCREMENT' )
		VERSION.REVISION = VERSION.REVISION and ( VERSION.REVISION + 1 ) or 0
		update ()
	end

	if escape == 'p' or escape == 'prepare' then

		print ( 'PREPARE' )
		prepare ()
	end

	if escape == 'r' or escape == 'reset' then

		print ( 'RESET' )
		VERSION.REVISION = nil
		update ()
	end
end

print ( string.format ( 'MOAI SDK VERSION IS %d.%d.%d', VERSION.MAJOR, VERSION.MINOR, VERSION.REVISION or -1 ))
