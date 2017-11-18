MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

local action = prop:moveRot ( 0, 0, 360, 3 )
prop:moveLoc ( 0, 0, 128, 3 )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

img = MOAIImage.new()

function afterGrab ()
	img:writePNG ( "test.png" )
end

function onStop ()
	MOAIRenderMgr.grabNextFrame ( img, afterGrab )
	
end

action:setListener ( MOAIAction.EVENT_STOP, onStop )