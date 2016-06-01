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

generatePoints = function ( stream, total, xMin, yMin, xMax, yMax )

	for i = 1, total do

		local r = math.pow ( math.random (), 2 )
		local a = math.random () * math.pi * 2

		local hw = ( xMax - xMin ) / 2
		local hh = ( yMax - yMin ) / 2

		local x = xMin + hw + ( hw * ( math.cos ( a ) * r ))
		local y = yMin + hh + ( hh * ( math.sin ( a ) * r ))

		stream:writeFloat ( x )
		stream:writeFloat ( y )
	end

	stream:seek ( 0 )
end

points = MOAIMemStream.new ()
region = MOAIRegion.new ()

generateHull = function ()

	generatePoints ( points, 100, -172, -172, 172, 172 )
	region:convexHull ( points )
end

onDraw = function ()

	drawPoints ( points )
	region:drawDebug ()
end

onMouseLeftEvent = function ( down )
	if down then
		points = MOAIMemStream.new ()
		generateHull ()
	end
end

onMouseRightEvent = function ( down )
	if down then
		generateHull ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )
MOAIInputMgr.device.mouseRight:setCallback ( onMouseRightEvent )

generateHull ()

layer:setOverlayTable ({ onDraw })
