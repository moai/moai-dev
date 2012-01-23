MOAISim.openWindow( "test", 400,400)


print("start")
sampler = MOAIAudioSampler.new()

print("config")
sampler:setFrequency(44100)
sampler:setNumChannels(2)

print("prepare")
sampler:prepareBuffer(0.1,10)

print("start")
sampler:start()

print("loop")
th = MOAIThread.new()
th:run( function()
           while true do
              local data = sampler:read()
              if data and type(data)=="table" then
                 local maxlevel=0
                 for i,v in ipairs(data) do
                    if v > maxlevel then
                       maxlevel = v
                    end
                 end
                 local s = string.format( "%2.2f  ", maxlevel * 30 )
                 for i=1,maxlevel*30 do
                    s = s .. "."
                 end
                 print(s)
              end
              coroutine.yield()
           end           
        end)