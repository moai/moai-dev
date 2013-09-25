##Html Host Template

To run your moai game using html5, you can use this template. It has the moaijs runtime and required initialization code
and libraries included. It also includes a simple nodejs server app to serve your project during testing.

### Creating The ROM
Before you can do much, your assets and source files will need to be packaged into a single rom file. To do this 
you will need one of the emscripten tools.
Checkout or symlink Emscripten to this folder so that you can build your rom file 
```
git clone https://github.com/kripken/emscripten`
```

Now to create the rom, place your source and assets (in the folder layout they expect) into the src folder.
> The html5 host automatically will run `main.lua` from the `src/` folder when it loads

Then run the `buildrom.sh` file.
> Requires python to be on the path and emscripten in ./emscripten

This will package up your src folder into a file called `moaiapp.rom` and create a loader called `moaiapp.rom.js`.
These files will be placed into the www folder which is what you will need to deploy or serve to test your app.

Your host should now be ready to run!

### Testing/Running

To run your app you need to serve the contents of the www directory from a webserver.
> Using file:// will not work because of xhr2 security policy

There is a simple nodejs express server `app.js` in this folder which can be run with a copy of nodejs by
```
  > npm install
  > nodejs app.js
```

You can then see the host by visiting `http://localhost:3000/moai.html`

To refresh your code while developing you do not need to restart the nodejs server. Just rerun `buildrom.sh` and
refresh your browser.

You can poke your running app from the developer console in the browser by calling 
```
  moaijs.AKURunString('print("hello from moai"')
  or
  moaijs.AKURunScript('otherluafilehere.lua')
```

you can refresh the lua context with 
```
  moaijs.RefreshContext()
```

## Modify template

To change the size of the canvas, look for the canvas element `MoaiCanvas` and set the width and height you require. 

The width and height of the canvas are passed into moai as verticalResolution and horizontalResolution
like on mobile devices. 

`MoaiSim.OpenWindow()` (from lua code) will cause the canvas to resize to the requested size. The element with the Id 
`MoaiTitle` will be updated with the title of the window from OpenWindow.

The element with the id `MoaiStatus` is updated with the loading progress of the host. 

The only things the host needs are the 3 elements (they can be any type but canvas must be canvas) 
`MoaiCanvas` `MoaiTitle` `MoaiStatus`  and script tags (in order)  `moaihost.js`, `moaiapp.rom.js`, `moaijs.js`.

