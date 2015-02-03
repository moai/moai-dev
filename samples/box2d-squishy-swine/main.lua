----------------------------------------------------------------
-- Copyright (c) 2010-2013 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

RESOLUTION = 9
ITERATIONS = 3

OUTER_FREQUENCY		= 8
INNER_FREQUENCY		= 6
CORE_FREQUENCY		= 18
RESET_FREQUENCY		= 10 --if this is too low then the model can implode from too much impact

SHOW_SPRITES = true

CORE_FILTER		= { CATEGORY = 0x01,	MASK = 0x19,	INDEX = 0 }	-- 00001 11001
FOOT_FILTER		= { CATEGORY = 0x02,	MASK = 0x18,	INDEX = 0 } -- 00010 11000
SENSOR_FILTER	= { CATEGORY = 0x04,	MASK = 0x18,	INDEX = 0 } -- 00100 11000
SHELL_FILTER	= { CATEGORY = 0x08,	MASK = 0x07,	INDEX = 0 } -- 01000 00111
WORLD_FILTER	= { CATEGORY = 0x10,	MASK = 0x07,	INDEX = 0 } -- 10000 00111

ELBOW_FILTER	= { CATEGORY = 0x00,	MASK = 0x00,	INDEX = 0 }

function createTexture ( filename )
	
	local texture = MOAITexture.new ()
	texture:load ( filename )
	texture:setFilter ( MOAITexture.GL_LINEAR )
	return texture
end

