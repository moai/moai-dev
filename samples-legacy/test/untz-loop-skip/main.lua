-- Open the game window
MOAISim.openWindow( "Looping Sounds" , 800, 600 )
MOAISim.setStep ( 1 / 60 )

MOAIUntzSystem.initialize ()

--------------------------------------------
-- Change this stuff around for testing
--------------------------------------------

local volume = 0.9
local track = "track1/"                 -- You can swap this with "track1/" as well.
local ext = "ogg"                       -- You can wap this with "ogg" as well.
local load_from_buffer = true           -- Whether MOAIUntzSampleBuffer to load the sound
local load_into_memory = true           -- Whether Load the file into memory when using MOAIUntzSound:load ()
local print_position = true             -- Print the current position with MOAIUntzSound:getPosition ()
local sound_to_play = "start_and_loop"  -- "start" - just the start, "loop" - just the loop, "start_and_loop" - start and loop in a single file

--------------------------------------------
-- Print some info
--------------------------------------------

print ( "" )
print ( "Testing '" .. track .. "' in '" .. ext .. "' format." )
if load_from_buffer then

    print ( "Loading sounds from MOAIUntzSampleBuffer" )

else

    if load_into_memory then

        print ( "Loading sounds from disk into memory" )

    else

        print ( "Loading sounds from disk, streaming" )

    end

end

print ( "Volume set to " .. volume )

print ( "" )

--------------------------------------------
-- Start testing
--------------------------------------------

-- Starting (intro) part of track
local start = MOAIUntzSound.new ()

if load_from_buffer then

    local buff = MOAIUntzSampleBuffer.new ()
    buff:load ( track .. "start." .. ext )
    start:load ( buff )

else

    start:load ( track .. "start." .. ext, load_into_memory )

end
start:setVolume ( volume )

print ( "Start Length: " .. start:getLength () )

-- Looping part of track
local loop = MOAIUntzSound.new ()
if load_from_buffer then

    local buff = MOAIUntzSampleBuffer.new ()
    buff:load ( track .. "loop." .. ext )
    loop:load ( buff )

else

    loop:load ( track .. "loop." .. ext, load_into_memory )

end
loop:setLooping ( true )
loop:setVolume ( volume )

print ( "Loop Length: " .. loop:getLength () )

-- Intro and loop merged together
local start_and_loop = MOAIUntzSound.new ()
if load_from_buffer then

    local buff = MOAIUntzSampleBuffer.new ()
    buff:load ( track .. "start_and_loop." .. ext )
    start_and_loop:load ( buff )

else

    start_and_loop:load ( track .. "start_and_loop." .. ext, load_into_memory )

end
start_and_loop:setLooping ( true )
start_and_loop:setLoopPoints ( start:getLength (), start_and_loop:getLength () )
start_and_loop:setVolume ( volume )

print ( "Start and Loop Length: " .. start_and_loop:getLength () )

--------------------------------------------
-- Change what plays
--------------------------------------------

local sound = nil
if sound_to_play == "start" then

    sound = start

elseif sound_to_play == "loop" then

    sound = loop

else

    sound = start_and_loop

end

local prev_position = sound:getPosition ()
sound:play ()


if print_position then

    local function looper ()

        while true do

            local curr_position = sound:getPosition ()
            if prev_position > curr_position then

                print ( "" )
                print ( "Looping" )
                print ( "" )

            end

            print ( "Current position: " .. curr_position )
            prev_position = curr_position

            coroutine.yield ()

        end

    end

    local thread = MOAICoroutine.new()
    thread:run ( looper, self )

end


