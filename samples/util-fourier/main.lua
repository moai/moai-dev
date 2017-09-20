----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- great demo on an excellent site
-- http://madebyevan.com/dft/
-- also fun
-- http://betterexplained.com/articles/an-interactive-guide-to-the-fourier-transform/
-- http://toxicdump.org/stuff/FourierToy.swf

signal			= MOAIMemStream.new ()
transform		= MOAIMemStream.new ()
reconstruction	= MOAIMemStream.new ()

for i = 1, 128 do
	--local s = ( i >= 56 and i <= 72 ) and 1 or 0
	--signal:writeFloat ( s )

	signal:writeFloat ( math.cos ((( i - 1 ) * math.pi * 2 ) / 128 ))
end

fourier = MOAIFourier.new ()
fourier:init ( 128 )
fourier:setOutputType ( MOAIFourier.OUTPUT_COMPLEX )

signal:seek ( 0 )
fourier:transform (
	signal,
	MOAIFourier.SAMPLE_FLOAT,
	false,
	transform,
	MOAIFourier.SAMPLE_FLOAT
)

transformMax = 0
transform:seek ( 0 )
for i = 1, 128 do
	local s = math.abs ( transform:readFloat ())
	transformMax = transformMax < s and s or transformMax
end

fourier:init ( 128, true )
fourier:setOutputType ( MOAIFourier.OUTPUT_REAL )

transform:seek ( 0 )
fourier:transform (
	transform,
	MOAIFourier.SAMPLE_FLOAT,
	true,
	reconstruction,
	MOAIFourier.SAMPLE_FLOAT
)

MOAISim.openWindow ( "test", 768, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 768, 512 )
viewport:setScale ( 768, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
MOAISim.pushRenderPass ( layer )

drawSignal = function ( stream, yOff, yScale )

	yScale = yScale or 1

	MOAIGfxMgr.setPenColor ( 0, 0, 1, 1 )

	stream:seek ( 0 )
	for i = 1, 128 do

		local s = stream:readFloat ()

		local x = ( i - 64 ) * 4
		local y = ( s * yScale * 32 ) + yOff

		MOAIDraw.drawLine ( x, yOff, x, y )
		MOAIDraw.fillCircle ( x, y, 2 )
	end
end

drawTransform = function ( stream, yOff )

	stream:seek ( 0 )
	for i = 1, 128 do

		local offset = i < 64 and 64 or -64

		local xR = (( i - 64 ) + offset ) * 4
		local xI = xR + 1

		local yR = (( stream:readFloat () / transformMax ) * 64 ) + yOff
		local yI = (( stream:readFloat () / transformMax ) * 64 ) + yOff

		MOAIGfxMgr.setPenColor ( 0, 1, 0, 1 )
		MOAIDraw.drawLine ( xI, yOff, xI, yI )
		MOAIDraw.fillCircle ( xI, yI, 2 )

		MOAIGfxMgr.setPenColor ( 0, 0, 1, 1 )
		MOAIDraw.drawLine ( xR, yOff, xR, yR )
		MOAIDraw.fillCircle ( xR, yR, 2 )
	end
end


onDraw = function ()

	drawSignal ( signal, 128 )
	drawTransform ( transform, 0 )
	drawSignal ( reconstruction, -128, 1 / 128 )
end

layer:setOverlayTable ({ onDraw })
