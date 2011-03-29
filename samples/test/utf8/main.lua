print ("CAN I HAZ LUABURGER?")

--MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

layer = MOAILayer2D.new ()
--layer:showDebugLines ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )
viewport:setOffset ( -1, 1 )
layer:setViewport ( viewport )

charcodes = 'φωψ';

font = MOAIFont.new ()
font:loadFromTTF ( "times.ttf", charcodes, 12, 163 )

textbox = MOAITextBox.new ()
textbox:setString ( charcodes )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( 0, 0, 320, 480 )
layer:insertProp ( textbox )

shader = MOAISimpleShader.new ()
shader:setColor ( 0, 0, 0, 1 )
shader:seekColor ( 1, 0, 0, 1, 1.5 )
textbox:setShader ( shader )

textbox:spool ()

MOAISim.openWindow ( "textbox", 320, 480 )
