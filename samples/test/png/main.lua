----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

cathead = MOAIImage.new ()
cathead:load ( 'moai.png' )

out = MOAIImage.new ()
out:init ( 128, 128, MOAIImage.COLOR_FMT_RGB_888 )
out:copyBits ( cathead, 0, 32, 0, 32, 128, 64 )
out:bleedRect ( 0, 32, 128, 96 )
out:bleedRect ( 32, 32, 96, 96 )
out:writePNG ( 'out.png' )

