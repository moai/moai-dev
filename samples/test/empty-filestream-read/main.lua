----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

stream = MOAIFileStream.new ()
stream:open ( 'test.bin', MOAIFileStream.READ_WRITE_NEW )

print ( stream:read8 ())
print ( stream:read16 ())
print ( stream:read32 ())
print ( stream:readU8 ())
print ( stream:readU16 ())
print ( stream:readU32 ())
print ( stream:readFloat ())
print ( stream:readDouble ())
print ( stream:readFormat ( 's8' ))

stream:close ()
