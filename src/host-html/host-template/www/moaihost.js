
/*
* Author: David Pershouse
*/

//stop leakage
var MoaiJS = (function() {

//wrap our funcs into a global because we can
function MoaiJS(canvas, total_memory, onTitleChange, onStatusChange, onError ) {
	this.canvas = canvas;
	this.onTitleChange = onTitleChange;
	this.onStatusChange = onStatusChange;
	this.onError = onError;
	this.emscripten = null;
	this.total_memory = total_memory;
	this.loadedFileSystem = null;

    console.log("MoaiJS Init");
}

MoaiJS.prototype.initEmscripten = function() {
	if (this.emscripten) return;
    var Opts = { //our required emscripten settings
    		canvas: this.canvas,
    		setStatus: this.onStatusChange,
    		printErr: this.onError,
    		noExitRuntime: true,
    		totalDependencies: 0,
    		TOTAL_MEMORY: this.total_memory
    	};
	console.log("MoaiJS Emscripten Init");
	this.emscripten = window.CreateMoaiRuntime(Opts);

	//bring our emscripten host functions into the normal javascript world
	var Module = this.emscripten;
	this.RefreshContext = Module.cwrap('RefreshContext','number',null);
	this.AKURunString = Module.cwrap('AKURunString','number',['string']);
	this.AKURunScript = Module.cwrap('AKURunScript','number',['string']);
	this.AKUSetWorkingDirectory = Module.cwrap('AKUSetWorkingDirectory','number',['string']);
	this.AKUGetSimStep = Module.cwrap('AKUGetSimStep','number',null);
	this.AKUEnqueueKeyboardShiftEvent = Module.cwrap('AKUEnqueueKeyboardShiftEvent','number',['number','number','number']);
	this.onPaint = Module.cwrap('onPaint','number',null);
	this.onReshape = Module.cwrap('onReshape','number',['number','number']);
	this.onTimer = Module.cwrap('onTimer','number',null);
	this.onMouseButton = Module.cwrap('onMouseButton', 'number',['number','number']);
	this.onMouseMove = Module.cwrap('onMouseMove', 'number',['number','number']);
	this.onKeyDown = Module.cwrap('onKeyDown', 'number',['number']);
	this.onKeyUp = Module.cwrap('onKeyUp','number',['number']);

	//callbacks
	this.emscripten.SetOpenWindowFunc(this.OpenWindowFunc.bind(this));
    this.emscripten.SetSaveFunc(this.SaveFile.bind(this));
}

MoaiJS.prototype.loadFileSystem = function(romUrl) {
   var that = this;
   function makeProgress(prefix) {
		return function(done,total) {
	   		that.onStatusChange(prefix+": "+done+"/"+total);
		}
  };
  this.initEmscripten(); 
  console.log("MoaiJS Load Filesystem");
  this.loadedFileSystem = window.LoadRom(this.emscripten,romUrl, makeProgress('Loading Data'), makeProgress('Installing File'));
}

MoaiJS.prototype.run = function(mainLua, romUrl) {
	var that = this;
	if (!this.loadedFileSystem) {
		if (!romUrl) {
			console.log("No file system specified or loaded");
			return;
		}
		this.loadFileSystem(romUrl);
	}

	this.loadedFileSystem.then(function(){
	   console.log("MoaiJS Filesystem Loaded");	
  	   that.emscripten.addOnPreMain(function(){ that.runhost(mainLua); });
  	   that.emscripten.run();   
  	}).rethrow();
}


MoaiJS.prototype.renderloop = function() {
	this.onPaint();
	this.emscripten.requestAnimationFrame(this.renderloop.bind(this));
}

MoaiJS.prototype.updateloop = function() {
	this.onTimer();
}

MoaiJS.prototype.mousedown = function(e) {
    this.onMouseButton(0,0); //MOUSE_LEFT, MOUSE_DOWN
}

MoaiJS.prototype.mouseup = function(e) {
    this.onMouseButton(0,1); //MOUSE_LEFT, MOUSE_UP
}

MoaiJS.prototype.mousemove = function(e) {
	
	var can = this.canvas;
    var canX = e.offsetX || e.layerX; 
    var canY = e.offsetY || e.layerY;
    this.onMouseMove(Math.floor(canX*this.canvasScale),Math.floor(canY*this.canvasScale));
}


var getASCIIKey= function(event) {
      if (event['ctrlKey'] || event['altKey'] || event['metaKey']) return null;

      var keycode = event['keyCode'];

      /* The exact list is soooo hard to find in a canonical place! */

      if (48 <= keycode && keycode <= 57)
        return keycode; // numeric  TODO handle shift?
      if (65 <= keycode && keycode <= 90)
	return event['shiftKey'] ? keycode : keycode + 32;
      if (106 <= keycode && keycode <= 111)
	return keycode - 106 + 42; // *,+-./  TODO handle shift?

      switch (keycode) {
        case 27: // escape
        case 32: // space
        case 61: // equal
          return keycode;
      }

      var s = event['shiftKey'];
      switch (keycode) {
        case 186: return s ? 58 : 59; // colon / semi-colon
        case 187: return s ? 43 : 61; // add / equal (these two may be wrong)
        case 188: return s ? 60 : 44; // less-than / comma
        case 189: return s ? 95 : 45; // dash
        case 190: return s ? 62 : 46; // greater-than / period
        case 191: return s ? 63 : 47; // forward slash
        case 219: return s ? 123 : 91; // open bracket
        case 220: return s ? 124 : 47; // back slash
        case 221: return s ? 125 : 93; // close braket
        case 222: return s ? 34 : 39; // single quote
      }

      return null;
    };




MoaiJS.prototype.keydown = function(e) {
	if (e.keycode == 16) {
		this.AKUEnqueueKeyboardShiftEvent(0,0,1); //device, keyboard, true
	} else {
		var key = getASCIIKey(e);
		this.onKeyDown(key?key:e.keyCode);
	}
	e.preventDefault();

	if (e.keyCode == 8) {
        return false; //eat backspace !
    }
}

MoaiJS.prototype.keyup = function(e) {
	if (e.keyCode == 16) {
		this.AKUEnqueueKeyboardShiftEvent(0,0,0); //device, keyboard, false
	} else {
		var key = getASCIIKey(e);
		this.onKeyUp(key?key:e.keyCode);
	}
	e.preventDefault();
}

MoaiJS.prototype.keypress = function(e) {
	if (e.keyCode == 8) {
		return false;
	}
}


MoaiJS.prototype.OpenWindowFunc = function(title,width,height) {
	var canvas = this.canvas;
	if (this.onTitleChange) { this.onTitleChange(title); }
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
	canvas.addEventListener("mouseout",(function() { canvas.blur(); this.mouseup(); }).bind(this),false);

	//grab keys
	canvas.addEventListener("keydown",this.keydown.bind(this),false);
	canvas.addEventListener("keyup",this.keyup.bind(this),false);

	canvas.addEventListener("keypress",this.keypress.bind(this), false);


	//now start rendering and updationg
	this.startUpdates();
	this.emscripten.requestAnimationFrame(this.renderloop.bind(this));

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
		that.canvasScale = canvas.width/$(canvas).width();
	  }

	return canvas;
};

MoaiJS.prototype.startUpdates = function() {
	var step = this.AKUGetSimStep() || ((1000/60)/1000)
	if (this.moaiInterval) {
		window.clearInterval(this.moaiInterval);
	}
	this.moaiInverval = window.setInterval( this.updateloop.bind(this), step*1000);
}

MoaiJS.prototype.stopUpdates = function() {
	if (this.moaiInterval) {
		window.clearInterval(this.moaiInterval);
	}
	this.moaiInverval = null;
}

MoaiJS.prototype.pause = function() {
	this.stopUpdates();
}

MoaiJS.prototype.unpause = function() {
	this.startUpdates();
}

MoaiJS.prototype.runhost = function(mainLua) {
	console.log("runhost called");
	console.log("restoring save state");
	this.restoreDocumentDirectory();
	console.log("refreshing context");
	this.RefreshContext();
	console.log("setting working directory");
	this.AKUSetWorkingDirectory('/src');
	console.log("setting up canvas");
	this.AKURunString('MOAIEnvironment.horizontalResolution = '+this.canvas.width);
	this.AKURunString('MOAIEnvironment.verticalResolution = '+this.canvas.height);
	var main = mainLua || 'main.lua'
	console.log("launching "+main);
	this.AKURunScript(main);
}

//Courtesy of Mozilla
function b64ToUint6 (nChr) {
     
    return nChr > 64 && nChr < 91 ? 
            nChr - 65
        : nChr > 96 && nChr < 123 ?
            nChr - 71
        : nChr > 47 && nChr < 58 ?
            nChr + 4
        : nChr === 43 ?
            62
        : nChr === 47 ?
            63
        :
            0;
     
}
//Courtesy of Mozilla     
function base64DecToArr (sBase64) {
     
    var
        sB64Enc = sBase64.replace(/[^A-Za-z0-9\+\/]/g, ""), nInLen = sB64Enc.length,
        nOutLen = nInLen * 3 + 1 >> 2, aBytes = new Uint8Array(nOutLen);
     
    for (var nMod3, nMod4, nUint24 = 0, nOutIdx = 0, nInIdx = 0; nInIdx < nInLen; nInIdx++) {
        nMod4 = nInIdx & 3;
        nUint24 |= b64ToUint6(sB64Enc.charCodeAt(nInIdx)) << 18 - 6 * nMod4;
        if (nMod4 === 3 || nInLen - nInIdx === 1) {
            for (nMod3 = 0; nMod3 < 3 && nOutIdx < nOutLen; nMod3++, nOutIdx++) {
                aBytes[nOutIdx] = nUint24 >>> (16 >>> nMod3 & 24) & 255;
            }
            nUint24 = 0;
     
        }
    }
     
    return aBytes;
}

function dataUriToArr(sDataUri) {
	return base64DecToArr(sDataUri.split(',')[1]);
}


MoaiJS.prototype.restoreDocumentDirectory = function() {
   var docsjson  =  window.localStorage.getItem('moai-docs');
   if (!docsjson) return;
   var docs = JSON.parse(docsjson);
   for (var path in docs ) {
   	  //restore each file
   	  var data =  dataUriToArr(docs[path]);
   	  this.emscripten._RestoreFile(path,data);
 	}
}

MoaiJS.prototype.saveToDocumentDirectory = function(path,filedata) {
	var docsjson = localStorage.getItem('moai-docs');
	if (!docsjson) docsjson="{}";
	var docs = JSON.parse(docsjson);
	docs[path] = filedata;
	localStorage.setItem('moai-docs',JSON.stringify(docs));
}

MoaiJS.prototype.SaveFile = function(path,data) {
	//get data as arraybuffer
	var dataArray = (data.buffer)? data : new Uint8Array(data);
	var dataBlob = new Blob([dataArray],{'type':'application/octet-stream'});
	var reader = new FileReader();
	var that = this;
	reader.onload =function(event) {
		that.saveToDocumentDirectory(path,event.target.result);
	};
	reader.readAsDataURL(dataBlob);
};

return MoaiJS
})(); //end module

