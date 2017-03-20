----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

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

	local vbo = MOAIVertexBuffer.new ()
	vbo:reserve ( 36 * vertexFormat:getVertexSize ())
	
	writeFace ( vbo, p [ 1 ], p [ 2 ], p [ 3 ], p [ 4 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 4 ], p [ 3 ], p [ 7 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 8 ], p [ 7 ], p [ 6 ], p [ 5 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 5 ], p [ 6 ], p [ 2 ], p [ 1 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 5 ], p [ 1 ], p [ 4 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 2 ], p [ 6 ], p [ 7 ], p [ 3 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])

	local mesh = MOAIMesh.new ()
	mesh:setTexture ( texture )

	mesh:setVertexBuffer ( vbo, vertexFormat )
	mesh:setTotalElements ( vbo:countElements ( vertexFormat ))
	mesh:setBounds ( vbo:computeBounds ( vertexFormat ))
	
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.MESH_SHADER ))
	
	return mesh
end

function makeCube ( size, texture )

	size = size * 0.5
	return makeBoxMesh ( -size, -size, -size, size, size, size, texture )
end

kpmPropMtx = MOAIMatrix.new ()

kpmProp = MOAIProp.new ()
kpmProp:setDeck ( makeCube ( 64, 'test.png' ))
kpmProp:setCullMode ( MOAIGraphicsProp.CULL_BACK )
kpmProp:setParent ( kpmPropMtx )
kpmProp:setLoc ( 0, 0, 32 )

patternPropMtx = MOAIMatrix.new ()

patternProp = MOAIProp.new ()
patternProp:setDeck ( makeCube ( 16, 'moai.png' ))
patternProp:setCullMode ( MOAIGraphicsProp.CULL_BACK )
patternProp:setParent ( patternPropMtx )
patternProp:setLoc ( 0, 0, 8 )

propLayer = MOAIPartitionViewLayer.new ()

MOAIMarkerMgr.setListener (

    MOAIMarkerMgr.EVENT_VIDEO_START,

    function ()

        MOAIMarkerMgr.loadKPMDataSet ( 'pinball' )

        MOAIMarkerMgr.loadPattern ( 'hiro.patt', 40.0 )
        
        local VIEW_WIDTH, VIEW_HEIGHT = MOAIGfxMgr.getViewSize ()
        local VIDEO_HEIGHT, VIDEO_WIDTH = MOAIMarkerMgr.getVideoSize ()
        local VIDEO_ASPECT = ( VIDEO_HEIGHT / VIDEO_WIDTH )

        local XOFF = ( VIEW_WIDTH - ( VIEW_HEIGHT / VIDEO_ASPECT )) / 2
        local YOFF = ( VIEW_HEIGHT - ( VIEW_WIDTH * VIDEO_ASPECT )) / 2

        local viewport = MOAIViewport.new ()
        --viewport:setSize ( 0, YOFF, VIEW_WIDTH, ( VIEW_WIDTH * VIDEO_ASPECT ) + YOFF )
        viewport:setSize ( XOFF, 0, ( VIEW_HEIGHT / VIDEO_ASPECT ) + XOFF, VIEW_HEIGHT )
        viewport:setScale ( VIDEO_WIDTH, VIDEO_HEIGHT )

        local videoLayer = MOAIPartitionViewLayer.new ()
        videoLayer:setViewport ( viewport )
        videoLayer:pushRenderPass ()

        local videoProp = MOAIProp.new ()
        videoProp:setDeck ( MOAIMarkerMgr.getVideoDeck ())
        videoProp:setPartition ( videoLayer )
        videoProp:setRot ( 0, 0, -90 )

        local camera = MOAIMarkerMgr.getVideoCamera ()

        propLayer:setViewport ( viewport )
        propLayer:setCamera ( camera )
        propLayer:pushRenderPass ()
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_KPM_BEGIN,
    function ( pageNo )
        kpmProp:setPartition ( propLayer )
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_KPM_END,
    function ( pageNo )
        kpmProp:setPartition ()
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_KPM_UPDATE,
    function ( pageNo )
        MOAIMarkerMgr.getKPMMatrix ( kpmPropMtx )
        print ( MOAIMarkerMgr.getKPMPosition2D ())
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_MARKER_BEGIN,
    function ( patternID, markerID )
        patternProp:setPartition ( propLayer )
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_MARKER_END,
    function ( patternID, markerID )
        patternProp:setPartition ()
    end
)

MOAIMarkerMgr.setListener (
    MOAIMarkerMgr.EVENT_MARKER_UPDATE,
    function ( patternID, markerID )
        MOAIMarkerMgr.getMarkerMatrix ( markerID, patternPropMtx )
        print ( MOAIMarkerMgr.getMarkerPosition2D ( markerID ))
    end
)

MOAIMarkerMgr.start ()
