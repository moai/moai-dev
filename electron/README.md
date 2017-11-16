# electron-quick-start

A basic Electron application needs just these files:

- `package.json` - Points to the app's main file and lists its details and dependencies.
- `main.js` - Starts the app and creates a browser window to render HTML. This is the app's **main process**.
- `index.html` - A web page to render. This is the app's **renderer process**.

You can learn more about each of these components within the [Quick Start Guide](http://electron.atom.io/docs/tutorial/quick-start).

## To Use

Install latest [emscripten](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html)

Put moai lua sources under `lua/`

```bash
# Install dependencies
npm install
# Build moaijs.wasm with emscripten. This will copy moai.js into ./lib/
npm run build_moai
# Run the app
npm start
```

```bash
# Package mac os X and windows apps
npm run build
```