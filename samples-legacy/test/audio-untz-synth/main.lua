
MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck(gfxQuad)
layer:insertProp ( prop )
prop:moveRot(360,10)


-------------------

MOAIUntzSystem.initialize ()

-- get wav data from file
local sampleFromFile = MOAIUntzSampleBuffer.new()
sampleFromFile:load('mono16.wav')


-- prepare on-mem buffer
local totalLenSec = 1
local freq = 44100
local nChan = 1
local sampleOnMemory = MOAIUntzSampleBuffer.new()
sampleOnMemory:prepareBuffer( nChan, freq * totalLenSec , freq )


function generateSinTone( note, n,level )
   local data = {}
   for i=1,n do
      data[i] = math.sin( i / note ) * level -- set wavedata from -1 to 1
   end
   return data
end

local note = 3
local duration = 0.2
local level = 0.3
local wavedata = generateSinTone( note, freq*nChan*duration, level )
sampleOnMemory:setData( wavedata,1 ) -- 1: use data from first index of wavedata
note = note + 1 -- get lower sounds
wavedata = generateSinTone( note, freq*nChan*duration, level )

sampleOnMemory:setData( wavedata, freq*nChan*duration + 1 ) 


-- to mix wav file and on-mem sounds
memsound = MOAIUntzSound.new()
memsound:load(sampleOnMemory)
memsound:setVolume(1)
memsound:setLooping(true)
memsound:play()

filesound = MOAIUntzSound.new()
filesound:load(sampleFromFile)
filesound:setVolume(1)
filesound:setLooping(false)
filesound:play()



th = MOAIThread.new()
th:run(function()
          while true do
             print("st:", filesound:isPlaying() )
             coroutine.yield()
          end          
       end)