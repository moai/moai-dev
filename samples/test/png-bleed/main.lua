----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

test = MOAIImage.new ()
test:load ( 'test.png' )

out = MOAIImage.new ()
out:init ( 256, 256, MOAIImage.COLOR_FMT_RGB_888 )

out:copyBits ( test, 0, 0, 1, 1, 64, 64 )
out:copyBits ( test, 64, 0, 67, 1, 64, 64 )
out:copyBits ( test, 0, 64, 1, 67, 64, 64 )
out:copyBits ( test, 64, 64, 67, 67, 64, 64 )

out:bleedRect ( 1, 1, 65, 65 )
out:bleedRect ( 67, 1, 131, 65 )
out:bleedRect ( 1, 67, 65, 131 )
out:bleedRect ( 67, 67, 131, 131 )

out:writePNG ( 'out.png' )
