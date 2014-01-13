----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

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

----------------------------------------------------------------
-- EXAMPLE
----------------------------------------------------------------

ws = MOAIWebSocket.new()

function onMessageReceived( msg ) 
	print("WebSocket: " .. msg )
end

function onConnected( msg ) 
	print("WebSocket: " .. msg )
	ws:write("Hello")
	ws:write("help")
end

function onClosed( msg ) 
	print("WebSocket: " .. msg )
end

function onFailed( msg ) 
	print("WebSocket: " .. msg )
end

ws:setListener ( MOAIWebSocket.ON_MESSAGE, onMessageReceived )
ws:setListener ( MOAIWebSocket.ON_CONNECT, onConnected )
ws:setListener ( MOAIWebSocket.ON_CLOSE, onClosed )
ws:setListener ( MOAIWebSocket.ON_FAIL, onFailed )

ws:start("ws://10.41.18.247:8080/ws")

