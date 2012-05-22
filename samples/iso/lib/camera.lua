----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function makeDimetricCamera ()
	local camera = MOAICamera.new ()
	camera:setOrtho ( true )
	camera:setNearPlane ( 10000 )
	camera:setFarPlane ( -10000 )
	camera:setRot ( 60, 0, 135 )
	return camera
end

function makeIsometricCamera ()
	local camera = MOAICamera.new ()
	camera:setOrtho ( true )
	camera:setNearPlane ( 10000 )
	camera:setFarPlane ( -10000 )
	camera:setRot ( 54.736, 0, 135 )
	return camera
end