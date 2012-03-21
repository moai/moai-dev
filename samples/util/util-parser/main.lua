----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

----------------------------------------------------------------
function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end

indent = 0

----------------------------------------------------------------
function printIndent ( n )

	for i = 1, n do
		printf ( '  ' )
	end
end

----------------------------------------------------------------
function onStartNT ( id, line, name )

	printIndent ( indent )
	printf ( 'Start terminal: %d %d %s\n', id, line, name )
	indent = indent + 1
end

----------------------------------------------------------------
function onEndNT ( id )

	indent = indent - 1
	printIndent ( indent )
	printf ( 'End terminal: %d\n', id )
end

----------------------------------------------------------------
function onT ( id, line, text )

	printIndent ( indent )
	printf ( 'Terminal: %d %d %s\n', id, line, text )
end

parser = MOAIParser.new ()

parser:setCallbacks ( onStartNT, onEndNT, onT )
parser:loadRules ( 'simple.cgt' )
parser:loadFile ( 'test.txt' )
parser:traverse ()


