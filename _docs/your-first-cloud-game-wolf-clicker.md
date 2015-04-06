---
title: "Your First Cloud Game : Wolf Clicker"
---

This article will explain the basics of creating a simple app utilizing Moai Cloud as well as going over some techniques for Scene Management, Input, and Saving on various devices in a scalable way.

This tutorial assumes the reader has at intermediate understanding of Lua and programming.

This tutorial assumes you have read Moai SDK Basics Part One, Moai SDK Basics Part Two, Moai Cloud Basics Part One and Moai Cloud Basics Part Two.

------------------------------------------------------------------------

To start check out the WolfClicker sample in the SDK. To run the sample you need to pass "platforms\\platform-\*.lua" "main.lua". Where \* is either windows or osx. ( Note: adding other platforms is left as an exercise to the reader )

The basic idea is you can click the Wolf, but only every minute! The number of clicks is stored on the cloud, as well as the time until the user can click again. Clicking "New Game" will create a new user and on subsequent launches you will be allowed to continue.

main.lua
--------

```lua
 require "elements"
 require "modules/cloud-manager"  
 require "modules/state-manager"  
 require "modules/input-manager"  
 require "modules/savefile-manager"
 
 MOAISim.openWindow ( "Wolf Clicker", SCREEN_WIDTH, SCREEN_HEIGHT )
 
 viewport = MOAIViewport.new()
 viewport:setSize( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT )
 viewport:setScale( SCREEN_UNITS_X, SCREEN_UNITS_Y )
 
 -- seed random numbers
 math.randomseed( os.time () )
 
 savefiles.get "user" 
 globalData = {}
 
 JUMP_TO = nil
 ----------------------------------------------------------------
 if JUMP_TO then
  statemgr.push( JUMP_TO )
 ----------------------------------------------------------------
 else
  statemgr.push "states/state-splash.lua"
 end
 ----------------------------------------------------------------
 
 -- Start the game!
 statemgr.begin()
```

The start adds in several modules that will be used ( More on each one later ) as well as defining the window. Besides the module calls ( savefiles and statemgr ) there isn't too much going on here. The key here is how "platform-\*.lua" works here. By executing this file first, you can set device specific properties, such as the SCREEN\_WIDTH and SCREEN\_HEIGHT that are used later.

globalData in this file allows us to keep data between states.

state-manager.lua
-----------------

Next up, lets take a look at "state-manager.lua". I won't go over everything here, but the basic idea is to separate the game into various 'states' (often called 'scenes' as well). Each of these states are lua tables that have certain functions attached to them, which are called by the state manager at the appropriate time. These function are:

- onFocus, called whenever the state is moved to the top of the stack

- onLoseFocus, called whenever the state is moved from the top of the stack

- onInput, called when the state is at the top of the stack and input occurs

- onLoad, called when the state is added to the stack

- onUnload, called when the state is removed from the stack

- onUpdate, called every frame while the state is on the top of the stack

The basic use of the state manager is to call "statemgr.push()" ( As seen in main.lua ) to push a state file onto the stack, then "statemgr.begin()" to start running the state manager. From there, states can be managed with swap, push and pop. In Wolf Clicker these states are:

- Splash Screen

- Main Menu

- Game

This is where most of the logic for the game is. The splash screen is very simple and gives you an idea of how a state can be made. From here we'll head to "state-main-menu.lua"

------------------------------------------------------------------------

We'll start in the OnLoad function since this is basically the first part of the state that is called. Here we make the simple title textbox and create the only button on the screen. You'll see a call to elements:

```lua
 playButton = elements.makeTextButton( font, "resources/button.png", 206, 150, 60 )
  
 playButton:setCallback( function (self) 
  
  local thread = MOAIThread.new()
  thread:run( mainMenu.StartGameCloud, mainMenu.newGame )
  
end )
```

This is is a module ( "elements.lua" ) that is used to store the convenience functions.

"elements.lua" consists of two functions, a function to make a button and a function to make a button with text inside of it. This allows us to avoid rewriting common code for objects that appear multiple times.

savefile-manager.lua
--------------------

Next up is the "savefiles" module "savefile-manager.lua". This just contains some wrappers to make cross-platform save files easier to deal with. In this next section of code, it checks whether the user has a game already started

```lua
 if savefiles.get "user" .fileexist then 
  playButton:setString "Continue"
  playButton.newGame = false
 else
  playButton:setString "New Game"
  playButton.newGame = true
 end
```

The main operation of "savefiles" is the "get" method which will allow us to get information about a file. "fileexist" tells us if the file was opened from disk. Other important properties are "data" which contains the data saved and "saveGame" which saves the data.

The "onInput" function is where input will take place. This one is fairly simple:

```lua
 mainMenu.onInput = function (self)
  if inputmgr:up() then
    
    local x, y = mainLayer:wndToWorld( inputmgr:getTouch () )
    
    playButton:updateClick( false, x, y )
    
  elseif inputmgr:down() then
    
    local x, y = mainLayer:wndToWorld( inputmgr:getTouch () )
    
    playButton:updateClick( true, x, y )
  end
 
 end
```

You'll notice it simply passes the input to the button, which handles the input, and calls the callback.

state-game.lua
--------------

We'll head over to "state-game.lua" now. You'll notice its very similar to "state-main-menu.lua", the only major difference being the update function, which sets the time to the next click and the number of clicks. This is the 'game' logic.

The next thing is the cloud interaction. From a higher level, the cloud is the only one that can determine if it is time to click. If we store this data on the client-side people could cheat by modifying it.

However, we need to know this information on the client-side and it would be wasteful to ask the cloud every second how much time is left. So, we keep track of this information on the client side, but never save the actual number of clicks.

If we go back to "state-main-menu.lua" there is a function called "StartGameCloud" at the top. This function uses the cloud module "cloud-manager.lua". This file is mostly helper functions to assist with easily creating and debugging more complex cloud applications. There are a couple functions to help encode data into URLs at the top. Below are the key functions "CreateGetTask" and "CreatePostTask". You can see their usage back in "state-main-menu.lua":

```lua
 --create a new user on the cloud if its a new game
 if isNewGame then
  task = cloud.createPostTask( "users", {name = "Adam"} )
  result,code = task:waitFinish()
    
  if result then
    saveFile.data = result
    saveFile:saveGame()
  else
    print( "failed to create user, code " .. code )
  end   
 end
```

The important thing to note is that this is create in a MOAIThread:

```lua
 local thread = MOAIThread.new()
 thread:run( mainMenu.StartGameCloud, mainMenu.newGame )
```

Why did I choose to do it this way? When using some APIs somtimes calls need to be chained together, one after another. Once you start having 5+ calls chained together, it can be difficult to visualize and debug them. If you look at "StartGameCloud" its easy to follow the logic flow.

Going over to "state-game.lua" you'll see a similar function at the top called "ClickCloud". This handles the callbacks for the game, similar to the way the menu works.

Server side logic
-----------------

Now we'll head over to the server side logic. It can be found in the \_cloud directory "main.lua"

(UNDER CONSTRUCTION)
