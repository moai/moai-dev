/* Promise lib (c) Jonathan Gotti - licence: https://github.com/malko/d.js/LICENCE.txt @version 0.6.0*/
!function(a){"use strict";function b(a){j(function(){throw a})}function c(b){return this.then(b,a)}function d(b){return this.then(a,b)}function e(b,c){return this.then(function(a){return k(b)?b.apply(null,l(a)?a:[a]):t.onlyFuncs?a:b},c||a)}function f(a){function b(){a()}return this.then(b,b),this}function g(a){return this.then(function(b){return k(a)?a.apply(null,l(b)?b.splice(0,0,void 0)&&b:[void 0,b]):t.onlyFuncs?b:a},function(b){return a(b)})}function h(c){return this.then(a,c?function(a){throw c(a),a}:b)}function i(a,b){var c=o(a);if(1===c.length&&l(c[0])){if(!c[0].length)return t.fulfilled([]);c=c[0]}var d=[],e=t(),f=c.length;if(f)for(var g=(function(a){c[a]=t.promisify(c[a]),c[a].then(function(g){a in d||(d[a]=b?c[a]:g,--f||e.resolve(d))},function(g){a in d||(b?(d[a]=c[a],--f||e.resolve(d)):e.reject(g))})}),h=0,i=f;i>h;h++)g(h);else e.resolve(d);return e.promise}var j,k=function(a){return"function"==typeof a},l=function(a){return Array.isArray?Array.isArray(a):a instanceof Array},m=function(a){return!(!a||!(typeof a).match(/function|object/))},n=function(b){return b===!1||b===a||null===b},o=function(a,b){return[].slice.call(a,b)},p="undefined",q=typeof TypeError===p?Error:TypeError;if(typeof process!==p&&process.nextTick)j=process.nextTick;else if(typeof MessageChannel!==p){var r=new MessageChannel,s=[];r.port1.onmessage=function(){s.length&&s.shift()()},j=function(a){s.push(a),r.port2.postMessage(0)}}else j=function(a){setTimeout(a,0)};var t=function(b){function i(){if(0!==s){var a,b=u,c=0,d=b.length,e=~s?0:1;for(u=[];d>c;c++)(a=b[c][e])&&a(p)}}function l(a){function b(a){return function(b){return c?void 0:(c=!0,a(b))}}var c=!1;if(s)return this;try{var d=m(a)&&a.then;if(k(d)){if(a===v)throw new q("Promise can't resolve itself");return d.call(a,b(l),b(o)),this}}catch(e){return b(o)(e),this}return r(function(){p=a,s=1,i()}),this}function o(a){return s||r(function(){try{throw a}catch(b){p=b}s=-1,i()}),this}var p,r=(a!==b?b:t.alwaysAsync)?j:function(a){a()},s=0,u=[],v={then:function(a,b){var c=t();return u.push([function(b){try{n(a)?c.resolve(b):c.resolve(k(a)?a(b):t.onlyFuncs?b:a)}catch(d){c.reject(d)}},function(a){if((n(b)||!k(b)&&t.onlyFuncs)&&c.reject(a),b)try{c.resolve(k(b)?b(a):b)}catch(d){c.reject(d)}}]),0!==s&&r(i),c.promise},success:c,error:d,otherwise:d,apply:e,spread:e,ensure:f,nodify:g,rethrow:h,isPending:function(){return!(0!==s)},getStatus:function(){return s}};return v.toSource=v.toString=v.valueOf=function(){return p===a?this:p},{promise:v,resolve:l,fulfill:l,reject:o}};t.deferred=t.defer=t,t.nextTick=j,t.alwaysAsync=!0,t.onlyFuncs=!0,t.resolved=t.fulfilled=function(a){return t(!0).resolve(a).promise},t.rejected=function(a){return t(!0).reject(a).promise},t.wait=function(a){var b=t();return setTimeout(b.resolve,a||0),b.promise},t.delay=function(a,b){var c=t();return setTimeout(function(){try{c.resolve(a.apply(null))}catch(b){c.reject(b)}},b||0),c.promise},t.promisify=function(a){return a&&k(a.then)?a:t.resolved(a)},t.all=function(){return i(arguments,!1)},t.resolveAll=function(){return i(arguments,!0)},t.nodeCapsule=function(a,b){return b||(b=a,a=void 0),function(){var c=t(),d=o(arguments);d.push(function(a,b){a?c.reject(a):c.resolve(arguments.length>2?o(arguments,1):b)});try{b.apply(a,d)}catch(e){c.reject(e)}return c.promise}},typeof window!==p&&(window.D=t),typeof module!==p&&module.exports&&(module.exports=t)}();

