--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

svg = require ( 'svg' )

layer = MOAILayer2D.new ()

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

svg.parse ( 'images/bird.svg', 0, 0, 1, 1, onProp, onText )

print ( bounds.xMin, bounds.yMin, bounds.xMax, bounds.yMax )

local width = bounds.xMax - bounds.xMin
local height = bounds.yMax - bounds.yMin

MOAISim.openWindow ( "test", width, height )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, -height )
viewport:setOffset ( -1, 1 )

layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
