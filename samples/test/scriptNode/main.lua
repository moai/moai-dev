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

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
sprite:moveRot ( 360, 1.5 )

layer:insertProp ( sprite )
MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "cathead", 320, 480 )

function onUpdate ( self )
	print ( self.name )
end

node1 = MOAIScriptNode.new ()
node1.name = "Node 1"
node1:setCallback ( onUpdate )

node2 = MOAIScriptNode.new ()
node2.name = "Node 2"
node2:setDependency ( node1 )
node2:setCallback ( onUpdate )

function main ()
	while true do
		node1:scheduleUpdate ()
		coroutine.yield ()
	end
end

thread = MOAIThread.new ()
thread:run ( main )
