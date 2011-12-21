----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


grid = MOAIGridSpace.new ()
grid:setSize ( 4, 4, 16, 8 )
grid:setShape ( MOAIGridSpace.DIAMOND_SHAPE )

function test ( x, y, xExpected, yExpected )
	x, y = grid:locToCoord ( x, y )
	
	if x == xExpected and y == yExpected then
		print ( x, y, 'OK!' )
	else
		print ( x, y, 'NOT OK!' )
	end
end

test ( 1, 1,	0, 0 )
test ( 3, 3,	1, 1 )
test ( 3, 5,	1, 1 )
test ( 1, 7,	0, 2 )
test ( 15, 1,	1, 0 )
test ( 13, 3,	1, 1 )
test ( 13, 5,	1, 1 )
test ( 15, 7,	1, 2 )
test ( 7, 9,	1, 1 )
test ( 5, 11,	0, 2 )
test ( 5, 13,	0, 2 )
test ( 7, 15,	1, 3 )
test ( 9, 9,	1, 1 )
test ( 11, 11,	1, 2 )
test ( 11, 13,	1, 2 )
test ( 9, 15,	1, 3 )
