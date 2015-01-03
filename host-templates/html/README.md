# HTML5 Host Template

To run your Moai game using HTML5, you can use this template. It includes the required initialization code and libraries. It also includes a simple Node.js server app to serve your project during testing.

## Building the Moai SDK

The HTML5 host requires a JavaScript version of the Moai SDK. You can build it from the C++ source files via Emscripten. See the `README.md` file in `src/host-html` for details. Once you have built `moaijs.js`, copy it to `host-templates/html/www/lib` (you'll have to create the `lib` directory manually) Alternatively, create a symbolic link. This way, you won't have to repeat the copy step when you re-build the SDK.

## Creating the ROM
Before you can do much, your assets and source files will need to be packaged into a single ROM file. To do this you will need Python 2.7+ installed. Make sure your Python `bin` directory is on the path.

To create the ROM, place your sources and assets (in the folder layout they expect) into the `host-templates/html/src` folder (again, create the folder manually or just symlink it). The HTML5 host automatically will run `main.lua` from the `src/` folder when it loads and will treat the `src/` folder as root of the virtual filesystem.

Then run the `buildrom.sh` or buildrom.bat (for Windows) file. This will package up your src folder into a file called `moaiapp.rom` and create a metadata file called `moaiapp.rom.json`.
These files will automatically be placed into the `host-templates/html/www` folder which is what you will need to deploy or serve to test your app.

Your host should now be ready to run!

## Testing/Running

To run your app you need to serve the contents of the www directory from a webserver.
> Using file:// will not work because of xhr2 security policy.

There is a simple Node.js express server `app.js` in this folder which can be run with a copy of Node.js by
```
  > npm install
  > node app.js
```

You can then see the host by visiting `http://localhost:3000/index.html` this is an example of the host in an iframe, the fullscreen
version of the host (which is used in the iframe) is located at `/moai.html`.

Alternatively, you can download the [Mongoose server](http://cesanta.com/mongoose.shtml). Simply place the executable into your `www` directory and execute it.

To refresh your code while developing you do not need to restart the server. Just rerun `buildrom.sh` and
refresh your browser.

You can poke your running app from the developer console in the browser by calling 
```
  player.moai.AKURunString('print("hello from moai"')
  or
  player.moai.AKURunScript('otherluafilehere.lua')
```

You can refresh the lua context with 
```
  player.moai.RefreshContext()
```

## Modifying the Template

To change the size of the player look for the iframe and change its width and height properties

The width and height of the player canvas are passed into moai as verticalResolution and horizontalResolution
like on mobile devices. 

`MoaiSim.OpenWindow()` (from lua code) will cause the canvas to resize to the requested size. The element with the class 
`moai-title` will be updated with the title of the window from OpenWindow.

The element with the class `moai-status` is updated with the loading progress of the host. 
