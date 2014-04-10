##Html Host Template

To run your moai game using html5, you can use this template. It has the moaijs runtime and required initialization code
and libraries included. It also includes a simple nodejs server app to serve your project during testing.

### Creating The ROM
Before you can do much, your assets and source files will need to be packaged into a single rom file. To do this 
you will need python 2.7+ installd

Now to create the rom, place your source and assets (in the folder layout they expect) into the src folder. 
> The html5 host automatically will run `main.lua` from the `src/` folder when it loads and will treat the src/ folder as root of the virtual filesystem

Then run the `buildrom.sh` or buildrom.bat (for windows) file.
> Requires python to be on the path

This will package up your src folder into a file called `moaiapp.rom` and create a metadata file called `moaiapp.rom.json`.
These files will be placed into the www folder which is what you will need to deploy or serve to test your app.

Your host should now be ready to run!

### Testing/Running

To run your app you need to serve the contents of the www directory from a webserver.
> Using file:// will not work because of xhr2 security policy

There is a simple nodejs express server `app.js` in this folder which can be run with a copy of nodejs by
```
  > npm install
  > node app.js
```

You can then see the host by visiting `http://localhost:3000/index.html` this is an example of the host in an iframe, The fullscreen
version of the host (which is used in the iframe) is located at /moai.html

To refresh your code while developing you do not need to restart the nodejs server. Just rerun `buildrom.sh` and
refresh your browser.

You can poke your running app from the developer console in the browser by calling 
```
  player.moai.AKURunString('print("hello from moai"')
  or
  player.moai.AKURunScript('otherluafilehere.lua')
```

you can refresh the lua context with 
```
  player.moai.RefreshContext()
```

## Modify template

To change the size of the player look for the iframe and change its width and height properties

The width and height of the player canvas are passed into moai as verticalResolution and horizontalResolution
like on mobile devices. 

`MoaiSim.OpenWindow()` (from lua code) will cause the canvas to resize to the requested size. The element with the class 
`moai-title` will be updated with the title of the window from OpenWindow.

The element with the class `moai-status` is updated with the loading progress of the host. 



