----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if  pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIFileStream' )
end

function test ()
	
	MOAITestMgr.beginTest ( 'MOAIFileStream.zipFile' )
	success = true
	
	---------------------------------------------------------------
	
	stream = MOAIFileStream.new ()
	stream:open ( 'test.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing 8 bit read and write capabilities 
		stream:write8 ( 10, 12, -14 )
		if stream: getLength () ~= i * 3 + 3 then
			evaluate (false, '8 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val1, val2, val3, bytes = stream:read8 ( 3 )
		
		if stream:getCursor () ~= i * 3 + 3 then 
			evaluate (false, '8 bit getCursor')
		end
		
		if val1 ~= 10 or val2 ~= 12 or val3 ~= -14  then 
			evaluate (false, '8 bit read') 
		end
		
		if bytes ~= 1 then
			evaluate (false, '8 bit read') 
		end
		i = i + 1
		
	end

	stream:close ()
	
	---------------------------------------------------------------
	
	stream = MOAIFileStream.new ()
	stream:open ( 'test2.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing 16 bit read and write capabilities 
		stream:write16 ( 16 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, '16 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	--print ( stream:getLength())
	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read16 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, '16 bit getCursor')
		end
		
		if val ~= 16 then 
			evaluate (false, '16 bit read')
		end
		
		if bytes ~= 2 then
			evaluate (false, '16 bit read') 
		end
		i = i + 1
	end
	
	stream:close ()
	
	---------------------------------------------------------------
	
	stream = MOAIFileStream.new ()
	stream:open ( 'test3.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing 32 bit read and write capabilities 
		stream:write32 ( -32 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, '32 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read32 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, '32 bit getCursor')
		end
		
		if val ~= -32 then 
			evaluate (false, '32 bit read') 
		end
		
		if bytes ~= 4 then
			evaluate (false, '32 bit read') 
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	
	stream = MOAIFileStream.new ()
	stream:open ( 'test4.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Double read and write capabilities 
		stream:writeDouble ( 390 )
		if stream:getLength () ~= i + 1 then
			evaluate (false, 'Double getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readDouble ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Double getCursor')
		end
		
		if val ~= 390 then 
			evaluate (false, 'Double read') 
		end
		
		if bytes ~= 8 then
			evaluate (false, 'Double read')
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test5.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Float read and write capabilities 
		stream:writeFloat ( 7 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Float getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readFloat ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Float getCursor')
		end
		
		if val ~= 7 then 
			evaluate (false, 'Float read')
		end
		
		if bytes ~= 4 then
			evaluate (false, 'Float read')
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test6.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Format read and write capabilities 
		stream:writeFormat ( 's8,s16,s32,u8,u16,u32,f,d', 1, 2, 3, 4, 5, 6, 1.5, 2.5 )
		if stream: getLength () ~= i * 26 + 26 then
			evaluate (false, 'Format getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val1, val2, val3, val4, val5, val6, val7, val8, val9, bytes = stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' )
		
		if stream:getCursor () ~= i * 26 + 26 then 
			evaluate (false, 'Format getCursor')
		end
		
		if val1 ~= 's8,s16,s32,u8,u16,u32,f,d' or val2 ~= 1 or val3 ~= 2 or val4 ~= 3 or val5 ~= 4 or val6 ~= 5 
		or val7 ~= 6 or val8 ~= 1.5 or val9 ~= 2.5 then 
			evaluate (false, 'Format read')
		end
		
		if bytes ~= 4 then
			evaluate (false, 'Format read')
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test7.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 8 bit read and write capabilities 
		stream:writeU8 ( -8 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Unsigned 8 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU8 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Unsigned 8 bit getCursor')
		end
		
		if val ~= 248 then 
			evaluate (false, 'Unsigned 8 bit read')
		end
		
		if bytes ~= 1 then
			evaluate (false, 'Unsigned 8 bit read')
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test8.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 16 bit read and write capabilities 
		stream:writeU16 ( -16 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Unsigned 16 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU16 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Unsigned 16 bit getCursor')
		end
		
		if val ~= 65520 then 
			evaluate (false, 'Unsigned 16 bit read')
		end
		
		if bytes ~= 2 then
			evaluate (false, 'Unsigned 16 bit read') 
		end
		i = i + 1 
	end
	
	stream:close ()

	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test9.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 32 bit read and write capabilities 
		stream:writeU32 ( -32 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Unsigned 32 bit getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		--print ( stream:readU32())
		val, bytes = stream:readU32 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Unsigned 32 bit getCursor')
		end
		
		if val ~= 4294967264 then 
			evaluate (false, 'Unsigned 32 bit read')
		end
		
		if bytes ~= 4 then
			evaluate (false, 'Unsigned 32 bit read')
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIFileStream.new ()
	stream:open ( 'test10.zip', MOAIFileStream.READ_WRITE_NEW )
	
	i = 0
	
	while 100 > i do 	-- Testing write capabilities 
		stream:write ( 'uberstrikewrite' )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Write getLength')
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		print ( stream:readU32())
		val, bytes = stream:read ( 15 )  -- Testing read capabilities
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Read getCursor')
		end
		
		if val ~= ( 'uberstrikewrite' ) then 
			evaluate (false, 'Read read')
		end
		
		if bytes ~= 15 then
			evaluate (false, 'Read read') 
		end
		i = i + 1 
	end
	
	stream:close ()

	
	MOAITestMgr.endTest ( success )
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
