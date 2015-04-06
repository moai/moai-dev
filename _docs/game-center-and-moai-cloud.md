---
title: "Game Center and Moai Cloud"
---

Game Center and Moai Cloud
--------------------------

MoaiSDK has an implementation for iOS's Game Center service that you can use to store achievements and leaderboards for your game. However, this becomes limited when you take your game cross platform. That's where Moai Cloud comes in. Moai Cloud has services that allow you to do the same thing on any platform. Check out [Moai Achievements](moai-achievements.html) and [Moai Leaderboard](moai-leaderboard.html) for details on how to integrate these services into your game.

Using Both
----------

Game Center can be a selling point for many gamers who like to rack up achievement points and compare scores with friends. With Moai, there is no reason why you can't enable that feature for your iOS users as well as have a unified interface for players across all your platforms. Simply report scores and achievements to both Moai Cloud and Game Center. Then define a custom interface and populate it with the data from Moai Cloud. This allows you to have the same data shown across all your platforms with the added bonus of iOS users' Game Center accounts gaining points from your game.

##### Lua Code

```lua
-- Call this when your app starts to login to Game Center
MOAIGameCenterIOS.authenticatePlayer() 
```

-- Report score

```lua
function reportScore (scoreToReport)
   -- Post to Moai Cloud
   local data = "userid=123XYZ&username=JoePlayer&score="
                .. tostring( scoreToReport )
                .. "&sort=desc&unique=true"

   local task = MOAIHttpTask.new()
   task:setHeader( "x-clientKey", "myClientKeyHere" )
   task:httpPost( "http://services.moaicloud.com/moai/leaderboard/MyLeaderboard", data )
     
   -- Post to Game Center
   MOAIGameCenterIOS.reportScore( scoreToReport, "MyLeaderboard" )
end
```

More on Game Center
-------------------

Another thing that can be done with a side-by-side implementation is the players' Game Center alias can be grabbed and used as a username for Moai Cloud. This keeps the user from having to input a name again to report scores to Moai Cloud.

```lua
MOAIGameCenterIOS.getPlayerAlias()
```

It is also safe to make sure Game Center is supported on the user's system and that they are currently logged in. The following function handles both.

```lua
MOAIGameCenterIOS.isSupported()
```

Lastly, if you are using Game Center by itself, there are calls that have iOS display default modal views for the user's achievements and leaderboards.

```lua
MOAIGameCenterIOS.showDefaultAchievements()
MOAIGameCenterIOS.showDefaultLeaderboard()
```
