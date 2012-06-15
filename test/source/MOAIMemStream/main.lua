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
		stream:write8 ( 10, 12, -14 )
		if stream: getLength () ~= i * 3 + 3 then
			evaluate (false, '8 bit getLength')
			break
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
			break
		end
		
		if val1 ~= 10 or val2 ~= 12 or val3 ~= -14  then 
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
		stream:write16 ( 16 )
		if stream: getLength () ~= i * 2 + 2 then
			evaluate (false, '16 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read16 ()
		
		if stream:getCursor () ~= i * 2 + 2 then 
			evaluate (false, '16 bit getCursor')
			break
		end
		
		if val ~= 16 then 
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
		stream:write32 ( -32 )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, '32 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:read32 ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, '32 bit getCursor')
			break
		end
		
		if val ~= -32 then 
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
		stream:writeDouble ( 390 )
		if stream:getLength () ~= i * 8 + 8 then
			evaluate (false, 'Double getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readDouble ()
		
		if stream:getCursor () ~= i * 8 + 8 then 
			evaluate (false, 'Double getCursor')
			break
		end
		
		if val ~= 390 then 
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
		stream:writeFloat ( 7 )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, 'Float getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readFloat ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, 'Float getCursor')
			break
		end
		
		if val ~= 7 then 
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
		stream:writeFormat ( 's8,s16,s32,u8,u16,u32,f,d', 1, 2, 3, 4, 5, 6, 1.5, 2.5 )
		if stream: getLength () ~= i * 26 + 26 then
			evaluate (false, 'Format getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val1, val2, val3, val4, val5, val6, val7, val8, bytes = stream:readFormat ( 's8,s16,s32,u8,u16,u32,f,d' )
		
		if stream:getCursor () ~= i * 26 + 26 then 
			evaluate (false, 'Format getCursor')
			break
		end
		
		if val1 ~= 1 or val2 ~= 2 or val3 ~= 3 or val4 ~= 4 or val5 ~= 5 or val6 ~= 6 
		or val7 ~= 1.5 or val8 ~= 2.5 then 
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
		stream:writeU8 ( -8 )
		if stream: getLength () ~= i + 1 then
			evaluate (false, 'Unsigned 8 bit getLength')
			break
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
			break
		end
		
		if val ~= 248 then 
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
		stream:writeU16 ( -16 )
		if stream: getLength () ~= i * 2 + 2 then
			evaluate (false, 'Unsigned 16 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU16 ()
		
		if stream:getCursor () ~= i * 2 + 2 then 
			evaluate (false, 'Unsigned 16 bit getCursor')
			break
		end
		
		if val ~= 65520 then 
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
		stream:writeU32 ( -32 )
		if stream: getLength () ~= i * 4 + 4 then
			evaluate (false, 'Unsigned 32 bit getLength')
			break
		end 
		
		i = i + 1
	end
	
	stream:flush ()

	stream:seek ()	
	
	i = 0
	
	while 100 > i do 
		val, bytes = stream:readU32 ()
		
		if stream:getCursor () ~= i * 4 + 4 then 
			evaluate (false, 'Unsigned 32 bit getCursor')
			break
		end
		
		if val ~= 4294967264 then 
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

	
	MOAITestMgr.endTest ( success )
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
