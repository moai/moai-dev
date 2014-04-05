----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIGfxDevice.setClearDepth ( true )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setSortMode ( MOAILayer.SORT_NONE ) -- don't need layer sort
MOAISim.pushRenderPass ( layer )

vertexFormat = MOAIVertexFormat.new ()

vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

vbo = MOAIVertexBuffer.new ()
vbo:setFormat ( vertexFormat )
vbo:reserveVerts ( 8 )

-- 1: top back left
vbo:writeFloat ( -64, 64, -64 )
vbo:writeFloat ( 0, 1 )
vbo:writeColor32 ( 0, 1, 1 )

-- 2: top back right
vbo:writeFloat ( 64, 64, -64 )
vbo:writeFloat ( 1, 1 )
vbo:writeColor32 ( 1, 0, 1 )

-- 3: top front right
vbo:writeFloat ( 64, 64, 64 )
vbo:writeFloat ( 1, 0 )
vbo:writeColor32 ( 0, 1, 0 )

-- 4: top front left
vbo:writeFloat ( -64, 64, 64 )
vbo:writeFloat ( 0, 0 )
vbo:writeColor32 ( 1, 0, 0 )

-- 5: bottom back left
vbo:writeFloat ( -64, -64, -64 )
vbo:writeFloat ( 0, 1 )
vbo:writeColor32 ( 1, 1, 1 )

-- 6: bottom back right
vbo:writeFloat ( 64, -64, -64 )
vbo:writeFloat ( 1, 1 )
vbo:writeColor32 ( 1, 0, 0 )

-- 7: bottom front right
vbo:writeFloat ( 64, -64, 64 )
vbo:writeFloat ( 1, 0 )
vbo:writeColor32 ( 0, 0, 1 )

-- 8: bottom front left
vbo:writeFloat ( -64, -64, 64 )
vbo:writeFloat ( 0, 0 )
vbo:writeColor32 ( 1, 1, 0 )

vbo:bless ()

ibo = MOAIIndexBuffer.new ()
ibo:reserve ( 36 )

-- front
ibo:setIndex ( 1, 3 )
ibo:setIndex ( 2, 4 )
ibo:setIndex ( 3, 8 )
ibo:setIndex ( 4, 8 )
ibo:setIndex ( 5, 7 )
ibo:setIndex ( 6, 3 )

-- right
ibo:setIndex ( 7, 2 )
ibo:setIndex ( 8, 3 )
ibo:setIndex ( 9, 7 )
ibo:setIndex ( 10, 7 )
ibo:setIndex ( 11, 6 )
ibo:setIndex ( 12, 2 )

-- back
ibo:setIndex ( 13, 1 )
ibo:setIndex ( 14, 2 )
ibo:setIndex ( 15, 6 )
ibo:setIndex ( 16, 6 )
ibo:setIndex ( 17, 5 )
ibo:setIndex ( 18, 1 )

-- left
ibo:setIndex ( 19, 4 )
ibo:setIndex ( 20, 1 )
ibo:setIndex ( 21, 5 )
ibo:setIndex ( 22, 5 )
ibo:setIndex ( 23, 8 )
ibo:setIndex ( 24, 4 )

-- top
ibo:setIndex ( 25, 2 )
ibo:setIndex ( 26, 1 )
ibo:setIndex ( 27, 4 )
ibo:setIndex ( 28, 4 )
ibo:setIndex ( 29, 3 )
ibo:setIndex ( 30, 2 )

-- bottom
ibo:setIndex ( 31, 8 )
ibo:setIndex ( 32, 5 )
ibo:setIndex ( 33, 6 )
ibo:setIndex ( 34, 6 )
ibo:setIndex ( 35, 7 )
ibo:setIndex ( 36, 8 )

mesh = MOAIMesh.new ()
mesh:setTexture ( "white.png" )
mesh:setVertexBuffer ( vbo )
mesh:setIndexBuffer ( ibo )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setCullMode ( MOAIProp.CULL_BACK )
prop:setDepthTest ( MOAIProp.DEPTH_TEST_LESS_EQUAL )
prop:moveRot ( 0, 360, 0, 3 )
layer:insertProp ( prop )

camera = MOAICamera.new ()
camera:setOrtho ( true )
camera:setShearByZ ( 0, 0.5 )
camera:setLoc ( 0, 100, 200 )
layer:setCamera ( camera )
