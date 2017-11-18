--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

require "elements"
require "platforms/platform-windows"
require "modules/cloud-manager"	
require "modules/state-manager"	
require "modules/input-manager"	
require "modules/savefile-manager"

MOAISim.openWindow ( "Wolf Clicker", SCREEN_WIDTH, SCREEN_HEIGHT )

viewport = MOAIViewport.new ()
viewport:setSize ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT )
viewport:setScale ( SCREEN_UNITS_X, SCREEN_UNITS_Y )

-- seed random numbers
math.randomseed ( os.time ())

savefiles.get ( "user" )
globalData = {}

JUMP_TO = nil
----------------------------------------------------------------
if 	JUMP_TO	then
	statemgr.push ( JUMP_TO )
----------------------------------------------------------------
else
	statemgr.push ( "states/state-splash.lua" )	
end
----------------------------------------------------------------

-- Start the game!
statemgr.begin ()
