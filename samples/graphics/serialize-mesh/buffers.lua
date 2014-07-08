--MOAI
local deserializer = ... or MOAIDeserializer.new ()
local base64 = MOAIDeserializer.base64Decode

local function init ( objects )

	--Initializing Tables
	local table

	table = objects [ 0x01 ]
	table.vbo = objects [ 0x02 ]
	table.ibo = objects [ 0x04 ]
	table.vertexFormat = objects [ 0x06 ]

	table = objects [ 0x03 ]
	table.mBuffer = base64 ( 'TYxBDgAgCMP6NOLT+dgUUCQZh5JuYA624gQnyfVT8mpGj8tX++VIs+/6/mVm32N/A2==' )

	table = objects [ 0x05 ]
	table.mTotalIndices = 36
	table.mHint = 22
	table.mIndices = 'LcqxEcAwDAMxylKs/Td2ChS8R8EkqX+th5tXLy+PfjwcLR5dbS6/o5cf'

	deserializer:initObject ( objects [ 0x02 ], 'MOAIVertexBuffer', nil, nil, objects [ 0x03 ]) --MOAIVertexBuffer
	deserializer:initObject ( objects [ 0x04 ], 'MOAIIndexBuffer', nil, nil, objects [ 0x05 ]) --MOAIIndexBuffer
	deserializer:initObject ( objects [ 0x06 ], 'MOAIVertexFormat', nil, nil, nil) --MOAIVertexFormat
end

--Declaring Objects
local objects = {

	--Declaring Tables
	[ 0x01 ] = {},
	[ 0x03 ] = {},
	[ 0x05 ] = {},

	--Declaring Instances
	[ 0x02 ] = deserializer:registerObjectID ( deserializer:createObject ( 'MOAIVertexBuffer' ), 0x02 ),
	[ 0x04 ] = deserializer:registerObjectID ( deserializer:createObject ( 'MOAIIndexBuffer' ), 0x04 ),
	[ 0x06 ] = deserializer:registerObjectID ( deserializer:createObject ( 'MOAIVertexFormat' ), 0x06 ),

}

init ( objects )

--Returning Root
return objects [ 0x01 ]
