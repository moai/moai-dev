----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
 
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
 
layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
 
mimage = MOAIImage.new ()
mimage:load ( "./numbers.png", MOAIImage.TRUECOLOR + MOAIImage.PREMULTIPLY_ALPHA )
width, height = mimage:getSize ()
 
mdeck = MOAITileDeck2D.new ()
mdeck:setSize ( 8, 8 )
mdeck:setRect ( -width / 16, height / 16, width / 16, -height / 16 )
mdeck:setTexture ( mimage )
 
prop = MOAIProp2D.new ()
prop:setDeck ( mdeck )
prop:setScl( 1, -1 )
layer:insertProp ( prop )
 
mcurve = MOAIAnimCurve.new ()
mcurve:reserveKeys ( 6 )
mcurve:setKey ( 1, 0.0, 1, MOAIEaseType.FLAT )
mcurve:setKey ( 2, 0.2, 2, MOAIEaseType.FLAT )
mcurve:setKey ( 3, 0.4, 3, MOAIEaseType.FLAT )
mcurve:setKey ( 4, 0.6, 4, MOAIEaseType.FLAT )
mcurve:setKey ( 5, 0.8, 5, MOAIEaseType.FLAT )
mcurve:setKey ( 6, 1.0, 5, MOAIEaseType.FLAT )
 
manim = MOAIAnim:new ()
manim:reserveLinks ( 1 )
manim:setMode ( MOAITimer.LOOP )
manim:setLink ( 1, mcurve, prop, MOAIProp2D.ATTR_INDEX )
manim:setListener ( MOAITimer.EVENT_TIMER_LOOP, function ( anim, counter )
  print ( counter )  if counter == 2 then
        manim:stop()
  end
end)
 
manim:start()