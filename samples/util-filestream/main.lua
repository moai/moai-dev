----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

stream = MOAIFileStream.new ()
stream:open ( 'test.bin', MOAIFileStream.READ_WRITE_NEW )

stream:write8 ()
stream:write8 ( -8, 9, -10 )
stream:write16 ( -16 )
stream:write32 ( -32 )
stream:writeU8 ( -8 )
stream:writeU16 ( -16 )
stream:writeU32 ( -32 )
stream:write ( 'foobarbaz' )
stream:writeFormat ( 's8,s16,s32,u8,u16,u32,f,d', 1, 2, 3, 4, 5, 6, 1.5, 2.5 )
stream:flush ()

stream:seek ()

print ( stream:read8 ( 3 ))
print ( stream:read16 ())
print ( stream:read32 ())
print ( stream:readU8 ())
print ( stream:readU16 ())
print ( stream:readU32 ())
print ( stream:read ( 9 ))
print ( stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' ))

stream:close ()