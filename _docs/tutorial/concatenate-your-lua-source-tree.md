---
title: "Concatenate your Lua source tree"
---

The following code packs your source tree into one single file, and prints it to stdout. Each file is actually wrapped in a function, and is only executed on `require()` or `dofile()`. `loadfile()` just returns the wrapper.

If present in the root directory, `main.lua` will be run when the aggregate file is loaded. If not, the packer assumes that you're packing libraries and doesn't run anything.

`require()` works as expected, provided you use the dot notation for submodules `require"module.submodule"`.

`dofile()` and `loadfile()` behave as if the current directory was the root of your source tree, and don't support `.` and `..` in paths.

Usage
-----

```
$ lua pack.lua path/to/your/source/tree > out.lua
```

Notes
-----

-   This script requires the `lfs` library, which can optionally be loaded with luarocks.
-   For require to work, you must use the dot `"."` as a submodule separator.
    -   good: `require "module.submodule"`
    -   bad: `require "module/submodule"`
-   Only `.lua` files are included.
-   "`require "mod.submod"`" will look for "`module/submodule.lua`" then for "`module/submodule/init.lua`". If you want to get that functionality without the packer, you must add your game root directory in the package.path as follows, for example at the top of main.lua or in a config.lua file loaded before main.lua:

```
 package.path = "./?.lua;./?/init.lua" .. package.path:sub(8,-1)
```

:   This allows you to put Lua libraries that use "`init.lua`" as loader in your game tree. Packages in your local tree will shadow the ones installed system-wide (if any).

Example
-------

Suppose that you have the following source tree:

```
foo
 +-- bar.lua
 |     content: print "Bar"
 +-- baz
 |     +-- baz.lua
 |     |     content: print "Baz"
 |     +-- qux
 |          +-- init.lua
 |                content: print( ... )
 |                         return "Quux"   
 +-- main.lua
       content: require "bar" 
                dofile "baz/baz.lua"
                print( require "baz.qux" )
```

You use

```
$ lua pack.lua foo > foo.lua
```

and get the following:

### Result

```lua
-- foo.lua
--------------------------------------
package.preload['baz.qux'] = function (...)
print( ... )
return "Quux"
end

--------------------------------------
package.preload['main'] = function (...)
require "bar" 
dofile "baz/baz.lua"
print( require "baz.qux" )
end

--------------------------------------
package.preload['baz.baz'] = function (...)
print"Baz"
end

--------------------------------------
package.preload['bar'] = function (...)
print"Bar"
end
-----------------------------------------------
 
do
  if not package.__loadfile then
    local original_loadfile = loadfile
    local function lf (file)
      local hndl = file:gsub( "%.lua$", "" )
                       :gsub( "/", "." )
                       :gsub( "\\", "." )
                       :gsub( "%.init$", "" )
      return package.preload[hndl] or original_loadfile( name )
    end
 
    function dofile (name)
      return lf( name )()
    end
 
    loadfile, package.__loadfile = lf, loadfile
  end
end

dofile"main.lua"
```

When run, **`moai foo.lua`** gives

```
Bar 
Baz  
baz.qux
Quux
```

The code
--------

```lua
-- pack.lua
-- released under the Romantic WTF Public License
-- http://getmoai.com/wiki/index.php?title=User:Pygy/Romantic_WTF_Public_License

args = {...}
--require"luarocks.loader"
fs = require"lfs"
files = {}
 
root = args[1]:gsub( "/$", "" )
              :gsub( "\\$", "" )
 
function scandir (root, path)
-- adapted from http://keplerproject.github.com/luafilesystem/examples.html
    path = path or ""
    for file in fs.dir( root..path ) do
        if file ~= "." and file ~= ".." then
            local f = path..'/'..file
            local attr = lfs.attributes( root..f )
            assert (type( attr ) == "table")
            if attr.mode == "directory" then
                scandir( root, f )
            else
              if file:find"%.lua$" then
                hndl = (f:gsub( "%.lua$", "" )
                                 :gsub( "/", "." )
                                 :gsub( "\\", "." )
                                 :gsub( "^%.", "" )
                               ):gsub( "%.init$", "" )
                files[hndl] = io.open( root..f ):read"*a"
              end
            end
        end
    end
end
 
scandir( root )
 
acc={}
 
local wrapper = { "\n--------------------------------------\npackage.preload['"
                , nil, "'] = function (...)\n", nil, "\nend\n" }
for k,v in pairs( files ) do
  wrapper[2], wrapper[4] = k, v
  table.insert( acc, table.concat(wrapper) )
end
 
table.insert(acc, [[
-----------------------------------------------
 
do
  if not package.__loadfile then
    local original_loadfile = loadfile
    local function lf (file)
      local hndl = file:gsub( "%.lua$", "" )
                       :gsub( "/", "." )
                       :gsub( "\\", "." )
                       :gsub( "%.init$", "" )
      return package.preload[hndl] or original_loadfile( name )
    end
 
    function dofile (name)
      return lf( name )()
    end
 
    loadfile, package.__loadfile = lf, loadfile
  end
end
]])
if files.main then table.insert( acc, '\ndofile"main.lua"' ) end
print( table.concat( acc ) )
```
