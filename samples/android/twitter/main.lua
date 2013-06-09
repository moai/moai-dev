----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local consumerKey = "PASTE_CONSUMER_KEY_HERE"
local consumerSecret = "PASTE_CONSUMER_SECRET_HERE"
local callbackUrl = "PASTE_CALLBACK_URL_HERE"
----------------------------------------------------------------

MOAISim.openWindow ( "Test Twitter", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

----------------------------------------------------------------

-- Initialize with OAuth consumerKey and consumerSecret acquired from developer.twitter.com when registering the app
MOAITwitterAndroid.init(consumerKey, consumerSecret, callbackUrl)

require "AccessTokenIo"

local accessToken, err = readAccessTokenFromDisk()
if not accessToken then
    print("No access token available, user will have to log in.", err)
else
    -- If the access token was persisted, give it to MOAITwitterAndroid to authenticate the user without their
    -- interaction
    MOAITwitterAndroid.setAccessToken(accessToken.token, accessToken.tokenSecret)
end

-------------------------------
-- Setup login event listeners
-------------------------------
-- Successful login
MOAITwitterAndroid.setListener(MOAITwitterAndroid.SESSION_DID_LOGIN, function(token, tokenSecret)
   writeAccessTokenToDisk(token, tokenSecret) 
   MOAITwitterAndroid.sendTweet("Hello world from MOAI on Android!")
end)

-- Failed login.  The exception is logged at the debug level in MOAITwitter.java
MOAITwitterAndroid.setListener(MOAITwitterAndroid.SESSION_DID_NOT_LOGIN, function()
    print("Login Failed")
end)

-------------------------------
-- Setup tweet event listeners
-------------------------------
-- Successful tweet
MOAITwitterAndroid.setListener(MOAITwitterAndroid.TWEET_SUCCESSFUL, function()
    print ("Tweet sent!")
end)

-- Failed tweet.  The exception is logged at the debug level in MOAITwitter.java
MOAITwitterAndroid.setListener(MOAITwitterAndroid.TWEET_CANCELLED, function()
    print("Tweet Failed")
end)


-- Check if we are logged in by the access token, if not ask the user to log in.
if not MOAITwitterAndroid.isLoggedIn() then
    print("Begin Login")
    MOAITwitterAndroid.login()
else
    MOAITwitterAndroid.sendTweet("Hello world from MOAI on Android!")
end

