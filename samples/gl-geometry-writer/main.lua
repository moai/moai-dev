----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

frameBuffer = MOAIGfxMgr.getFrameBuffer ()
frameBuffer:setClearDepth ( true )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

function makeCube ( size )
	
	local y = 0

	local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

	local vtxStream = MOAIMemStream.new ()
	local idxStream = MOAIMemStream.new ()

	for i = 1, 16 do

		local hSize = size * 0.5

		MOAIGeometryWriter.writeCube ( vtxFormat, vtxStream, size, 0, y, 0 )

		MOAIGeometryWriter.applyLinearGradient ( vtxFormat, vtxStream,
			-hSize, y - hSize, -hSize,
			hSize, y + hSize, hSize,
			0, 0, 0, 1,
			1, 0, 0, 1,
			false,
			false,
			MOAIGeometryWriter.COLOR_OVERWRITE
		)
		
		vtxStream:seek ( 0, MOAIMemStream.SEEK_END )

		size = hSize
		y = y + ( size * 1.5 )
	end

	vtxStream:seek ( 0 )
	MOAIGeometryWriter.snapCoords ( vtxFormat, vtxStream, 0.1 )
	MOAIGeometryWriter.pruneVertices ( vtxFormat, vtxStream, idxStream )

	local mesh = MOAIGeometryWriter.getMesh ( vtxFormat, vtxStream, idxStream, 2 )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

	return mesh
end

local mesh = makeCube ( 128 )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 0, 360, 6 )
prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
prop:setDepthTest ( MOAIGraphicsProp.DEPTH_TEST_LESS )
layer:insertProp ( prop )
