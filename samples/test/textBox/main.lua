print ("CAN I HAZ LUABURGER?")

--MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

scene = MOAILayer2D.new ()
scene:showDebugLines ()
MOAISim.pushRenderPass ( scene )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
scene:setViewport ( viewport )

camera = MOAICamera2D.new ()
scene:setCamera ( camera )

partition = MOAIPartition.new ()
scene:setPartition ( partition );

charcodes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-"
test = "THIS IS A \n TEST WITH A LOT OF TEXT SO I CAN TRY AND FIGURE OUT WHY THIS THING IS LOOKING WEIRD EVERY TIME I GO BOOP BOOP BLAH BLAHBLAHBLAH BALH BALHA BALHLAB BALHAL bAlh \n BLHA bLHABLHABLAH LABLhABAL ABLh"

verdana = MOAIFont.new ()
verdana:load ( "FontVerdana18.png", charcodes )

textbox = MOAITextBox.new ()
textbox:setString ( test )
textbox:setFont ( verdana )
textbox:setTextSize ( verdana:getScale ())
textbox:setRect ( -150, 134, 150, 230 )
textbox:setYFlip ( true )
partition:insertPrim ( textbox )

textbox:spool ()
textbox:setRect ( -150, -230, 150, 230 )

--textbox:moveRot ( -90, 3 )
--textbox:moveLoc ( -150, 0, 3 )

MOAISim.openWindow ( "textbox", 320, 480 )
