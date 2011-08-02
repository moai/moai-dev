----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- Mount archives here
MOAIFileSystem.mount ( "luaArchive.zip", "", 1 )
MOAIFileSystem.mount ( "archive.zip", "", 1 )

-- Set write directory here
--MOAIFileSystem.setWriteDirectory ( )

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

oldLoadFile = loadfile
loadfile = function ( name )
	
	local moaiName = name
	s, e = string.find ( name, ".lua" )
	if e == nil or e ~= string.len ( name ) then
		moaiName = name .. ".lua"
	end
	
	if not MOAIFileSystem.checkFileExists ( moaiName ) then
		print ( "File not found in archives, try normal dofile" )
		return oldLoadFile ( name )
	else 
		return MOAIFileSystem.loadLuaFile ( moaiName )
	end	
end