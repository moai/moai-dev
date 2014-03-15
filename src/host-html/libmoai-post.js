    Module.OpenWindowCallback=  null;
    Module.SaveCallback = null;
    Module['SetOpenWindowFunc'] = function(openwindowfunc) {
      Module.OpenWindowCallback = openwindowfunc;
    }

    Module['SetSaveFunc'] = function(savefunc) {
      Module.SaveCallback = savefunc;
    }

	return Module;	
}