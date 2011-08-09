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
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

----------------------------------------------------------------
-- require 'config-physfs' to allow zip files being mounted
require "config-physfs"

-- check if a file exists
print ( MOAIFileSystem.checkFileExists ( "luaFile.lua" ))

-- require now checks the physfs file system before defaulting to normal require
luaBlock = require "luaFile"

-- print a string from the required file above
print ( luaBlock.helloString )

-- dofile now checks the physfs file system before defaulting to normal dofile
dofile ( "luaDoFile" )