function createTexturePackerDeckRaw ( filename )

	local frames = dofile ( filename..".lua" ).frames

	local tex = createTexture ( filename .. ".png" )
	local xtex, ytex = tex:getSize ()

	-- Annotate the frame array with uv quads and geometry rects
	for i, frame in ipairs ( frames ) do
		-- convert frame.uvRect to frame.uvQuad to handle rotation

		local uv = frame.uvRect
		local q = {}
		if not frame.textureRotated then
			-- From Moai docs: "Vertex order is clockwise from upper left (xMin, yMax)"

			q.x0, q.y0 = uv.u0, uv.v0
			q.x1, q.y1 = uv.u1, uv.v0
			q.x2, q.y2 = uv.u1, uv.v1
			q.x3, q.y3 = uv.u0, uv.v1

		else
			-- Sprite data is rotated 90 degrees CW on the texture
			-- u0v0 is still the upper-left
			q.x3, q.y3 = uv.u0, uv.v0
			q.x0, q.y0 = uv.u1, uv.v0
			q.x1, q.y1 = uv.u1, uv.v1
			q.x2, q.y2 = uv.u0, uv.v1

		end

		frame.uvQuad = q

		-- convert frame.spriteColorRect and frame.spriteSourceSize
		-- to frame.geomRect.
		-- MUZZEDIT. Trimming now works, and images are centered

		local cr = frame.spriteColorRect
		local r = {}
		r.x0 = cr.x
		r.y0 = cr.y + cr.height
		r.x1 = cr.x + cr.width
		r.y1 = cr.y
		r.x0 = r.x0 - frame.spriteSourceSize.width/2
		r.y0 = -r.y0 + frame.spriteSourceSize.height/2
		r.x1 = r.x1 -frame.spriteSourceSize.width/2
		r.y1 = -r.y1 + frame.spriteSourceSize.height/2

		frame.geomRect = r
	end

	-- Construct the deck
	local deck = MOAIGfxQuadDeck2D.new ()
	deck:setTexture ( tex )
	deck:reserve ( #frames )
	local names = {}
	for i, frame in ipairs ( frames ) do
		local q = frame.uvQuad
		local r = frame.geomRect
		names[frame.name] = i
		deck:setUVQuad ( i, q.x0,q.y0, q.x1,q.y1, q.x2,q.y2, q.x3,q.y3 )
		deck:setRect ( i, -.5,-.5, .5,.5 )
	end

	deck.getIndex = function ( self, name )
		
		local index = names [ name ]
		if index == nil then print ('Error: ( '..name..' ) does not exist in sprite sheet') end
		return index
	end

	return deck
end

local function vecDot ( x1, y1, x2, y2 )
  return ( x1 * x2 ) + ( y1 * y2 )
end

local function vecAngle ( x1, y1, x2, y2 )  
  return math.deg ( math.acos ( vecDot ( x1, y1, x2, y2 )))
end

local function vecLen ( x, y )
  return math.sqrt ( x^2 + y^2 )
end

local function vecNorm ( x, y )
  local len = vecLen ( x, y )
  return x / len, y / len
end

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end

-- x, y: the center of the circle
-- head: the top body in the jiggly shape
-- points: the other bodies in the jiggly shape
local function positionSprite ( sprite, x, y, head, points )

  local headX, headY = head:getPosition ()
  local headAxisX, headAxisY = vecNorm ( headX - x, headY - y )
  local angle = vecAngle ( headAxisX, headAxisY, 0, 1 )
  
  angle = ( headAxisX < 0 ) and angle or -angle
   sprite:setRot ( angle )
  
  local sideAxisX, sideAxisY = headAxisY, -headAxisX
  
  local xMin, xMax = 0, 0
  local yMin, yMax = 0, 0
  
  for i, p in ipairs ( points ) do
  
    local px, py = p:getPosition ()
    px, py = px - x, py - y
  
    local xm = vecDot ( px, py, sideAxisX, sideAxisY )
    local ym = vecDot ( px, py, headAxisX, headAxisY )
    
    xMin = xm < xMin and xm or xMin
    xMax = xm > xMax and xm or xMax
    
    yMin = ym < yMin and ym or yMin
    yMax = ym > yMax and ym or yMax
  end
  
  local w = xMax - xMin
  local h = yMax - yMin
  
  sprite:setScl ( w * 1.5, h * 1.5 )
  
  local hOff = xMin + ( w / 2 )
  local vOff = yMin + ( h / 2 )
  
  local ux, uy = x + ( sideAxisX * hOff ) + ( headAxisX * vOff ), y + ( sideAxisY * hOff ) + ( headAxisY * vOff )
  sprite:setLoc ( ux, uy )
  
end

local width = 640
local height = 480
MOAISim.openWindow ( "test", width ,height  )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale (width / (width/640), height / (height/480))

layer = MOAILayer2D.new ()
layer:setPartition( MOAIPartition.new() )
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

allPigs = {}
pigs = 0

stickyQueue = {}
stickyFloorQueue = {}

function onPigCollide ( event, thisFixture, thatFixture, arbiter )
  
	if thatFixture.pig and ( thatFixture.sticky or thisFixture.sticky ) then
		table.insert ( stickyQueue, { body1 = thisFixture:getBody (), body2 = thatFixture:getBody (), type = "pig" } )
	elseif thatFixture.floor and thisFixture.sticky then
		table.insert ( stickyQueue, { body1 = thisFixture:getBody (), body2 = thatFixture:getBody (), type = "floor" } )
		thisFixture:setSensor ( true )
	elseif thisFixture.pig and thatFixture.mud then
	  
		thisFixture.sticky = true
		thisFixture.sprite.anim.wiggle:stop ()
		thisFixture.sprite:setIndex ( 5 )
		thisFixture.sprite.anim = thisFixture.sprite.dirty_anim
		thisFixture.sprite.anim.wiggle:start ()
		thisFixture:setSensor ( true )
	end
end

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setGravity ( 0, -10 )
world:setUnitsToMeters ( 1/48 )
world:setIterations ( ITERATIONS, ITERATIONS )
world:start ()

world:setDebugDrawFlags ( MOAIBox2DWorld.DEBUG_DRAW_SHAPES + MOAIBox2DWorld.DEBUG_DRAW_JOINTS + MOAIBox2DWorld.DEBUG_DRAW_PAIRS )
layer:setUnderlayTable ({ world })

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -(800/2), -200, 800/2, -300)
fixture2:setFilter ( WORLD_FILTER.CATEGORY, WORLD_FILTER.MASK, WORLD_FILTER.INDEX )
fixture2.floor = true

