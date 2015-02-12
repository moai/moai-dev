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

local function addPoint ( points, x, y, z )

	local point = {}
	point.x = x
	point.y = y
	point.z = z

	table.insert ( points, point )
end

local function vec_cross(p1,p2)
	local ret = {}
	ret.x = p1.y*p2.z - p1.z*p2.y
	ret.y = p1.z*p2.x - p1.x*p2.z
	ret.z = p1.x*p2.y - p1.y*p2.x
	return ret
end

local function vec_add(p1,p2)
	local ret = {}
	ret.x = p1.x + p2.x
	ret.y = p1.y + p2.y
	ret.z = p1.z + p2.z
	return ret
end

local function vec_scale(const,p1)
	local ret = {}
	ret.x = p1.x * const
	ret.y = p1.y * const
	ret.z = p1.z * const
	return ret
end

local function vec_length(p1)
	return math.sqrt(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z)
end

local function vec_normalize(p1)
	return vec_scale(1/vec_length(p1),p1)
end

function makeBoxMesh ( xMin, yMin, zMin, xMax, yMax, zMax, texture )

	local function pushPoint ( points, x, y, z )

		local point = {}
		point.x = x
		point.y = y
		point.z = z

		table.insert ( points, point )
	end

	local function writeTri ( vbo, p1, p2, p3, uv1, uv2, uv3 )

		local normal = vec_cross(vec_add(p2,vec_scale(-1,p1)),vec_add(p3,vec_scale(-1,p1)))
		normal = vec_normalize(normal)

		vbo:writeFloat(p1.x, p1.y, p1.z)
		vbo:writeFloat(normal.x,normal.y,normal.z)
		vbo:writeFloat(uv1.x, uv1.y)
		vbo:writeColor32(0,0,0)

		vbo:writeFloat(p2.x, p2.y, p2.z)
		vbo:writeFloat(normal.x,normal.y,normal.z)
		vbo:writeFloat(uv2.x, uv2.y)
		vbo:writeColor32(0,0,0)

		vbo:writeFloat(p3.x, p3.y, p3.z)
		vbo:writeFloat(normal.x,normal.y,normal.z)
		vbo:writeFloat(uv3.x, uv3.y)
		vbo:writeColor32(0,0,0)
	end

	local function writeFace ( vbo, p1, p2, p3, p4, uv1, uv2, uv3, uv4 )
		writeTri( vbo, p1, p2, p4, uv1, uv2, uv4 )
		writeTri( vbo, p2, p3, p4, uv2, uv3, uv4 )
	end

	local p = {}

	--box points
	addPoint ( p, xMin, yMax, zMax ) -- p1
	addPoint ( p, xMin, yMin, zMax ) -- p2
	addPoint ( p, xMax, yMin, zMax ) -- p3
	addPoint ( p, xMax, yMax, zMax ) -- p4

	addPoint ( p, xMin, yMax, zMin ) -- p5
	addPoint ( p, xMin, yMin, zMin  ) -- p6
	addPoint ( p, xMax, yMin, zMin  ) -- p7
	addPoint ( p, xMax, yMax, zMin  ) -- p8

	local uv = {}
	addPoint ( uv, 0, 0, 0 )
	addPoint ( uv, 0, 1, 0 )
	addPoint ( uv, 1, 1, 0 )
	addPoint ( uv, 1, 0, 0 )

	local vertexFormat = MOAIVertexFormat.new ()
	vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
	vertexFormat:declareNormal ( 2, MOAIVertexFormat.GL_FLOAT, 3 )
	vertexFormat:declareUV ( 3, MOAIVertexFormat.GL_FLOAT, 2 )
	vertexFormat:declareColor ( 4, MOAIVertexFormat.GL_UNSIGNED_BYTE )

	local vbo = MOAIGfxBuffer.new ()
	vbo:reserve ( 36 * vertexFormat:getVertexSize ())

	writeFace ( vbo, p [ 1 ], p [ 2 ], p [ 3 ], p [ 4 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 4 ], p [ 3 ], p [ 7 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 8 ], p [ 7 ], p [ 6 ], p [ 5 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 5 ], p [ 6 ], p [ 2 ], p [ 1 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 5 ], p [ 1 ], p [ 4 ], p [ 8 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])
	writeFace ( vbo, p [ 2 ], p [ 6 ], p [ 7 ], p [ 3 ], uv [ 1 ], uv [ 2 ], uv [ 3 ], uv [ 4 ])

	local file = assert ( io.open ( 'shader.vsh', mode ))
	local vsh = file:read ( '*all' )
	file:close ()

	file = assert ( io.open ( 'shader.fsh', mode ))
	local fsh = file:read ( '*all' )
	file:close ()

	local program = MOAIShaderProgram.new ()

	program:setVertexAttribute( 1, 'position' )
	program:setVertexAttribute( 2, 'normal' )
	program:setVertexAttribute( 3, 'uv' )
	program:setVertexAttribute( 4, 'color' )
	
	program:reserveUniforms ( 2 )
	program:declareUniform ( 1, 'transform', MOAIShaderProgram.UNIFORM_MATRIX_F4 )
	program:declareUniform ( 2, 'normalTransform', MOAIShaderProgram.UNIFORM_MATRIX_F3 )
	
	program:reserveGlobals ( 2 )
	program:setGlobal ( 1, 1, MOAIShaderProgram.GLOBAL_WORLD_VIEW_PROJ )
	program:setGlobal ( 2, 2, MOAIShaderProgram.GLOBAL_WORLD_VIEW_PROJ_NORM )
	
	program:load ( vsh, fsh )

	local shader = MOAIShader.new ()
	shader:setProgram ( program )
	
	local mesh = MOAIMesh.new ()

	mesh:setVertexBuffer ( vbo, vertexFormat )
	mesh:setTotalElements ( vbo:countElements ( vertexFormat ))
	mesh:setBounds ( vbo:computeBounds ( vertexFormat ))

	mesh:setShader ( shader )
	mesh:setTexture ( texture )
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )

	return mesh
end

function makeCube ( size, texture )

	size = size * 0.5
	return makeBoxMesh ( -size, -size, -size, size, size, size, texture )
end

local mesh = makeCube ( 128, 'moai.png' )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 360, 0, 12 )
prop:setCullMode ( MOAIProp.CULL_BACK )
layer:insertProp ( prop )