---
title: "Code snips"
---

### Memory Check

Memory checkers are useful for checking for memory leaks at the such. This is a useful function.

```lua
local collect = collectgarbage
local lastCheck = {sysMem = 0}
function checkMem (say)
   collect()
   local sysMem = collect("count") * .001
   if say == true or lastCheck.sysMem ~= sysMem then
      lastCheck.sysMem = sysMem
      print( "Mem: " .. math.floor(sysMem*1000)*.001 .. "MB \t" )
   end
end

checkMem( true ) --will print even if memory is the same as the previous check
checkMem( false ) --will only print if memory is different to the previous value
```

### Rounding

[Lua-users.org SimpleRound](http://lua-users.org/wiki/SimpleRound)

```lua
function round (num, idp)
    local mult = 10^(idp or 0)
    return math.floor( num * mult + 0. 5)/mult
end

round( 2.256, 1 ) --returns 2.3
```

### performWithDelay

```lua
function performWithDelay (delay, func, repeats, ...)
 local t = MOAITimer.new()
 t:setSpan( delay/100 )
 t:setListener( MOAITimer.EVENT_TIMER_LOOP,
   function ()
     t:stop()
     t = nil
     func( unpack( arg ) )
        if repeats then
            if repeats > 1 then
                performWithDelay( delay, func, repeats - 1, unpack( arg ) )
            elseif repeats == 0 then
               performWithDelay( delay, func, 0, unpack( arg ) )
            end
        end
   end
 )
 t:start()
 end
```

### trace

Similar to ActionScript's trace function. It prints out a message prepended with the location of the function call.

```lua
local locationFormat = "%s(%s):"
function trace (...)
    local info = debug.getinfo( 2, "Sl" )
    return print(locationFormat:format( info.source, info.currentline), ... )
end
```