fixture3 = worldBody:addRect ( -380, -300, -300, 300)
fixture3:setFilter ( WORLD_FILTER.CATEGORY, WORLD_FILTER.MASK, WORLD_FILTER.INDEX )
fixture3.mud = true

fixture4 = worldBody:addRect ( 300, -300, 380, 300)
fixture4:setFilter ( WORLD_FILTER.CATEGORY, WORLD_FILTER.MASK, WORLD_FILTER.INDEX )
fixture4.mud = true

deck = createTexturePackerDeckRaw ( "squish" )

function makeAnimCurve ( t )
	local animcurve = MOAIAnimCurve.new ()
	animcurve:reserveKeys ( #t )

	for i = 1, #t do
		animcurve:setKey ( i, t [ i ].t, t [ i ].v, MOAIAnimCurve.FLAT )
	end

	return animcurve
end

cleanBlinkCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 1 },
	[ 2 ] = { t = 2, v = 4 },
	[ 3 ] = { t = 2.1, v = 4 }
})
cleanSniffCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 1 },
	[ 2 ] = { t = 2, v = 2 },
	[ 3 ] = { t = 2.1, v = 1 },
	[ 4 ] = { t = 2.2, v = 2 },
	[ 5 ] = { t = 2.3, v = 2 }
})
cleanWiggleCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 1 },
	[ 2 ] = { t = 0.1, v = 3 },
	[ 3 ] = { t = 0.2, v = 3 }})
dirtyBlinkCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 5 },
	[ 2 ] = { t = 2, v = 8 },
	[ 3 ] = { t = 2.1, v = 8 }
})
dirtySniffCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 5 },
	[ 2 ] = { t = 2, v = 6 },
	[ 3 ] = { t = 2.1, v = 5 },
	[ 4 ] = { t = 2.2, v = 6 },
	[ 5 ] = { t = 2.3, v = 6 }
})
dirtyWiggleCurve = makeAnimCurve ({
	[ 1 ] = { t = 0, v = 5 },
	[ 2 ] = { t = 0.1, v = 7 },
	[ 3 ] = { t = 0.2, v = 7 }
})

