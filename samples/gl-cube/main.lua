----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
	:setViewSize ( 320, 480 )
	:setViewScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

camera = MOAICamera.new ()
camera:setType ( MOAICamera.CAMERA_TYPE_3D )
print ( camera:getFocalLength ( 320 ))
--camera:moveLoc ( 0, 0, camera:getFocalLength ( 320 ), 6 )
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
--camera:moveLoc ( 0, 0, -500, 6 )
layer:setCamera ( camera )

function makeBoxMesh ( xMin, yMin, zMin, xMax, yMax, zMax, texture )
	
	local function pushPoint ( points, x, y, z )
	
		local point = {}
		point.x = x
		point.y = y
		point.z = z
		
		table.insert ( points, point )
	end

	local function writeTri ( vbo, p1, p2, p3, uv1, uv2, uv3 )
		
		vbo:writeFloat ( p1.x, p1.y, p1.z )
		vbo:writeFloat ( uv1.x, uv1.y )
		vbo:writeColor32 ( 1, 1, 1 )
		
		vbo:writeFloat ( p2.x, p2.y, p2.z )
		vbo:writeFloat ( uv2.x, uv2.y )
		vbo:writeColor32 ( 1, 1, 1 )

		vbo:writeFloat ( p3.x, p3.y, p3.z )
		vbo:writeFloat ( uv3.x, uv3.y  )
		vbo:writeColor32 ( 1, 1, 1 )
	end
	
	local function writeFace ( vbo, p1, p2, p3, p4, uv1, uv2, uv3, uv4 )

		writeTri ( vbo, p1, p2, p4, uv1, uv2, uv4 )
		writeTri ( vbo, p2, p3, p4, uv2, uv3, uv4 )
	end
	
	local p = {}
	
	pushPoint ( p, xMin, yMax, zMax ) -- p1
	pushPoint ( p, xMin, yMin, zMax ) -- p2
	pushPoint ( p, xMax, yMin, zMax ) -- p3
	pushPoint ( p, xMax, yMax, zMax ) -- p4
	
	pushPoint ( p, xMin, yMax, zMin ) -- p5
	pushPoint ( p, xMin, yMin, zMin  ) -- p6
	pushPoint ( p, xMax, yMin, zMin  ) -- p7
	pushPoint ( p, xMax, yMax, zMin  ) -- p8

	local uv = {}
	
	pushPoint ( uv, 0, 0, 0 )
	pushPoint ( uv, 0, 1, 0 )
	pushPoint ( uv, 1, 1, 0 )
	pushPoint ( uv, 1, 0, 0 )
	
	local vertexFormat = MOAIVertexFormat.new ()
	vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
	vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
	vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

	local vertexBuffer = MOAIVertexBuffer.new ()
	vertexBuffer:reserve ( 36 * vertexFormat:getVertexSize ())
	
	writeFace ( vertexBuffer, p [ 1 ], p [ 2 ], p [ 3 ], p [ 4 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vertexBuffer, p [ 4 ], p [ 3 ], p [ 7 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vertexBuffer, p [ 8 ], p [ 7 ], p [ 6 ], p [ 5 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vertexBuffer, p [ 5 ], p [ 6 ], p [ 2 ], p [ 1 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vertexBuffer, p [ 5 ], p [ 1 ], p [ 4 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vertexBuffer, p [ 2 ], p [ 6 ], p [ 7 ], p [ 3 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])

	local mesh = MOAIMesh.new ()

    mesh:setTotalElements ( vertexBuffer:countElements ( vertexFormat ))
    mesh:setBounds ( vertexBuffer:computeBounds ( vertexFormat ))

	mesh:gfx ()
		:setTexture ( texture )
    	:setShader ( MOAIShaderMgr.MESH_SHADER )
    	:setVertexFormat ( vertexFormat )
		:setVertexBuffer ( vertexBuffer )
    	:setCullFunc ( MOAIGraphicsProp.CULL_BACK )

	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )

	return mesh
end

function makeCube ( size, texture )

	size = size * 0.5
	return makeBoxMesh ( -size, -size, -size, size, size, size, texture )
end

local mesh = makeCube ( 128, '../resources/moai.png' )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setLoc ( 0, 0, 0 )
prop:moveRot ( 360, 360, 0, 6 )
prop:setPartition ( layer )
