----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 1024, 1024 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 1024 )
viewport:setScale ( 1024, -1024 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setRect ( -512, -512, 512, 512 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

function main ()

	while true do
	
		local texture = MOAITexture.new ()
		texture:load ( "cathead.png", MOAITexture.TRUECOLOR )
		gfxQuad:setTexture ( texture )
		
		for i = 1, 100 do
			coroutine.yield ()
		end
		
		texture:release ()
		texture = nil
		
		coroutine.yield ()
		
		collectgarbage ( 'collect' )
		
		coroutine.yield ()
	
	end
end

thread = MOAIThread.new ()
thread:run ( main )
