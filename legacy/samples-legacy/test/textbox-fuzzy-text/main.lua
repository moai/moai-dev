MOAISim.openWindow ( "test", 512, 512 )
 
viewport = MOAIViewport.new ()
viewport:setSize( 512, 512 )
viewport:setScale( 256, 256 )
 
layer = MOAILayer.new()
layer:setViewport( viewport )
 
text = 'Example Text'
charcode = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 _'
 
font = MOAIFont.new()
font:load( 'Dwarves.TTF' )
font:preloadGlyphs( charcode, 70, 72 )
 
textbox = MOAITextBox.new()
textbox:setString( text )
textbox:setRect( -128, -128, 128, 128 )
textbox:setYFlip( true )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
textbox:setColor( 0, 0, 1, 1 )
textbox:setFont( font )
textbox:setLoc ( 0, 0, 0 )
layer:insertProp( textbox )
 
MOAIGfxDevice.setClearColor( 200/255, 200/255, 255/255, 1 )
 
MOAISim.pushRenderPass ( layer )