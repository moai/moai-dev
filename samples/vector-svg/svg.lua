--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

module ( 'svg', package.seeall )

local	CIRCLE_RESOLUTION = 64 -- this should really be adaptive; improve MOAIVectorTesselator

local	gTess			= nil
local	gStyleStack		= nil

local	gOnProp			= nil
local	gOnText			= nil

--==============================================================
-- interface
--==============================================================

local		applyStyles					= nil
local		nextNumber					= nil
local		nextPathCommand				= nil
local		nextPoint					= nil
local		numbers						= nil
local		onCircle					= nil
local		onEllipse					= nil
local		onGroup						= nil
local		onLine						= nil
local		onPath						= nil
local		onPolygon					= nil
local		onPolyline					= nil
local		onRect						= nil
local		onText						= nil
			parse						= nil
local		parseClassTags				= nil
local		parseColor					= nil
local		parsePath					= nil
local		points						= nil
local		popStyle					= nil
local		pushStyle					= nil
local		pushStyleWithAttributes		= nil
local		pushStyleWithTransform		= nil
local		pushQuadratic				= nil
local		splitString					= nil

--==============================================================
-- implementation
--==============================================================

----------------------------------------------------------------
applyStyles = function ()

	local style = gStyleStack [ #gStyleStack ] or {}

	gTess = gTess or MOAIVectorTesselator.new ()
	gTess:clearTransforms ()

	for i, s in ipairs ( gStyleStack ) do
		local transform = rawget ( s, 'transform' )
		if transform then
			local a, b, c, d, tx, ty = parseMatrix ( transform )
			gTess:pushTransform ( a, b, c, d, tx, ty )
		end
	end

	local scale = gTess:drawingToWorldVec ( 1, 0 )

	gTess:setCircleResolution ( CIRCLE_RESOLUTION )
	gTess:setLightVec ( 0.9, -1, 0 )
	gTess:setShadowColor ( 0, 0, 0, 0.25 )
	gTess:setLightColor ( 1, 1, 1, 0.5 )

	gTess:setExtrude ( style.extrude or 0 )

	gTess:setFillStyle ( style.fillStyle or MOAIVectorTesselator.FILL_NONE )

	gTess:setStrokeStyle ( style.strokeStyle or MOAIVectorTesselator.STROKE_NONE )
	gTess:setStrokeWidth (( style.strokeWidth or 1 ) * scale )
	gTess:setCapStyle ( style.capStyle or MOAIVectorTesselator.CAP_BUTT )
	gTess:setJoinStyle ( style.joinStyle or MOAIVectorTesselator.JOIN_MITER )
	gTess:setMiterLimit ( style.miterLimit or 4 )

	if style.fillColor then
		local r, g, b, a = parseColor ( style.fillColor )
		gTess:setFillColor ( r, g, b, a )
		print ( 'fill color', r, g, b, a )
	else
		gTess:setFillColor ( 0, 0, 0, 1 )
		print ( 'fill color', 0, 0, 0, 1 )
	end

	if style.strokeColor then
		local r, g, b, a = parseColor ( style.strokeColor )
		gTess:setStrokeColor ( r, g, b, a )
	else
		gTess:setStrokeColor ( 0, 0, 0, 1 )
	end
end

----------------------------------------------------------------
nextNumber = function ( s, i )

	local o = s

	-- clip the string to something number-like
	local a, b = string.find ( s, '%-?%.?[%d]+[%d%.]*', i )
	if not a then return end

	-- chop off any non-number garbage
	s = string.sub ( s, a, b )
	local head, tail = a, b + 1

	-- find the first decimal (if any)
	local d = string.find ( s, '%.', 1 )

	if d then
		-- clip to end of decimal portion (if any)
		local a, b = string.find ( s, '%d+', d + 1 )
		b = a and b or d - 1
		s = string.sub ( s, 1, b )
		tail = head + b
	end

	return tonumber ( s ), tail
end

----------------------------------------------------------------
nextPathCommand = function ( s )

	local commandSet = '[CcHhLlMmQqSsTtVvZz]'
	local a = string.find ( s, commandSet )

	if a then
		local cmd = string.sub ( s, a, a )
		local b = string.find ( s, commandSet, a + 1 )
		b = b and b - 1 or #s
		return string.upper ( cmd ), 'Z' < cmd, string.sub ( s, a + 1, b ), b + 1
	end
end

----------------------------------------------------------------
nextPoint = function ( s, i )

	local x, l = nextNumber ( s, i )

	if x then

		local y, j = nextNumber ( s, l )

		if y then
			return x, y, j
		end
	end
end

----------------------------------------------------------------
numbers = function ( s )
	return function ()
		local n, l = nextNumber ( s )
		s = n and string.sub ( s, l ) or s
		return n
	end
end

----------------------------------------------------------------
onCircle = function ( text, attributes, style )
	applyStyles ()

	local cx = tonumber ( attributes.cx ) or 0
	local cy = tonumber ( attributes.cy ) or 0
	local r = tonumber ( attributes.r ) or 0

	gTess:pushEllipse ( cx, cy, r, r )
end

----------------------------------------------------------------
onEllipse = function ( text, attributes, style )
	applyStyles ()

	local cx = tonumber ( attributes.cx ) or 0
	local cy = tonumber ( attributes.cy ) or 0
	local rx = tonumber ( attributes.rx ) or 0
	local ry = tonumber ( attributes.ry ) or 0

	gTess:pushEllipse ( cx, cy, rx, ry )
end

----------------------------------------------------------------
onGroup = function ( text, attributes, style )

	local propFromDrawing = function ( drawing, depthTest )

		if drawing and drawing:finish () then

			local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

			local vtxBuffer = MOAIGfxBuffer.new ()
			local idxBuffer = MOAIGfxBuffer.new ()
			local totalElements = drawing:getTriangles ( vtxBuffer, idxBuffer, 2 );

			local mesh = MOAIMesh.new ()
			mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
			mesh:setIndexBuffer ( idxBuffer )
			mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
			mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
			mesh:setTotalElements ( totalElements )
			mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

			local prop = MOAIProp.new ()
			prop:setDeck ( mesh )

			if not style.visible then
				prop:setColor ( 0, 0, 0, 0 )
			end

			prop.fillColor = fillColor
			prop.strokeColor = strokeColor

			gOnProp ( prop, style )
		end
	end

	propFromDrawing ( gTess )
	gTess = nil
end

----------------------------------------------------------------
onLine = function ( text, attributes, style )
	applyStyles ()

	local x1 = tonumber ( attributes.x1 ) or 0
	local y1 = tonumber ( attributes.y1 ) or 0
	local x2 = tonumber ( attributes.x2 ) or 0
	local y2 = tonumber ( attributes.y2 ) or 0

	gTess:pushPoly ( x1, y1, x2, y2, false )
end

----------------------------------------------------------------
onPath = function ( text, attributes, style )
	applyStyles ()

	print ( 'ON PATH' )

	local filled = style.fillStyle == MOAIVectorTesselator.FILL_SOLID

	if filled then
		print ( 'pushCombo' )
		gTess:pushCombo ()
	end

	local subpath = false
	local x1, y1, cx, cy = 0, 0, 0, 0
	for cmd, rel, data in pathCommands ( attributes.d ) do

		-- any command but Z will start a new subpath
		-- we also skip this for M since M will always create a new subpath
		if not ( subpath or cmd == 'Z' or cmd == 'M' ) then
			gTess:pushPoly ( false ) -- start a new subpath
			subpath = true
		end

		-- cubic curve
		if cmd == 'C' then

			local f = points ( data )
			while true do

				local x2, y2 = f ()
				local x3, y3 = f ()
				local x4, y4 = f ()

				if not ( x2 and x3 and x4 ) then break end

				if rel then
					x2, y2 = x2 + x1, y2 + y1
					x3, y3 = x3 + x1, y3 + y1
					x4, y4 = x4 + x1, y4 + y1
				end

				gTess:pushBezierVertices ( x1, y1, x2, y2, x3, y3, x4, y4 )
				x1, y1 = x4, y4
				cx, cy = x3, y3
			end
		end

		-- horizontal line
		if cmd == 'H' then

			for x in numbers ( data ) do
				x1 = rel and x + x1 or x
				gTess:pushVertex ( x1, y1 )
			end

			cx, cy = x1, y1
		end

		-- line or move
		if cmd == 'L' or cmd == 'M' then

			if cmd == 'M' then

				if subpath then
					gTess:finish () -- finish the current subpath
				end

				gTess:pushPoly ( false ) -- start a new subpath
				subpath = true
			end

			for x, y in points ( data ) do
				x1 = rel and x + x1 or x
				y1 = rel and y + y1 or y
				gTess:pushVertex ( x1, y1 )
			end

			cx, cy = x1, y1
		end

		-- quad curve
		if cmd == 'Q' then

			local f = points ( data )
			while true do

				local x2, y2 = f ()
				local x3, y3 = f ()

				if not ( x2 and x3 ) then break end

				if rel then
					x2, y2 = x2 + x1, y2 + y1
					x3, y3 = x3 + x1, y3 + y1
				end

				pushQuadratic ( x1, y1, x2, y2, x3, y3 )
				x1, y1 = x3, y3
			end
		end

		-- shorthand cubic curve
		if cmd == 'S' then

			local f = points ( data )
			while true do

				local x3, y3 = f ()
				local x4, y4 = f ()

				if not ( x3 and x4 ) then break end

				if rel then
					x3, y3 = x3 + x1, y3 + y1
					x4, y4 = x4 + x1, y4 + y1
				end

				local x2 = ( 2 * x1 ) - cx
				local y2 = ( 2 * y1 ) - cy

				gTess:pushBezierVertices ( x1, y1, x2, y2, x3, y3, x4, y4 )
				x1, y1 = x4, y4
				cx, cy = x3, y3
			end
		end

		-- shorthand quad curve
		if cmd == 'T' then

			local f = points ( data )
			while true do

				local x3, y3 = f ()

				if not ( x3 ) then break end

				if rel then
					x3, y3 = x3 + x1, y3 + y1
				end

				local x2 = ( 2 * x1 ) - cx
				local y2 = ( 2 * y2 ) - cy

				pushQuadratic ( x1, y1, x2, y2, x3, y3 )
				x1, y1 = x4, y4
				cx, cy = x2, y2
			end
		end

		-- vertical line
		if cmd == 'V' then
			for y in numbers ( data ) do
				y1 = rel and y + y1 or y
				gTess:pushVertex ( x1, y1 )
			end
			cx, cy = x1, y1
		end

		-- close path
		if cmd == 'Z' and subpath then
			gTess:setPolyClosed ( true )
			gTess:finish ()
			subpath = false
		end
	end

	if subpath then
		gTess:finish ()
	end

	if filled then
		gTess:finish () -- finish the combo
	end
end

----------------------------------------------------------------
onPolygon = function ( text, attributes, style )
	applyStyles ()

	gTess:pushPoly ( true )
	for x, y in points ( attributes.points ) do
		gTess:pushVertex ( x, y )
	end
	gTess:finish ()
end

----------------------------------------------------------------
onPolyline = function ( text, attributes, style )
	applyStyles ()

	gTess:pushPoly ( false )
	for x, y in points ( attributes.points ) do
		gTess:pushVertex ( x, y )
	end
	gTess:finish ()
end

----------------------------------------------------------------
onRect = function ( text, attributes, style )
	applyStyles ()

	local x0 = tonumber ( attributes.x ) or 0
	local y0 = tonumber ( attributes.y ) or 0
	local x1 = x0 + ( tonumber ( attributes.width ) or 0 )
	local y1 = y0 + ( tonumber ( attributes.height ) or 0 )

	if x0 ~= x1 and y0 ~= y1 then
		gTess:pushRect ( x0, y0, x1, y1 )
	end
end

----------------------------------------------------------------
onText = function ( text, attributes, style )

	if text then

		applyStyles ()

		local x = attributes.x and tonumber ( string.match ( attributes.x, '[-+%d%.]+' ) or '0' ) or 0
		local y = attributes.y and tonumber ( string.match ( attributes.y, '[-+%d%.]+' ) or '0' ) or 0

		x, y = gTess:drawingToWorld ( x, y )

		local xScale, yScale = gTess:drawingToWorldVec ( 1, -1 )

		if gOnText then
			gOnText ( text, style, x, y, xScale, yScale )
		end
	end
end

----------------------------------------------------------------
parse = function ( filename, xOff, yOff, xScl, yScl, onProp, onText )

	print ( 'PARSING SVG', filename )

	gStyleStack = {}

	local style = pushStyleWithTransform ( xScl, 0, xOff, yScl, 0, yOff )
	style.visible = true

	local handlers = {
		circle		= onCircle,
		ellipse		= onEllipse,
		g			= onGroup,
		line		= onLine,
		path		= onPath,
		polygon		= onPolygon,
		polyline	= onPolyline,
		rect		= onRect,
		text		= onText,
		tspan		= onText,
	}

	gOnProp		= onProp
	gOnText		= onText

	local stream = MOAIFileStream.new ()
	stream:open ( filename )

	for event, element in MOAIXmlParser.events ( stream ) do

		print ( event, element )

		if event == MOAIXmlParser.ELEMENT or event == MOAIXmlParser.ELEMENT_BEGIN then
			pushStyleWithAttributes ( element.getAttributes ())
		end

		if event == MOAIXmlParser.ELEMENT or event == MOAIXmlParser.ELEMENT_END then
			local handler = handlers [ element.getName ()]
			local style = gStyleStack [ #gStyleStack ]
			if handler and ( style.visible or style.id ) then
				handler ( element.getText (), element.getAttributes (), style )
			end
			popStyle ()
		end
	end
end

----------------------------------------------------------------
parseColor = function ( color )

	local nonHex, nonHexCount = string.gsub ( color, "%X", "" )
	if string.sub ( color, 1, 1 ) == "#" and nonHexCount == 1 then
		local r = tonumber ( string.sub ( color, 2, 3 ), 16 ) / 255
		local g = tonumber ( string.sub ( color, 4, 5 ), 16 ) / 255
		local b = tonumber ( string.sub ( color, 6, 7 ), 16 ) / 255
		return r, g, b, 1
	else
		return 0, 0, 0, 1
	end
end

----------------------------------------------------------------
pathCommands = function ( s )
	return function ()
		local cmd, rel, data, l = nextPathCommand ( s )
		s = cmd and string.sub ( s, l ) or s
		return cmd, rel, data
	end
end

----------------------------------------------------------------
parseClassTags = function ( style, class )

	if class then

		local values = splitString ( class, ' ' )

		for i, value in ipairs ( values ) do
			for i, entry in ipairs ( gClassTags ) do
				if string.match ( value, entry.pattern ) then
					style [ entry.tag ] = entry.value and entry.value or value
				end
			end
		end
	end
end

----------------------------------------------------------------
parseMatrix = function ( matrix )

	local f = numbers ( matrix )

	local a = f ()
	local b = f ()
	local c = f ()
	local d = f ()

	local tx = f ()
	local ty = f ()

	return a, b, c, d, tx, ty
end

----------------------------------------------------------------
points = function ( s )
	return function ()
		local x, y, l = nextPoint ( s )
		s = x and string.sub ( s, l ) or s
		return x, y
	end
end

----------------------------------------------------------------
popStyle = function ()

	gStyleStack [ #gStyleStack ] = nil
end

----------------------------------------------------------------
pushStyle = function ()

	local topStyle = gStyleStack [ #gStyleStack ]
	local style = {}
	table.insert ( gStyleStack, style )

	if topStyle then
		setmetatable ( style, { __index = topStyle, __nexindex = style })
	end

	return style
end

----------------------------------------------------------------
pushStyleWithAttributes = function ( attributes )

	local style = pushStyle ()

	if attributes.stroke and attributes.stroke ~= 'none' then
		style.strokeColor = attributes.stroke
		style.strokeStyle = MOAIVectorTesselator.STROKE_CENTER
	end

	style.strokeWidth = tonumber ( attributes [ 'stroke-width' ])

	if attributes [ 'stroke-linecap' ] then

		local linecap = attributes [ 'stroke-linecap' ]

		if linecap == 'round' then
			style.capStyle = MOAIVectorTesselator.CAP_ROUND
		elseif linecap == 'square' then
			style.capStyle = MOAIVectorTesselator.CAP_SQUARE
		elseif linecap == 'butt' then
			style.capStyle = MOAIVectorTesselator.CAP_BUTT
		end
	end

	if attributes [ 'stroke-linejoin' ] then

		local linejoin = attributes [ 'stroke-linejoin' ]

		if linejoin == 'round' then
			style.joinStyle = MOAIVectorTesselator.JOIN_ROUND
		elseif linejoin == 'bevel' then
			style.joinStyle = MOAIVectorTesselator.JOIN_BEVEL
		elseif linejoin == 'miter' then
			style.joinStyle = MOAIVectorTesselator.JOIN_MITER
		end
	end

	style.miterLimit = tonumber ( attributes [ 'stroke-miterlimit' ])

	if attributes.fill and attributes.fill ~= 'none' then
		style.fillColor = attributes.fill
		style.fillStyle = MOAIVectorTesselator.FILL_SOLID
	end

	if attributes [ 'font-family' ] then
		style.fontName = string.sub ( attributes [ 'font-family' ], 2, -2 )
	end

	style.fontSize = tonumber ( attributes [ 'font-size' ])

	style.transform = attributes.transform
	style.visible = attributes.display and attributes.display ~= 'none'

	style.id = attributes.id

	parseClassTags ( style, attributes.class )

	return style
end

----------------------------------------------------------------
pushStyleWithTransform = function ( a, b, c, d, tx, ty )

	local style = pushStyle ()
	style.transform = string.format ( '%f,%f,%f,%f,%f,%f', a, b, c, d, tx, ty )
	return style
end

----------------------------------------------------------------
pushQuadratic = function ( x1, y1, x2, y2, x3, y3 )

	local twoThirds = 2 / 3

	local cx1 = x1 + ( twoThirds * ( x2 - x1 ))
	local cy1 = y1 + ( twoThirds * ( y2 - y1 ))

	local cx2 = x3 + ( twoThirds * ( x2 - x3 ))
	local cy2 = y3 + ( twoThirds * ( y2 - y3 ))

	gTess:pushBezierVertices ( x1, y1, cx1, cy1, cx2, cy2, x3, y3 )
end

---------------------------------------------------------------
splitString = function ( str, sep )

	local sep, fields = sep or ":", {}
	local pattern = str.format ( "([^%s]+)", sep )
	str:gsub( pattern, function ( c ) fields [ #fields + 1 ] = c end )
	return fields
end
