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

	layer:insertProp ( self.prop )
end

local function removeProps ( self, layer )

	layer:removeProp ( self.prop )
end

local function newPlayer ( self )

	local player = MOAIAnim.new ()
	player:reserveLinks ( 1 )
		
	local prop = MOAIProp2D.new ()
	prop:setParent ( root )
	prop:setDeck ( self.quadListDeck )
	
	player:setLink ( 1, self.curve, prop, MOAIProp2D.ATTR_INDEX )
	
	player.prop = prop
	player.flash = self
	
	player.insertProps = insertProps
	player.removeProps = removeProps
	
	player:apply ( 0 )
	
	return player
end

function loadQuadListDeck ( spec )
		
	local quadListDeck = MOAIGfxQuadListDeck2D.new ()
	
	quadListDeck:reserveUVQuads ( #spec.uvRects )
		for j, uvRect in ipairs ( spec.uvRects ) do
			
			if uvRect.r then
				quadListDeck:setUVQuad ( j,
					uvRect.x0, uvRect.y0,
					uvRect.x0, uvRect.y1,
					uvRect.x1, uvRect.y1,
					uvRect.x1, uvRect.y0
				)
			else
				quadListDeck:setUVRect ( j, uvRect.x0, uvRect.y0, uvRect.x1, uvRect.y1 )
			end
		end

		quadListDeck:reserveQuads ( #spec.quads )
		for j, quad in ipairs ( spec.quads ) do
			if quad.x0 ~= nil then
				quadListDeck:setRect ( j, quad.x0, quad.y0, quad.x1, quad.y1 )
			else
				quadListDeck:setQuad ( j,
					quad.v0.x, quad.v0.y,
					quad.v1.x, quad.v1.y,
					quad.v2.x, quad.v2.y,
					quad.v3.x, quad.v3.y
				)
			end
		end
	
	quadListDeck:reservePairs ( #spec.prims )
	for j, prim in ipairs ( spec.prims ) do
		quadListDeck:setPair ( j, prim.uv, prim.q )
	end
	
	quadListDeck:reserveLists ( #spec.sprites )
	for j, prop in ipairs ( spec.sprites ) do
		quadListDeck:setList ( j, prop.base, prop.size )
	end
	
	return quadListDeck
end

function parseFlash ( flashAnim )

	local step = 1 / flashAnim.fps
	
	local quadListDeck = loadQuadListDeck ( flashAnim.spriteDeck )
	
	local texture = MOAITexture.new ()
	texture:load ( flashAnim.texture )
	texture:setFilter ( MOAITexture.GL_LINEAR, MOAITexture.GL_LINEAR )
	quadListDeck:setTexture ( texture )
	
	local curve = MOAIAnimCurve.new ()
	curve :reserveKeys ( #flashAnim.frames )

	for j, frame in pairs ( flashAnim.frames ) do
		local t = frame.start * step
		curve:setKey ( j, t, frame.id, MOAIEaseType.FLAT )
	end
	
	local flash = {}
	flash.quadListDeck = quadListDeck
	flash.curve = curve
	
	flash.width = flashAnim.width
	flash.height = flashAnim.height
	
	flash.newPlayer = newPlayer
	
	return flash
end

fla = dofile ( 'flash-flipbook-bird.fla.lua' )

viewport = MOAIViewport.new ()
viewport:setSize ( fla.width, fla.height )
viewport:setScale ( fla.width, -fla.height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

MOAISim.pushRenderPass ( layer )
MOAISim.setClearColor ( 1, 1, 1, 1 )
MOAISim.openWindow ( "flash", fla.width, fla.height )

flash = parseFlash ( fla )

player = flash:newPlayer ()
player:insertProps ( layer )
player:setMode ( MOAITimer.LOOP )
player:start ()

