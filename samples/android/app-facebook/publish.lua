----------------------------------------------------------------
-- Copyright (c) 2013 - present Herding Cats and Ricard Sole
-- All Rights Reserved.
----------------------------------------------------------------

local FB = {}

function FB.init ()
    MOAIFacebookAndroid.setListener ( MOAIFacebookAndroid.DID_LOGIN, FB.onFBDidLogin )
    MOAIFacebookAndroid.login ()
end

function FB.onFBDidLogin ()
    MOAIDialog.showDialog ( "Display", "Logged in", "Cool", "Awesome", "Amazing", true, FB.publish )
    -- When print is fixed, we should change showDialog for print
    -- print ( "logged in" )
end

function FB.publish ( code )
    link = "http://github.com/moai/moai-dev"
    picture = "https://github.com/moai/moai-dev/blob/master/docs/attribution%20images/moaiattribution_vert_black.png?raw=true"
    name = "Get MOAI"
    caption = "Write your games in Lua and run them on..."
    description = "Write your games in Lua and run them on Android, iOS, Linux, Mac OSX and Windows!"
    MOAIFacebookAndroid.publish ( link, picture, name, caption, description )
    nextFun2 ()
end

----------------------------------------------------------------

function nextFun2 ()
    MOAISim.openWindow ( "test", 320, 480 )

    viewport = MOAIViewport.new ()
    viewport:setSize ( 320, 480 )
    viewport:setScale ( 320, 480 )

    layer = MOAILayer2D.new ()
    layer:setViewport ( viewport )
    MOAISim.pushRenderPass ( layer )

    gfxQuad = MOAIGfxQuad2D.new ()
    gfxQuad:setTexture ( "moai.png" )
    gfxQuad:setRect ( -64, -64, 64, 64 )

    prop = MOAIProp2D.new ()
    prop:setDeck ( gfxQuad )
    layer:insertProp ( prop )

    prop:moveRot ( 540, 2.0 )
end

FB.init ()
