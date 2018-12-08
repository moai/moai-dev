    Module.OpenWindowCallback=  null;

    Module.EnterFullScreenCallback = null;
    Module['SetEnterFullScreenFunc']  = function(func) {
      Module.EnterFullScreenCallback = func;
    }

    Module.ExitFullScreenCallback = null;
    Module['SetExitFullScreenFunc']  = function(func) {
      Module.ExitFullScreenCallback = func;
    }

    Module.SaveCallback = null;
    Module['SetOpenWindowFunc'] = function(openwindowfunc) {
      Module.OpenWindowCallback = openwindowfunc;
    }

    Module['SetSaveFunc'] = function(savefunc) {
      Module.SaveCallback = savefunc;
    }

