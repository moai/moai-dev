
--[[
	Demo for a basic GUI system

	This demonstrates loading a GUI layout file.
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

-- Respond to button 2 being clicked. This responder uses the method of a class, in order
-- to demonstrate how this is done.
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

MOAISim.openWindow("Various From Layout", width, height)

-- Create the GUI, passing in the dimensions of the screen
local g = gui.GUI(width, height)

-- Search through these for specified resources
g:addToResourcePath(filesystem.pathJoin("resources", "fonts"))
g:addToResourcePath(filesystem.pathJoin("resources", "gui"))
g:addToResourcePath(filesystem.pathJoin("resources", "media"))
g:addToResourcePath(filesystem.pathJoin("resources", "themes"))
g:addToResourcePath(filesystem.pathJoin("resources", "layouts"))

-- Add the gui layer to the rendering stack, making sure its always high up on the
-- layer stack
layermgr.addLayer("gui", 99999, g:layer())

-- This sets up the theme to be used for widgets. If images aren't set
-- manually for a widget, the system looks to the theme for the images
-- to use.
g:setTheme("basetheme.lua")

-- The font used for text
g:setCurrTextStyle("default")

-- Load the widgets from the specified file
-- Parameter 1: filename of the layout
-- Parameter 2: (optional) a prefix added to the name (from the layout file) of each widget; this
--		is to help avoid name collisions
-- Returns three values, but the second is the only one we're really interested in
local roots, widgets, groups = g:loadLayout(resources.getPath("variouslayout.lua"), "various")

-- Check if the buttons are a part of the layout file. If they are, register their events
if (nil ~= widgets.variousbutton1) then
	local button = widgets.variousbutton1.window
	button:registerEventHandler(button.EVENT_BUTTON_CLICK, nil, _handleButton1Click)
end

local buttonHandler = ButtonHandler()
if (nil ~= widgets.variousbutton2) then
	local button = widgets.variousbutton2.window
	button:registerEventHandler(button.EVENT_BUTTON_CLICK, buttonHandler, "handleButtonClick")
end

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




