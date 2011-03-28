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
	
	quadListDeck:reserveUVRects ( #spec.uvRects )
	for j, uvRect in ipairs ( spec.uvRects ) do
		quadListDeck:setUVRect ( j, uvRect.x0, uvRect.y0, uvRect.x1, uvRect.y1 )
		if uvRect.r then
			quadListDeck:setUVRectTransform ( j, MOAIGfxQuadListDeck2D.UV_ROTATE_90 )
		end
	end

	quadListDeck:reserveQuads ( #spec.quads )
	for j, quad in ipairs ( spec.quads ) do
		if quad.x0 ~= nil then
			quadListDeck:setRect ( j, quad.x0, quad.y0, quad.x1, quad.y1 )
		else
			quadListDeck:setQuad ( j, quad.v0.x, quad.v0.y, quad.v1.x, quad.v1.y, quad.v2.x, quad.v2.y, quad.v3.x, quad.v3.y )
		end
	end
	
	quadListDeck:reservePrims ( #spec.prims )
	for j, prim in ipairs ( spec.prims ) do
		quadListDeck:setPrim ( j, prim.uv, prim.q )
	end
	
	quadListDeck:reserveSprites ( #spec.sprites )
	for j, prop in ipairs ( spec.sprites ) do
		quadListDeck:setSprite ( j, prop.base, prop.size )
	end
	
	return quadListDeck
end

function parseFlash ( flashAnim )

	local step = 1 / flashAnim.fps
	
	local quadListDeck = loadQuadListDeck ( flashAnim.spriteDeck )
	quadListDeck:setTexture ( flashAnim.texture )
	
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

flash = parseFlash ( dofile ( 'Exporter_test.fla.lua' ))

viewport = MOAIViewport.new ()
viewport:setSize ( flash.width, flash.height )
viewport:setScale ( flash.width, -flash.height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "flash", flash.width, flash.height )

player = flash:newPlayer ()
player:insertProps ( layer )
player:start ()

