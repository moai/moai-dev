----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function makeNode ( name )
	local node = MOAINode.new ()
	node:setListener ( MOAINode.EVENT_NODE_POST_UPDATE, function () print ( 'update', name ) end )
	return node
end

a = makeNode ( 'A' )
b = makeNode ( 'B' )
c = makeNode ( 'C' )
d = makeNode ( 'D' )
e = makeNode ( 'E' )

b:setNodeLink ( a )
c:setNodeLink ( b )

a:scheduleUpdate ()
c:forceUpdate ()
