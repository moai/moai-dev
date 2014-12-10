----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com----------------------------------------------------------------
 
MOAISim.openWindow ( "test", 960, 480 )
viewport = MOAIViewport.new ()
viewport:setSize ( 960, 480 )
viewport:setScale ( 960,480)
screen_width=960

--MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_CELLS, 2, 1, 1, 1 )
--MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

Dialog = {}

function Dialog:new ( o )
 
    o = o or {
                text = "Level Complete?",
				button1text = "Button 1",
                button1img = "moai.png",
                button2text = "Button 2",
                button2img = "moai.png",
                button3text = "Button 3",
                button3img = "moai.png",
                button4text = "Button 4",
                button4img = "moai.png",
                startx = 0 - 2.5 * ( 128 + 5 ),
                starty = -100,
                width = 133
    }
 
    setmetatable ( o, self )
    self.__index = self
    return o
end
 
function Dialog:show ()
	alert_layer = MOAILayer2D.new ()
	MOAISim.pushRenderPass ( alert_layer )
	alert_layer:setViewport ( viewport )
	 
	alert_partition = MOAIPartition.new ()
	alert_layer:setPartition ( alert_partition )
	 
	prop = MOAIProp2D.new ()
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "background.png" )
	gfxQuad:setRect ( -300, -200, 300, 200 )
	prop:setDeck ( gfxQuad )
	alert_layer:insertProp ( prop )
	
	self:addButton ( self.startx + self.width * 1, self.starty, 1, 1, self.button1text, self.button1img )
	self:addButton ( self.startx + self.width * 2, self.starty, 1, 1, self.button2text, self.button2img )
	self:addButton ( self.startx + self.width * 3, self.starty, 1, 1, self.button3text, self.button3img )
	self:addButton ( self.startx + self.width * 4, self.starty, 1, 1, self.button4text, self.button4img )
	self:addText ( self.text )
	 
	if MOAIInputMgr.device.pointer then
       
        -- mouse input
        MOAIInputMgr.device.pointer:setCallback (
			function ( x, y )
			self:pointerCallback ( x, y )
			end
			)
 
        MOAIInputMgr.device.mouseLeft:setCallback (
			function ( eventType )
			self:clickCallback ( eventType )
			end
			)
	else
 
        -- touch input
        MOAIInputMgr.device.touch:setCallback (
       
			function ( eventType, idx, x, y, tapCount )

				self:pointerCallback ( x, y )
	   
				if eventType == MOAITouchSensor.TOUCH_DOWN then
					self:clickCallback ( true )
				elseif eventType == MOAITouchSensor.TOUCH_UP then
					self:clickCallback ( false )
				end
			end
        )
	end
 
end
 
function Dialog:addText ( text )
	charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
	 
	font = MOAIFont.new ()
	font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )
	 
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12 )
	textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	print ( "fs = 12" )
	textbox:setRect ( -192, 64, 192, 196 )
	textbox:setYFlip ( true )
	alert_layer:insertProp ( textbox )
end

function Dialog:addButton ( x, y, xScl, yScl, name, texture )
	local prop = MOAIProp2D.new ()
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( texture )
	gfxQuad:setRect ( -64, -64, 64, 64 )
	prop:setDeck ( gfxQuad )
	prop:setPriority ( 5 )
	prop:setLoc ( x, y )
	prop:setScl ( xScl, yScl )
	prop.name = name
	alert_partition:insertProp ( prop )
end
 
function Dialog:pointerCallback ( x, y )
	local oldX = mouseX
	local oldY = mouseY    
	mouseX, mouseY = alert_layer:wndToWorld ( x, y )
end
               
function Dialog:clickCallback( down )
       
	if down then

		mouseX, mouseY = MOAIInputMgr.device.pointer:getLoc () -- TODO: fix to also work with touch sensor
		mouseX, mouseY = alert_layer:wndToWorld ( mouseX, mouseY )
		pick = alert_partition:propForPoint ( mouseX, mouseY )
	   
		if pick then
			print ( pick.name )
			if pick.name == "Button 1" then
				alert_layer:seekLoc ( 960 / 2, 480 / 2, 0, 2)
				action = alert_layer:seekScl (0, 0, 0, 2 )
			end
			if pick.name == "Button 2" then
				alert_layer:seekLoc ( -500, 0, 0, 5 )
			end
		end
	else
		if pick then
			pick = nil
		end
	end
end

main_layer = MOAILayer2D.new ()
MOAISim.pushRenderPass ( main_layer )
main_layer:setViewport ( viewport )
prop = MOAIProp2D.new ()
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "background.png" )
gfxQuad:setRect ( -512, -512, 512, 512 )
prop:setDeck ( gfxQuad )
main_layer:insertProp ( prop )

myDialog=Dialog:new ()
myDialog:show ()
