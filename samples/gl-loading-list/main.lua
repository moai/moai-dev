----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- this sample is only meaningful when runing in a multi-threaded host that implements
-- a separate loading display list and thread. see SDL desktop host for an example
-- of this.

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture1 = MOAITexture.new ()
texture1:load ( '../resources/moai.png' )
texture1:scheduleForGPUCreate ( MOAIGfxMgr.LOADING_LIST )

texture2 = MOAITexture.new ()
texture2:load ( '../resources/test.png' )
texture2:scheduleForGPUCreate ( MOAIGfxMgr.LOADING_LIST )

onCreated = function ( texture )

	print ( 'CREATED TEXTURE:', texture )

	texture.ready = true

	if texture1.ready and texture2.ready then

		print ( 'READY' )

		gfxQuad = MOAIGfxQuad2D.new ()
		gfxQuad:setTexture ( texture1 )
		gfxQuad:setRect ( -128, -128, 128, 128 )
		gfxQuad:setUVRect ( 0, 0, 1, 1 )

		prop = MOAIProp2D.new ()
		prop:setDeck ( gfxQuad )
		layer:insertProp ( prop )
		prop:moveRot ( 360, 5 )

		gfxQuad = MOAIGfxQuad2D.new ()
		gfxQuad:setTexture ( texture2 )
		gfxQuad:setRect ( -128, -128, 128, 128 )
		gfxQuad:setUVRect ( 0, 0, 1, 1 )

		prop = MOAIProp2D.new ()
		prop:setDeck ( gfxQuad )
		prop:setColor ( 1, 1, 1, 0 )
		layer:insertProp ( prop )
		prop:moveRot ( -360, 5 )
	end
end

texture1:setListener ( MOAITexture.GFX_EVENT_CREATED, function () onCreated ( texture1 ) end )
texture2:setListener ( MOAITexture.GFX_EVENT_CREATED, function () onCreated ( texture2 ) end )
