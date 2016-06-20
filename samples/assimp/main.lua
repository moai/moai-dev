----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

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

local vertexFormat = MOAIVertexFormat.new ()
vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
vertexFormat:declareColor ( 2, MOAIVertexFormat.GL_UNSIGNED_BYTE )

aiScene = MOAIAssimpScene.new ()
aiScene:load ( 'collada/duck.dae',
	MOAIAssimpScene.TRIANGULATE,
	MOAIAssimpScene.OPTIMIZE_MESHES,
	MOAIAssimpScene.IMPROVE_CACHE_LOCALITY
)

local aiMeshes = aiScene:getMeshes ()

for i, aiMesh in ipairs ( aiMeshes or {}) do

	print ( 'FOUND A MESH!' )

	local vbo = MOAIVertexBuffer.new ()
	local ibo = MOAIIndexBuffer.new ()
	
	aiMesh:getVertices ( vbo, vertexFormat )
	local nIndices = aiMesh:getIndices ( ibo, 4 )
	
	local mesh = MOAIMesh.new ()

	mesh:setVertexBuffer ( vbo, vertexFormat )
	mesh:setIndexBuffer ( ibo )
	mesh:setTotalElements ( nIndices )
	mesh:setBounds ( vbo:computeBounds ( vertexFormat ))
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
	
	local prop = MOAIProp.new ()
	prop:setDeck ( mesh )
	--prop:setScl ( 100, 100, 100 )
	prop:moveRot ( 360, 360, 0, 6 )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	layer:insertProp ( prop )
end
