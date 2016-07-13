----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 520, 680 )

viewport = MOAIViewport.new ()
viewport:setSize ( 520, 680 )
viewport:setScale ( 520, 680 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setFieldOfView ( 65.0 )
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'prevPosition' )
program:setVertexAttribute ( 3, 'nextPosition' )
program:setVertexAttribute ( 4, 'time' )
program:setVertexAttribute ( 5, 'side' )
program:setVertexAttribute ( 6, 'edge' )

program:reserveUniforms ( 6 )
program:declareUniform ( 1, 'transform',	MOAIShaderProgram.UNIFORM_MATRIX_F4 )
program:declareUniform ( 2, 'color',		MOAIShaderProgram.UNIFORM_VECTOR_F4 )
program:declareUniform ( 3, 'viewWidth',	MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 4, 'viewHeight',	MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 5, 'dash',			MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 6, 'radius',		MOAIShaderProgram.UNIFORM_FLOAT )

program:reserveGlobals ( 4 )
program:setGlobal ( 1, 1, MOAIShaderProgram.GLOBAL_WORLD_VIEW_PROJ )
program:setGlobal ( 2, 2, MOAIShaderProgram.GLOBAL_PEN_COLOR )
program:setGlobal ( 3, 3, MOAIShaderProgram.GLOBAL_VIEW_WIDTH )
program:setGlobal ( 4, 4, MOAIShaderProgram.GLOBAL_VIEW_HEIGHT )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

shader = MOAIShader.new ()
shader:setProgram ( program )

shader:setAttr ( 5, 0.0 )
shader:setAttr ( 6, 5.0 )

local vertexFormat = MOAIVertexFormat.new ()
vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 ) -- current
vertexFormat:declareAttribute ( 2, MOAIVertexFormat.GL_FLOAT, 3 ) -- previous
vertexFormat:declareAttribute ( 3, MOAIVertexFormat.GL_FLOAT, 3 ) -- next
vertexFormat:declareAttribute ( 4, MOAIVertexFormat.GL_FLOAT, 1 ) -- time
vertexFormat:declareAttribute ( 5, MOAIVertexFormat.GL_FLOAT, 1 ) -- side
vertexFormat:declareAttribute ( 6, MOAIVertexFormat.GL_FLOAT, 1 ) -- order

local pushVert
local writeLine
local writeVertex

----------------------------------------------------------------
pushVert = function ( verts, x, y, z )

	table.insert ( verts, { x = x, y = y, z = z })
end

----------------------------------------------------------------
writeLine = function ( stream, verts, nFacets )

	for i = 1, #verts - 1 do

        local a = i
        local b = i + 1

        writeVertex ( stream, verts, a,	0, 0, 0 )
        writeVertex ( stream, verts, b, 1, 1, 0 )
        writeVertex ( stream, verts, a, 1, 0, 1 )

        writeVertex ( stream, verts, a, 0, 0, 0 )
        writeVertex ( stream, verts, b, 0, 1, 1 )
        writeVertex ( stream, verts, b, 1, 1, 0 )

        if i < #verts - 1 then 

            for j = 1, nFacets do
            
                local rt0 = ( j - 1 ) / nFacets
                local rt1 = j / nFacets

                local lt0 = 1.0 - rt0
                local lt1 = 1.0 - rt1

                writeVertex ( stream, verts, b, 0, 1, lt0 )
                writeVertex ( stream, verts, b, 1, 1, rt1 )
                writeVertex ( stream, verts, b, 1, 1, rt0 )

                writeVertex ( stream, verts, b, 0, 1, lt0 )
                writeVertex ( stream, verts, b, 0, 1, lt1 )
                writeVertex ( stream, verts, b, 1, 1, rt1 )
            end
        end
    end
end

----------------------------------------------------------------
writeVertex = function ( stream, verts, index, side, order, time )

	local v0 = verts [ index ]

	stream:writeFloat ( v0.x, v0.y, 0.0 )

	local p
	local n

	if side <= 0.0 then

		n = math.max ( index - 1, 1 )
		p = math.min ( index + 1, #verts )
	else
		p = math.max ( index - 1, 1 )
		n = math.min ( index + 1, #verts )
	end

	p = verts [ p ]
	n = verts [ n ]

	stream:writeFloat ( p.x, p.y, 0.0 )
	stream:writeFloat ( n.x, n.y, 0.0 )
	stream:writeFloat ( time )

	if side <= 0.0 then

		stream:writeFloat ( -1.0 )
		stream:writeFloat ( order )
	else
		stream:writeFloat ( 1.0 )
		stream:writeFloat ( order == 0.0 and 1.0 or 0.0 )
	end 
end

local verts = {}

--pushVert ( verts, -100, 100, 0 )
--pushVert ( verts, 100, 100, 0 )
--pushVert ( verts, 100, -100, 0 )
--pushVert ( verts, -100, -100, 0 )
--pushVert ( verts, -100, 100, 0 )

pushVert ( verts, 0, 100, 0 )
pushVert ( verts, 0, 0, 0 )
pushVert ( verts, 100, 0, 0 )
pushVert ( verts, 100, -100, 0 )
pushVert ( verts, -100, -100, 0 )
pushVert ( verts, -100, 100, 0 )
pushVert ( verts, 0, 0, 0 )

--pushVert ( verts, 100, 0, 0 )
--pushVert ( verts, -114.315, 0, 0 )
--pushVert ( verts, -100.0, 0, 0 )
--pushVert ( verts, -50, 0, 0 )

--pushVert ( verts, -100, 2, 0 )
--pushVert ( verts, 100, 2, 0 )
--pushVert ( verts, 100, -1, 0 )
--pushVert ( verts, -100, -2, 0 )

local vtxStream = MOAIMemStream.new ()
writeLine ( vtxStream, verts, 4 )

vtxStream:seek ( 0 )

local vbo = MOAIVertexBuffer.new ()
vbo:copyFromStream ( vtxStream )

local mesh = MOAIMesh.new ()

mesh:setVertexBuffer ( vbo, vertexFormat )

mesh:setTotalElements ( vbo:countElements ( vertexFormat ))
mesh:setBounds ( vbo:computeBounds ( vertexFormat ))

mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( shader )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setDepthMask ( false )
prop:setDepthTest ( MOAIGraphicsProp.DEPTH_TEST_DISABLE )
--prop:setRot ( 0, 270, 0 )
--prop:moveRot ( 0, 360, 0, 12 )
prop:moveRot ( 360, 360, 0, 24 )
--prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
layer:insertProp ( prop )