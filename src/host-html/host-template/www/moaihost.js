
/*
* Author: David Pershouse
*/

//wrap our funcs into a global because we can
moaijs = {}
function wrapNativeFuncs() {
	moaijs.RefreshContext = Module.cwrap('RefreshContext','number',null);

	moaijs.AKURunString = Module.cwrap('AKURunString','number',['string']);
	moaijs.AKURunScript = Module.cwrap('AKURunScript','number',['string']);
	moaijs.AKUSetWorkingDirectory = Module.cwrap('AKUSetWorkingDirectory','number',['string']);
	moaijs.AKUGetSimStep = Module.cwrap('AKUGetSimStep','number',null);
	moaijs.AKUEnqueueKeyboardShiftEvent = Module.cwrap('AKUEnqueueKeyboardShiftEvent','number',['number','number','number']);


	moaijs.onPaint = Module.cwrap('onPaint','number',null);

	moaijs.onReshape = Module.cwrap('onReshape','number',['number','number']);

	moaijs.onTimer = Module.cwrap('onTimer','number',null);

	moaijs.onMouseButton = Module.cwrap('onMouseButton', 'number',['number','number']);
	moaijs.onMouseMove = Module.cwrap('onMouseMove', 'number',['number','number']);

	moaijs.onKeyDown = Module.cwrap('onKeyDown', 'number',['number']);
	moaijs.onKeyUp = Module.cwrap('onKeyUp','number',['number']);
}




moaijs.renderloop = function() {
	moaijs.onPaint();
	Module.requestAnimationFrame(moaijs.renderloop);
}

moaijs.updateloop = function() {
	moaijs.onTimer();
}

moaijs.mousedown = function(e) {
    moaijs.onMouseButton(0,0); //MOUSE_LEFT, MOUSE_DOWN
}

moaijs.mouseup = function(e) {
    moaijs.onMouseButton(0,1); //MOUSE_LEFT, MOUSE_UP
}

moaijs.mousemove = function(e) {
	var can = Module.canvas;
	var canX = e.pageX - can.offsetLeft;
    var canY = e.pageY - can.offsetTop;
    moaijs.onMouseMove(canX,canY);
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




moaijs.keydown = function(e) {

	console.log("keydown",e.keyCode);
	if (e.keycode == 16) {
		moaijs.AKUEnqueueKeyboardShiftEvent(0,0,1); //device, keyboard, true
	} else {
		var key = getASCIIKey(e);
		moaijs.onKeyDown(key?key:e.keyCode);
	}
	e.preventDefault();

	if (e.keyCode == 8) {
        return false; //eat backspace !
    }
}

moaijs.keyup = function(e) {
	console.log("keyup",e.keyCode);
	if (e.keyCode == 16) {
		moaijs.AKUEnqueueKeyboardShiftEvent(0,0,0); //device, keyboard, false
	} else {
		var key = getASCIIKey(e);
		moaijs.onKeyUp(key?key:e.keyCode);
	}
	e.preventDefault();
}

moaijs.keypress = function(e) {
	if (e.keyCode == 8) {
		return false;
	}
}


moaijs.OpenWindowFunc = function(title,width,height) {
	var canvas = document.getElementById('MoaiCanvas');
	var moaititle = document.getElementById('MoaiTitle');
	moaititle.innerHTML = title;
	canvas.width = width;
	canvas.height = height;

	Module.canvas = canvas;
	canvas.focus();
	Browser.createContext(canvas,true,true);
	//hook mouse
	canvas.addEventListener("mousedown",moaijs.mousedown,false);
	canvas.addEventListener("mouseup",moaijs.mouseup,false);
	canvas.addEventListener("mousemove",moaijs.mousemove,false);

	//grab focus on hover
	canvas.addEventListener("mouseover",function() { canvas.focus(); },false);
	canvas.addEventListener("mouseout",function() { canvas.blur(); moaijs.mouseup(); },false);

	//grab keys
	canvas.addEventListener("keydown",moaijs.keydown,false);
	canvas.addEventListener("keyup",moaijs.keyup,false);

	canvas.addEventListener("keypress",moaijs.keypress, false);


	//now start rendering and updationg
	var step = moaijs.AKUGetSimStep() || ((1000/60)/1000)
	var moaiInverval = window.setInterval( moaijs.updateloop, step*1000);
	Module.requestAnimationFrame(moaijs.renderloop);
};

moaijs.runhost = function() {
	wrapNativeFuncs();
	moaijs.restoreDocumentDirectory();
	moaijs.RefreshContext();
	moaijs.AKUSetWorkingDirectory('/src');
	moaijs.AKURunString('MOAIEnvironment.horizontalResolution = '+Module.canvas.width);
	moaijs.AKURunString('MOAIEnvironment.verticalResolution = '+Module.canvas.height);
	moaijs.AKURunScript('main.lua');
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


moaijs.restoreDocumentDirectory = function() {
   var docsjson  =  window.localStorage.getItem('moai-docs');
   if (!docsjson) return;
   var docs = JSON.parse(docsjson);
   for (var path in docs ) {
   	  //restore each file
   	  var data =  dataUriToArr(docs[path]);
   	  Module._RestoreFile(path,data);
 	}
}

moaijs.saveToDocumentDirectory = function(path,filedata) {
	var docsjson = localStorage.getItem('moai-docs');
	if (!docsjson) docsjson="{}";
	var docs = JSON.parse(docsjson);
	docs[path] = filedata;
	localStorage.setItem('moai-docs',JSON.stringify(docs));
}

moaijs.SaveFile = function(path,data) {
	//get data as arraybuffer
	var dataArray = (data.buffer)? data : new Uint8Array(data);
	var dataBlob = new Blob([dataArray],{'type':'application/octet-stream'});
	var reader = new FileReader();
	reader.onload =function(event) {
		moaijs.saveToDocumentDirectory(path,event.target.result);
	};
	reader.readAsDataURL(dataBlob);
};



moaijs.run = function() {
	console.log("moai host run");
	Module = (typeof Module === "undefined")?{}:Module;
      Module['canvas'] = document.getElementById('MoaiCanvas');
      Module['setStatus'] = function(status) {
      	 console.log(status);
      	 document.getElementById('MoaiStatus').innerHTML = status;
      };
      Module['printErr'] = function(text) {
            text = Array.prototype.slice.call(arguments).join(' ');
            console.log('Error:'+text);
          }
      Module['noExitRuntime'] = true;
      Module['totalDependencies'] = 0;

      Module['monitorRunDependencies'] = function(left) {
          this.totalDependencies = Math.max(this.totalDependencies, left);
          Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
          if (!left) {
            Module.run();
            console.log('MoaiJS Running. Waiting For Host');
            $(function() { moaijs.runhost(); });
          }
        }

}

moaijs.run();

