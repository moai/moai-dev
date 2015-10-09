----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "Mounting test.zip: " .. tostring ( MOAIFileSystem.mountVirtualDirectory ( "testzip", "../resources/test.zip" )))

print ( "Checking if path exists: testzip/ - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/" )))
print ( "Checking if path exists: testzip - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip" )))
print ( "Checking if path exists: testzip/assets/ - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/" )))
print ( "Checking if path exists: testzip/assets - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets" )))
print ( "Checking if path exists: testzip/assets/levels/ - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/levels/" )))
print ( "Checking if path exists: testzip/assets/levels - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/levels" )))
print ( "Checking if path exists: testzip/assets/levels/../../assets/ - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/assets/levels/../../assets/" )))
print ( "Checking if path exists: testzip/bogus/ - " .. tostring ( MOAIFileSystem.checkPathExists ( "testzip/bogus/" )))
