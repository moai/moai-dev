----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local dirs = MOAIFileSystem.listDirectories ( '../' )

for i, dir in ipairs ( dirs ) do
	print ( i, dir )
	local files = MOAIFileSystem.listFiles ( '../' .. dir )
	for j, file in ipairs ( files ) do
		print ( '  ', j, file )
	end
end
