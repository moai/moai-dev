--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

module ( "savefiles", package.seeall )

----------------------------------------------------------------
----------------------------------------------------------------
-- variables
----------------------------------------------------------------
local saveFiles = {}

----------------------------------------------------------------
-- exposed functions
----------------------------------------------------------------
function get ( filename )

	if not saveFiles [ filename ] then
		saveFiles [ filename ] = makeSaveFile ( filename ) 
		saveFiles [ filename ]:loadGame ()
	end	
	
	return saveFiles [ filename ]
end

----------------------------------------------------------------
-- local functions
----------------------------------------------------------------
function makeSaveFile ( filename )

	local savefile = {}
	
	savefile.filename = filename
	savefile.fileexist = false
	savefile.data = nil
	
	----------------------------------------------------------------
	savefile.loadGame = function ( self )

		local fullFileName = self.filename .. ".lua"
		local workingDir
		
		if DEVICE then
			workingDir = MOAIFileSystem.getWorkingDirectory ()
			MOAIFileSystem.setWorkingDirectory ( MOAIEnvironment.documentDirectory )
		end
		
		if MOAIFileSystem.checkFileExists ( fullFileName ) then
			local file = io.open ( fullFileName, 'rb' )
			savefile.data = dofile ( fullFileName )
			self.fileexist = true
		else
			savefile.data = {}
			self.fileexist = false
		end

		if DEVICE then
			MOAIFileSystem.setWorkingDirectory ( workingDir )
		end
		
		return self.fileexist
	end
	
	----------------------------------------------------------------
	savefile.saveGame = function ( self )

		local fullFileName = self.filename .. ".lua"
		local workingDir
		local serializer = MOAISerializer.new ()

		self.fileexist = true
		serializer:serialize ( self.data )
		local gamestateStr = serializer:exportToString ()
		
		if not DEVICE then
			local file = io.open ( fullFileName, 'wb' )
			file:write ( gamestateStr )
			file:close ()
			
		else
			workingDir = MOAIFileSystem.getWorkingDirectory () 
			MOAIFileSystem.setWorkingDirectory ( MOAIEnvironment.documentDirectory )

			local file = io.open ( fullFileName, 'wb' )
			file:write ( gamestateStr )
			file:close ()
			MOAIFileSystem.setWorkingDirectory ( workingDir )
		end
	end

	return savefile
end
