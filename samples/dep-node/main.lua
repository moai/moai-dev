----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
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

d:setNodeLink ( c )
e:setNodeLink ( d )

print ( 'E->D->C->B->A' )
print ( 'SCHEDULED NODE A' )
a:scheduleUpdate ()

print ( 'FORCE UPDATE NODE C' )
c:forceUpdate ()

print ( 'FORCE UPDATE NODE C' )
c:forceUpdate ()
c:forceUpdate ()
b:forceUpdate ()

print ( 'NODEMGR UPDATE' )
MOAINodeMgr.update ()
