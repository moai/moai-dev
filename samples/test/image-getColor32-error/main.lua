----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------
img = MOAIImage.new()
	
img:load ( 'moai.png' )
img:setColor32 ( 80, 15, 50 )
print ( img:getColor32 ( 80, 15 ))