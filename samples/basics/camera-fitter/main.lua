----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

fitter = MOAICameraFitter2D.new ()
fitter:setViewport ( viewport )
fitter:setBounds ( -1000, -64, 1000, 10000 )
fitter:setMin ( 256 )
fitter:start ()
layer:setCamera ( fitter )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( 256, 0, 3 )
layer:insertProp ( prop )

anchor = MOAICameraAnchor2D.new ()
anchor:setParent ( prop )
anchor:setWorldRect ( -64, -64, 64, 64 )
anchor:setScreenRect ( -64, -64, 64, 64 )
fitter:insertAnchor ( anchor )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( -256, 0, 3 )
layer:insertProp ( prop )

anchor = MOAICameraAnchor2D.new ()
anchor:setParent ( prop )
anchor:setWorldRect ( -64, -64, 64, 64 )
anchor:setScreenRect ( -64, -64, 64, 64 )
fitter:insertAnchor ( anchor )

function spin ()

	local r = 0

	while true do
		viewport:setRotation ( 0 )
		r = r + 1
		coroutine:yield ()
	end
end

thread = MOAIThread.new ()
thread:run ( spin )
