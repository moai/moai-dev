----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
	
textbox = MOAITextBox.new ()
layer:insertProp ( textbox )
	

text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
textbox:setString ( text )

print ( textbox:getStringBounds ( 20, 50 )) 

