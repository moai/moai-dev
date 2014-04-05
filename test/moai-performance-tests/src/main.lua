-- import
local tests = require "tests"

-- Setting the FPS
MOAISim.setStep(1 / 60)

-- Setting of the operation of the Loop.
MOAISim.clearLoopFlags()
MOAISim.setLoopFlags(MOAISim.SIM_LOOP_ALLOW_BOOST)
MOAISim.setLoopFlags(MOAISim.SIM_LOOP_LONG_DELAY)

-- Sets the boost threshold
MOAISim.setBoostThreshold(0)

-- init
SCREEN_WIDTH = MOAIEnvironment.horizontalResolution or 640
SCREEN_HEIGHT = MOAIEnvironment.verticalResolution or 480

-- Open
MOAISim.openWindow ( "test", SCREEN_WIDTH, SCREEN_HEIGHT )

-- Performance measurement.
local timer = MOAITimer.new()
timer:setMode(MOAITimer.LOOP)
timer:setSpan(2)
timer:setListener(MOAITimer.EVENT_TIMER_LOOP,
    function()
        print("-------------------------------------------")
        print("FPS:", MOAISim.getPerformance())
        print("Draw:", MOAIRenderMgr.getPerformanceDrawCount())
    end)
timer:start()

local currentIndex = 1
local currentTest = tests[currentIndex]
currentTest.start()

local function switch()
    currentTest.stop()
    currentIndex = currentIndex + 1
    currentIndex = currentIndex > #tests and 1 or currentIndex
    currentTest = tests[currentIndex]
    currentTest.start()
end
 
local onTouch = function(eventType, idx, x, y, tapCount)
    if eventType == MOAITouchSensor.TOUCH_UP then
        switch()
    end
end
 
local onClick = function(down)
    if not down then
        switch()
    end
end
 
if MOAIInputMgr.device.touch then
    MOAIInputMgr.device.touch:setCallback(onTouch)
else
    MOAIInputMgr.device.mouseLeft:setCallback(onClick)
end