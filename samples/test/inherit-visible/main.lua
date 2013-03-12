----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

parent = MOAIProp2D.new ()
parent:setDeck ( gfxQuad )
parent:setLoc(-10,10)
layer:insertProp ( parent )

child = MOAIProp2D.new ()
child:setDeck ( gfxQuad )
child:setLoc(10,-10)
layer:insertProp ( child )

child:setAttrLink ( MOAIProp.INHERIT_VISIBLE, parent, MOAIProp.ATTR_VISIBLE )
printf ( 'child.INHERIT_VISIBLE  <- parent.ACTUAL_VISIBLE\n\n' )

parent:setVisible ( false )
child:setVisible ( true )
parent:forceUpdate ()
child:forceUpdate ()

printf ( 'parent:setVisible ( false )\n' )
printf ( 'child:setVisible ( true )\n' )
printf ( 'parent ATTR_LOCAL_VISIBLE: %d\n', parent:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_LOCAL_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_VISIBLE ))
printf ( '\n' )

parent:setVisible ( false )
child:setVisible ( false )
parent:forceUpdate ()
child:forceUpdate ()

printf ( 'parent:setVisible ( false )\n' )
printf ( 'child:setVisible ( false )\n' )
printf ( 'parent ATTR_LOCAL_VISIBLE: %d\n', parent:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_LOCAL_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_VISIBLE ))
printf ( '\n' )

parent:setVisible ( true )
child:setVisible ( true )
parent:forceUpdate ()
child:forceUpdate ()
printf ( 'parent:setVisible ( true )\n' )
printf ( 'child:setVisible ( true )\n' )
printf ( 'parent ATTR_LOCAL_VISIBLE: %d\n', parent:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_LOCAL_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_VISIBLE ))
printf ( '\n' )

parent:setVisible ( true )
child:setVisible ( false )
parent:forceUpdate ()
child:forceUpdate ()

printf ( 'parent:setVisible ( true )\n' )
printf ( 'child:setVisible ( false )\n' )
printf ( 'parent ATTR_LOCAL_VISIBLE: %d\n', parent:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_LOCAL_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_LOCAL_VISIBLE ))
printf ( 'child ATTR_VISIBLE: %d\n', child:getAttr ( MOAIProp.ATTR_VISIBLE ))
printf ( '\n' )
