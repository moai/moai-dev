# HTML5 Host Template

To run your Moai game using HTML5, you need the following things: the host template, `moaijs.js`, and the ROM containing your assets.

- The *host template* in this folder (`host-templates/html`) includes the required initialization code and libraries. It also comes with a simple Node.js server app to serve your project during testing.
- *`moaijs.js`* contains the core Moai SDK in JavaScript. You can build it from the C++ Moai sources.
- *The ROM* is a binary blob that contains all your scripts and assets. You can build it from a `src` directory.

## Building the Moai SDK

The HTML5 host requires a JavaScript version of the Moai SDK. You can build it from the C++ source files via Emscripten. See the `README.md` file in `src/host-html` for details. The result is a file `moaijs.js` in the newly-created directory `lib/html`. 

## Setting Up Your Output Directory

Now, create a directory that you want to use as output directory. This is where the host template files, `moaijs.js`, and the ROM files will all end up in.

Now navigate into the `util` directory and execute the following command:

`moaiutil.bat host-html --use-symlink --output-dir <output directory>`

Replace `<output directory>` with your output directory. Note that you can shorten `--use-symlink` to `-s` and `--output-dir` to -`o`.

This will copy the host template from `host-templates/html` to your output directory. It will also create a symbolic link on your hard-disk that makes the generated `moaijs.js` file show up in the `www/lib` sub-folder of your output directory. The advantage of the symbolic link is that any time you re-compile Moai, your output directory automatically contains the current version without any copying. If you leave off the `-use-symlink` option, the file will be copied instead.

## Creating the ROM

Before you can do much, your assets and source files will need to be packaged into a single ROM file. To do this you will need Python 2.7+ installed. Make sure your Python `bin` directory is on the path.

To create the ROM, place your sources and assets (in the folder layout they expect) into the `<output dir>/src` folder (again, create the folder manually or symlink it). The HTML5 host automatically will run `main.lua` from the `src/` folder when it loads and will treat the `src/` folder as root of the virtual filesystem.

Then run the `buildrom.sh` or `buildrom.bat` (for Windows) file. This will package up your src folder into a file called `moaiapp.rom` and create a metadata file called `moaiapp.rom.json`.
These files will automatically be placed into the `<output dir>/www` folder which is what you will need to deploy or serve to test your app.

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

Alternatively, you can download the [Mongoose server](http://cesanta.com/mongoose.shtml). Simply place the executable into your `www` directory and execute it. Note that Mongoose doesn't seem to like it if `joaijs.js` is symbolically linked.

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
