----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SAMPLE_RATE = 44100
WINDOW_SIZE = 1024
WINDOW_BYTES = WINDOW_SIZE * 2
OFFSET = WINDOW_SIZE / 2
DELAY = 0.125

sampleStream = MOAIMemStream.new ()

sampler = MOAIAudioSamplerCocoa.new ()
sampler:init ( 44100, 1, 2048 )
sampler:setStream ( sampleStream )
sampler:setListener ( MOAIAudioSamplerCocoa.EVENT_BUFFER, onSampleBufferFull )
sampler:start ()

MOAISim.openWindow ( "test", 1024, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 512 )
viewport:setScale ( 1024, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
MOAISim.pushRenderPass ( layer )

fourier = MOAIFourier.new ()
fourier:init ( WINDOW_SIZE )
fourier:setOutputType ( MOAIFourier.OUTPUT_REAL )

frequencyBuffer = MOAIByteStream.new ()
frequencyBuffer:open ( WINDOW_SIZE * 4 * 2 )

startTime = MOAISim.getDeviceTime ()
elapsedFrames = 0

onDraw = function ()

	local elapsedTime	= MOAISim.getDeviceTime () - startTime - DELAY

	if elapsedTime < 0 then return end

	local length		= sampleStream:getLength ()

	local targetFrame	= math.floor (( elapsedTime * SAMPLE_RATE ) / WINDOW_SIZE )
	local totalFrames	= elapsedFrames + math.floor ( length / WINDOW_BYTES )

	if elapsedFrames ~= targetFrame and targetFrame < totalFrames then

		sampleStream:seek (( targetFrame - elapsedFrames ) * WINDOW_BYTES )
		frequencyBuffer:seek ( 0 )

		fourier:transform (
			sampleStream,
			MOAIFourier.SAMPLE_S16,
			false,
			frequencyBuffer,
			MOAIFourier.SAMPLE_FLOAT
		)

		elapsedFrames = targetFrame + 1
		sampleStream:discardFront ()
	end

	if frequencyBuffer:getLength () > 0 then

		frequencyBuffer:seek ( 0 )

		for i = 1, WINDOW_SIZE do

			local offset = i < OFFSET and OFFSET or -OFFSET
			local x = ( i - OFFSET ) + offset
			local y = frequencyBuffer:readFloat () / 2

			MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
			MOAIDraw.drawLine ( x, 0, x, y )
		end
	end
end

layer:setOverlayTable ({ onDraw })
