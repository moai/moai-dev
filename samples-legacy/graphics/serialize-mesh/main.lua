----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 320 )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, 320 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

local vertexFormat = MOAIVertexFormat.new ()
vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
vertexFormat:declareColor ( 2, MOAIVertexFormat.GL_UNSIGNED_BYTE )

function makeBoxMesh ( xMin, yMin, zMin, xMax, yMax, zMax )
	
	local function writeVert ( vbo, x, y, z, r, g, b )
		vbo:writeFloat ( x, y, z )
		vbo:writeColor32 ( r, g, b )
	end
	
	local function writeTri ( ibo, p1, p2, p3 )
		ibo.idx = ibo.idx or 1
		ibo:setIndex ( ibo.idx + 0, p1 )
		ibo:setIndex ( ibo.idx + 1, p2 )
		ibo:setIndex ( ibo.idx + 2, p3 )
		ibo.idx = ibo.idx + 3
	end
	
	local function writeFace ( ibo, p1, p2, p3, p4 )
		writeTri ( ibo, p1, p2, p4 )
		writeTri ( ibo, p2, p3, p4 )
	end

	local vbo = MOAIVertexBuffer.new ()
	vbo:setFormat ( vertexFormat )
	vbo:reserveVerts ( 8 )
	
	writeVert ( vbo, xMin, yMax, zMax, 1, 0, 0 ) -- p1
	writeVert ( vbo, xMin, yMin, zMax, 0, 1, 0 ) -- p2
	writeVert ( vbo, xMax, yMin, zMax, 0, 0, 1 ) -- p3
	writeVert ( vbo, xMax, yMax, zMax, 1, 1, 0 ) -- p4
	
	writeVert ( vbo, xMin, yMax, zMin, 0, 1, 1 ) -- p5
	writeVert ( vbo, xMin, yMin, zMin, 1, 0, 1 ) -- p6
	writeVert ( vbo, xMax, yMin, zMin, 1, 0, 0 ) -- p7
	writeVert ( vbo, xMax, yMax, zMin, 0, 1, 0 ) -- p8
	
	vbo:bless ()
	
	local ibo = MOAIIndexBuffer.new ()
	ibo:reserve ( 36 )
	
	writeFace ( ibo, 1, 2, 3, 4 )
	writeFace ( ibo, 4, 3, 7, 8 )
	writeFace ( ibo, 8, 7, 6, 5 )
	writeFace ( ibo, 5, 6, 2, 1 )
	writeFace ( ibo, 5, 1, 4, 8 )
	writeFace ( ibo, 2, 6, 7, 3 )
	
	ibo.idx = nil

	local serializer = MOAISerializer.new ()
	
	local _getObjectTables = serializer.getObjectTables
	serializer.getObjectTables = function ( self, object, classname )
		print ( 'GET OBJECT TABLES', classname )
		if classname == 'MOAIVertexFormat' then return end
		return _getObjectTables ( self, object, classname )
	end

	serializer:serializeToFile ( 'buffers.lua', { vertexFormat = vertexFormat, vbo = vbo, ibo = ibo })

	local mesh = MOAIMesh.new ()
	mesh:setVertexBuffer ( vbo )
	mesh:setIndexBuffer ( ibo )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	
	return mesh
end

function makeCube ( size )
	
	size = size * 0.5
	return makeBoxMesh ( -size, -size, -size, size, size, size )
end

function loadMesh ( filename )
	
	deserializer = MOAIDeserializer.new ()
	
	local _createObject = deserializer.createObject
	deserializer.createObject = function ( self, classname )
		print ( 'CREATE', classname )
		if classname == 'MOAIVertexFormat' then return vertexFormat end
		return _createObject ( self, classname )
	end
	
	local _initObject = deserializer.initObject
	deserializer.initObject = function ( self, object, classname, userTable, memberTable, initTable )
		print ( 'INIT', classname )
		if classname == 'MOAIVertexFormat' then return end
		_initObject ( self, object, classname, userTable, memberTable, initTable )
	end
	
	local buffers = loadfile ( filename )( deserializer )
	
	local vertexFormat, vbo, ibo = buffers.vertexFormat, buffers.vbo, buffers.ibo
	
	vbo:setFormat ( vertexFormat )
	vbo:bless ()
	
	local mesh = MOAIMesh.new ()
	mesh:setVertexBuffer ( vbo )
	mesh:setIndexBuffer ( ibo )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	
	return mesh
end

prop = MOAIProp.new ()
prop:setDeck ( makeCube ( 96 ))
prop:setLoc ( -64, 0, 0 )
prop:moveRot ( 360, 360, 0, 6 )
prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
layer:insertProp ( prop )

prop = MOAIProp.new ()
prop:setDeck ( loadMesh ( 'buffers.lua' ))
prop:setLoc ( 64, 0, 0 )
prop:moveRot ( 360, 360, 0, 6 )
prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
layer:insertProp ( prop )
