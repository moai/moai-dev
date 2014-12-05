----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello!" )

function onConnectivityChanged ( code )
	print ( "onConnectivityChanged: " )

	if ( code == MOAIEnvironment.CONNECTION_TYPE_WIFI ) then
		print ( "wifi network available" )
	elseif ( code == MOAIEnvironment.CONNECTION_TYPE_WWAN ) then
		print ( "mobile carrier network available" )
	else
		print ( "connectivity lost" )
	end
end

code = MOAIEnvironment.getConnectionType ()
if ( code == MOAIEnvironment.CONNECTION_TYPE_WIFI ) then
	print ( "wifi network available" )
elseif ( code == MOAIEnvironment.CONNECTION_TYPE_WWAN ) then
	print ( "mobile carrier network available" )
else
	print ( "no connection" )
end

MOAIEnvironment.setListener ( MOAIEnvironment.CONNECTIVITY_CHANGED, onConnectivityChanged )

----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 90, 5.0 )
