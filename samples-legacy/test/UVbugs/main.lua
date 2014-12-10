----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

------------------------READ ME!!!!!----------------------------
----------------------------------------------------------------
-- Although I only have a text box in this, this bug
-- also arises in particles.  The cause seems to be the
-- same: if the priority is higher than the prop with
-- the UV transform assigned, the texture will go haywire
-- when the UV tranform is in action, and will only look
-- correct when the UV transform is at the coordinate (0,0).
----------------------------------------------------------------


MOAISim.openWindow ( "test", 640, 640 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 640 )
viewport:setScale ( 640, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

tex = MOAITexture.new ()
tex:load ( "test.png" )
tex:setWrap ( true )

quad = MOAIGfxQuad2D.new ()
quad:setTexture ( tex )
quad:setRect ( -256, -256, 256, 256 )
quad:setUVRect ( 0, 0, 4, 4 )

transform = MOAITransform.new ()

prop = MOAIProp2D.new ()
prop:setDeck ( quad )
prop:setPriority ( 4 )
layer:insertProp ( prop )

prop:setUVTransform ( transform )

local function makeImgFont ( filename )

	local image = MOAITexture.new ()
	image:setFilter ( MOAITexture.GL_LINEAR_MIPMAP_LINEAR )
	image:load ( filename .. ".png", 0 )

	local fontFunc = assert ( loadfile ( filename .. ".lua" ))
	local font = fontFunc()
	
	font:setTexture ( image )
	
	return font
end

font = makeImgFont ( "screebie16" )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale () )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
textbox:setYFlip ( true )
textbox:setRect ( -256, -50, 256, 50 )
textbox:setLoc ( 0, -300 )
textbox:setString ( "priority is 1" )
textbox:setPriority ( 1 )
layer:insertProp ( textbox )

function main ()
	
	local i = 1
	repeat
		coroutine.yield ()
		MOAIThread.blockOnAction ( transform:seekLoc ( 2, 0, 2, MOAIEaseType.SOFT_EASE_OUT ) )
		MOAIThread.blockOnAction ( transform:seekLoc ( 0, 0, 2, MOAIEaseType.SOFT_EASE_IN ) )
		i = i + 1
		textbox:setPriority ( i )
		textbox:setString ( "priority is " .. i )
	until false
end

thread = MOAIThread.new ()
thread:run ( main )