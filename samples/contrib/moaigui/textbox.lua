
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

-- Turn off the texture loading logging
MOAILogMgr.setLogLevel(MOAILogMgr.LOG_NONE)

-- Setup your basic window
local width = 320
local height = 480

MOAISim.openWindow("Text Box", width, height)

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

-- Create a text box, a scrollable box of text
local text = g:createTextBox()
text:setPos(5, 10)
text:setDim(90, 80)
text:setLineHeight(4)

-- Add lots of text
text:addText("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the ")
text:addText("industry\'s standard dummy text ever since the 1500s, when an unknown printer took a galley of type and ")
text:addText("scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap ")
text:addText("into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the ")
text:addText("release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing ")
text:addText("software like Aldus PageMaker including versions of Lorem Ipsum.")

-- Ends the current line, and adds an extra for space.
text:newLine(2)
text:addText("It is a long established fact that a reader will be distracted by the readable content of a page when ")
text:addText("looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution ")
text:addText("of letters, as opposed to using \'Content here, content here\', making it look like readable English. ")
text:addText("Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, ")
text:addText("and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have ")
text:addText("evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).")
text:addText("Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical ")
text:addText("Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at ")
text:addText("Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a ")
text:addText("Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the ")
text:addText("undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et ")
text:addText("Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the ")
text:addText("theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, \'Lorem ipsum ")
text:addText("dolor sit amet..\', comes from a line in section 1.10.32.")

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




