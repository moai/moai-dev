----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local bytes = MOAIFileSystem.loadFile ( 'sample32.out' )
bytes = MOAILuaUtil.convert ( bytes )
loadstring ( bytes )()