//assume Jquery

function MoaiPlayer(element) {
	var el = $(element)
	var template = '<div class="moai-window"> \
                         <div class="moai-header"> \
                            <strong class="moai-title">MOAI</strong><span class="moai-status">Loading..</span> \
                             <div style="clear:both"></div> \
                         </div> \
                            <div class="moai-canvas-wrapper"><canvas class="moai-canvas" width="960" height="640" tabindex='1'></canvas></div> \
                        <div class="moai-footer"> \
                            <div class="moai-attrib"> \
                            Made with Moai \
                            www.getmoai.com \
                            Copyright (c) 2010-2012 Zipline Games, Inc. All Rights Reserved. \
                            </div> \
                        </div> \
                    </div>';

    el.innerHTML = template;


	var titleEl = el.find("moai-title").first();
	var statusEl = el.find("moai-status").first();
	var canvasEl = el.find("moai-canvas").first();
	
	//get settings
	this.url = el.attr('data-url') || 'moaiapp.rom';
	this.script = el.attr('data-script') || 'main.lua';
	var ram = parseInt(el.attr('data-ram') || "48" ,10);
	var title = el.attr('data-title') || 'Moai Player';



	function onTitleChange(title) {
		titleEl.innerHTML = title;
	}

	function onStatusChange(status) {
		statusEl.innerHTML = status;
	}

	function onError(err) {
		console.log("ERROR: ",err);
	}

	onTitleChange(title);
	this.moai = new MoaiJS(canvasEl[0],ram*1024*1024,onTitleChange,onStatusChange,onError);
}

MoaiPlayer.prototype.run = function() {
	this.moai.run(this.script, this.url);	
}

MoaiPlayer.prototype.stop = function() {
	//TODO unhook events and free up moai host ram
	this.moai = null;
}

MoaiPlayer.prototype.pause = function() {
	
}

MoaiPlayer.prototype.unpause = function() {
	
}



//TODO replace below with new MoaiJS calls
var player = new MoaiPlayer($("testplayer"));
player.run();