var MoaiJS = (function() {

const path = require('path')

function MoaiJS(canvas, total_memory, onTitleChange, onStatusChange, onError, onPrint, onResolutionChange, wasmfile ) {
	this.canvas = canvas;
	this.onTitleChange = onTitleChange;
	this.onStatusChange = onStatusChange;
	this.onError = onError;
	this.onPrint = onPrint;
	this.onResolutionChange = onResolutionChange;
	this.emscripten = null;
	this.total_memory = total_memory;
	this.loadedFileSystems = []; //array of filesystem urls to load
	this.wasmfile = wasmfile || "moaijs.wasm"; //allow override of wasm location incase it is in shared folder
	
	console.log("MoaiJS Init");
}

MoaiJS.prototype.instantiateWasm = function(info, receiveInstance) {
	return 
		fetch(this.wasmfile, { credentials: 'same-origin' })
		.then(function(response) {
			if (!response['ok']) {
				throw "failed to load wasm binary file at '" + wasmBinaryFile + "'";
			}
			return response['arrayBuffer']();
		})
		.then(function(binary) {
			return WebAssembly.instantiate(binary, info)
		})
		.then(function(output) {
			receiveInstance(output['instance']);
		})
		.catch(function(reason) {
			console.error('failed to asynchronously prepare wasm: ' + reason);
		});
}

MoaiJS.prototype.getEmscripten = function() {
	if (this.emscripten) return this.emscripten;
	
	var wasmLoaded = D.defer();
	var me = this;
	var moaiJSModuleKey = 'moaijsModule';
	var getWasmInstance = function(info) {

		var webFrame = window.require('electron').webFrame;
		webFrame.registerURLSchemeAsPrivileged('file');

		if (!window[moaiJSModuleKey]) {
			return fetch(me.wasmfile, { credentials: 'same-origin' })
				.then(function(response) {
					if (!response['ok']) {
						throw "failed to load wasm binary file at '" + wasmBinaryFile + "'";
					}
					return response['arrayBuffer']();
				})
				.then(function(binary) {
					return WebAssembly.instantiate(binary, info)
				})
				.then(function(output) { 
					window[moaiJSModuleKey] = output['module']; //stash it for next use
					return output['instance'];
				})
		}
		else {
			return WebAssembly.instantiate(window[moaiJSModuleKey], info)
		}
	}


	var instantiateWasm = function(info, receiveInstance) {
		return getWasmInstance(info)
			.then(function(instance) {
				receiveInstance(instance);
				console.log("got wasm okay waiting for runtime init");
			})
			.catch(function(reason) {
				console.error('failed to asynchronously prepare wasm: ' + reason);
				wasmLoaded.reject('failed to asynchronously prepare wasm: ' + reason);
			});
	}

	var Opts = { //our required emscripten settings
			canvas: this.canvas,
			setStatus: this.onStatusChange,
			print: this.onPrint,
			printErr: this.onError,
			noExitRuntime: true,
			totalDependencies: 0,
			onRuntimeInitialized: function() { console.log("got runtime init resolving"); wasmLoaded.resolve(); },
			instantiateWasm: instantiateWasm,

			// With electron we are Browser and Node at the same time.
			// Emscripten doesn't have support for environment like this (NODE and WEB are mutually exclusive).
			// Here I'm overriding environment to be NODE for node-fs to work,
			// but I'm not sure about possible side effects of not being WEB (everything seems to work).
			ENVIRONMENT: "NODE"
		};

	console.log("MoaiJS Emscripten Init");
	var em = window.CreateMoaiRuntime(Opts);

	//bring our emscripten host functions into the normal javascript world
	var Module = em;
	this.RefreshContext = Module.cwrap('RefreshContext','number',null);
	this.AKURunString = function(str) {
		Module.cwrap('AKULoadFuncFromString','number',['string','number','string'])(str,str.length,'AKURunString');
		Module.cwrap('AKUCallFunc','number',null)();
	}
	this.AKURunScript = function(str) {
		Module.cwrap('AKULoadFuncFromFile','number',['string'])(str);
		Module.cwrap('AKUCallFunc','number',null)();
	}

	this.AKUSetWorkingDirectory = Module.cwrap('AKUSetWorkingDirectory','number',['string']);
	this.AKUGetSimStep = Module.cwrap('AKUGetSimStep','number',null);
	this.onPaint = Module.cwrap('onPaint','number',null);
	this.onReshape = Module.cwrap('onReshape','number',['number','number']);
	this.onTimer = Module.cwrap('onTimer','number',null);
	this.onMouseButton = Module.cwrap('onMouseButton', 'number',['number','number']);
	this.onMouseMove = Module.cwrap('onMouseMove', 'number',['number','number']);
	this.onKeyDown = Module.cwrap('onKeyDown', 'number',['number']);
	this.onKeyUp = Module.cwrap('onKeyUp','number',['number']);
	this.onChar = Module.cwrap('onChar','number',['number']);

	//callbacks
	em.SetOpenWindowFunc(this.OpenWindowFunc.bind(this));
	// em.SetSaveFunc(this.SaveFile.bind(this));
	this.emscripten = wasmLoaded.promise.then(function() {
		em.then = null; //false promises are the worst! override the one from Modularize that is never actually fired.
		return em;
	}).error(function(e) { console.error(e); });
	return this.emscripten;
}

MoaiJS.prototype.run = function(mainLua) {
	var that = this;
	this.getEmscripten().then(function(emscripten) {
		console.log("Run main.lua");
		emscripten.run();
		that.runhost(mainLua, emscripten);
	}).rethrow();
}

MoaiJS.prototype.runString = function(luaStr) {
	var that = this;
	that.AKURunString(luaStr);
}


MoaiJS.prototype.renderloop = function() {
	this.onPaint();
	this.getEmscripten().then(function(emscripten) { 
		emscripten.requestAnimationFrame(this.renderloop.bind(this))
	}.bind(this));
}

MoaiJS.prototype.updateloop = function() {
	this.onTimer();
}

MoaiJS.prototype.mousedown = function(e) {

	var btn = e.button;

	if (btn != 2) {
		this.onMouseButton(0, 0); //MOUSE_LEFT, MOUSE_DOWN
	} else {
		this.onMouseButton(2, 0);
	}
   
}

MoaiJS.prototype.cancelMouseButtons = function() {
	this.onMouseButton(0, 1);
	this.onMouseButton(2, 1);
};

MoaiJS.prototype.mouseup = function(e) {

	var btn = e.button;

	if (btn != 2) {
		this.onMouseButton(0, 1); //MOUSE_LEFT, MOUSE_Up
	} else {
		this.onMouseButton(2, 1);
	}
}

MoaiJS.prototype.mousemove = function(e) {
	
	var can = this.canvas;
	var canX = e.offsetX || e.layerX; 
	var canY = e.offsetY || e.layerY;
	this.onMouseMove(Math.floor(canX*this.canvasScale),Math.floor(canY*this.canvasScale));
}

var charLookup = {
	0x20:' ',
	0x30:'0', 0x31:'1', 0x32:'2', 0x33:'3', 0x34:'4', 0x35:'5', 0x36:'6', 0x37:'7', 0x38:'8', 0x39:'9',
	0x41:'a', 0x42:'b', 0x43:'c', 0x44:'d', 0x45:'e', 0x46:'f', 0x47:'g', 0x48:'h', 0x49:'i', 0x4A:'j',
	0x4B:'k', 0x4C:'l', 0x4D:'m', 0x4E:'n', 0x4F:'o', 0x50:'p', 0x51:'q', 0x52:'r', 0x53:'s', 0x54:'t',
	0x55:'u', 0x56:'v', 0x57:'w', 0x58:'x', 0x59:'y', 0x5A:'z',
	0xBA:';', 0xBB:'=', 0xBC:',', 0xBD:'-', 0xBE:'.', 0xBF:'/', 0xC0:'`', 0xDB:'[', 0xDC:'\\', 0xDD:']', 0xDE:"'" 
}

function getMoaiKeyCode(keyCode) {
	// Browsers send virtual key codes, see http://msdn.microsoft.com/library/windows/desktop/dd375731.
	// Moai's key codes are based on virtual key codes, with two modifications:
	// - Keys that produce a character on a US keyboard use that character's ASCII code instead
	// - To avoid clashes, all regular key codes are offset by 256.
	if (keyCode == 0) return;
	if (keyCode in charLookup) return charLookup[keyCode].charCodeAt(0);
	return keyCode + 256;
};

function keyIsPrintable(keyCode) {
	return keyCode in charLookup;
}

MoaiJS.prototype.keydown = function(e) {
	var moaiKeyCode = getMoaiKeyCode(e.keyCode);
	if (moaiKeyCode) {
		this.onKeyDown(moaiKeyCode);
	}
	
	// If we prevent further event handling, keypress() will not be called either.
	// So let's do that only for keys that don't represent a character.
	// That's not perfect (Ctrl+D will still be handled by the brower),
	// but it's the best I could come up with.
	if (!keyIsPrintable(e.keyCode)) {
		e.preventDefault();
	}
	
	if (e.keyCode == 8) {
		return false; //eat backspace !
	}
}

MoaiJS.prototype.keyup = function(e) {
	var moaiKeyCode = getMoaiKeyCode(e.keyCode);
	if (moaiKeyCode) {
		this.onKeyUp(moaiKeyCode);
	}

	e.preventDefault();
}

MoaiJS.prototype.keypress = function(e) {
	if (e.which) {
		this.onChar(e.which);
	}

	e.preventDefault();
	if (e.keyCode == 8) {
		return false;
	}
}

MoaiJS.prototype.OpenWindowFunc = function(title,width,height) {
	var canvas = this.canvas;
	if (this.onTitleChange) { 
		this.onTitleChange(title); 
	}
	if (this.onResolutionChange) {
		this.onResolutionChange(width,height)
	}
	this.canvas.style.display = "block";

	canvas.width = width;
	canvas.height = height;
	this.canvasScale = canvas.width/$(canvas).width();

	canvas.focus();

	//hook mouse
	canvas.addEventListener("mousedown",this.mousedown.bind(this),false);
	canvas.addEventListener("mouseup",this.mouseup.bind(this),false);
	canvas.addEventListener("mousemove",this.mousemove.bind(this),false);

	//grab focus on hover
	canvas.addEventListener("mouseover",function() { canvas.focus(); },false);
	canvas.addEventListener("mouseout",(function() { canvas.blur(); this.cancelMouseButtons(); }).bind(this),false);

	//grab keys
	canvas.addEventListener("keydown", this.keydown.bind(this), false);
	canvas.addEventListener("keyup", this.keyup.bind(this), false);
	canvas.addEventListener("keypress", this.keypress.bind(this), false);

	canvas.addEventListener("contextmenu", function(e) { 
		e.preventDefault();
		return false;
	});
	//now start rendering and updationg
	this.startUpdates();
	this.getEmscripten().then(function(emscripten) {
		emscripten.requestAnimationFrame(this.renderloop.bind(this))
	}.bind(this));

	window.addEventListener("resize", resizeThrottler, false);

	  var resizeTimeout;
	  function resizeThrottler() {
		// ignore resize events as long as an actualResizeHandler execution is in the queue
		if ( !resizeTimeout ) {
		  resizeTimeout = setTimeout(function() {
			resizeTimeout = null;
			resizeHandler();
		 
		   // The actualResizeHandler will execute at a rate of 15fps
		   }, 66);
		}
	  }
	  var that = this;
	  function resizeHandler() {
		that.recalibrateInput();
	  }

	return canvas;
};

MoaiJS.prototype.recalibrateInput = function() {
	this.canvasScale = this.canvas.width / $(this.canvas).width();
}

MoaiJS.prototype.startUpdates = function() {
	var step = this.AKUGetSimStep() || ((1000/60)/1000)
	if (this.moaiInterval) {
		window.clearInterval(this.moaiInterval);
	}
	this.moaiInterval = window.setInterval( this.updateloop.bind(this), step*1000);
}

MoaiJS.prototype.stopUpdates = function() {
	if (this.moaiInterval) {
		window.clearInterval(this.moaiInterval);
	}
	this.moaiInterval = null;
}

MoaiJS.prototype.pause = function() {
	this.stopUpdates();
}

MoaiJS.prototype.unpause = function() {
	this.startUpdates();
}

MoaiJS.prototype.hostinit = function(emscripten) {

	const rootPath = path.join(__dirname, "lua");
	emscripten["FS_mkdir"]("/moai_app");
	emscripten["FS_mount"](emscripten["FS_filesystems"]["NODEFS"], {root: rootPath}, "/moai_app");

	console.log("refreshing context");
	this.RefreshContext();
	console.log("setting working directory");
	this.AKUSetWorkingDirectory('/moai_app');
	console.log("setting up canvas");
	this.AKURunString('MOAIEnvironment.horizontalResolution = '+$(this.canvas).parent().width());
	this.AKURunString('MOAIEnvironment.verticalResolution = '+$(this.canvas).parent().height());
}

MoaiJS.prototype.runhost = function(mainLua, emscripten) {
	console.log("runhost called");
	console.log("restoring save state");
	this.hostinit(emscripten);
	var main = mainLua || 'main.lua'
	console.log("launching "+main);
	this.AKURunScript(main);
}

return MoaiJS
})(); //end module

