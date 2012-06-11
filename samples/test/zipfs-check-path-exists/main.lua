----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

-- broken code here
print ( "Mounting test.zip: " .. tostring ( MOAIFileSystem.mountVirtualDirectory ( "testzip", "test.zip" )))

print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip" )))
print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/" )))
print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/" )))
print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets" )))
print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/levels/" )))
print ( "Checking if path exists: " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/levels" )))