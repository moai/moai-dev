
//wrap our funcs into a global because we can
moaijs = {}

moaijs.RefreshContext = Module.cwrap('RefreshContext','number',null);

moaijs.AKURunString = Module.cwrap('AKURunString','number',['string']);
moaijs.AKURunScript = Module.cwrap('AKURunScript','number',['string']);
moaijs.AKUSetWorkingDirectory = Module.cwrap('AKUSetWorkingDirectory','number',['string']);
moaijs.AKUGetSimStep = Module.cwrap('AKUGetSimStep','number',null);

moaijs.onPaint = Module.cwrap('onPaint','number',null);

moaijs.onReshape = Module.cwrap('onReshape','number',['number','number']);

moaijs.onTimer = Module.cwrap('onTimer','number',null);

moaijs.onMouseButton = Module.cwrap('onMouseButton', 'number',['number','number']);
moaijs.onMouseMove = Module.cwrap('onMouseMove', 'number',['number','number']);

moaijs.onKeyDown = Module.cwrap('onKeyDown', 'number',['number']);
moaijs.onKeyUp = Module.cwrap('onKeyUp','number',['number']);


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

moaijs.keydown = function(e) {

	console.log("keydown",e.keyCode);
	moaijs.onKeyDown(e.keyCode);
	if (e.keyCode == 8) {
		e.preventDefault();
		e.stopPropagation();
        return false; //eat backspace !
    }
}

moaijs.keyup = function(e) {
	console.log("keyup",e.keyCode);
	moaijs.onKeyUp(e.keyCode);
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
	canvas.addEventListener("mouseout",function() { canvas.blur(); },false);

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