function addSprite ( x, y, r0, r1, quality )

	local fixture = {}
	local fixtureb = {}
	local body = {}
	local joint = {}
	local spring = {}
	local bolt = {}

	local x, y, r0, r1 = x or 0, y or 0, r0 or 15, r1 or 30
	local quality = quality or 8

	local centerBody = world:addBody ( MOAIBox2DBody.DYNAMIC, x, y )
	local centerFixture = centerBody:addCircle ( 0, 0, r0 )
	centerFixture:setDensity ( 2 )
	centerFixture:setFriction ( 1 )
	centerFixture:setFilter ( CORE_FILTER.CATEGORY, CORE_FILTER.MASK, CORE_FILTER.INDEX )
	centerFixture.pig = true

	local outerFixture = centerBody:addCircle ( 0, 0, r1 )
	outerFixture:setDensity ( 0.5 )
	outerFixture:setFriction ( 1 )
	outerFixture:setFilter ( SHELL_FILTER.CATEGORY, SHELL_FILTER.MASK, SHELL_FILTER.INDEX )
	outerFixture.pig = true

	local sensor = centerBody:addCircle ( 0, 0, r1+2.5 )
	sensor.sticky = false
	sensor.pig = true
	sensor:setFilter ( SENSOR_FILTER.CATEGORY, SENSOR_FILTER.MASK, SENSOR_FILTER.INDEX )
	sensor:setCollisionHandler ( onPigCollide, MOAIBox2DArbiter.BEGIN )
	sensor:setSensor ( true )

	centerBody:resetMassData ()
	centerBody.links = {}

	centerBody:setAngularDamping ( 0.5 )

	for i = 1, quality do
		local n = ( i - 1 ) * ( 360 / quality )
		local angle = n * math.pi / 180
		local ptx, pty = x + r1 * math.sin ( angle ), y + r1 * math.cos ( angle )
		local ptx2, pty2 = x + r0 * math.sin ( angle ), y + r0 * math.cos ( angle )

		body [ i ] = world:addBody ( MOAIBox2DBody.DYNAMIC, ptx, pty )
		body [ i ]:setFixedRotation ( true )
		fixture [ i ] = body [ i ]:addCircle ( 0, 0, 5 )
		fixture [ i ]:setDensity ( 1 )
		fixture [ i ]:setFriction ( 5 )
		fixture [ i ]:setFilter ( FOOT_FILTER.CATEGORY, FOOT_FILTER.MASK, FOOT_FILTER.INDEX )
		fixture [ i ].myx = ptx
		fixture [ i ].myy = pty

		body [ i ]:resetMassData ()

		fixtureb [ i ] = {}
		fixtureb [ i ].myx = ptx2
		fixtureb [ i ].myy = pty2
	end

	for i = 1, quality do
		joint [ 1 ] = {}
		joint [ 1 ][ i ] = world:addDistanceJoint ( body [ i ], body [ ( i % quality ) + 1 ], fixture [ i ].myx, fixture [ i ].myy, fixture [ ( i % quality ) + 1 ].myx, fixture [ ( i % quality ) + 1 ].myy )
		joint [ 1 ][ i ]:setFrequency ( OUTER_FREQUENCY )
		joint [ 1 ][ i ]:setDampingRatio ( 0 )

		joint [ 2 ] = {}
		joint [ 2 ][ i ] = world:addDistanceJoint ( body [ i ], centerBody , fixture [ i ].myx, fixture [ i ].myy, fixtureb [ i ].myx, fixtureb [ i ].myy )
		joint [ 2 ][ i ]:setFrequency ( CORE_FREQUENCY )
		joint [ 2 ][ i ]:setDampingRatio ( 0 )

		joint [ 3 ] = {}
		joint [ 3 ][ i ] = world:addDistanceJoint ( body [ i ], centerBody, fixture [ i ].myx, fixture [ i ].myy, fixtureb [ ( i % quality ) + 1 ].myx, fixtureb [ ( i % quality ) + 1 ].myy )
		joint [ 3 ][ i ]:setFrequency ( INNER_FREQUENCY )
		joint [ 3 ][ i ]:setDampingRatio ( 0 )

		joint [ 4 ] = {}
		joint [ 4 ][ i ] = world:addDistanceJoint ( centerBody, body [ ( i % quality ) + 1 ], fixtureb [ i ].myx, fixtureb [ i ].myy, fixture [ ( i % quality ) + 1 ].myx, fixture [ ( i % quality ) + 1 ].myy )
		joint [ 4 ][ i ]:setFrequency ( INNER_FREQUENCY )
		joint [ 4 ][ i ]:setDampingRatio ( 0 )

		joint [ 5 ] = {}
		joint [ 5 ][ i ] = world:addDistanceJoint ( body [ i ], centerBody, fixture [ i ].myx, fixture [ i ].myy, fixture [ i ].myx, fixture [ i ].myy )
		joint [ 5 ][ i ]:setFrequency ( RESET_FREQUENCY )
		joint [ 5 ][ i ]:setDampingRatio ( 0 )
	end

	local sprite = MOAIProp2D.new ()
	sprite:setDeck ( deck )
	sprite:setIndex ( 1 )
	sprite.body = centerBody
	if not SHOW_SPRITES then sprite:setColor ( 0, 0, 0, 0 ) end
	layer:insertProp ( sprite )
  
	local function makeAnim ( curve )
		local anim = MOAIAnim.new ()
		anim:reserveLinks ( 1 )
		anim:setLink ( 1, curve, sprite, MOAIProp.ATTR_INDEX )
		anim:setMode ( MOAITimer.LOOP )
		return anim
	end
  
	sprite.clean_anim = {
		blink	= makeAnim ( cleanBlinkCurve ),
		sniff	= makeAnim ( cleanSniffCurve ),
		wiggle	= makeAnim ( cleanWiggleCurve )
	}

	sprite.dirty_anim = {
		blink	= makeAnim ( dirtyBlinkCurve ),
		sniff	= makeAnim ( dirtySniffCurve ),
		wiggle	= makeAnim ( dirtyWiggleCurve )
	}
  
	sprite.anim = sprite.clean_anim
	sprite.anim.blink:start ()

	sensor.sprite = sprite
  
	table.insert ( allPigs, { sensor = sensor, sprite = sprite, centerBody = centerBody, centerFixture = centerFixture, body = body, fixture = fixture } )
