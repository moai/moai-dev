----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SAMPLE_RATE = 44100
WINDOW_SIZE = 1024
WINDOW_BYTES = WINDOW_SIZE * 2
BLEND = 0.125

MOAISim.openWindow ( "test", 1024, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 512 )
viewport:setScale ( 1024, 512 )

layer = MOAITableViewLayer.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
layer:pushRenderPass ()

fourier = MOAIFourier.new ()
fourier:init ( WINDOW_SIZE, true )
fourier:setOutputType ( MOAIFourier.OUTPUT_OCTAVES, 3, 44100 )
fourier:setWindowFunction ( MOAIFourier.HANN )

--for i = 1, WINDOW_SIZE do
--	print ( 'index', i, fourier:getFrequencyForIndex ( i ))
--end

--for i = 1, fourier:countOctaves () do
--	print ( 'octave', i, fourier:getWidthOfOctave ( i ))
--end

bands = fourier:countBands ()

octaves = MOAIByteStream.new ()
octaves:open ( bands * 4 )

meter = MOAIByteStream.new ()
meter:open ( bands * 4 )

for i = 1, bands do
	meter:writeFloat ( 0 )
end

sampleStream = MOAIMemStream.new ()

onSampleBufferFull = function ()

	sampleStream:seek ( 0 )
	octaves:seek ( 0 )

	fourier:transform (
		sampleStream,
		MOAIFourier.SAMPLE_S16,
		false,
		octaves,
		MOAIFourier.SAMPLE_FLOAT
	)

	octaves:seek ( 0 )
	meter:seek ( 0 )

	for i = 1, bands do

		local newResult = octaves:readFloat ()
		local oldResult = meter:readFloat ()

		meter:seek ( meter:getCursor () - 4 )
		meter:writeFloat (( newResult * BLEND ) + ( oldResult * ( 1 - BLEND )))
	end

	sampleStream:seek ( 0 )
end

sampler = MOAIAudioSamplerCocoa.new ()
sampler:init ( 44100, 1, WINDOW_SIZE )
sampler:setStream ( sampleStream )
sampler:setListener ( MOAIAudioSamplerCocoa.EVENT_BUFFER, onSampleBufferFull )
sampler:start ()

startTime = MOAISim.getDeviceTime ()
elapsedFrames = 0

onDraw = function ()

    MOAIDraw.bind ()

	meter:seek ( 0 )

	local span = 1024 / bands

	for i = 1, bands do

		local x = (( i - 1 ) * span ) - ( 1024 / 2 )
		local y = ( meter:readFloat () * 64 ) - 256

		MOAIDraw.setPenColor ( 1, 0, 0, 1 )
		MOAIDraw.fillRect ( x, y, x + span, -256 )
	end
end

layer:setRenderTable ({ onDraw })
