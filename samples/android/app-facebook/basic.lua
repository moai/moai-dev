----------------------------------------------------------------
-- Copyright (c) 2013 - present Herding Cats and Ricard Sole
-- All Rights Reserved.
----------------------------------------------------------------

local FB = {}

FB.APP_ID = "YOUR VERY OWN APP ID"

function FB.init ()
    MOAIFacebookAndroid.setListener ( MOAIFacebookAndroid.DID_LOGIN, FB.onFBDidLogin )
    MOAIFacebookAndroid.setListener ( MOAIFacebookAndroid.DID_LOGOUT, FB.onFBDidLogout )
    MOAIFacebookAndroid.init ( FB.APP_ID )
    MOAIFacebookAndroid.login ()
end

function FB.onFBDidLogin ()
    MOAIDialog.showDialog ( "Display", "Logged in", "Cool", "Awesome", "Amazing", true, nextFun )
end

function FB.onFBDidLogout ()
    MOAIDialog.showDialog ( "Display", "Logged out", "Cool", "Awesome", "Amazing", true, nextFun2 )
end

function nextFun ( code )
    MOAIFacebookAndroid.logout ()
end

----------------------------------------------------------------

function nextFun2 ( code )
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
