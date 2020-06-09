----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAITableViewLayer.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
layer:pushRenderPass ()

drawPoints = function ( draw, stream )

    draw:bindVectorPresets ()

	local total = stream:getLength () / 8
	stream:seek ( 0 )

	draw:setPenColor ( 0, 0, 1, 1 )

	for i = 1, total do

		local x = stream:readFloat ()
		local y = stream:readFloat ()

		draw:fillCircle ( x, y, 2 )
	end
end

points = MOAIMemStream.new ()
region = MOAIRegion.new ()

onDraw = function ( draw )

	drawPoints ( draw, points )
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

layer:setRenderTable ({ onDraw })
