----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

prop = MOAIProp.new ()
prop:setDeck ( '../resources/moai.png' )
prop:setPartition ( layer )

curve1 = MOAIAnimCurve.new ()
curve1:reserveKeys ( 3 )
curve1:setKey ( 1, 0, -22.5 )
curve1:setKey ( 2, 0.25, 22.5 )
curve1:setKey ( 3, 0.5, -22.5 )
curve1:setWrapMode ( MOAIAnimCurve.WRAP )

curve2 = MOAIAnimCurve.new ()
curve2:reserveKeys ( 3 )
curve2:setKey ( 1, 0, -45 )
curve2:setKey ( 2, 1.5, 45 )
curve2:setKey ( 3, 3, -45 )
curve2:setWrapMode ( MOAIAnimCurve.WRAP )

--anim = MOAIAnim.new ()
--anim:reserveLinks ( 1 )
--anim:setLink ( 1, curve1, prop, MOAIProp.ATTR_Z_ROT )
--anim:setMode ( MOAITimer.CONTINUE )
--anim:start ()

loop1 = MOAIAnimLoop.new ()
loop1:setCurve ( 1, curve1 )

loop2 = MOAIAnimLoop.new ()
loop2:setCurve ( 1, curve2 )

mixer = MOAIAnimMixer.new ()
mixer:reserveChannels ( 1 )
mixer:setChannelType ( 1, MOAIAnimMixer.CHANNEL_TYPE_FLOAT )
mixer:setChannelLink ( 1, prop, MOAIProp.ATTR_Z_ROT )
mixer:buildChannels ()
mixer:start ()

clip1 = mixer:pushClip ( loop1 )
clip2 = mixer:pushClip ( loop2 )

mixer:setWeight ( clip2, 0.0 )

on = false

function onMouseLeftEvent ( down )

	if down == true then

		if on then
			mixer:transition ( clip2, MOAIAnimMixer.ANIM_TRACK_WEIGHT, 0.0, 2.0 )
		else
			mixer:transition ( clip2, MOAIAnimMixer.ANIM_TRACK_WEIGHT, 1.0, 2.0 )
		end

		on = not on
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )
