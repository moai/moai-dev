----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

local function printTable ( t )
	for k, v in pairs ( t ) do
		print ( k, v )
	end
	print ()
end

print ( MOAIScriptNode )
print ()

local node = MOAIScriptNode.new ()
node.foo = 1
node.bar = 2

print ( node )
print ( 'foo', node.foo )
print ( 'bar', node.bar )
print ( node.getClass )
print ( node.getClassName )
print ( node.setInterface )
print ( node.setMembers )
print ()

local members = {}
members.baz = 3
members.qux = 4

node:setMembers ( members )

print ( node )
print ( 'foo', node.foo )
print ( 'bar', node.bar )
print ( 'baz', node.baz )
print ( 'qux', node.qux )
print ( node.getClass )
print ( node.getClassName )
print ( node.setInterface )
print ( node.setMembers )
print ()

local interface = {}
interface.__index = interface
interface.FOO = 1
interface.BAR = 2

node:setInterface ( interface )
print ( node )
print ( 'foo', node.foo )
print ( 'bar', node.bar )
print ( 'baz', node.baz )
print ( 'qux', node.qux )
print ( node.getClass )
print ( node.getClassName )
print ( node.setInterface )
print ( node.setMembers )
print ( 'FOO', node.FOO )
print ( 'BAR', node.BAR )
print ()

local node1 = MOAIScriptNode.new ()
local node2 = MOAIScriptNode.new ()

local refs = node1:getRefTable ()

node1:pin ( node2 )
printTable ( refs )

node1:pin ( node2 )
printTable ( refs )

node1:unpin ( node2 )
printTable ( refs )

node1:unpin ( node2 )
printTable ( refs )



