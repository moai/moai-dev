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
-- override lua require, dofile and loadfile (in config)
dofile ( "config-physfs.lua" )

-- check if a file exists
MOAIFileSystem.setWorkingDirectory ( MOAIFileSystem.getBaseDirectory ())
print ( MOAIFileSystem.getWorkingDirectory ())

MOAIFileSystem.addArchive ( "archive.zip" )
print ( MOAIFileSystem.checkFileExists ( "file.txt" ))


MOAIFileSystem.addArchive ( "luaArchive.zip" )
module = require ( "luaFile" )
print ( module.helloString )
dofile ( "luaDoFile.lua" )