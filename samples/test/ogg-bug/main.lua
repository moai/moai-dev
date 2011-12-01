MOAIUntzSystem.initialize ()

thread = MOAIThread.new ()

thread:run (

   function ()
   sound = MOAIUntzSound.new ()
   sound:load ( 'correctding.ogg', false)
   sound:setLooping ( false )
   sound:setVolume(1)
   sound:play()
       while true do
           coroutine.yield ()
           sound = nil
           MOAISim.forceGarbageCollection ()
       end
   end
)
