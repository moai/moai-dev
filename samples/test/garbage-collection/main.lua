----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

texture = MOAITexture.new ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )

layer = MOAILayer2D.new ()
layer:insertProp ( prop )

texture = nil
gfxQuad = nil
prop = nil
layer = nil

print ( "--------------------------------" )
MOAISim.reportHistogram ()

print ( "--------------------------------" )
MOAISim.forceGarbageCollection ()
MOAISim.reportHistogram ()	



