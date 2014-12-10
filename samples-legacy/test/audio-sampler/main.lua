-- sampler playback demo

MOAISim.openWindow( "test", 400,400)

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


local freq = 44100 --5000 --11025 --22050 --44100
MOAIUntzSystem.initialize ( freq, 8192, MOAIUntzSystem.RECORDABLE ) 
--MOAIUntzSystem.initialize ( freq, 8192 )

function minmax(data)
   local min,max=99999,-9999999
   for i=1,#data do
      if data[i] < min then min = data[i] end
      if data[i] > max then max = data[i] end
   end
   return min,max
end
function levelstring(min,max)
   local total = math.abs(min) + math.abs(max)
   if total < 0.01 then
      return " "
   elseif total < 0.05 then
      return "."
   elseif total < 0.15 then
      return ".."
   elseif total < 0.5 then
      return "..." .. max
   else
      return "****" .. max
   end
end

function lowpass(data,alpha)
   local out = {}
   local y_old = 0
   for i,x in ipairs(data) do
      local y = alpha * x + ( 1.0 - alpha ) * y_old
      out[i] = y
      y_old = y
   end
   return out
end

-- common factors
local unit = 0.3
local nChannels = 1


-- setup recorder
local sampler = MOAIAudioSampler.new()
sampler:setFrequency(freq)
sampler:setNumChannels(nChannels)
sampler:prepareBuffer(unit,10)
sampler:start()

-- setup player


local playbuf = MOAIUntzSampleBuffer.new()
playbuf:prepareBuffer( nChannels, freq * unit, freq )

print("sound new")
local snd = MOAIUntzSound.new()
print("sound load")
snd:load(playbuf)
print("sound loaded")
snd:setVolume(1)
snd:setLooping(false)
snd:play()


-- setup tape
local tape={}
local toPlay,toRec=true, true



local delayAdjuster = 0.85 -- because of 60Hz MOAI thread timing, it slightly delays to play.


th = MOAIThread.new()
th:run( function()
           local toggledAt = MOAISim.getDeviceTime()
           
           while true do
              local curtime = MOAISim.getDeviceTime()

              -- record it!
              if toRec then
                 local data = sampler:read( "short" ) -- float:-1~1 char:-127~127 short:-32767~32767
                 if data then
                    print( data[1], data[2] )
                    for i,v in ipairs(data) do data[i] = v / 32767 end
                 end
                 
                 if data and type(data)=="table" then
                    local min,max = minmax(data)
                    print( string.format( "RC %d                   %s", #tape, levelstring(min,max)))
                    local data = lowpass(data,0.01) -- to avoid howling
                    table.insert( tape, data )
                 end
              end

              -- then play it!              
              if toPlay then
                 if not playedAt then playedAt = curtime end
                 if playedAt < curtime - unit * delayAdjuster then 
                    playedAt = curtime
                    
                    local data = tape[1]
                    if data then
                       local min,max = minmax(data)
                       local ls = levelstring(min,max)
                       print( string.format( "PL %d %s", #tape, ls ) )
                       prop:seekLoc( 0 + max * 50 , 0, 0.5 )
                       
                       playbuf:setData( data, 1 )
                       snd:setPosition(0)
                       snd:play()
                    end
                    
                    table.remove(tape,1)
                 end
              end
              
              coroutine.yield()
           end           
        end)