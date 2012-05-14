MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )
prop:moveRot ( 360, 1.5 )
--[[function pollInput ()
	
	while true do
		
		coroutine.yield ()
		
		if MOAIInputMgr.device.touch:isDown () then
						
			local x, y = layer:wndToWorld ( MOAIInputMgr.device.touch:getTouch ())	
			prop:setLoc ( x, y )
			print ( 'x:' .. x .. ' y:' .. y )
			
		end
	end
end

thread = MOAIThread.new ()
thread:run ( pollInput )
]]--
MOAIInputMgr.device.touch:setCallback (
	function (eventType, id, x, y, tapCount)
		if eventType == MOAITouchSensor.TOUCH_DOWN or 
		   eventType == MOAITouchSensor.TOUCH_MOVE then
			print ( 'x:' .. x .. ' y:' .. y )
			prop:setLoc ( layer:wndToWorld(x,y) )
		elseif eventType == MOAITouchSensor.TOUCH_CANCEL then
			print "Touch Cancel"
		end
	end
)