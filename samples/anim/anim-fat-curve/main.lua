----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

floatCurve = MOAIAnimCurve.new ()
floatCurve:reserveKeys ( 2 )
floatCurve:setKey ( 1, 0, 0 )
floatCurve:setKey ( 2, 1, 100 )

print ( floatCurve:getValueAtTime ( 0.5 ))

vec2Curve = MOAIAnimCurve.new ()
vec2Curve:reserveKeys ( 2, MOAIAnimCurve.TYPE_VEC2 )
vec2Curve:setKey ( 1, 0 )
vec2Curve:setKey ( 2, 1 )
vec2Curve:setSample ( 1, 0, 0 )
vec2Curve:setSample ( 2, 50, 100 )

print ( vec2Curve:getValueAtTime ( 0.5 ))

vec3Curve = MOAIAnimCurve.new ()
vec3Curve:reserveKeys ( 2, MOAIAnimCurve.TYPE_VEC3 )
vec3Curve:setKey ( 1, 0 )
vec3Curve:setKey ( 2, 1 )
vec3Curve:setSample ( 1, 0, 0, 0 )
vec3Curve:setSample ( 2, 25, 50, 100 )

print ( vec3Curve:getValueAtTime ( 0.5 ))
