
--[[
	Demo for a basic GUI system

	This demonstrates creating a widget list programmatically.
]]

local gui = require "gui\\gui"
local resources = require "gui\\support\\resources"
local filesystem = require "gui\\support\\filesystem"
local inputconstants = require "gui\\support\\inputconstants"
local layermgr = require "layermgr"

-- Turn off the texture loading logging
MOAILogMgr.setLogLevel(MOAILogMgr.LOG_NONE)

-- Setup your basic window
local width = 320
local height = 480

MOAISim.openWindow("Widget List", width, height)

-- Create the GUI, passing in the dimensions of the screen
local g = gui.GUI(width, height)

-- Search through these for specified resources
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

-- Create a widget list, and have it take up the entire window
local widgetList = g:createWidgetList()
widgetList:setPos(0, 0)
widgetList:setDim(100, 100)

-- Sets the image used when a row is selected. In this case, a white box
widgetList:setSelectionImage(resources.getPath("selection image.png"))

-- Add two columns, and set their widths
widgetList:addColumn("Name", 80)
widgetList:addColumn("Data", 14)

-- Height of the rows. Labels don't need much room, but other widgets (eg. Images) might
widgetList:setRowHeight(15)

-- Set the widgets that will be used for each column. The first parameter is the column
-- to be set. The second and third define the function to be used for the widget creation.
-- The third parameter is the name of the actual function, while the second parameter
-- specifies the object (ie table) the function belongs to. Normally, this is simply the
-- created GUI object, and the function associated with the creation of the desired widget.
-- However, this could also be a unique user-created widget type.
widgetList:setColumnWidget(1, g, "createLabel")
widgetList:setColumnWidget(2, g, "createLabel")

-- widgetList:setMaxSelections(3)		-- This allows up to three selections to be made

-- Add some rows, and fill it with some junk data
for i = 1, 30 do
	local row = widgetList:addRow()

	-- The return from getCell is the widget created by setColumnWidget, so the normal
	-- functionality for the widget is available.
	row:getCell(1):setText("Name " .. i)
	row:getCell(2):setText(tostring(i))
end

-- Callback functions for input
-- When the user provides input, it must be sent into the GUI system so that it can
-- be handled properly.
function onKeyboardEvent(key, down)
	if (down == true) then
		g:injectKeyDown(key)
	else
		g:injectKeyUp(key)

		-- Get the current selections made by the user, and print them out
		local selections = widgetList:getSelections()
		if (nil == selections) then return end

		print("---------------------")
		for i = 1, #selections do
			print(selections[i])
		end
	end
end

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
MOAIInputMgr.device.keyboard:setCallback(onKeyboardEvent)
MOAIInputMgr.device.pointer:setCallback(onPointerEvent)
MOAIInputMgr.device.mouseLeft:setCallback(onMouseLeftEvent)
MOAIInputMgr.device.mouseMiddle:setCallback(onMouseMiddleEvent)
MOAIInputMgr.device.mouseRight:setCallback(onMouseRightEvent)




