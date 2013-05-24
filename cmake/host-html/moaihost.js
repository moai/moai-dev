//patch our missing callbacks
Module._OpenWindowFunc = function(title,width,height) {
	console.log("OpenWindowFunc",title,width,height)
}



//wrap our funcs into a global because we can
Moaijs = {}

Moaijs.RefreshContext = Module.cwrap('RefreshContext','number',null);

Moaijs.AKURunString = Module.cwrap('AKURunString','number',['string']);

Moaijs.onPaint = Module.cwrap('onPaint','number',null);

Moaijs.onReshape = Module.cwrap('onReshape','number',['number','number']);

Moaijs.onTimer = Module.cwrap('onTimer','number',null);




