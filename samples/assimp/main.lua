----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--FILENAME = '../../3rdparty/assimp/test/models-nonbsd/ogre/ogresdk/ninja.mesh'
FILENAME = '../../3rdparty/assimp/test/models/collada/duck.dae'

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

frameBuffer = MOAIGfxMgr.getFrameBuffer ()
frameBuffer:setClearDepth ( true )

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
vertexFormat:declareCoord ()
vertexFormat:declareUV ()
vertexFormat:declareColor ()
vertexFormat:declareBoneIndices ()
vertexFormat:declareBoneWeights ()
vertexFormat:declareBoneCount ()

aiScene = MOAIAssimpScene.new ()
aiScene:load ( FILENAME,
	MOAIAssimpScene.TRIANGULATE,
	MOAIAssimpScene.FLIP_UVS
)

local aiMeshes = aiScene:getMeshes ()
local aiMaterials = aiScene:getMaterials ()

for i, aiMaterial in ipairs ( aiMaterials ) do

	print ( 'FOUND A MATERIALS!' )

	local json = MOAIJsonParser.encode ( aiMaterial, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
	print ( json )
	print ()
end

for i, aiMesh in ipairs ( aiMeshes ) do

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
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.MESH_SHADER ))
	mesh:setTexture ( '../../3rdparty/assimp/test/models/collada/duckCM.tga' )
	
	local prop = MOAIProp.new ()
	prop:setDeck ( mesh )
	prop:setLoc ( 0, -80, 0 )
	prop:setRot ( 0, -90, 0 )
	prop:moveRot ( 0, 360, 0, 6 )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	prop:setDepthTest ( MOAIGraphicsProp.DEPTH_TEST_LESS )
	layer:insertProp ( prop )
end
