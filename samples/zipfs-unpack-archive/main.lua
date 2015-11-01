
function unpackArchiveTo ( zipfile, outputDir )
	
	mountPoint = "unpackArchiveToTemp/"

	function stripMountName ( string )
		local mountLen = mountPoint:len()
		string = string:sub ( mountLen, string:len())
		return string
	end

	function extractDir ( dirName )
	
		dirs = MOAIFileSystem.listDirectories ( dirName )
		files = MOAIFileSystem.listFiles ( dirName )

		if files ~= nil then
			for i, name in ipairs ( files ) do
				local databuffer = MOAIDataBuffer.new ()
				databuffer:load ( dirName .. name )
				local saveName = outputDir .. stripMountName ( dirName ) .. name
				databuffer:save ( saveName )
			end
		end

		if dirs ~= nil then
			for i, name in ipairs ( dirs ) do
				extractDir ( dirName .. name )
			end
		end
	end

	MOAIFileSystem.mountVirtualDirectory ( mountPoint, zipfile )

	extractDir ( mountPoint )
	
	MOAIFileSystem.mountVirtualDirectory ( nil, nil )

end

unpackArchiveTo ( "../resources/test.zip", '../temp/extract/' )
print ( "done ")
