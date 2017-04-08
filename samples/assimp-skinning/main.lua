----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.setGCActive ( false )

local printAsJson = function ( t )
	if t then
		print ( t and MOAIJsonParser.encode ( t, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS ))
	end
end

--FILENAME = '../../3rdparty/assimp/test/models-nonbsd/ogre/ogresdk/ninja.mesh'
--FILENAME = '../../3rdparty/assimp/test/models/collada/duck.dae'
FILENAME = '../../../moai-content/guard/boblampclean.md5mesh'

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 1024, 1024 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 1024 )
viewport:setScale ( 1024, 1024 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setClearDepth ( true )
layer:setClearColor ( 0.5, 0.5, 0.5, 1 )
layer:pushRenderPass ()

camera = MOAICamera.new ()
camera:setType ( MOAICamera.CAMERA_TYPE_3D )
--camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
camera:setLoc ( 0, 0, camera:getFocalLength ( 100 ))
layer:setCamera ( camera )

local vertexFormat = MOAIVertexFormat.new ()
vertexFormat:declareCoord ()
vertexFormat:declareUV ()
vertexFormat:declareColor ()
vertexFormat:declareBoneIndices ()
vertexFormat:declareBoneWeights ()
--vertexFormat:declareBoneCount ()

local program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )
program:setVertexAttribute ( 4, 'boneIndices' )
program:setVertexAttribute ( 5, 'boneWeights' )
program:setVertexAttribute ( 6, 'boneCount' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'transforms', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4, 2 )
program:declareUniform ( 2, 'bones', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4, 1 )

program:reserveGlobals ( 2 )
program:setGlobal ( 1, MOAIShaderProgram.GLOBAL_MODEL_TO_WORLD_MTX, 1, 1 )
program:setGlobal ( 2, MOAIShaderProgram.GLOBAL_WORLD_TO_CLIP_MTX, 1, 2 )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

aiScene = MOAIAssimpScene.new ()
aiScene:load ( FILENAME,
	MOAIAssimpScene.TRIANGULATE,
	MOAIAssimpScene.FLIP_UVS
)

print ( 'ANIMATIONS', aiScene:countAnimations ())

local aiMeshes		= aiScene:getMeshes ()
local aiMaterials	= aiScene:getMaterials ()
local aiRootNode	= aiScene:getRootNode ()
local aiAnimations	= aiScene:getAnimations ()

--print ( 'NODEZ!' )
--printAsJson ( aiRootNode )

makeTransform = function ( mtx )

	--print ( mtx [ 1 ][ 1 ], mtx [ 1 ][ 2 ], mtx [ 1 ][ 3 ], mtx [ 1 ][ 4 ])
	--print ( mtx [ 2 ][ 1 ], mtx [ 2 ][ 2 ], mtx [ 2 ][ 3 ], mtx [ 2 ][ 4 ])
	--print ( mtx [ 3 ][ 1 ], mtx [ 3 ][ 2 ], mtx [ 3 ][ 3 ], mtx [ 3 ][ 4 ])

	local transform = MOAIMatrix.new ()
	transform:setMatrix (
		mtx [ 1 ][ 1 ], mtx [ 1 ][ 2 ], mtx [ 1 ][ 3 ], mtx [ 1 ][ 4 ],
		mtx [ 2 ][ 1 ], mtx [ 2 ][ 2 ], mtx [ 2 ][ 3 ], mtx [ 2 ][ 4 ],
		mtx [ 3 ][ 1 ], mtx [ 3 ][ 2 ], mtx [ 3 ][ 3 ], mtx [ 3 ][ 4 ]
	)
	return transform
end

local buildTransformTree
buildTransformTree = function ( aiNode, directory )

	print ( aiNode.name )

	local transform = makeTransform ( aiNode.transformation )
	--local control = MOAIMatrix.new ()
	--control:setParent ( transform )

	if directory and aiNode.name then
		directory [ aiNode.name ] = transform
	end

	for i, aiChild in ipairs ( aiNode.children or {}) do
		local child = buildTransformTree ( aiChild, directory )
		child:setParent ( transform )
	end

	return transform
end

nodeDirectory = {}
local root = buildTransformTree ( aiRootNode, nodeDirectory )

local invRoot = MOAIMatrix.new ()
invRoot:setMatrix ( root:getMatrix ())
invRoot:invert ()
root:setParent ( invRoot )

printMatrix = function ( r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4 )

	print ( r1c1, r1c2, r1c3, r1c4 )
	print ( r2c1, r2c2, r2c3, r2c4 )
	print ( r3c1, r3c2, r3c3, r3c4 )
end

timer = MOAITimer.new ()

if aiAnimations [ 1 ] then

	local aiAnimation = aiAnimations [ 1 ]

	local curves = {}
	local nChannels = aiAnimation:countChannels ()
	local duration = aiAnimation:getDuration ()

	--nChannels = 0

	timer:setSpan ( 0, duration )
	timer:setMode ( MOAITimer.LOOP )
	timer:start ()

	for j = 1, nChannels do
		local channelName = aiAnimation:getChannelName ( j )
		if not curves [ channelName ] then

			local curve = aiAnimation:getAnimCurve ( j )
			curves [ channelName ] = curve

			curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

			local node = nodeDirectory [ channelName ]
			if node then
				node:setAttrLink ( MOAIMatrix.ATTR_MATRIX, curve, MOAIAnimCurve.ATTR_VALUE )
			end
		end
	end

	aiAnimation.timer = timer
	aiAnimation.curves = curves
end

local textures = {}

for i, aiMaterial in ipairs ( aiMaterials ) do

	local path = '../../../moai-content/guard/' .. aiMaterial.TEXTURE_DIFFUSE [ 1 ].path
	print ( path )

	local texture = MOAITexture.new ()
	texture:load ( path )

	textures [ i ] = texture

	--local json = MOAIJsonParser.encode ( aiMaterial, MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
	--print ( json )
	--print ()
end

for i, aiMesh in ipairs ( aiMeshes ) do

	local meshName = aiMesh:getName ()
	print ( 'FOUND A MESH!', meshName )

	local aiBones = aiMesh:getBones ()

	local shader = MOAIShader.new ()
	shader:setProgram ( program )
	shader:resizeUniformArray ( 2, #aiBones )

	local bones = {}

	--print ( 'BONEZ!' )
	--printAsJson ( aiBones )

	for j, aiBone in ipairs ( aiBones ) do

		--print ( 'BONE', j, aiBone.name )

		local bone = makeTransform ( aiBone.offsetMatrix )
		bone:setParent ( nodeDirectory [ aiBone.name ])
		bones [ j ] = bone

		shader:setAttrLink ( shader:getAttributeID ( 2, j ), bone, MOAIMatrix.TRANSFORM_TRAIT )
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
	mesh:setTexture ( textures [ aiMesh:getMaterialIndex ()])
	
	local prop = MOAIProp.new ()
	prop:setDeck ( mesh )
	prop:setRot ( -90, 0, 0 )
	prop:setLoc ( 0, -40, 0 )
	--prop:moveRot ( 0, 360, 0, 6 )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	prop:setDepthTest ( MOAIGraphicsProp.DEPTH_TEST_LESS )
	prop:setPartition ( layer )

	prop.bones = bones
end
