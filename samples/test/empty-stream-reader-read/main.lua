----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

stream = MOAIMemStream.new ()
stream:open ()

reader = MOAIStreamReader.new ()
reader:openBase64 ( stream )
print ( reader:read ( 1 ))
reader:close ()

stream:close ()