end

function pointerCallback ( x, y )
	worldX, worldY = layer:wndToWorld ( x, y )

	if pick then
		mouseJoint:setTarget(worldX, worldY)
	end
end

function clickCallback ( down )
	if down then
		pick = layer:getPartition():propForPoint ( worldX, worldY)
		if pick then
			mouseBody = world:addBody( MOAIBox2DBody.DYNAMIC )
			mouseBody:setTransform(worldX, worldY)

			mouseJoint = world:addMouseJoint(mouseBody, pick.body, worldX, worldY,  10000.0 * pick.body:getMass());
			mouseJoint:setDampingRatio(2);
			pick.anim.blink:stop ()
			pick.anim.sniff:stop ()
			pick.anim.wiggle:start ()
		end
	else
		if pick then
			--also destroys joint
			local anim = pick.anim
			anim.wiggle:stop ()
			anim.blink:start ()
			mouseBody:destroy()
			mouseBody = nil
			pick = nil
		end
	end
end

function rightclickCallback ( down )
	if down then
	    local size = { 16, 24, 32, 40, 48 }
	    local s = 3
	    local quality = math.floor ( size [ s ] / 2 )
		addSprite( worldX, worldY, 12, 32, RESOLUTION )
	    pigs = pigs + 1
	    print ( "Pigs: " .. pigs )
	end
end


if MOAIInputMgr.device.pointer then
    MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
    MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
    MOAIInputMgr.device.mouseRight:setCallback ( rightclickCallback )
else
    MOAIInputMgr.device.touch:setCallback (
        function ( eventType, idx, x, y, tapCount )
            local isDown = eventType == MOAITouchSensor.TOUCH_DOWN
            if (tapCount > 1) then
                rightclickCallback(isDown)
            elseif eventType ~= MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
                clickCallback(isDown)
            elseif eventType == MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
            end
        end
    )
end

scaleThread = MOAIThread.new ()
scaleThread:run ( function ()
	repeat
		coroutine.yield ()
		for i = 1, #allPigs do
			local x, y = allPigs [ i ].centerBody:getPosition ()
			positionSprite ( allPigs [ i ].sprite, x, y, allPigs [ i ].body [ 1 ], allPigs [ i ].body )
		end
	until false
end )

