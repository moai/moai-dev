----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( 'test', 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local file1 = 'moai.png'
local file2 = 'hexcolors.png'
local filename = file1

texture = MOAITexture.new ()
texture:setReloader ( function ()
	print ( 'LOADER' )
	texture:load ( filename )
	filename = filename == file1 and file2 or file1
end )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

----------------------------------------------------------------
function onMouseLeftEvent ( down )
	if not down then
		MOAIGfxResourceMgr.purgeResources ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )