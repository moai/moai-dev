----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIJsonParser' )
end

function printTable ( t, tableName, indentationLevel ) 
          
    if type ( t ) ~= "table" then 
         print ( "WARNING: printTable received \"" .. type ( t ) .. "\" instead of table. Skipping." ) 
         return 
    end 
     
    local topLevel = false 
     
    if ( not tableName ) and ( not indentationLevel ) then 
          
         topLevel = true 
         indentationLevel = 1 
          
         print ( "\n----------------------------------------------------------------" ) 
         print ( tostring ( t ) .. "\n" )
    else 
         print ( "\n" .. string.rep ( "\t", indentationLevel - 1 ) .. tableName .. " = {" ) 
    end 
     
    if t then 
         for k,v in pairs ( t ) do 
               
              if ( type ( v ) == "table" ) then  
                    
                   printTable ( v, tostring ( k ), indentationLevel + 1 ) 
                    
              elseif ( type ( v ) == "string" ) then 
                    
                   print ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = \"" .. tostring ( v ) .. "\"," ) 
              else 
               
                   print ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = " .. tostring ( v ) .. "," ) 
              end 
         end 
    end 
     
    if topLevel then 
         print ( "\n----------------------------------------------------------------\n" )
    else 
         print ( string.rep ( "\t", indentationLevel - 1 ) .. "},\n" ) 
    end 
end

function test ()
	
	MOAITestMgr.beginTest ( 'MOAIJsonParser' )
	success = true
	
	---------------------------------------------------------------------------
		
	object = {
	foo = "str1",
	bar = 3,
	baz = 123.456,
	fee = {
		a = 1,
		b = 2,
		c = 3,
		},
	fie = {
		4,
		5,
		6,
		},
	fum = true,
	}
	
	str = MOAIJsonParser.encode ( object )
	 
	object2 = MOAIJsonParser.decode ( str )
	
	evaluate ( object2.foo == object.foo, 'object.foo is different' )
	evaluate ( object2.bar == object.bar, 'object.bar is different' )
	evaluate ( object2.baz == object.baz, 'object.baz is different' )
	evaluate ( object2.fee.a == object.fee.a, 'object.fee.a is different' )
	evaluate ( object2.fee.b == object.fee.b, 'object.fee.b is different' )
	evaluate ( object2.fee.c == object.fee.c, 'object.fee.c is different' )
	evaluate ( object2.fie[ 1 ] == object.fie[ 1 ], 'object.fie1 is different' )
	evaluate ( object2.fie[ 2 ] == object.fie[ 2 ], 'object.fie2 is different' )
	evaluate ( object2.fie[ 3 ] == object.fie[ 3 ], 'object.fie3 is different' )
	evaluate ( object2.fum == object.fum, 'object.fum is different' )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
