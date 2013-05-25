
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



moaijs.renderloop = function() {
	moaijs.onPaint();
	Module.requestAnimationFrame(moaijs.renderloop);
}

moaijs.updateloop = function() {
	moaijs.onTimer();
}

moaijs.OpenWindowFunc = function(title,width,height) {
	var canvas = document.getElementById('MoaiCanvas');
	var moaititle = document.getElementById('MoaiTitle');
	moaititle.innerHTML = title;
	canvas.width = width;
	canvas.height = height;

	Module.canvas = canvas;
	Browser.createContext(canvas,true,true);
	//now start rendering and updationg
	var step = moaijs.AKUGetSimStep() || ((1000/60)/1000)
	var moaiInverval = window.setInterval( moaijs.updateloop, step*1000);
	Module.requestAnimationFrame(moaijs.renderloop);
};

moaijs.runhost = function() {
	moaijs.RefreshContext();
	moaijs.AKUSetWorkingDirectory('/hello-moai');
	moaijs.AKURunScript('main.lua');
}
