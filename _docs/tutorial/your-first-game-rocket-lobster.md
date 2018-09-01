---
title: "Your First Game : Rocket Lobster"
---

This tutorial will take you through the beginnings of implementing a small game in Moai. The example game you'll build here is inspired by [Atari's Missile Command](http://en.wikipedia.org/wiki/Missile_Command), and should give you a basic grasp of some core gameplay features like randomly spawning enemies, player input, explosions, and a clear failure condition.

Enter: Rocket Lobster. The game consists of enemy missiles flying down from the top of the screen towards the player’s bases at the bottom. The player is tasked with shooting defensive missiles of their own up to stop the enemy missiles from reaching their lobster. Er, base.

This tutorial assumes the reader has at least a basic understanding of Lua and programming. It is suggested you read [Moai SDK Basics Part One](../basics/moai-sdk-basics-part-one.html) and [Moai SDK Basics Part Two](../basics/moai-sdk-basics-part-two.html) to become more familiar with a few of the Moai classes used in this tutorial.

In order to run the tutorial, first download the [RocketLobster tutorial files](http://getmoai.com/images/getmoai/rocket_lobster/files%20for%20rocketlobster.zip). This will contain the all the image files, the font file, and the completed code (as well as a blank .lua file).

Now that the introduction’s out of the way, it’s time to create your first game. The first thing to do is create the game window.

```lua
-- This will print “Hello Moai” on the debug screen.
print ( "hello, moai!" )
-- This opens up a window with the title of “Rocket Lobster” with
-- a width of 320 pixels and a height of 480 pixels.
MOAISim.openWindow ( "Rocket Lobster", 320, 480 )
--This will set the viewport and the layer for rendering.
viewport = MOAIViewport.new ()
viewport:setScale ( 320, 480 )
viewport:setSize ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
```

More in depth information on viewports and layers can be found in the [Moai SDK Basics Part One](../basics/moai-sdk-basics-part-one.html) article.

Go ahead and run the program so far and you should get:

![]({{ site.baseurl }}/img/docs/RLobster1.jpg)

Now that the game window is set up, you're ready to start creating and inserting objects. Below the set up, go ahead and add in the following code.

```lua
lobsterGfx = MOAIGfxQuad2D.new ()
lobsterGfx:setTexture ( "openlobster.png" )
lobsterGfx:setRect ( -128, -128, 128, 128 )

base = MOAIProp2D.new ()
base:setDeck ( lobsterGfx )
base:setLoc ( 0, -240 )

layer:insertProp ( base )
```

This will create and insert the lobster base onto the game window.

```lua
-- Creates a new GfxQuad with the name of lobsterGfx
lobsterGfx = MOAIGfxQuad2D.new ()
-- Assigns the texture "openlobster.png" to that lobsterGfx Quad
lobsterGfx:setTexture ( "openlobster.png" )
-- Sets the size of the quad
lobsterGfx:setRect ( -128, -128, 128, 128 )

-- Creates a new Prop with the name of base
base = MOAIProp2D.new ()
-- Assigns the lobsterGfx textured quad to the prop "base"
base:setDeck ( lobsterGfx )
-- Sets the location of the base
base:setLoc ( 0, -240 )

-- Inserts the prop "base" into the layer so it can be seen.
layer:insertProp ( base )
```

Now that it’s all in there you should see the following:

![]({{ site.baseurl }}/img/docs/RLobster2.jpg)

Now that the base is onscreen, it’s time to place a rocket.

```lua
rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

--------------------------------

function makeRocket ()
 local rocket = MOAIProp2D.new ()
 rocket:setDeck ( rocketGfx )
 layer:insertProp ( rocket )
end

makeRocket ()
```

This should look fairly similar to the base making, with one major change.

```lua
function makeRocket ()
 local rocket = MOAIProp2D.new ()
 rocket:setDeck ( rocketGfx )
 layer:insertProp ( rocket )
end

makeRocket ()
```

Since you will eventually want to make more than one rocket (unlike the base, where one is fine), it would make more sense to create a rocket making function. Later this function will be expanded somewhat, but for now, this will work.

After adding in the code and running the program, you should be presented with the following:

![]({{ site.baseurl }}/img/docs/RLobster3.jpg)

The rocket has appeared onscreen, now it’s time to make it move. With this step, things are going to get a bit more complicated; This section introduce threads as well as functions within functions.

The next section will take the previous code and add a new function into the makeRocket function, and also change the coordinates of the base to a constant. For organization’s sake, place your constants at the top of the code, right above the window initialization.

```lua
baseX = 0
baseY = -240
```

Now that the constants are in, change the base creation code to refer to them.

```lua
base = MOAIProp2D.new ()
base:setDeck ( lobsterGfx )
base:setLoc ( baseX, baseY ) -- New base coordinates

layer:insertProp ( base )
```

This code just changed the way that the base gets its location. For clarity later on, you can just use baseX and baseY. Now to make the rocket move!

```lua
function makeRocket ()
 local rocket = MOAIProp2D.new ()
 rocket:setDeck ( rocketGfx )
 layer:insertProp ( rocket )

 ----------------
 function rocket:launch ()
    
   self.thread = MOAIThread:new ()
        
   self.thread:run (
        
     function ()
       MOAIThread.blockOnAction ( self:seekLoc ( baseX, baseY, 3.0, MOAIEaseType.LINEAR ))
     end    
   )
 end

 return rocket
end

rocket = makeRocket ()
rocket:launch ()
```

The main thing added in this step is the rocket:launch function.

```lua
function rocket:launch ()
    
  self.thread = MOAIThread:new ()
        
  self.thread:run (
        
    function ()
      MOAIThread.blockOnAction ( self:seekLoc ( baseX, baseY, 3.0, MOAIEaseType.LINEAR ))
    end 
  )
end
```

The rocket:launch function contains a MOAICoroutine.blockOnAction and inside that the rocket's movement starts.

`self:seekLoc ( baseX, baseY, 3.0, MOAIEaseType.LINEAR ) )` will make the rocket (self) seek the location (seekLoc) of baseX (for x coords), baseY (for y coords) with a timer of 3.0 (3 seconds from start to finish), and an ease type of linear (meaning the speed goes at a constant speed.) These all get run in a thread as part of the rocket:launch function.

```lua
rocket = makeRocket ()  -- Make the rocket
rocket:launch ()
```

Now after all those are in, running the game will get you this:

![]({{ site.baseurl }}/img/docs/RLobster4.jpg)

Now that a rocket is flying towards the lobster/base with reckless abandon, it’s time to make it actually do something. Exploding sounds like a good idea. In order to do this, you will first need to create the explosion and then call it when the rocket hits its target. This step is two in one.

First step is to create the explosion. This will have to be inserted above the rocket creation, since the rocket will call the explosion.

```lua
explosionGfx = MOAIGfxQuad2D.new ()
explosionGfx:setTexture ( "fire.png" )
explosionGfx:setRect ( -32, -32, 32, 32 )

--------------------------------

function makeExplosion ()

 local explosion = MOAIProp2D.new ()
 explosion:setDeck ( explosionGfx )
 layer:insertProp ( explosion )

 ----------------

 function explosion:explode ()

   for i = 1, 128 do
     self:setScl ( i / 64 )
     coroutine.yield ()
   end

   layer:removeProp ( explosion )
 end

 return explosion
end
```

This should look very similar to the rocket. The explosionGfx is still created outside everything, and then inside a function calling that graphic onto the newly created prop, but now there is an explosion:explode function. Have a look at that for a moment.

```lua
function explosion:explode ()

  for i = 1, 128 do
    self:setScl ( i / 64 )
    coroutine.yield ()
  end

  layer:removeProp ( explosion )
end

return explosion
```

In this for loop, the goal is to set the size of the explosion and slowly make it grow and to cycle through that until it is finished, then it will remove itself from the game. (layer:removeProp ( explosion ) )

Now that the explosion has been created, it’s time to add the explosion to the rocket. This bit of code will be added inside the rocket:launch function after “layer:removeProp.” Here is the newly updated rocket.

```lua
function rocket:launch ()
    
 self.thread = MOAIThread:new ()
    
 self.thread:run (
            
   function ()
                
     MOAIThread.blockOnAction ( self:seekLoc ( baseX, baseY, 3.0, MOAIEaseType.LINEAR ))
                
     layer:removeProp ( self )
            
     local explosion = makeExplosion ()
     explosion:setLoc ( self:getLoc () )
     explosion:explode ()
                
     layer:removeProp ( base )
   end  
 )
end
```

Running that, you should see:

![]({{ site.baseurl }}/img/docs/RLobster5.jpg)

Now that the rocket explodes and blows up the base, the player is going to need a way to defend that base. Here comes the ally rocket. This step will be very involved, and you will have to code several things. Create the game loop, create an ally rocket and an enemy rocket type, assign a target for each rocket, create player input, change the direction the rocket points in, launch multiple enemy rockets, and change the size of the explosion depending on if it hits the base or not.

So get ready to jump in!

First things first, you're going to declare a few math utility functions. Nothing special here. They do, however, have to be declared above everything else, since they will be used in other functions, so just put them right below the setup constants.

```lua
--------------------------------

function angle ( x1, y1, x2, y2 )
 return math.atan2 ( y2 - y1, x2 - x1 ) * ( 180 / math.pi )
end

----------------------------

function distance ( x1, y1, x2, y2 )
 return math.sqrt ((( x2 - x1 ) ^ 2 ) + (( y2 - y1 ) ^ 2 ))
end
--------------------------------
```

Next we’ll change the size of the explosion.

OLD:

```lua
function explosion:explode ()

 for i = 1, 128 do
   self:setScl ( i / 64 )
   coroutine.yield ()
 end

 layer:removeProp ( explosion )
end
```

NEW:

```lua
function explosion:explode ( size )

 for i = 1, size do
   self:setScl ( i / 64 )
   coroutine.yield ()
 end

 layer:removeProp ( explosion )
end
```

You'll be plugging in a size variable into the explode function, and using that to determine the size of the explosion. Later on you will declare that size variable depending on which type of rocket you launch. Now it's time to add a considerable chunk to the rocket making function. First, however, you’ll need a few more constants; these will be used to determine rocket speed . Again, declare these with the other constants at the top of the code:

```lua
MIN_ENEMY_SPEED = 200
MAX_ENEMY_SPEED = 300

ALLY_SPEED = 300
```

Here’s the new one in its entirety.

```lua
function makeRocket ( isAlly, startX, startY, targetX, targetY, speed )

local travelDist = distance ( startX, startY, targetX, targetY )
local travelTime = travelDist / speed
local rocket = MOAIProp2D.new ()

rocket:setDeck ( rocketGfx )
layer:insertProp ( rocket )
rocket:setLoc ( startX, startY )
rocket:setRot ( angle ( startX, startY, targetX, targetY ) + 90 )
rocket.isAlly = isAlly               -- declare

----------------

function rocket:explode ( size )

local x, y = self:getLoc ()
local explosion = makeExplosion ( x, y, size )
layer:removeProp ( self )

self.thread:stop ()
end

----------------

function rocket:main ()

MOAICoroutine.blockOnAction ( self:seekLoc ( targetX, targetY, travelTime,   MOAIEaseType.LINEAR ))

if self.isAlly then
self:explode ( 64 )
else
self:explode ( 128 )
layer:removeProp ( base )
end
end

----------------

rocket.thread = MOAICoroutine.new ()
rocket.thread:run ( rocket.main, rocket )
end

------------------------

function launchEnemyRocket ( startX, startY )

makeRocket ( false, startX, startY, BASE_X, BASE_Y, math.random (  MIN_ENEMY_SPEED, MAX_ENEMY_SPEED ))
end

--------------------------------

function launchAllyRocket ( targetX, targetY )

     makeRocket ( true, BASE_X, BASE_Y, targetX, targetY, ALLY_SPEED )
end
```

The first thing to notice is that the rockets' speed are now conrolled using the variable travelTime instead of the number 3.0. Make sure you don’t miss adding it to MOAICoroutine.blockOnAction. Another major change to the makeRocket function is the “isAlly” variable. This will determine if the rocket is one of the friendly rockets, or an enemy rocket. You'll pass this variable through when you create the rocket and it will determine the spawn location.

First, go ahead and set up two different types of rockets.

```lua
function launchEnemyRocket ( startX, startY )

 makeRocket ( false, startX, startY, BASE_X, BASE_Y, math.random ( MIN_ENEMY_SPEED, MAX_ENEMY_SPEED ))
end
```

When the launchEnemyRocket function is called, a start location is run through it; it then plugs that into the makeRocket function, and also declares that isAlly = false. Finally, a rocket speed is determined with math.random and the constants declared earlier.

```lua
function launchAllyRocket ( targetX, targetY )

 makeRocket ( true, BASE_X, BASE_Y, targetX, targetY, ALLY_SPEED )
end
```

Similar to the launchEnemyRocket, you plug the variables in, except this time you declare isAlly = true. Now that we have our new rocket launching functions, it’s time to create a game loop so that we can run them more than once!

```lua
mainThread = MOAICoroutine.new ()
 mainThread:run (

 function ()

   local frames = 0
   while true do
     coroutine.yield ()
     frames = frames + 1
     if frames >= 90 then
       frames = 0
       launchEnemyRocket ( math.random ( -160, 160 ), 300 )
     end
     if MOAIInputMgr.device.mouseLeft:down () then
       launchAllyRocket ( layer:wndToWorld (MOAIInputMgr.device.pointer:getLoc () ))
     end
   end
 end
)
```

So here it is, the game loop. Pretty small, pretty simple, but powerful. Let’s go through the sections of it.

First the main thread is declared ( mainThread = MOAICoroutine.new () ) and then that thread is run with the launch function inside.

That function first declares frames = 0, and then yields. During that yield, 1 is added to frame every cycle. Finally, when the frame count hits 90, it gets reset to 0 and then launchEnemyRocket is run with a Y value of 300 and an X value of (math.random ( -160, 160 ), allowing it to spawn at any spot along that line, to add some randomness to the rockets.

As well as spawning in the enemy rockets, you can now take in player input. “MOAIInputMgr.device.mouseLeft:down” means that when the mouseLeft button has been pressed, then the game will launchAllyRocket with the coordinates taken from ( layer:wndToWorld (MOAIInputMgr.device.pointer:getLoc ()) meaning that wherever the mouse pointer is at the moment of the button press, that location will be translated into coordinates that the game will use as the target location.

And here’s what the game looks like at this point:

![]({{ site.baseurl }}/img/docs/RLobster7.jpg)

Now that we finally can launch our own rockets and they will explode, it’s time to make those explosions actually do something.

You're rounding the bases and almost finished with your first game! This step introduces hit detection on the explosions, and rearranges a few previous functions. First, you will need to create a table of enemy rockets that will be used to check if the explosion hit the rocket or not.

You can make that table right after you create the rocketGfx quad.

```lua
rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

enemyRockets = {}
```

Now it’s time to start plugging the enemy rockets into this table. This is done by editing the makeRocket function and placing the following code right after “rocket.isAlly = isAlly”

```lua
rocket:setLoc ( startX, startY )
rocket:setRot ( angle ( startX, startY, targetX, targetY ) + 90 )
rocket.isAlly = isAlly

<source lang="Lua">
if not isAlly then
 enemyRockets [ rocket ] = rocket
end
```

And now the enemy rockets have been placed into a table! Now it's time to create a hit detection function for the rocket explosion. This will need to be placed inside the explosion:makeExplosion function, but above the explosion:main function.

```lua
function explosion:checkHit ( prop )

 local x1, y1 = self:getLoc ()
 local x2, y2 = prop:getLoc ()
 return distance ( x1, y1, x2, y2 ) <= self.size
end
```

This one looks small, but there’s a lot going on. The function itself takes in a prop (one of the enemy rockets) and compares the location of the explosion against the location of the prop. It then returns distance and if the location is less than or equal to the size of the prop, it will explode. Now you'll need to change the rocket:explode function to handle what happens when the rocket is told to explode. That's done by adding enemyRockets [ self] = nil to the function.

```lua
function rocket:explode ( size )

 local x, y = self:getLoc ()
 local explosion = makeExplosion ( x, y, size )
 layer:removeProp ( self )

 enemyRockets [ self ] = nil
 self.thread:stop ()
end
```

After that, just edit the explosion:main function to run the new checkHit function.

```lua
function explosion:main ()

 for i = 1, self.size do
   self:setFrame ( -i, -i, i, i )
   for rocket in pairs ( enemyRockets ) do
     if self:checkHit ( rocket ) then
       rocket:explode ( 64 )
     end 
   end
   coroutine.yield ()
 end
 layer:removeProp ( self )
end
```

And there we go, we are all finished with hit detection. An ally rocket that explodes will look like:

![]({{ site.baseurl }}/img/docs/RLobster8.jpg)

This is the home stretch; the game loop is looping, rockets are flying in all directions, explosions can blow up other things, and the base goes away when it gets hit. The final missing piece is some sort of game over system. You’ll need to stop all rockets from launching after the base has been blown up, and also to create a game over message. Here are the final steps.

First, create a game over bool. You can just declare this right above the enemyRockets table.

```lua
gameOver = false
enemyRockets = {}
```

That’s easy enough. Now you’ll need the game to end once the base has been blown up and removed. That’s as easy as putting gameOver = true into the rocket:main thread, like so

```lua
-- hide the base
layer:removeProp ( base )
-- set the game over flag
gameOver = true
```

Now when the base gets blown up, gameOver will be set to true. But then what happens? Simple, you just stop running the main game loop.

OLD:

```lua
function ()

     local frames = 0
     while true do
          coroutine.yield ()
          frames = frames + 1
```

NEW:

```lua
function ()

     local frames = 0
          -- Once gameOver = true, the main thread stops running.
          while not gameOver do
               coroutine.yield ()
               frames = frames + 1
```

And now, for the final step: placing a game over message onto the screen after the game ends. Right after the while loop ends, you can create a text box. Here is what that looks like:

```lua
-- This creates a font and loads up the arialbd.tff file for that font.
local font = MOAIFont.new ()
font:loadFromTTF ( "arialbd.ttf", 
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?!", 
  12, 163 )

local textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -160, -80, 160, 80 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
textbox:setYFlip ( true )
textbox:setString ( "You are dead!" ) --- All of these set the parameters of the textbox.
layer:insertProp ( textbox )
textbox:spool () --- And finally, this places the textbox into the layer, spooling it along one letter at a time.
```

And that’s it! We finally have finished Rocket Lobster.

![]({{ site.baseurl }}/img/docs/RLobster9.jpg)

In case you were interested, here's what the final code should look like:

```lua
--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc.
-- All Rights Reserved.
-- http://getmoai.com
--==============================================================
--==============================================================
-- set up
--==============================================================

print ( "hello, moai!" )
SCREEN_UNITS_X = 320
SCREEN_UNITS_Y = 480
SCREEN_WIDTH = SCREEN_UNITS_X
SCREEN_HEIGHT = SCREEN_UNITS_Y

BASE_X = 0
BASE_Y = -240
MIN_ENEMY_SPEED = 200
MAX_ENEMY_SPEED = 300
ALLY_SPEED = 300

MOAISim.openWindow ( "Rocket Lobster", SCREEN_WIDTH, SCREEN_HEIGHT )
viewport = MOAIViewport.new ()
viewport:setScale ( SCREEN_UNITS_X, SCREEN_UNITS_Y )
viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

--==============================================================
-- utility functions
--==============================================================

--------------------------------
function angle ( x1, y1, x2, y2 )

 return math.atan2 ( y2 - y1, x2 - x1 ) * ( 180 / math.pi )
end
--------------------------------
function distance ( x1, y1, x2, y2 )

 return math.sqrt ((( x2 - x1 ) ^ 2 ) + (( y2 - y1 ) ^ 2 ))
end
--==============================================================
-- base
--==============================================================

lobsterGfx = MOAIGfxQuad2D.new ()
lobsterGfx:setTexture ( "openlobster.png" )
lobsterGfx:setRect ( -128, -128, 128, 128 )

base = MOAIProp2D.new ()
base:setDeck ( lobsterGfx )
base:setLoc ( BASE_X, BASE_Y )

layer:insertProp ( base )
--==============================================================
-- explosion rig
--==============================================================

explosionGfx = MOAIGfxQuad2D.new ()
explosionGfx:setTexture ( "fire.png" )
explosionGfx:setRect ( -32, -32, 32, 32 )

--------------------------------

function makeExplosion ( x, y, size )

 local explosion = MOAIProp2D.new ()
 explosion:setDeck ( explosionGfx )
 explosion:setLoc ( x, y )
 explosion.size = size
 layer:insertProp ( explosion )

----------------

 function explosion:checkHit ( prop )

   local x1, y1 = self:getLoc ()
   local x2, y2 = prop:getLoc ()
   return distance ( x1, y1, x2, y2 ) <= self.size
 end

----------------

 function explosion:main ()

   for i = 1, self.size do
     self:setFrame ( -i, -i, i, i )
     for rocket in pairs ( enemyRockets ) do
       if self:checkHit ( rocket ) then
         rocket:explode ( 64 )
       end
     end
     coroutine.yield ()
   end
   layer:removeProp ( self )
 end

 explosion.thread = MOAICoroutine.new ()
 explosion.thread:run ( explosion.main, explosion )
end

--==============================================================
-- rocket
--==============================================================

rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

gameOver = false
enemyRockets = {}

--------------------------------

function makeRocket ( isAlly, startX, startY, targetX, targetY, speed )

 local travelDist = distance ( startX, startY, targetX, targetY )
 local travelTime = travelDist / speed
 local rocket = MOAIProp2D.new ()

 rocket:setDeck ( rocketGfx )
 layer:insertProp ( rocket )
 rocket:setLoc ( startX, startY )
   rocket:setRot ( angle ( startX, startY, targetX, targetY ) + 90 )

 rocket.isAlly = isAlly
 if not isAlly then
   enemyRockets [ rocket ] = rocket
 end

 ----------------

 function rocket:explode ( size )

   local x, y = self:getLoc ()
   local explosion = makeExplosion ( x, y, size )
   layer:removeProp ( self )
   enemyRockets [ self ] = nil
   self.thread:stop ()
 end

 ----------------

 function rocket:main ()

   MOAICoroutine.blockOnAction ( self:seekLoc ( targetX, targetY, travelTime, MOAIEaseType.LINEAR ))
   if self.isAlly then
     self:explode ( 64 )
   else
     self:explode ( 128 )
     gameOver = true
     layer:removeProp ( base )
   end
 end
 rocket.thread = MOAICoroutine.new ()
 rocket.thread:run ( rocket.main, rocket )
end

--------------------------------

function launchEnemyRocket ( startX, startY )

 makeRocket ( false, startX, startY, BASE_X, BASE_Y, math.random ( MIN_ENEMY_SPEED, MAX_ENEMY_SPEED ))
end

--------------------------------

function launchAllyRocket ( targetX, targetY )

 makeRocket ( true, BASE_X, BASE_Y, targetX, targetY, ALLY_SPEED )
end

--==============================================================
-- game loop
--==============================================================

mainThread = MOAICoroutine.new ()
mainThread:run (

 function ()

   local frames = 0
   while not gameOver do
     coroutine.yield ()
     frames = frames + 1
     if frames <= 90 then
       frames = 0
       launchEnemyRocket ( math.random ( -160, 160 ), 300 )
     end
     if MOAIInputMgr.device.mouseLeft:down () then
       launchAllyRocket ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc () ))
     end
   end

   local font = MOAIFont.new ()
   font:loadFromTTF ( "arialbd.ttf", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?!", 12, 163 )

   local textbox = MOAITextBox.new ()
   textbox:setFont ( font )
   textbox:setTextSize ( font:getScale ())
   textbox:setRect ( -160, -80, 160, 80 )
   textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
   textbox:setYFlip ( true )
   textbox:setString ( "You are dead!" )
   layer:insertProp ( textbox )
   textbox:spool ()
 end
)
```
