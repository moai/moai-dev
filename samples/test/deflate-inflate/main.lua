----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

m = dofile ( 'SaveLoad.lua' )

-- load the working table
tab = {}
tab = m:loadTable ( 'working' )
m:saveTable ( tab, 'mynewtable' )

-- enable to verify that the table returned by loadTable exists
-- printTable ( tab )

-- load and inflate the table we just deflated and saved
tab2 = m:loadTableInflate ( 'mynewtable' )
print ( tab2 )
printTable ( tab2 )
