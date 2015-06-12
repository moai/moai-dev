----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

--[[
	static int	_init									( lua_State* L );
	static int	_isAccelerometerAvailable				( lua_State* L );
	static int	_isDeviceMotionAvailable				( lua_State* L );
	static int	_isGyroAvailable						( lua_State* L );
	static int	_isMagnetometerAvailable				( lua_State* L );
	static int	_setAccelerometerActive					( lua_State* L );
	static int	_setAccelerometerUpdateInterval			( lua_State* L );
	static int	_setGyroActive							( lua_State* L );
	static int	_setGyroUpdateInterval					( lua_State* L );
	static int	_setDeviceMotionActive					( lua_State* L );
	static int	_setDeviceMotionUpdateInterval			( lua_State* L );
	static int	_setMagnetometerActive					( lua_State* L );
	static int	_setMagnetometerUpdateInterval			( lua_State* L );
]]--

local makeVectorCallback = function ( name )

	print ( 'CALLBACK', name )

	return function ( x, y, z )
		print ( name, x, y, z )
	end
end

if MOAIMotionMgrIOS then

	MOAIMotionMgrIOS.init ()

	if MOAIMotionMgrIOS.isAccelerometerAvailable () then
		MOAIMotionMgrIOS.setAccelerometerActive ()
		MOAIMotionMgrIOS.rawMotion.accelerometer:setCallback ( makeVectorCallback ( 'accelerometer' ))
	end

	if MOAIMotionMgrIOS.isDeviceMotionAvailable () then
		MOAIMotionMgrIOS.setDeviceMotionActive ()
		MOAIMotionMgrIOS.deviceMotion.accelerometer:setCallback ( makeVectorCallback ( 'device accelerometer' ))
		MOAIMotionMgrIOS.deviceMotion.gravity:setCallback ( makeVectorCallback ( 'device gravity' ))
		MOAIMotionMgrIOS.deviceMotion.gyro:setCallback ( makeVectorCallback ( 'device gyro' ))
		MOAIMotionMgrIOS.deviceMotion.magnetometer:setCallback ( makeVectorCallback ( 'device magnetometer' ))
	end

	if MOAIMotionMgrIOS.isHeadingAvailable () then
		MOAIMotionMgrIOS.setHeadingActive ()
		MOAIMotionMgrIOS.location.compass:setCallback ( function ( heading )
			print ( 'compass', heading )
			prop:setRot ( -heading )
		end )
	end

	if MOAIMotionMgrIOS.isGyroAvailable () then
		MOAIMotionMgrIOS.setGyroActive ()
		MOAIMotionMgrIOS.rawMotion.gyro:setCallback ( makeVectorCallback ( 'gyro' ))
	end

	if MOAIMotionMgrIOS.isMagnetometerAvailable () then
		MOAIMotionMgrIOS.setMagnetometerActive ()
		MOAIMotionMgrIOS.rawMotion.magnetometer:setCallback ( makeVectorCallback ( 'magnetometer' ))
	end
end