stickyThread = MOAIThread.new ()
stickyThread:run ( function ()
    
	repeat
		coroutine.yield ()

		if MOAIInputMgr.device.keyboard:keyDown ( "s" ) then
			SHOW_SPRITES = not SHOW_SPRITES
			for i = 1, #allPigs do
				if SHOW_SPRITES then
					allPigs [ i ].sprite:setColor ( 1, 1, 1, 1 )
				else
					allPigs [ i ].sprite:setColor ( 0, 0, 0, 0 )
				end
			end
		end

   		if #stickyQueue > 0 then

			local connection = {}
			for i = #stickyQueue, 1, -1 do

				local skip = false

				if stickyQueue [ i ].type == "pig" then

					for j = 1, #connection do
						if connection [ j ].body1 == stickyQueue [ i ].body2 and connection [ j ].body2 == stickyQueue [ i ].body1 then
							skip = true
						end
					end

					if not skip then
						local bodya, bodyb = stickyQueue [ i ].body1, stickyQueue [ i ].body2

						local ax, ay = bodya:getPosition ()
						local bx, by = bodyb:getPosition ()

						local len = math.sqrt (( bx - ax )^2 + ( by - ay )^2 )

						local axisX = bx - ax
						local axisY = by - ay

						local midx, midy = (ax + bx) / 2, (ay + by) / 2

						local ball1 = world:addBody ( MOAIBox2DBody.DYNAMIC, midx, midy )
						local ballFix1 = ball1:addCircle ( 0, 0, 5 )
						ballFix1:setDensity ( 1 )
						ballFix1:setFriction ( 0 )
						ballFix1:setFilter ( ELBOW_FILTER.CATEGORY, ELBOW_FILTER.MASK, ELBOW_FILTER.INDEX )
						ball1:resetMassData ()

						local ball2 = world:addBody ( MOAIBox2DBody.DYNAMIC, midx, midy )
						local ballFix2 = ball2:addCircle ( 0, 0, 5 )
						ballFix2:setDensity ( 1 )
						ballFix2:setFriction ( 0 )
						ballFix2:setFilter ( ELBOW_FILTER.CATEGORY, ELBOW_FILTER.MASK, ELBOW_FILTER.INDEX )
						ball2:resetMassData ()

						local rjoint = world:addRevoluteJoint ( ball1, ball2, midx, midy )

						local pjoint1 = world:addPrismaticJoint ( ball1, bodya, midx, midy, -axisX, -axisY )
						pjoint1:setLimit ( 0, len )

						local djoint1 = world:addDistanceJoint ( ball1, bodya, midx, midy, ax, ay )
						djoint1:setFrequency ( 12 )
						djoint1:setDampingRatio ( 0 )

						local pjoint2 = world:addPrismaticJoint ( ball2, bodyb, midx, midy, axisX, axisY  )
						pjoint2:setLimit ( 0, len )

						local djoint2 = world:addDistanceJoint ( ball2, bodyb, midx, midy, bx, by )
						djoint2:setFrequency ( 12 )
						djoint2:setDampingRatio ( 0 )

						local djoint3 = world:addDistanceJoint ( bodya, bodyb, ax, ay, bx, by )
						djoint3:setFrequency ( 12 )
						djoint3:setDampingRatio ( 0 )

						bodya.tower = bodya.tower or bodyb.tower
						bodyb.tower = bodyb.tower or bodya.tower

						if not bodya.stuckToFloor then
							bodya:setAngularDamping ( 500 )
						end

						if not bodyb.stuckToFloor then
							bodyb:setAngularDamping ( 500 )
						end

						table.insert ( connection, stickyQueue [ i ])
					end

				elseif stickyQueue [ i ].type == "floor" then

					local bodya, bodyb = stickyQueue [ i ].body1, stickyQueue [ i ].body2
					if not bodya.stuckToFloor then

						bodya:setAngularDamping ( 1000 )

						local ax, ay = bodya:getPosition ()
						local floorbod = world:addBody ( MOAIBox2DBody.DYNAMIC, ax, -200 )
						local floorfix = floorbod:addRect ( -10, -2, 10, 0 )
						floorfix:setDensity ( 1 )
						floorfix:setFriction ( 1 )
						floorfix:setFilter ( ELBOW_FILTER.CATEGORY, ELBOW_FILTER.MASK, ELBOW_FILTER.INDEX )
						floorbod:resetMassData ()

						local rjoint = world:addRevoluteJoint ( floorbod, bodyb, ax, -200 )
						local djoint = world:addDistanceJoint ( bodya, floorbod, ax, ay, ax, -200 )
						djoint:setFrequency ( 20 )
						djoint:setDampingRatio ( 0 )
	      
						local pjoint1 = world:addPrismaticJoint ( bodya, floorbod, ax, ay, ax, -200 )
						pjoint1:setLimit ( -10, 20 )

						bodya.stuckToFloor = true
						bodya.tower = true
					end
				end

			table.remove ( stickyQueue, i )
			end
		end

	until false
end )