//assume Jquery

function MoaiPlayer(element, skipTemplate) {
	var el = $(element)
	var infoEl = el.find(".moai-info");
	this.el = el;
	
	var titleEl = el.find(".moai-title").first();
	var statusEl = el.find(".moai-status").first();
	var canvasEl = el.find(".moai-canvas").first();
	this.canvas = canvasEl[0];

	var canvasWrapperEl = el.find(".moai-canvas-wrapper").first();
	this.canvasWrapper = canvasWrapperEl;

	//get settings
	this.script = el.attr('data-script') || 'main.lua';
	var ram = parseInt(el.attr('data-ram') || "48", 10);
	var title = el.attr('data-title') || 'Moai Player';
	var wasm = el.attr('data-wasm');
	var paused = false;

	function onTitleChange(title) {
		titleEl.html(title);
	}

	function onStatusChange(status) {
		statusEl.html(status);
	}

	this.onResolutionChange = function(width, height) {
		//resize canvas after moaijs has finished its stuff
		window.setTimeout(function() {
			this.resizeCanvas();
		}.bind(this), 10);
	}

	this.onError = function(err) {
		console.log("ERROR: ",err);
	};

	this.onPrint = function(x) {
		console.log(x);
	};

	function onPrint(x) {
		this.onPrint(x);
	}

	function onError(x) {
		this.onError(x);
	}
	onTitleChange(title);

	window.addEventListener("resize", resizeThrottler, false);

	var resizeTimeout;
	function resizeThrottler() {
		// ignore resize events as long as an actualResizeHandler execution is in the queue
		if ( !resizeTimeout ) {
			resizeTimeout = setTimeout(function() {
				resizeTimeout = null;
				resizeHandler();
				// The actualResizeHandler will execute at a rate of 15fps
			}, 66);
		}
	}
	
	var resizeHandler = function() {
		this.resizeCanvas();
	}.bind(this);

	this.initMoai = function() {
		this.moai = new MoaiJS(canvasEl[0], ram * 1024 * 1024, onTitleChange, onStatusChange, onError.bind(this), onPrint.bind(this), this.onResolutionChange.bind(this), wasm);
		return this.moai.initMoai
	}
}


