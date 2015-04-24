----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 100 )
MOAIGfxDevice.setClearColor ( 0.25, 0, 0.5 )

local viewport = MOAIViewport.new ()
viewport:setSize ( 480, 100 )
viewport:setScale ( 480, 100 )

local layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local font = MOAIFont.new ()
font:loadFromTTF ( 'VL-PGothic.ttf' )

local textChars = { "E", "d", "i", "t", " ", "m", "e", "!" } -- This is Unicode-safe
local cursorIndex = #textChars -- 0-based

local editingText = ""

-- Show text label
local label = MOAITextLabel.new ()
label:setLoc ( -230, 0 )
label:setFont ( font )
label:setTextSize ( 32 )
label:setYFlip ( true )
label:setAlignment ( MOAITextBox.LEFT_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( label )

-- Set label text
function updateLabel ()
	label:setString ( table.concat ( textChars ) .. editingText )
end
updateLabel ()

function updateTextInputRect()
	if #editingText > 0 then
		-- TODO:getTextBounds does not return the actual range.
		local x1, y1, x2, y2 = label:getTextBounds ( math.max(#label:getText() - #editingText, 1), #editingText )
		local offsetX, offsetY = -115, 50
		local xMin, yMin, _ =layer:worldToWnd( x1 + offsetX, y1 + offsetY, 0 )
		local xMax, yMax, _ =layer:worldToWnd( x2 + offsetX, y2 + offsetY, 0 )
		MOAISim.setTextInputRect(xMin, yMin, xMax, yMax)
	end

end

-- Show cursor
local cursorColor = MOAIColor.new ()
local cursorDeck = MOAIScriptDeck.new ()
cursorDeck:setRect ( 0, 0, 1, 1 ) -- dummy
cursorDeck:setDrawCallback ( function ( index )
	MOAIGfxDevice.setPenColor ( cursorColor:getColor ())
	local cursorWidth = 2
	local cursorHeight = 30
	local cursorX
	if cursorIndex + #editingText > 0 then
		-- Let's be UTF-8-safe
		local byteIndex = #( table.concat ( textChars, "", 1, cursorIndex )) + #editingText
		local x1, y1, x2, y2 = label:getTextBounds ( 1, byteIndex )
		cursorX = x2 - x1 + 2
	else
		cursorX = 0
	end
	MOAIDraw.fillRect( cursorX - cursorWidth / 2, -cursorHeight / 2, cursorX + cursorWidth / 2, cursorHeight / 2 )
end )
local cursorProp = MOAIProp2D.new ()
cursorProp:setLoc ( -230, 0 )
cursorProp:setDeck ( cursorDeck )
layer:insertProp ( cursorProp )

-- Make cursor blink
local resetCursor = false
local blinkThread = MOAICoroutine.new ()
blinkThread:run ( function ()
	local showCursor = true
	while true do
		if showCursor then
			cursorColor:setColor ( 1, 1, 1, 1 )
		else
			cursorColor:setColor ( 0, 0, 0, 0 )
		end
		
		for i = 1, 40 do
			coroutine:yield()
			if resetCursor then break end
		end
		
		if resetCursor then
			showCursor = true
			resetCursor = false
		else
			showCursor = not showCursor
		end
	end
end )

-- Handle key-down events
MOAIInputMgr.device.keyboard:setKeyCallback ( function ( keyCode, down )
	if #editingText > 0 then
		return
	end
	if down then
		resetCursor = true
		
		if keyCode == MOAIKeyCode.LEFT then
			if cursorIndex > 0 then
				cursorIndex = cursorIndex - 1
			end
		elseif keyCode == MOAIKeyCode.RIGHT then
			if cursorIndex < #textChars then
				cursorIndex = cursorIndex + 1
			end
		elseif keyCode == MOAIKeyCode.HOME then
			cursorIndex = 0
		elseif keyCode == MOAIKeyCode.END then
			cursorIndex = #textChars
		elseif keyCode == MOAIKeyCode.BACKSPACE then
			if cursorIndex > 0 then
				table.remove ( textChars, cursorIndex )
				cursorIndex = cursorIndex - 1
				updateLabel ()
			end
		elseif keyCode == MOAIKeyCode.DELETE then
			if cursorIndex < #textChars then
				table.remove ( textChars, cursorIndex + 1 )
				updateLabel ()
			end
		end
	end
end )

MOAIInputMgr.device.keyboard:setCharCallback ( function ( c )
	table.insert ( textChars, cursorIndex + 1, c )
	editingText = ""
	updateLabel ()
	cursorIndex = cursorIndex + 1
end )

MOAIInputMgr.device.keyboard:setEditCallback ( function ( text, start, length )
	print(text, start, length)
	editingText = text
	updateLabel ()
	updateTextInputRect ()
end )

