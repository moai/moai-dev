----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
MOAISim.pushRenderPass ( layer )

drawPoints = function ( stream )

	local total = stream:getLength () / 8
	stream:seek ( 0 )

	MOAIGfxMgr.setPenColor ( 0, 0, 1, 1 )

	for i = 1, total do

		local x = stream:readFloat ()
		local y = stream:readFloat ()

		MOAIDraw.fillCircle ( x, y, 2 )
	end
end

points = MOAIMemStream.new ()
region = MOAIRegion.new ()

onDraw = function ()

	drawPoints ( points )
	region:drawDebug ()
end

onMouseLeftEvent = function ( down )

	if down then

		local x, y = layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc ())

		points:seek ( points:getLength ())
		points:writeFloat ( x )
		points:writeFloat ( y )
		points:seek ( 0 )

		if points:getLength () / 8 >= 3 then
			region:convexHull ( points )
		end
	end
end

onMouseRightEvent = function ( down )
	if down then
		points:discardAll ()
		region:clear ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )
MOAIInputMgr.device.mouseRight:setCallback ( onMouseRightEvent )

layer:setOverlayTable ({ onDraw })
