print ( "hello, Android!" )

MOAIHeyzapAndroid.start ()
-- MOAIHeyzapAndroid.loadInterstitial ()

----------------------------------------------------------------

width = 640
height = 960
MOAISim.openWindow ( "test", width, height )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 18, 163 )

textbox = MOAITextBox.new ()
textbox:setRect ( -50, -50, 500, 200 )
textbox:setYFlip ( true )
textbox:setString ( "Tap screen to fetch and display an ad." )
textbox:setTextSize ( 18, 163 )
textbox:setFont ( font )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( textbox )

function clickCallback ( down )
    if (down) then
        if MOAIHeyzapAndroid.isInterstitialAvailable () then
            print ( "Showing Heyzap interstitial..." )
            MOAIHeyzapAndroid.showInterstitial ()
            textbox:setString( "Tap screen to fetch an ad..." )
        else
            print ( "There is no interstitial. Loading interstitial..." )
            MOAIHeyzapAndroid.loadInterstitial ()
        end
    end
end

MOAIInputMgr.device.touch:setCallback (
    function ( eventType, idx, x, y, tapCount )
        if eventType == MOAITouchSensor.TOUCH_DOWN then
            clickCallback ( false )
        elseif eventType == MOAITouchSensor.TOUCH_UP then
            clickCallback ( true )
        end
    end
)

function onAdAvailable()
    print ( "Successfully loaded an ad! Tap to display! " )
end

function onFetchFail()
    print ( "Failed to load an ad! :( " )
end

function onAdClose()
    print ( "Failed to load an ad! :( " )
end

--------------------------------------------------------

MOAIHeyzapAndroid.setListener ( MOAIHeyzapAndroid.INTERSTITIAL_AVAILABLE, onAdAvailable )
MOAIHeyzapAndroid.setListener ( MOAIHeyzapAndroid.INTERSTITIAL_FETCH_FAILED, onFetchFail )
MOAIHeyzapAndroid.setListener ( MOAIHeyzapAndroid.INTERSTITIAL_CLICKED, onAdClose )
MOAIHeyzapAndroid.setListener ( MOAIHeyzapAndroid.INTERSTITIAL_HIDE, onAdClose )
MOAIHeyzapAndroid.setListener ( MOAIHeyzapAndroid.INTERSTITIAL_DISMISSED, onAdClose )
