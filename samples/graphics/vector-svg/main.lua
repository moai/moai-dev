--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

svg = require ( 'svg' )

MOAISim.openWindow ( "test", 1280, 1280 )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1280, 1280 )
viewport:setScale ( 1280, -1280 )
viewport:setOffset ( -1, 1 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onProp = function ( prop, style )

	local xMin, yMin, zMin, xMax, yMax, zMax = prop:getBounds ()

	if bounds then
		bounds.xMin = xMin < bounds.xMin and xMin or bounds.xMin
		bounds.yMin = yMin < bounds.yMin and yMin or bounds.yMin
		bounds.xMax = xMax > bounds.xMax and xMax or bounds.xMax
		bounds.yMax = yMax > bounds.yMax and yMax or bounds.yMax
	else
		bounds = { xMin = xMin, yMin = yMin, xMax = xMax, yMax = yMax }
	end
	
	--prop:setColor ( 0, 0, 0, 1 )
	layer:insertProp ( prop )
	--prop:setVisible ( style.visible )
end

onText = function ( text, style, x, y, xScale, yScale )
end

svg.parse ( 'images/tiger.svg', 0, 0, 1, 1, onProp, onText )

print ( bounds.xMin, bounds.yMin, bounds.xMax, bounds.yMax )