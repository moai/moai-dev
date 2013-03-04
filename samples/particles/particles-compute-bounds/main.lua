----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

--availableResolutions = {}

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

CONST = MOAIParticleScript.packConst

local r1 = MOAIParticleScript.packReg ( 1 )

----------------------------------------------------------------
local init = MOAIParticleScript.new ()

init:vecAngle			( r1, MOAIParticleScript.PARTICLE_DX, MOAIParticleScript.PARTICLE_DY )
init:sub				( r1, CONST ( 180.0 ), r1 )

local render = MOAIParticleScript.new ()

render:add				( MOAIParticleScript.PARTICLE_X, MOAIParticleScript.PARTICLE_X, MOAIParticleScript.PARTICLE_DX )
render:add				( MOAIParticleScript.PARTICLE_Y, MOAIParticleScript.PARTICLE_Y, MOAIParticleScript.PARTICLE_DY )

render:sprite			()
render:set				( MOAIParticleScript.SPRITE_ROT, r1 )
render:ease				( MOAIParticleScript.SPRITE_X_SCL, CONST ( 0.5 ), CONST ( 3 ), MOAIEaseType.SHARP_EASE_IN )
render:ease				( MOAIParticleScript.SPRITE_OPACITY, CONST ( 1 ), CONST ( 0 ), MOAIEaseType.EASE_OUT )

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 1 )
system:reserveSprites ( 256 )
system:reserveStates ( 2 )
system:setDeck ( texture )
system:setComputeBounds ( true )
system:start ()
layer:insertProp ( system )

state = MOAIParticleState.new ()
state:setTerm ( 3, 3 )
state:setInitScript ( init )
state:setRenderScript ( render )
system:setState ( 1, state )

emitter = MOAIParticleDistanceEmitter.new ()
emitter:setLoc ( 0, 0 )
emitter:setSystem ( system )
emitter:setMagnitude ( 0.125 )
emitter:setAngle ( 260, 280 )
emitter:setDistance ( 16 )
emitter:start ()

pad0 = {}
pad0["leftX"] = nil;
pad0["leftY"] = nil;
pad0["rightX"] = nil;
pad0["rightY"] = nil;


function pointerCallback ( x, y )
	
	mouseX, mouseY = layer:wndToWorld ( x, y )
	
	emitter:setLoc ( mouseX, mouseY )
	
	if MOAIInputMgr.device.mouseLeft:isUp () then
		emitter:reset ()
	end
end

function StickCallback1(x, y)
	StickCallback(1, x, y)
	--StickCallback(1, x, y)
	--print("[0][" .. a .. " " .. b .."][1][" .. c .. " " .. d .."]")

end
function StickCallback2(x, y)
	StickCallback(2, x, y)
end

function StickCallback(a, x, y)
	--pad0["leftX"] = x;
	--pad0["leftY"] = y;

	if math.sqrt(x * x + y * y) > 0.5 then
		print("["..a.."][" .. x .. " " .. y .."]")
	end
end
function TriggerCallback(x, y)
	--pad0["rightX"] = x;
	--pad0["rightY"] = y;

	--print("trigger [" .. x .. " " .. y .."]")
	--print("[" .. leftX .. " " .. leftY .."][" .. rightX .. " " .. rightY .."]")
end

function PadButtonCallback(foo, bar)
	barStr = "FALSE"
	if bar == true then
		barStr = "TRUE"
	end
	--print("pad button! ["..foo.."]");
	--print("[" .. pad0["leftX"] .. " " .. pad0["leftY"] .."][" .. pad0["rightX"] .. " " .. pad0["rightY"] .."]")
end

function SetResolution(res, fullscreen)
	SledgeGraphicsHandler:setMode(
		res.w,
		res.h,
		res.refresh,
		res.bpp,
		fullscreen
	)
	viewport:setSize (
		res.w,
		res.h
	)
	viewport:setScale (
		res.w,
		res.h
	)
	viewport:setOffset(0, 0)
end

function KeyboardCallback(foo)
	print("keyboard! ["..foo.."]");
	if foo == 29 then

		SetResolution(availableResolutions[46], 0)
		--print("available resolutions:", #availableResolutions)
	end
	if foo == 27 then		
		SetResolution(availableResolutions[35], 1)
	end
end

--print(MOAIInputMgr.device)
--print(MOAIInputMgr.device2)
--print(MOAIInputMgr.device.keyboard)
--print(MOAIInputMgr.device.pointer)
if MOAIInputMgr.device.pointer then
	MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
end

print(MOAIInputMgr.joy0)
print(MOAIInputMgr.joy0.stick0)
print(MOAIInputMgr.joy0.stick1)
print(MOAIInputMgr.joy0.buttons)
if MOAIInputMgr.joy0 then	
	if MOAIInputMgr.joy0.stick0 then
		MOAIInputMgr.joy0.stick0:setCallback(StickCallback1)
	end
	if MOAIInputMgr.joy0.stick1 then
		MOAIInputMgr.joy0.stick1:setCallback(StickCallback2)
	end
	if MOAIInputMgr.joy0.buttons then
		MOAIInputMgr.joy0.buttons:setCallback(KeyboardCallback)
	end
end

if MOAIInputMgr.device.keyboard then
	MOAIInputMgr.device.keyboard:setCallback ( KeyboardCallback )
end

--print(SledgeInputHandler)
--SledgeInputHandler.classHello()
--sih = SledgeInputHandler.new()
--SledgeInputHandler.setDeadzones()
--SledgeInputHandler:setDeadzones(100.0, 200.0, 300.0)
SledgeGraphicsHandler:getCurrentMode()

--graphicsmode = {}
--print("graphicsmode:"..graphicsmode[0])
--print("current mode: [".. resolutions[0].w .. " x " .. resolutions[0].h .. "]")