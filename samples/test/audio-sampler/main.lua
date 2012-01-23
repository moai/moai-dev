-- sampler playback demo

MOAISim.openWindow( "test", 400,400)

function minmax(data)
   local min,max=99999,-9999999
   for i=1,#data do
      if data[i] < min then min = data[i] end
      if data[i] > max then max = data[i] end
   end
   return min,max
end
function levelstring(data)
   local min,max = minmax(data)
   local total = math.abs(min) + math.abs(max)
   if total < 0.01 then
      return " "
   elseif total < 0.05 then
      return "."
   elseif total < 0.15 then
      return ".."
   else
      return "..."
   end
end

                       
-- common factors
local unit = 0.3
local nChannels = 1

-- setup recorder
local sampler = MOAIAudioSampler.new()
sampler:setFrequency(44100)
sampler:setNumChannels(nChannels)
sampler:prepareBuffer(unit,10)
sampler:start()

-- setup player
MOAIUntzSystem.initialize ()
local playbuf = MOAIUntzSampleBuffer.new()
playbuf:prepareBuffer( nChannels, 44100 * unit, 44100 )

local snd = MOAIUntzSound.new()
snd:load(playbuf)
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
                 local data = sampler:read()
                 if data and type(data)=="table" then
--                    print( string.format( "RC %d                   %s", #tape, levelstring(data)) )
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
                       print( string.format( "PL %d %s", #tape, levelstring(data) ) )
                       
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