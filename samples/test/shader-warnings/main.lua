----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

--[[
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 180, 0, 3 )
prop:moveLoc ( 0, 0, 128, 3 )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )
]]--

vertexFormat = MOAIVertexFormat.new ()

-- Moai's default shaders expect loc, uv, color
vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 2 )
vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

vbo = MOAIVertexBuffer.new ()
vbo:setFormat ( vertexFormat )
vbo:reserveVerts ( 4 )

vbo:writeFloat ( -64, -64 )
vbo:writeFloat ( 0, 1 )
vbo:writeColor32 ( 1, 0, 0 )

vbo:writeFloat ( 64, -64 )
vbo:writeFloat ( 1, 1 )
vbo:writeColor32 ( 1, 1, 0 )

vbo:writeFloat ( 64, 64 )
vbo:writeFloat ( 1, 0 )
vbo:writeColor32 ( 0, 1, 0 )

vbo:writeFloat ( -64, 64 )
vbo:writeFloat ( 0, 0 )
vbo:writeColor32 ( 0, 0, 1 )

vbo:bless ()

mesh = MOAIMesh.new ()
mesh:setTexture ( "moai.png" )
mesh:setVertexBuffer ( vbo )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLE_FAN )

prop = MOAIProp2D.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )