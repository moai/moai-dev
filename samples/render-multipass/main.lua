----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

WIN_WIDTH = 320
WIN_HEIGHT = 480

MOAISim.openWindow ( "test", WIN_WIDTH, WIN_HEIGHT )

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
    mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )

	mesh:gfx ()
		:setTexture ( texture )
    	:setShader ( MOAIShaderMgr.MESH_SHADER )
    	:setVertexFormat ( vertexFormat )
		:setVertexBuffer ( vertexBuffer )
    	:setCullFunc ( MOAIGraphicsProp.CULL_BACK )

	return mesh
end

function makeCube ( size, texture )
	size = size * 0.5
	return makeBoxMesh ( -size, -size, -size, size, size, size, texture )
end

local mesh = makeCube ( 128, '../resources/moai.png' )

OFFSCREEN = MOAIPool.addFactory (
	function ()
		local texture = MOAITexture.new ():init ( WIN_WIDTH, WIN_HEIGHT );
		local frameBuffer = MOAIFrameBuffer.new ();
		frameBuffer:addAttachment ( texture, MOAIFrameBuffer.COLOR );
		frameBuffer.color = texture;
		return frameBuffer;
	end
)

offscreenViewport = MOAIViewport.new ()
offscreenViewport:setViewSize ( WIN_WIDTH, WIN_HEIGHT )
offscreenViewport:setViewScale ( WIN_WIDTH, WIN_HEIGHT )

offscreenCamera = MOAICamera.new ()
offscreenCamera:setType ( MOAICamera.CAMERA_TYPE_3D )
offscreenCamera:setLoc ( 0, 0, offscreenCamera:getFocalLength ( WIN_WIDTH ))

offscreenLayer = MOAIPartitionViewLayer.new ()
offscreenLayer:setViewport ( offscreenViewport )
offscreenLayer:setCamera ( offscreenCamera )

offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( mesh )
offscreenProp:moveRot ( 360, 360, 0, 6 )
offscreenProp:setPartition ( offscreenLayer )

function render ( draw )

	local frameBuffer = MOAIPool.provision ( OFFSCREEN, scope )

	draw:setFrameBuffer ( frameBuffer )
	draw:clearSurface ()
	offscreenLayer:render ()

	draw:setFrameBuffer ()
	draw:setTexture ( frameBuffer.color )
	draw:compose ()

	MOAIPool.remit ( frameBuffer )
end

MOAIGfxMgr.pushRenderable ( render )
