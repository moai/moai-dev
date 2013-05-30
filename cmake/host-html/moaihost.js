
//wrap our funcs into a global because we can
moaijs = {}

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

moaijs.OnFileModified = function(path,data) {
	console.log("file changed",path);
};


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
	moaijs.RefreshContext();
	moaijs.AKUSetWorkingDirectory('/src');
	moaijs.AKURunScript('main.lua');
}
