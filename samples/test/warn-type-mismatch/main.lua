----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


curve = MOAIAnimCurve.new ()

curve.setKey ()
curve:setKey ( 'foo' )

layer = MOAILayer.new ()
layer:setCamera ( curve )
