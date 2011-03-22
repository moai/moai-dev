print ("CAN I HAZ LUABURGER?")

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

layer = MOAILayer2D.new ()
layer:showDebugLines ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )
viewport:setOffset ( -1, 1 )
layer:setViewport ( viewport )

charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!<>:\\"

font = MOAIFont.new ()
font:loadFromTTF ( "arial-rounded.ttf", charcodes, 12, 163 )

test = "The quick <c:f70>brown<c> fox jumps over the <c:7f3>lazy<c> dog. 0123456789"

textbox = MOAITextBox.new ()
textbox:setString ( test )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( 0, 0, 320, 480 )
layer:insertPrim ( textbox )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1, 16 )

textbox:reserveCurves ( 1 )
textbox:setCurve ( 1, curve )

textbox:spool ()

MOAISim.openWindow ( "textbox", 320, 480 )
