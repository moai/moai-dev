---
title: "Using ParticleHelper"
---

This page is about the community contributed particle script helper.

What's it used for?
-------------------

Moai features a particle system which is good enough for you to make any type of eye candies.

Particle system is always a performance critical part of game engine. Moai handles the hard jobs (updating and rendering) in the C host, and provide the users with a assembly-like generic script composition interface.

Honestly, the particle script composition interface is not easy to use, see the code below (MOAI\_PATH/samples/basic/particles-distanceEmitter/main.lua)

```lua
local r1 = MOAIParticleScript.packReg( 1 )
--------------------------------------------------------------
local init = MOAIParticleScript.new()

init:vecAngle( r1, MOAIParticleScript.PARTICLE_DX, MOAIParticleScript.PARTICLE_DY )
init:sub( r1, CONST ( 180.0 ), r1 )

local render = MOAIParticleScript.new ()

render:add( MOAIParticleScript.PARTICLE_X, 
            MOAIParticleScript.PARTICLE_X, 
            MOAIParticleScript.PARTICLE_DX )
render:add( MOAIParticleScript.PARTICLE_Y, 
            MOAIParticleScript.PARTICLE_Y, 
            MOAIParticleScript.PARTICLE_DY )

render:sprite()
render:set( MOAIParticleScript.SPRITE_ROT, r1 )
render:ease( MOAIParticleScript.SPRITE_X_SCL, 
             CONST( 0.5 ), CONST( 3 ), MOAIEaseType.SHARP_EASE_IN )
render:ease( MOAIParticleScript.SPRITE_OPACITY, 
             CONST( 1 ), CONST( 0 ), MOAIEaseType.EASE_OUT )
```

You have to split your script into many single instructions, it makes the script unreadable. Let's see what particle helper can help here(YOU\_MOAI\_PATH/samples/contrib/particle-helper/main.lua):

```lua
local reg = {} --shared register table

local init=makeParticleScript( function ()
 r1 = 180-vecAngle( p.dx, p.dy )   --r1 is a register
end, reg )

local render = makeParticleScript( function ()
 p.x = p.x+p.dx
 p.y = p.y+p.dy
 
 sprite()
 sp.rot = r1
 sp.sx = ease( 0.1, 3, EaseType.SHARP_EASE_IN )
 sp.opacity = ease( 1, 0, EaseType.EASE_OUT )
end, reg )
```

These two codes are doing exactly the same thing. Apparently, the latter one is much easier to understand and write.

How to use it?
--------------

With the helper, you can write particle script just like normal Lua code. However, there are limits (will mention later).

Particle helper has only one function:

```lua
function makeParticleScript (script_func, [register_table], ...)
```

-   **script\_func:** your particle script, it's a Lua function
-   **register\_table:** when you need to share register between init and render scripts, you can use the parameter to give them a shared register table.
-   **vararg part:** the rest of the arguments will be sent into script\_func, you can use them as normal arguments inside your script function.
-   the function returns a MOAIParticleScript, which you can use in MOAIParticleState.setInitScript() and MOAIParticleState.setRenderScript()

Inside the script\_func, you can use following variables (see detailed usage in SDK document: http://getmoai.com/docs/class\_m\_o\_a\_i\_particle\_script.html class MOAIParticleScript)

-   p.x = PARTICLE\_X
-   p.y = PARTICLE\_Y
-   p.dx = PARTICLE\_DX
-   p.dy = PARTICLE\_DY

<!-- -->

-   sp.x = SPRITE\_X\_LOC
-   sp.y = SPRITE\_Y\_LOC
-   sp.sx = SPRITE\_X\_SCL
-   sp.sy = SPRITE\_Y\_SCL
-   sp.rot = SPRITE\_ROT
-   sp.opacity = SPRITE\_OPACITY
-   sp.r = SPRITE\_RED
-   sp.g = SPRITE\_GREEN
-   sp.b = SPRITE\_BLUE
-   sp.glow = SPRITE\_GLOW
-   sp.idx = SPRITE\_IDX

<!-- -->

-   time = script:time()
-   EaseType = MOAIEaseType {EASE\_IN, EASE\_OUT, FLAT, LINEAR, SHARP\_EASE\_IN, SHARP\_EASE\_OUT, SMOOTH, SOFT\_EASE\_IN, SOFT\_EASE\_OUT, SOFT\_SMOOTH}

And following functions (see detailed usage in SDK document: http://getmoai.com/docs/class\_m\_o\_a\_i\_particle\_script.html class MOAIParticleScript):

-   rand (min, max)
-   randVec (reg1, reg2, min, max)
-   vecAngle(dx, dy)
-   cycle(v0, v1, v2)
-   wrap(v0, v1, v2)
-   ease(v1, v2, easeType=EaseType.LINEAR)
-   easeDelta (v1, v2, easeType=EaseType.LINEAR)
-   sprite ()

Limits
------

The particle helper is just a little wrapper done with Lua metamethod and function environment table, so it has its limits:

-   You cannot invoke any function other than the provided ones on the registers/particle variables, because they are just some ID holders.
-   Make sure there's no local variable named same as the script builtin symbols, e.g.

```lua
local p = 100
...
makeParticleScript( function()
  p.x = p.x+p.dx  --'p' here becomes upvalue, not builtin symbol
end )
```

-   There must be no more than 32 registers, it's limited by host.

Tips
----

-   Since particle script are actually composed and invoked in C host, there's no performance penalty for using the helper.
-   Particle helper can automatically generate temporary registers, so you can write multiple-level expressions like: sp.x=p.x+rand(0,time/10)
-   You can parameterize your particle script like this:

```lua
function template_spin ( rot_velocity )
  sprite()
  sp.rot = sp.rot+rot_velocity
end

-- 10 will get passed into template_spin as argument.
script_spin_10=makeParticleScript( template_spin, reg, 10 )
script_spin_50=makeParticleScript( template_spin, reg, 50 )
state_slow:setRenderScript( script_spin_10 )
state_fast:setRenderScript( script_spin_50 )
```

-   You can call Lua functions in your script function to affect the script creation. e.g.

```lua
local math = math --global table is not exposed to script, so we have to upvalue it
function template_circle (count, radius)
  count = count or 3
  radius = radius or 10  --default values
  local span = 360/count
  for i = 1 , count do
    local rad = i*span/180*math.pi
    sprite()   --create a sprite
    sp.x = p.x+math.cos( rad )*radius
    sp.y = p.y+math.sin( rad )*radius
  end
end

script_circle=makeParticleScript( template_circle, regs, 12, 100 )
```
