----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "Mounting test.zip: " .. tostring ( MOAIFileSystem.mountVirtualDirectory ( 'testzip', '../resources/test.zip' )))

print ( 'INFO: testzip/assets', MOAIFileSystem.getVirtualPathInfo ( 'testzip/assets' ))
print ( 'INFO: testzip/assets/levels', MOAIFileSystem.getVirtualPathInfo ( 'testzip/assets/levels' ))

print ( 'INFO: testzip/assets/levels/main.lua', MOAIFileSystem.getVirtualPathInfo ( 'testzip/assets/levels/main.lua' ))
print ( 'INFO: testzip/assets/levels/run.bat', MOAIFileSystem.getVirtualPathInfo ( 'testzip/assets/levels/run.bat' ))
print ( 'INFO: testzip/assets/levels/testhelpers.lua', MOAIFileSystem.getVirtualPathInfo ( 'testzip/assets/levels/testhelpers.lua' ))
