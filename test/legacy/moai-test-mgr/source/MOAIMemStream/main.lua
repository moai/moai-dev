----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIMemStream' )
end

function test ()
	
	MOAITestMgr.beginTest ( 'MOAIMemStream' )
	success = true
	
	---------------------------------------------------------------
	
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing 8 bit read and write capabilities 
		stream:write8 ( 10, i + 12, -( i + 14 ) )
		if stream: getLength () ~= i * 3 + 3 then
			evaluate (false, '8 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 35 )	
	if stream:read8 () ~= -25  then 
		evaluate (false, '8 bit seek forwards')
	end
	
	stream:seek ( -32, MOAIMemStream.SEEK_CUR ) 
	if stream:read8 () ~= 13 then
		evaluate (false, '8 bit seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val1, val2, val3, bytes = stream:read8 ( 3 )
		
		if stream:getCursor () ~= i * 3 + 3 then 
			evaluate (false, '8 bit getCursor')
			break
		end
		
		if val1 ~= 10 or val2 ~= i + 12 or val3 ~= -( i + 14 ) then 
			evaluate (false, '8 bit read') 
			break
		end
		
		if bytes ~= 3 then
			evaluate (false, '8 bit read') 
			break
		end
		i = i + 1
		
	end

	stream:close ()
	
	---------------------------------------------------------------
	
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing 16 bit read and write capabilities 
		stream:write16 ( i )
		if stream: getLength () ~= i * 2 + 2 then
			evaluate (false, '16 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 48 ) 
	if stream:read16 () ~= 24  then 
		evaluate (false, '16 bit seek forwards	')
	end
	
	stream:seek ( -40, MOAIMemStream.SEEK_CUR )
	if stream:read16 () ~= 5 then 
		evaluate (false, '16 bit seek back')
	end
	
	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read16 ()
		
		if stream:getCursor () ~= i * 2 + 2 then 
			evaluate (false, '16 bit getCursor')
			break
		end
		
		if val ~= i then 
			evaluate (false, '16 bit read')
			break
		end
		
		if bytes ~= 2 then
			evaluate (false, '16 bit read') 
			break
		end
		i = i + 1
	end
	
	stream:close ()
	
	---------------------------------------------------------------
	
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing 32 bit read and write capabilities 
		stream:write32 ( -i )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, '32 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 200 ) 
	if stream:read32 () ~= -50  then 
		evaluate (false, '32 bit seek forwards')
	end
	
	stream:seek ( -100, MOAIMemStream.SEEK_CUR )
	if stream:read32 () ~= -26 then 
		evaluate (false, '32 bit seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read32 ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, '32 bit getCursor')
			break
		end
		
		if val ~= -i then 
			evaluate (false, '32 bit read') 
			break
		end
		
		if bytes ~= 4 then
			evaluate (false, '32 bit read') 
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Double read and write capabilities 
		stream:writeDouble ( i + 30 )
		if stream:getLength () ~= i * 8 + 8 then
			evaluate (false, 'Double getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 600 )
	if stream:readDouble () ~= 105  then 
		evaluate (false, 'Double seek forward')
	end
	
	stream:seek ( -480, MOAIMemStream.SEEK_CUR )
	if stream:readDouble () ~= 46 then 
		evaluate (false, 'Double seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readDouble ()
		
		if stream:getCursor () ~= i * 8 + 8 then 
			evaluate (false, 'Double getCursor')
			break
		end
		
		if val ~= i + 30 then 
			evaluate (false, 'Double read') 
			break
		end
		
		if bytes ~= 8 then
			evaluate (false, 'Double read')
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Float read and write capabilities 
		stream:writeFloat ( i )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, 'Float getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 240 )
	if stream:readFloat () ~= 60  then 
		evaluate (false, 'Float seek forwards')
	end
	
	stream:seek ( -4, MOAIMemStream.SEEK_CUR ) 
	if stream:readFloat () ~= 60 then
		evaluate (false, 'Float seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readFloat ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, 'Float getCursor')
			break
		end
		
		if val ~= i then 
			evaluate (false, 'Float read')
			break
		end
		
		if bytes ~= 4 then
			evaluate (false, 'Float read')
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Format read and write capabilities 
		stream:writeFormat ( 's8,s16,s32,u8,u16,u32,f,d', i, i + 1, 3, 4, i + 12, 6, 1.5, i + 2.5 )
		if stream: getLength () ~= i * 26 + 26 then
			evaluate (false, 'Format getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 130 )
	val1, val2, val3, val4, val5, val6, val7, val8 = stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' )
	if val1 ~= 5 or val2 ~= 6 or val3 ~= 3 or val4 ~= 4 or val5 ~= 17 or val6 ~= 6 or val7 ~= 1.5 or val8 ~= 7.5 then 
		evaluate (false, 'Format seek forwards')
	end
	
	stream:seek ( -78, MOAIMemStream.SEEK_CUR )
	val1, val2, val3, val4, val5, val6, val7, val8 = stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' )
	if val1 ~= 3 or val2 ~= 4 or val3 ~= 3 or val4 ~= 4 or val5 ~= 15 or val6 ~= 6 or val7 ~= 1.5 or val8 ~= 5.5 then 
		evaluate (false, 'Format seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val1, val2, val3, val4, val5, val6, val7, val8, bytes = stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' )
		
		if stream:getCursor () ~= i * 26 + 26 then 
			evaluate (false, 'Format getCursor')
			break
		end
		
		if val1 ~= i or val2 ~= i + 1 or val3 ~= 3 or val4 ~= 4 or val5 ~= i + 12 or val6 ~= 6 
		or val7 ~= 1.5 or val8 ~= i + 2.5 then 
			evaluate (false, 'Format read')
			break
		end
		
		if bytes ~= 26 then
			evaluate (false, 'Format read')
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 8 bit read and write capabilities 
		stream:writeU8 ( -i - 1 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Unsigned 8 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 34 )
	if stream:readU8 () ~= 221  then 
		evaluate (false, 'Unsigned 8 bit seek forward')
	end
	
	stream:seek ( -19, MOAIMemStream.SEEK_CUR )
	if stream:readU8 () ~= 239  then 
		evaluate (false, 'Unsigned 8 bit seek back')
	end

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU8 ()
		
		if stream:getCursor () ~= i + 1 then 
			evaluate (false, 'Unsigned 8 bit getCursor')
			break
		end
		
		if val ~= 255 - i then 
			evaluate (false, 'Unsigned 8 bit read')
			break
		end
		
		if bytes ~= 1 then
			evaluate (false, 'Unsigned 8 bit read')
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 16 bit read and write capabilities 
		stream:writeU16 ( -i - 16 )
		if stream: getLength () ~= i * 2 + 2 then
			evaluate (false, 'Unsigned 16 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ( 140 )
	if stream:readU16 () ~= 65450 then 
		evaluate (false, 'Unsigned 16 bit seek forward')
	end
	
	stream:seek ( -10, MOAIMemStream.SEEK_CUR )
	if stream:readU16 () ~= 65454 then 
		evaluate (false, 'Unsigned 16 bit seek back')
	end
	
	stream:seek ( )
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU16 ()
		
		if stream:getCursor () ~= i * 2 + 2 then 
			evaluate (false, 'Unsigned 16 bit getCursor')
			break
		end
		
		if val ~= 65520 - i then 
			evaluate (false, 'Unsigned 16 bit read')
			break
		end
		
		if bytes ~= 2 then
			evaluate (false, 'Unsigned 16 bit read') 
			break
		end
		i = i + 1 
	end
	
	stream:close ()

	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing Unsigned 32 bit read and write capabilities 
		stream:writeU32 ( i + 9 )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, 'Unsigned 32 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 24 )
	if stream:readU32 () ~= 15  then 
		evaluate (false, 'Unsigned 32 bit seek forward')
	end

	stream:seek ( -8, MOAIMemStream.SEEK_CUR )
	if stream:readU32 () ~= 14  then 
		evaluate (false, 'Unsigned 32 bit seek back')
	end
	
	stream:seek ()	
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU32 ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, 'Unsigned 32 bit getCursor')
			break
		end
		
		if val ~= i + 9 then 
			evaluate (false, 'Unsigned 32 bit read')
			break
		end
		
		if bytes ~= 4 then
			evaluate (false, 'Unsigned 32 bit read')
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	-- Testing write capabilities 
		stream:write ( 'uberstrikewrite' )
		if stream: getLength () ~= i * 15 + 15 then
			evaluate (false, 'Write getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 1125 )
	if stream:read ( 15 ) ~= 'uberstrikewrite'  then 
		evaluate (false, 'Read seek')
	end
	
	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read ( 15 )  -- Testing read capabilities
		
		if stream:getCursor () ~= i * 15 + 15 then 
			evaluate (false, 'Read getCursor')
			break
		end
		
		if val ~= ( 'uberstrikewrite' ) then 
			evaluate (false, 'Read read')
			break
		end
		
		if bytes ~= 15 then
			evaluate (false, 'Read read') 
			break
		end
		i = i + 1 
	end
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	data = 'This is a stream writer test. Detta ar en strom forfattare prov.'
	len = #data

	
	writer = MOAIStreamWriter.new ()
	writer:openDeflate ( stream )
	
	i = 0
	
	while 100 > i do	-- Writes 100 instances of data and length
		writer:write (data, len) 
		i = i + 1
	end	
	
	writer:close ()

	stream:seek () 
	
	reader = MOAIStreamReader.new ()
	reader:openDeflate ( stream )
		
	reader:seek ( 127 )
	if reader:read ( len ) ~= 'This is a stream writer test. Detta ar en strom forfattare prov.' then
		evaluate (false, 'StreamWriter-openDeflate-seekforward')
	end
		
	reader:seek ()
	
	i = 0
	
	while 100 > i do
		data = reader:read (len) -- Reads in data and length
		if data ~= ( 'This is a stream writer test. Detta ar en strom forfattare prov.' ) then
			evaluate (false, 'StreamWriter-openDeflate-read')
			break
		end
		
		if len ~= 64 then
			evaluate (false, 'StreamWriter-openDeflate-read')
			break
		end
		
		i = i + 1
	end 
	
	
	reader:close ()
	
	stream:close ()
	
	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	data = 'This is a stream writer test. Ceci est un test ecrivain flux.'
	len = #data

	
	writer = MOAIStreamWriter.new ()
	writer:openBase64 ( stream )
	
	i = 0
	
	while 100 > i do	-- Writes 100 instances of data and length
		writer:write (data, len) 
		i = i + 1
	end	
	
	writer:close ()
	
	stream:seek () 
	
	reader = MOAIStreamReader.new ()
	reader:openBase64 ( stream )
	
	reader:seek ( 100 ) -- Yields the same result no matter where I seek.
	if reader:read ( len ) ~= 'This is a stream writer test. Ceci est un test ecrivain flux.' then
		evaluate (false, 'StreamWriter-base64-seekforward')
	end
	
	reader:seek ()
	
	i = 0
	
	while 100 > i do
		data = reader:read (len) -- Reads in data and length
		if data ~= ( 'This is a stream writer test. Ceci est un test ecrivain flux.' ) then
			evaluate (false, 'StreamWriter-base64-read')
			break
		end
		
		if len ~= 61 then
			evaluate (false, 'StreamWriter-base64-read')
			break
		end
		
		i = i + 1
	end 	
	
	reader:close ()
	
	stream:close ()
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
