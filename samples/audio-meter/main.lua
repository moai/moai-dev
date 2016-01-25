----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SAMPLE_RATE = 44100
WINDOW_SIZE = 1024
WINDOW_BYTES = WINDOW_SIZE * 2

MOAISim.openWindow ( "test", 1024, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 512 )
viewport:setScale ( 1024, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
MOAISim.pushRenderPass ( layer )

sampleStream = MOAIMemStream.new ()
sampleBuffer = MOAIMemStream.new ()

onSampleBufferFull = function ()

	sampleStream:seek ( 0 )
	sampleBuffer:seek ( 0 )

	sampleBuffer:writeStream ( sampleStream, WINDOW_BYTES )

	sampleStream:seek ( 0 )
end

sampler = MOAIAudioSamplerCocoa.new ()
sampler:init ( 44100, 1, WINDOW_SIZE )
sampler:setStream ( sampleStream )
sampler:setListener ( MOAIAudioSamplerCocoa.EVENT_BUFFER, onSampleBufferFull )
sampler:start ()

startTime = MOAISim.getDeviceTime ()
elapsedFrames = 0

fourier = MOAIFourier.new ()
fourier:init ( WINDOW_SIZE )
fourier:setOutputType ( MOAIFourier.OUTPUT_OCTAVES )

frequencyBuffer = MOAIByteStream.new ()
frequencyBuffer:open ( WINDOW_SIZE * 4 )

onDraw = function ()

	if sampleBuffer:getCursor () >= WINDOW_BYTES then

		sampleBuffer:seek ( 0 )
		frequencyBuffer:seek ( 0 )

		fourier:transform (
			sampleBuffer,
			MOAIFourier.SAMPLE_S16,
			false,
			frequencyBuffer,
			MOAIFourier.SAMPLE_FLOAT
		)

		sampleBuffer:seek ( 0 )
	end

	local length = frequencyBuffer:getLength () / 4

	if length > 0 then

		frequencyBuffer:seek ( 0 )

		local span = WINDOW_SIZE / length

		for i = 1, length do

			local x = (( i - 1 ) * span ) - ( WINDOW_SIZE / 2 )
			local y = math.abs ( frequencyBuffer:readFloat ()) * 128

			MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
			MOAIDraw.fillRect ( x, y, x + span, 0 )
		end
	end
end

layer:setOverlayTable ({ onDraw })
