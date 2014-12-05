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

prop:moveRot ( 540, 2.5 )

function onVideoCompleted ()
	print ( "onVideoCompleted: " )
end

function onVideoReady ()
	print ( "onVideoReady: " )
	
	MOAIMoviePlayer.play ()
end

MOAIMoviePlayer.setListener ( MOAIMoviePlayer.MOVIE_PLAYER_COMPLETED, onVideoCompleted )
MOAIMoviePlayer.setListener ( MOAIMoviePlayer.MOVIE_PLAYER_READY, onVideoReady )

MOAIMoviePlayer.init ( "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8" )