----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

if viewport.setScale then print ( 'has setScale ()' ) end

print ( viewport:getClassName ())

vpClass = viewport:getClass ()
print ( vpClass:getClassName ())

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

print ( layer:getClassName ())

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

print ( gfxQuad:getClassName ())

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )
prop:moveLoc ( 64, 0, 1.5 )
prop:moveScl ( 1, 1, 1.5 )

print ( prop:getClassName ())

anim = MOAIAnim.new ()
anim = nil
collectgarbage ( 'collect' )
print ( 'bleh' )