MoaiPlayer.prototype.resizeCanvas = function() {
	//we apply portrait class if the ratio of width to height is larger than our canvases
	var wantedRatio = this.canvas.width / this.canvas.height;
	var actualRatio = this.canvasWrapper.width() / this.canvasWrapper.height();
	this.el.find('.moai-canvas-wrapper').first().toggleClass("portrait", (actualRatio > wantedRatio));
}

MoaiPlayer.prototype.isSupported = function() {
	var gl = false;
	var testCanvas = document.createElement("canvas");
	try { gl = testCanvas.getContext("webgl"); }
	catch (x) { gl = null; }

	if (gl === null) {
		try { gl = testCanvas.getContext("experimental-webgl");}
		catch (x) { gl = null; }
	}

	if (!gl) {
		console.log("GLERROR: No gl context");
		return false;
	}


	if ("function" !== typeof gl.getParameter && "object" !== typeof gl.getParameter) {
		console.log("GLERROR: no getParameter function");
		return false;
	}

	var version = gl.getParameter(gl["VERSION"]);
	if (!version) {
		console.log("GLERROR: could not determine webgl version");
		return false;
	}

	var majorVerMatch = (version + "").match(/WebGL (\d*)\.\d*/);
	if (majorVerMatch.length < 2) {
		console.log("GLERROR: couldnt parse gl version: ",version);
		return false;
	}
	var majorVer = majorVerMatch[1] << 0;
	if (majorVer < 1) {
		console.log("GLERROR: major version must be above 1 got :",majorVer,"from:",version);
		return false;
	}

	gl = null;
	testCanvas = null;
	//all good here
	return true;
};

MoaiPlayer.prototype.run = function() {
	//init moai when they go to run to avoid chewing up ram on pages where the user isn't interested in playing our embedded game
	if (!this.moai) { this.initMoai(); }
	this.moai.run(this.script);
}

MoaiPlayer.prototype.runString = function(str) {
	if (!this.moai) { this.initMoai(); }
	this.moai.runString(str);
}

MoaiPlayer.prototype.stop = function() {
	//TODO unhook events and free up moai host ram
	this.moai.pause();
	this.moai = null;
}

MoaiPlayer.prototype.pause = function() {
	if (!this.moai) return;
	this.moai.pause();
}

MoaiPlayer.prototype.unpause = function() {
	if (!this.moai) return;
	this.moai.unpause();
}
