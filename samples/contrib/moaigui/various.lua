
--[[
	Demo for a basic GUI system

	This demonstrates creating various widgets programmatically.
]]

require "gui\\support\\class"

local gui = require "gui\\gui"
local resources = require "gui\\support\\resources"
local filesystem = require "gui\\support\\filesystem"
local inputconstants = require "gui\\support\\inputconstants"
local layermgr = require "layermgr"

ButtonHandler = class()

-- Respond to button 1 being clicked
function _handleButton1Click(event, data)
	print("Button 1 Click")
end

-- Respond to button 2 being clicked
function ButtonHandler:handleButtonClick(event, data)
	print("Button 2 Click")
end

function ButtonHandler:init()

end

-- Turn off the texture loading logging
MOAILogMgr.setLogLevel(MOAILogMgr.LOG_NONE)

-- Setup your basic window
local width = 320
local height = 480

MOAISim.openWindow("Various", width, height)

-- Create the GUI, passing in the dimensions of the screen
local g = gui.GUI(width, height)

-- Resource paths - search through these for specified resources
g:addToResourcePath(filesystem.pathJoin("resources", "fonts"))
g:addToResourcePath(filesystem.pathJoin("resources", "gui"))
g:addToResourcePath(filesystem.pathJoin("resources", "media"))
g:addToResourcePath(filesystem.pathJoin("resources", "themes"))

-- Add the gui layer to the rendering stack, making sure its always high up on the
-- layer stack
layermgr.addLayer("gui", 99999, g:layer())

-- This sets up the theme to be used for widgets. If images aren't set
-- manually for a widget, the system looks to the theme for the images
-- to use.
g:setTheme("basetheme.lua")

-- The font used for text
g:setCurrTextStyle("default")

-- Show some simple text
local label = g:createLabel()
label:setPos(5, 3)
label:setDim(50, 5)
label:setText("Just some simple text")

-- A couple buttons
local button = g:createButton()
button:setPos(5, 10)
button:setDim(30, 10)
button:setText("button 1")

-- -- Register the button click event for this widget. In this case, we are registering to a
-- -- standard function, so the second parameter is 'nil'
button:registerEventHandler(button.EVENT_BUTTON_CLICK, nil, _handleButton1Click)

local buttonHandler = ButtonHandler()
local button = g:createButton()
button:setPos(45, 10)
button:setDim(30, 20)
button:setText("image button")
button:setNormalImage(resources.getPath("moai.png"), 1, 1, 1, 1)
button:setHoverImage(resources.getPath("moai.png"), 1, 0, 0, 1)
button:setPushedImage(resources.getPath("moai.png"), 0, 1, 0, 1)
button:setDisabledImage(resources.getPath("moai.png"), 0, 0, 1, 1)

-- Register the button click event for this widget. This time, we are registering to a
-- method of a class, so the second parameter is the class instance, and the function is
-- referenced by name (so the object can call it by table index)
button:registerEventHandler(button.EVENT_BUTTON_CLICK, buttonHandler, "handleButtonClick")

-- Static image
local image = g:createImage()
image:setPos(5, 23)
image:setDim(22, 15)
image:setText("Image")
image:setImage(resources.getPath("moai.png"), 1, 1, 1, 1)
image:setTextAlignment(image.TEXT_ALIGN_CENTER, image.TEXT_ALIGN_BOTTOM)

-- -- Progress bar.
-- local progress = g:createProgressBar()
-- progress:setPos(45, 33)
-- progress:setDim(30, 5)
-- progress:setProgress(40)

-- A couple radio buttons
for i = 1, 3 do
	local radio = g:createRadioButton()
	radio:setPos((i - 1) * 25 + 5, 40)
	radio:setDim(20, 4)
	radio:setText("Radio")

	-- Assign all of these to the same group, so only one in this group can be selected at a time
	radio:setGroup(1)
end

-- Some check boxes
for i = 1, 3 do
	local check = g:createCheckBox()
	check:setPos((i - 1) * 25 + 5, 50)
	check:setDim(20, 4)
	check:setText("Check")
end

local label = g:createLabel()
label:setPos(5, 60)
label:setDim(30, 5)
label:setText("Slide Me")

-- A horizontal slider, with its current value displayed below the slider
local slider = g:createHorzSlider()
slider:setPos(35, 60)
slider:setDim(50, 5)
slider:setRange(1, 20)

local label = g:createLabel()
label:setPos(5, 70)
label:setDim(30, 5)
label:setText("Enter Text")

-- Allows entry of keyboard input
local edit = g:createEditBox()
edit:setPos(35, 70)
edit:setDim(50, 5)

-- Callback functions for input
-- When the user provides input, it must be sent into the GUI system so that it can
-- be handled properly.
function onKeyboardEvent(key, down)
	if (down == true) then
		g:injectKeyDown(key)
	else
		g:injectKeyUp(key)
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

function onPointerEvent(x, y)
	g:injectMouseMove(x, y)
end

function onMouseLeftEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.LEFT_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.LEFT_MOUSE_BUTTON)
	end
end

function onMouseMiddleEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.MIDDLE_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.MIDDLE_MOUSE_BUTTON)
	end
end

function onMouseRightEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.RIGHT_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.RIGHT_MOUSE_BUTTON)
	end
end

-- Register the callbacks for input
MOAIInputMgr.device.pointer:setCallback(onPointerEvent)
MOAIInputMgr.device.mouseLeft:setCallback(onMouseLeftEvent)
MOAIInputMgr.device.mouseMiddle:setCallback(onMouseMiddleEvent)
MOAIInputMgr.device.mouseRight:setCallback(onMouseRightEvent)




