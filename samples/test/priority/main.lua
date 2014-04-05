----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, gsl!" )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -64, -64, 64, 64 )

function addBtn ( scene, name, x, y, priority )

	sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	sprite:setPriority ( priority )
	sprite:setLoc ( x, y )
	
	partition:insertProp ( sprite )

end

scene = MOAILayer2D.new ()
MOAISim.pushRenderPass ( scene )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

camera = MOAITransform.new ()
scene:setCamera ( camera )

addBtn ( scene, "btn1.png", -0, -0, -2 )
addBtn ( scene, "btn2.png", -32, -32, -1 )
addBtn ( scene, "btn3.png", -64, -64, 0 )
addBtn ( scene, "btn4.png", -96, -96, 1 )
addBtn ( scene, "btn1.png", -128, -128, 2 )

MOAISim.openWindow ( "cathead", 320, 480 )
