----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end

local library
local timeline

local function insertProps ( self, layer )

	for i, v in ipairs ( self.props ) do
		layer:insertProp ( v )
	end
end

local function removeProps ( self, layer )

	for i, v in ipairs ( self.props ) do
		layer:removeProp ( v )
	end
end

local function newPlayer ( self )

	local layerSize = 6;

	local player = MOAIAnim.new ()
	player:reserveLinks ( #self.curves * layerSize )
	
	local root = MOAITransform.new ()
	local props = {}
	
	for i, curveSet in pairs ( self.curves ) do
		
		local prop = MOAIProp2D.new ()
		prop:setParent ( root )
		prop:setDeck ( self.deck )
		
		local c = ( i - 1 ) * layerSize
		
		player:setLink ( c + 1, curveSet.id, prop, MOAIProp2D.ATTR_INDEX )
		player:setLink ( c + 2, curveSet.x, prop, MOAITransform.ATTR_X_LOC )
		player:setLink ( c + 3, curveSet.y, prop, MOAITransform.ATTR_Y_LOC )
		player:setLink ( c + 4, curveSet.r, prop, MOAITransform.ATTR_Z_ROT )
		player:setLink ( c + 5, curveSet.xs, prop, MOAITransform.ATTR_X_SCL )
		player:setLink ( c + 6, curveSet.ys, prop, MOAITransform.ATTR_Y_SCL )
		
		table.insert ( props, prop )
	end

	player.root = root
	player.props = props
	player.flash = self
	
	player.insertProps = insertProps
	player.removeProps = removeProps
	
	player:apply ( 0 )
	
	return player
end

function parseFlash ( flashAnim )

	local step = 1 / flashAnim.fps
	local layerSize = 6
	
	local quadDeck = MOAIGfxQuadDeck2D.new ()
	quadDeck:setTexture ( flashAnim.texture )
	quadDeck:reserve ( #flashAnim.brushDeck )
	
	for i, brush in pairs ( flashAnim.brushDeck ) do
		
		quadDeck:setUVRect ( i, brush.u0, brush.v0, brush.u1, brush.v1 )
		
		local w = brush.w * 0.5;
		local h = brush.h * 0.5;
		
		if ( brush.r ) then
			quadDeck:setQuad ( i, h, w, h, -w, -h, -w, -h, w )
		else
			quadDeck:setRect ( i, -w, -h, w, h )
		end
	end
	
	local timeline = flashAnim.timeline
	local curves = {}
	
	for i, layer in pairs ( timeline.layers ) do
		
		local nKeys = #layer.frames
		
		local idCurve = MOAIAnimCurve.new ()
		idCurve:reserveKeys ( nKeys )
		
		local xCurve = MOAIAnimCurve.new ()
		xCurve:reserveKeys ( nKeys )
		
		local yCurve = MOAIAnimCurve.new ()
		yCurve:reserveKeys ( nKeys )
	
		local rCurve = MOAIAnimCurve.new ()
		rCurve:reserveKeys ( nKeys )
	
		local sxCurve = MOAIAnimCurve.new ()
		sxCurve:reserveKeys ( nKeys )
		
		local syCurve = MOAIAnimCurve.new ()
		syCurve:reserveKeys ( nKeys )
	
		for j, frame in pairs ( layer.frames ) do
			
			local t = frame.start * step
			idCurve:setKey ( j, t, frame.id, MOAIEaseType.FLAT )
			
			if ( frame.id > -1 ) then
			
				local transform = frame.transform
				
				local mode = MOAIEaseType.LINEAR
				local weight = frame.ease / 100
				
				if weight > 0 then
				
					mode = MOAIEaseType.EASE_IN
				
				elseif weight < 0 then
				
					mode = MOAIEaseType.EASE_OUT
					weight = -weight
				end
				
				xCurve:setKey ( j, t, transform.x, mode, weight )
				yCurve:setKey ( j, t, transform.y, mode, weight )
				rCurve:setKey ( j, t, transform.r, mode, weight )
				sxCurve:setKey ( j, t, transform.sx, mode, weight )
				syCurve:setKey ( j, t, transform.sy, mode, weight )
			end
		end
		
		local curveSet = {}
		
		curveSet.id = idCurve
		curveSet.x = xCurve
		curveSet.y = yCurve
		curveSet.r = rCurve
		curveSet.xs = sxCurve
		curveSet.ys = syCurve
		
		table.insert ( curves, curveSet )
	end
	
	local flash = {}
	flash.deck = quadDeck
	flash.curves = curves
	
	flash.width = flashAnim.width
	flash.height = flashAnim.height
	
	flash.newPlayer = newPlayer
	
	return flash
end

fla = dofile ( 'flash-tween.fla.lua' )

viewport = MOAIViewport.new ()
viewport:setSize ( fla.width, fla.height )
viewport:setScale ( fla.width, -fla.height )
viewport:setOffset ( -1, 1 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "flash", fla.width, fla.height )

flash = parseFlash ( fla )

player = flash:newPlayer ()
player:insertProps ( layer )
player:start ()

