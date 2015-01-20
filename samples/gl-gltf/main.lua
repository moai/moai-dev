----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIFileSystem.setWorkingDirectory ( '../resources/gltf/duck' )

local json

local fp = io.open ( 'duck.gltf', 'r' )
if fp then
	json = fp:read ( "*all" )
	fp:close ()
end

local gltf = json and MOAIJsonParser.decode ( json )

print ( 'GLTF', gltf )

for k, v in pairs ( gltf ) do
	print ( k, v )
end