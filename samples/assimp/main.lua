----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--FILENAME = '../../3rdparty/assimp/test/models-nonbsd/ogre/ogresdk/ninja.mesh'
--FILENAME = '../../3rdparty/assimp/test/models/collada/duck.dae'
FILENAME = '../temp/assimp/boblampclean.md5mesh'

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 1024, 1024 )
MOAIGfxMgr.setClearColor ( 0.5, 0.5, 0.5, 1 )

frameBuffer = MOAIGfxMgr.getFrameBuffer ()
frameBuffer:setClearDepth ( true )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 1024 )
viewport:setScale ( 1024, 1024 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
--camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
camera:setLoc ( 0, 0, camera:getFocalLength ( 100 ))
layer:setCamera ( camera )

local vertexFormat = MOAIVertexFormat.new ()
vertexFormat:declareCoord ()
vertexFormat:declareUV ()
vertexFormat:declareColor ()
vertexFormat:declareBoneIndices ()
vertexFormat:declareBoneWeights ()
vertexFormat:declareBoneCount ()

local program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
--program:setVertexAttribute( 2, 'normal' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'world', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4 )
program:declareUniform ( 2, 'viewProj', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4 )

program:reserveGlobals ( 2 )
program:setGlobal ( 1, MOAIShaderProgram.GLOBAL_WORLD, 1 )
program:setGlobal ( 2, MOAIShaderProgram.GLOBAL_VIEW_PROJ, 2 )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

local shader = MOAIShader.new ()
shader:setProgram ( program )

aiScene = MOAIAssimpScene.new ()
aiScene:load ( FILENAME,
	MOAIAssimpScene.TRIANGULATE,
	MOAIAssimpScene.FLIP_UVS
)

local aiMeshes		= aiScene:getMeshes ()
local aiMaterials	= aiScene:getMaterials ()
local aiRootNode	= aiScene:getRootNode ()

--if aiRootNode then
--	local json = MOAIJsonParser.encode ( aiRootNode, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
--	print ( json )
--end

local textures = {}

for i, aiMaterial in ipairs ( aiMaterials ) do

	local path = '../temp/assimp/' .. aiMaterial.TEXTURE_DIFFUSE [ 1 ].path
	print ( path )

	local texture = MOAITexture.new ()
	texture:load ( path )

	textures [ i ] = texture

	--local json = MOAIJsonParser.encode ( aiMaterial, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
	--print ( json )
	--print ()
end

for i, aiMesh in ipairs ( aiMeshes ) do

	print ( 'FOUND A MESH!' )

	local aiBones = aiMesh:getBones ()

	if aiBones then
		local json = MOAIJsonParser.encode ( aiBones, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
		print ( json )
	end

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
	mesh:setShader ( shader )
	--mesh:setTexture ( '../../3rdparty/assimp/test/models/collada/duckCM.tga' )
	mesh:setTexture ( textures [ aiMesh:getMaterialIndex ()])
	
	local prop = MOAIProp.new ()
	prop:setDeck ( mesh )
	prop:setRot ( -90, 0, 0 )
	prop:setLoc ( 0, -40, 0 )
	--prop:setLoc ( 0, -80, 0 )
	--prop:setRot ( 0, -90, 0 )
	prop:moveRot ( 0, 360, 0, 6 )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	prop:setDepthTest ( MOAIGraphicsProp.DEPTH_TEST_LESS )
	layer:insertProp ( prop )
end
