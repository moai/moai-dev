----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( MOAIFileSystem.mountVirtualDirectory ( "tropics", "tropics.zip" ))

MOAIUntzSystem.initialize ( 22050, 2048 )

local sound = MOAIUntzSound.new ()

sound:load ( 'tropics/resources/sound/tropical_spin.ogg' )

sound:play ()