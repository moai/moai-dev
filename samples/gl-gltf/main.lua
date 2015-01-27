----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

ARRAY_BUFFER			= 34962
ELEMENT_ARRAY_BUFFER	= 34963
DATA_BUFFER				= 'DATA_BUFFER'

MOAIFileSystem.setWorkingDirectory ( '../resources/gltf/duck' )

local json

local fp = io.open ( 'duck.gltf', 'r' )
if fp then
	json = fp:read ( "*all" )
	fp:close ()
end

local gltf = json and MOAIJsonParser.decode ( json )

local gBuffers = {}

for bufferViewName, bufferView in pairs ( gltf.bufferViews ) do
	
	local buffer = gBuffers [ bufferView.buffer ] or {}
	gBuffers [ bufferView.buffer ] = buffer

	local targets = buffer.targets or {}
	buffer.targets = targets

	local targetName = bufferView.target or DATA_BUFFER

	local target = targets [ targetName ] or {}
	targets [ targetName ] = target

	target [ bufferViewName ] = bufferView
end

for bufferName, buffer in pairs ( gBuffers ) do

	print ( 'buffer', bufferName )
	for targetName, target in pairs ( buffer.targets ) do
		print ( '\ttarget', targetName )

		for bufferViewName, bufferView in pairs ( target ) do
			print ( '\t\tbufferView', bufferViewName )
		end
	end
end
