
function gatherResults ( results, allResults )
	if MOAIFileSystem.checkFileExists ( results ) then
		
		inFile = MOAIFileStream.new ()
		outFile = MOAIFileStream.new ()
		
		inFile:open ( results, MOAIFileStream.READ )
		outFile:open ( allResults, MOAIFileStream.READ_WRITE_AFFIRM )
		
		outFile:seek ( 0, MOAIFileStream.SEEK_END )
		outFile:writeStream ( inFile )
		
		inFile:close ()
		outFile:close ()
	end
end
