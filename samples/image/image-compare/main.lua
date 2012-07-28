----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

img1 = MOAIImage.new ()
img1:load ( "moai.png" )

img2 = MOAIImage.new ()
img2:load ( "moai.png" )

equal = img1:compare ( img2 )

print ( 'Testing if moai.png and moai.png are equal: ' .. tostring ( equal ))

img2:load ( "moai2.png" )

equal = img1:compare ( img2 )

print ( 'Testing if moai.png and moai2.png are equal: ' .. tostring ( equal ))