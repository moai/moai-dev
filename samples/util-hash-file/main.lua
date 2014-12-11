
----------------------------------------------------------------
hashFile1 = function ( filename )
	
	local file = MOAIFileStream.new ()
	file:open ( filename )
	local data = file:read ()
	file:close ()
	
	local writer = MOAIHashWriter.new ()
	writer:openMD5 ()
	writer:write ( data )
	writer:close ()
	
	return writer:getHashHex ()
end

----------------------------------------------------------------
hashFile2 = function ( filename )
	
	local file = MOAIFileStream.new ()
	file:open ( filename )

	local writer = MOAIHashWriter.new ()
	writer:openMD5 ()
	writer:writeStream ( file )
	writer:close ()
	file:close ()
	
	return writer:getHashHex ()
end

print ( hashFile1 ( 'test.txt' ))
print ( hashFile2 ( 'test.txt' ))
print ( 'd174ab98d277d9f5a5611c2c9f419d9f' )