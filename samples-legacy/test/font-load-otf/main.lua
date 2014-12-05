----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!@#$%^&*():;'/-+\""
text = 'The quick brown fox jumps over the lazy dog. 0123456789'

font = MOAIFont.new ()
font:load ( 'FreeSansBold.otf' )
font:preloadGlyphs ( charcodes, 24 )
