----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

fmt = MOAIVertexFormat.new ()
fmt:declareCoord ( MOAIVertexFormat.GL_FLOAT, 2 )
fmt:declareColor ( MOAIVertexFormat.GL_UNSIGNED_BYTE, 4 )
fmt:declareUV ( MOAIVertexFormat.GL_FLOAT, 2 )

vbo = MOAIVertexBuffer.new ()
vbo:setFormat ( fmt )
vbo:reserveVerts ( 4 )
vbo:setPrimType ( MOAIVertexBuffer.GL_TRIANGLE_FAN )

vbo:writeFloat ( -64, -64 )
vbo:writeColor ( 1, 0, 0 )
vbo:writeFloat ( 0, 1 )

vbo:writeFloat ( 64, -64 )
vbo:writeColor ( 1, 1, 0 )
vbo:writeFloat ( 1, 1 )

vbo:writeFloat ( 64, 64 )
vbo:writeColor ( 0, 1, 0 )
vbo:writeFloat ( 1, 0 )

vbo:writeFloat ( -64, 64 )
vbo:writeColor ( 0, 0, 1 )
vbo:writeFloat ( 0, 0 )

vbo:bless ()

mesh = MOAIMesh.new ()
mesh:setTexture ( "cathead.png" )
mesh:setVertexBuffer ( vbo )

prop = MOAIProp2D.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "cathead", 320, 480 )
