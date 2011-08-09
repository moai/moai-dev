----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- Mount archives here - you can mount non archived folders on system drive as well, i.e. C: on windows - then have access to entire C: drive
MOAIFileSystem.mount ( "luaArchive.zip", "", 1 )
MOAIFileSystem.mount ( "archive.zip", "", 1 )

-- Set write directory here ( writing not yet supported, though directory can be set )
--MOAIFileSystem.setWriteDirectory ( )

-- Save a reference to require, then write our own to check the physfs file system
oldRequire = require
require = function ( name )
	
	local moaiName = name
	s, e = string.find ( name, ".lua" )
	if e == nil or e ~= string.len ( name ) then
		moaiName = name .. ".lua"
	end
	
	if not package.loaded [ moaiName ] then
	
		if not MOAIFileSystem.checkFileExists ( moaiName ) then
			print ( "File not found in archives, try normal require" )
			return oldRequire ( name )
		end
		
		package.loaded [ moaiName ] = true
		local res = MOAIFileSystem.loadAndRunLuaFile ( moaiName )
		if res ~= nil then
			package.loaded [ moaiName ] = res
		end
	end
	
	return package.loaded [ moaiName ]
end

-- Save a reference to dofile, then write our own to check the physfs file system
oldDoFile = dofile
dofile = function ( name )
	
	local moaiName = name
	s, e = string.find ( name, ".lua" )
	if e == nil or e ~= string.len ( name ) then
		moaiName = name .. ".lua"
	end
	
	if not MOAIFileSystem.checkFileExists ( moaiName ) then
		print ( "File not found in archives, try normal dofile" )
		return oldDofile ( name )
	else 
		return MOAIFileSystem.loadAndRunLuaFile ( moaiName )
	end	
end

-- Save a reference to loadfile, then write our own to check the physfs file system
oldLoadFile = loadfile
loadfile = function ( name )
	
	local moaiName = name
	s, e = string.find ( name, ".lua" )
	if e == nil or e ~= string.len ( name ) then
		moaiName = name .. ".lua"
	end
	
	if not MOAIFileSystem.checkFileExists ( moaiName ) then
		print ( "File not found in archives, try normal loadfile" )
		return oldLoadFile ( name )
	else 
		return MOAIFileSystem.loadLuaFile ( moaiName )
	end	
end