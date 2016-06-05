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
gfxQuad:setTexture ( '../resources/moai.png' )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

writeSine = function ( stream, hz, sampleRate, length )

	for i = 1, ( length * sampleRate ) do
		local t = (( i / sampleRate ) * hz ) % 1
		local s = math.sin ( t * math.pi * 2 )
		stream:write16 ( s < 0 and ( s * 32768 ) or ( s * 32767 ))
	end
end

writeTriangle = function ( stream, hz, sampleRate, length )

	for i = 1, ( length * sampleRate ) do
		local t = (( i / sampleRate ) * hz ) % 1
		t = t < 0.5 and ( t * 2 ) or ( 1 - (( t - 0.5 ) * 2 ))
		local s = ( t * 2 ) - 1
		stream:write16 ( s < 0 and ( s * 32768 ) or ( s * 32767 ))
	end
end

writeSawtooth = function ( stream, hz, sampleRate, length )

	for i = 1, ( length * sampleRate ) do
		local t = ((( i / sampleRate ) * hz ) + 0.25 ) % 1
		local s = ( t * 2 ) - 1
		stream:write16 ( s < 0 and ( s * 32768 ) or ( s * 32767 ))
	end
end

writeSquare = function ( stream, hz, sampleRate, length )

	for i = 1, ( length * sampleRate ) do
		local t = (( i / sampleRate ) * hz ) % 1
		t = t < 0.5 and 1 or -1
		local s = ( t * 2 ) - 1
		stream:write16 ( s < 0 and ( s * 32768 ) or ( s * 32767 ))
	end
end

stream = MOAIMemStream.new ()
writeSine ( stream, 1000, 44100, 1 )
writeTriangle ( stream, 1000, 44100, 1 )
writeSawtooth ( stream, 1000, 44100, 1 )
writeSquare ( stream, 1000, 44100, 1 )
stream:seek ( 0 )

length = stream:getLength ()

MOAIUntzSystem.initialize ()

buffer = MOAIUntzSampleBuffer.new ()
buffer:prepareBuffer ( 1, length, 44100 )
buffer:setRawData ( stream:read (), length, 1 )

sound = MOAIUntzSound.new ()
sound:load ( buffer )
sound:setVolume ( 1 )
sound:setLooping ( true )
sound:play ()
