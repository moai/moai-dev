/* Promise lib (c) Jonathan Gotti - licence: https://github.com/malko/d.js/LICENCE.txt @version 0.6.0*/
!function(a){"use strict";function b(a){j(function(){throw a})}function c(b){return this.then(b,a)}function d(b){return this.then(a,b)}function e(b,c){return this.then(function(a){return k(b)?b.apply(null,l(a)?a:[a]):t.onlyFuncs?a:b},c||a)}function f(a){function b(){a()}return this.then(b,b),this}function g(a){return this.then(function(b){return k(a)?a.apply(null,l(b)?b.splice(0,0,void 0)&&b:[void 0,b]):t.onlyFuncs?b:a},function(b){return a(b)})}function h(c){return this.then(a,c?function(a){throw c(a),a}:b)}function i(a,b){var c=o(a);if(1===c.length&&l(c[0])){if(!c[0].length)return t.fulfilled([]);c=c[0]}var d=[],e=t(),f=c.length;if(f)for(var g=(function(a){c[a]=t.promisify(c[a]),c[a].then(function(g){a in d||(d[a]=b?c[a]:g,--f||e.resolve(d))},function(g){a in d||(b?(d[a]=c[a],--f||e.resolve(d)):e.reject(g))})}),h=0,i=f;i>h;h++)g(h);else e.resolve(d);return e.promise}var j,k=function(a){return"function"==typeof a},l=function(a){return Array.isArray?Array.isArray(a):a instanceof Array},m=function(a){return!(!a||!(typeof a).match(/function|object/))},n=function(b){return b===!1||b===a||null===b},o=function(a,b){return[].slice.call(a,b)},p="undefined",q=typeof TypeError===p?Error:TypeError;if(typeof process!==p&&process.nextTick)j=process.nextTick;else if(typeof MessageChannel!==p){var r=new MessageChannel,s=[];r.port1.onmessage=function(){s.length&&s.shift()()},j=function(a){s.push(a),r.port2.postMessage(0)}}else j=function(a){setTimeout(a,0)};var t=function(b){function i(){if(0!==s){var a,b=u,c=0,d=b.length,e=~s?0:1;for(u=[];d>c;c++)(a=b[c][e])&&a(p)}}function l(a){function b(a){return function(b){return c?void 0:(c=!0,a(b))}}var c=!1;if(s)return this;try{var d=m(a)&&a.then;if(k(d)){if(a===v)throw new q("Promise can't resolve itself");return d.call(a,b(l),b(o)),this}}catch(e){return b(o)(e),this}return r(function(){p=a,s=1,i()}),this}function o(a){return s||r(function(){try{throw a}catch(b){p=b}s=-1,i()}),this}var p,r=(a!==b?b:t.alwaysAsync)?j:function(a){a()},s=0,u=[],v={then:function(a,b){var c=t();return u.push([function(b){try{n(a)?c.resolve(b):c.resolve(k(a)?a(b):t.onlyFuncs?b:a)}catch(d){c.reject(d)}},function(a){if((n(b)||!k(b)&&t.onlyFuncs)&&c.reject(a),b)try{c.resolve(k(b)?b(a):b)}catch(d){c.reject(d)}}]),0!==s&&r(i),c.promise},success:c,error:d,otherwise:d,apply:e,spread:e,ensure:f,nodify:g,rethrow:h,isPending:function(){return!(0!==s)},getStatus:function(){return s}};return v.toSource=v.toString=v.valueOf=function(){return p===a?this:p},{promise:v,resolve:l,fulfill:l,reject:o}};t.deferred=t.defer=t,t.nextTick=j,t.alwaysAsync=!0,t.onlyFuncs=!0,t.resolved=t.fulfilled=function(a){return t(!0).resolve(a).promise},t.rejected=function(a){return t(!0).reject(a).promise},t.wait=function(a){var b=t();return setTimeout(b.resolve,a||0),b.promise},t.delay=function(a,b){var c=t();return setTimeout(function(){try{c.resolve(a.apply(null))}catch(b){c.reject(b)}},b||0),c.promise},t.promisify=function(a){return a&&k(a.then)?a:t.resolved(a)},t.all=function(){return i(arguments,!1)},t.resolveAll=function(){return i(arguments,!0)},t.nodeCapsule=function(a,b){return b||(b=a,a=void 0),function(){var c=t(),d=o(arguments);d.push(function(a,b){a?c.reject(a):c.resolve(arguments.length>2?o(arguments,1):b)});try{b.apply(a,d)}catch(e){c.reject(e)}return c.promise}},typeof window!==p&&(window.D=t),typeof module!==p&&module.exports&&(module.exports=t)}();

var LoadRom = (function() {

/* rom cache */
function RomCache() {
      this.indexedDB = window.indexedDB || window.mozIndexedDB || window.webkitIndexedDB || window.msIndexedDB;

      this.IDB_RO = "readonly";
      this.IDB_RW = "readwrite";
      this.DB_NAME = 'EM_PRELOAD_CACHE';
      this.DB_VERSION = 1;
      this.METADATA_STORE_NAME = 'METADATA';
      this.PACKAGE_STORE_NAME = 'PACKAGES';
}      

RomCache.prototype.openDatabase = function() {
        var that = this;
        var d = D.defer();
        if (!this.indexedDB) {
            return D.rejected("no Indexed DB")
        }

        try {
          var openRequest = this.indexedDB.open(this.DB_NAME, this.DB_VERSION);
        } catch (e) {
            return D.rejected(e);
        }
        openRequest.onupgradeneeded = function(event) {
          var db = event.target.result;

          if(db.objectStoreNames.contains(that.PACKAGE_STORE_NAME)) {
            db.deleteObjectStore(that.PACKAGE_STORE_NAME);
          }
          var packages = db.createObjectStore(that.PACKAGE_STORE_NAME);

          if(db.objectStoreNames.contains(that.METADATA_STORE_NAME)) {
            db.deleteObjectStore(that.METADATA_STORE_NAME);
          }
          var metadata = db.createObjectStore(that.METADATA_STORE_NAME);
        };
        openRequest.onsuccess = function(event) {
          var db = event.target.result;
          d.resolve(db);
        };
        openRequest.onerror = function(error) {
          d.reject(error);
        };

        return d.promise;
};

RomCache.prototype.getDb = function() {
  if (!this.openedDatabase) {
    this.openedDatabase = this.openDatabase();
  }
  return this.openedDatabase;
}

RomCache.prototype.checkCachedPackage = function( packageName, uuid) {
   var that = this;
   return this.getDb().then(function(db) {
      return that.dbCheckCachedPackage(db, packageName, uuid)
   }).otherwise(function(e) {
        console.log("Can't check package cache :",e);
        return false;
   });
}

      /* Check if there's a cached package, and if so whether it's the latest available */
RomCache.prototype.dbCheckCachedPackage = function(db, packageName, uuid) {
        var d = D.defer();
        var transaction = db.transaction([this.METADATA_STORE_NAME], this.IDB_RO);
        var metadata = transaction.objectStore(this.METADATA_STORE_NAME);

        var getRequest = metadata.get(packageName);
        getRequest.onsuccess = function(event) {
          var result = event.target.result;
          if (!result) {
            return d.resolve(false);
          } else {
            return d.resolve(uuid=== result.uuid);
          }
        };
        getRequest.onerror = function(error) {
          d.reject(error);
        };
        return d.promise;
};

RomCache.prototype.fetchCachedPackage = function(packageName) {
   var that = this;
   return this.getDb().then(function(db) {
      return that.dbFetchCachedPackage(db, packageName)
   });   
};

RomCache.prototype.dbFetchCachedPackage = function(db, packageName) {
        var d = D.defer();
        var transaction = db.transaction([this.PACKAGE_STORE_NAME], this.IDB_RO);
        var packages = transaction.objectStore(this.PACKAGE_STORE_NAME);

        var getRequest = packages.get(packageName);
        getRequest.onsuccess = function(event) {
          var result = event.target.result;
          d.resolve(result);
        };
        getRequest.onerror = function(error) {
          d.reject(error);
        };
        return d.promise;
};

RomCache.prototype.cacheRemotePackage = function(packageName, packageData, packageMeta) {
   var that = this;
   return this.getDb().then(function(db) {
      return that.dbCacheRemotePackage(db, packageName, packageData, packageMeta);
   }).otherwise(function(e) {
           console.log("Can't cache package:",e);
           return false;
   });
};

RomCache.prototype.dbCacheRemotePackage = function(db, packageName, packageData, packageMeta) {
        var d = D.defer();
        var transaction = db.transaction([this.PACKAGE_STORE_NAME, this.METADATA_STORE_NAME], this.IDB_RW);
        var packages = transaction.objectStore(this.PACKAGE_STORE_NAME);
        var metadata = transaction.objectStore(this.METADATA_STORE_NAME);

        var putPackageRequest = packages.put(packageData, packageName);
        putPackageRequest.onsuccess = function(event) {
          var putMetadataRequest = metadata.put(packageMeta, packageName);
          putMetadataRequest.onsuccess = function(event) {
            d.resolve(packageData);
          };
          putMetadataRequest.onerror = function(error) {
            d.reject(error);
          };
        };
        putPackageRequest.onerror = function(error) {
          d.reject(error);
        };
        return d.promise;
};

/**
 	Load Json
 	then Read Rom 
 	then wait for FS
 	then install Files
 */

/**
  Grabs filesystem metadata json from the given url
*/
function LoadFileSystemJson(fsJsonUrl) {
  	var d = D.defer();
 	var xhr = new XMLHttpRequest();
    xhr.open('GET', fsJsonUrl, true);
    xhr.responseType = "json";
    xhr.onload = function(event) {
        var packageInfo = xhr.response;
        d.resolve(packageInfo);
    };
    xhr.onerror = function(event) {
    	d.reject("Failed to download "+fsJsonUrl+":"+xhr.statusText);
    };
    xhr.send(null);
    return d.promise;
}

/**
  Grabs the filesystem data blob from the given url
*/
function LoadRemoteFileSystemData(romPackageUrl, progress) {
	var d = D.defer();
	var xhr = new XMLHttpRequest();
    xhr.open('GET', romPackageUrl, true);
    xhr.responseType = 'arraybuffer';
    xhr.onload = function(event) {
        var packageData = xhr.response;
        d.resolve(packageData);
    };
    xhr.onprogress = function(event) {
    	progress(event.loaded, event.total);
    }
    xhr.onerror = function(event) {
    	d.reject("Failed to download FS data"+romPackageUrl+":"+xhr.statusText);
    };
    xhr.send(null);
    return d.promise;
}

/**
  Loads the filesystem data either via cache or via remoteUrl
  also caches it if it wasn't previously cached
*/
function LoadFileSystemData(packageName, uuid, romUrl, progress) {
   var romCache = new RomCache();
   return romCache.checkCachedPackage(packageName, uuid)
      .then(function(packageExists) {
        if (packageExists) {
          return romCache.fetchCachedPackage(packageName)
        } else {
          return LoadRemoteFileSystemData(romUrl, progress)
                  .then(function(packageRaw) {
                      //add it to the cache for next time
                      romCache.cacheRemotePackage(packageName, packageRaw, { uuid: uuid });
                      return packageRaw; //just return it, who cares if cache fails.
                  })
        }
    });
}


function installFileSystemData(emscripten, romPackageRaw) {
      var byteArray = new Uint8Array(romPackageRaw);
    
      // copy the entire loaded file into a spot in the heap. Files will refer to slices in that. They cannot be freed though.
      var ptr = emscripten['_malloc'](byteArray.length);
      emscripten['HEAPU8'].set(byteArray, ptr);
      return emscripten['HEAPU8'].subarray(ptr, ptr+byteArray.length);
}

function installFile(emscripten, romPackage, start, end, name) {
  var byteArray = romPackage.subarray(start, end);
  var d = D.defer() 
  emscripten['FS_createPreloadedFile'](name, null, byteArray, true, true, function() {
    d.resolve(true)
  }, function() {
    d.reject( 'Preloading file ' + name + ' failed');
  }, false, true); 
  return d.promise;
}

function installFiles(emscripten, romPackage, fsInfo, fileProgress) {
  var fileLoads = [];
  var fileCount = fsInfo.files.length;
  var loaded = 0;

  //create directories
  for (var i = 0; i < fsInfo.directories.length; i++) {
     emscripten['FS_createPath']('/', fsInfo.directories[i], true, true);
  }



  function fileComplete() {
     loaded = loaded + 1;
     fileProgress(loaded, fileCount);
     return true;
  }
  for (var i = 0; i < fileCount; i++) {
    var fileInfo = fsInfo.files[i];
    var filePromise = installFile(emscripten, romPackage, fileInfo.start, fileInfo.end, fileInfo.filename)
                      .then(fileComplete)
    fileLoads.push(filePromise);
  }
  //this is a promise for ALL the files, dont resolve until they are all installed into the filesystem
  return D.all(fileLoads);
}

//takes an emscripten instance and rom url, and returns a promise for the rom installed in that emscriptens
//filesystem
function doLoadRom(emscripten, romUrl, romProgress, fileProgress ) {
  var romJsonUrl = romUrl + ".json";

  var fileSystemInfo;
  var packageName = romUrl;
  
  var InstalledFileSystem = LoadFileSystemJson(romJsonUrl) //load json metadata
  .then(function(fsInfo) {
    //load the rom blob from cache or fetch
    fileSystemInfo = fsInfo;
    return LoadFileSystemData(packageName, fsInfo.package_uuid, romUrl, romProgress);
  })
  .then(function(romPackageRaw) { 
    //install into emscripten heap
    return installFileSystemData(emscripten, romPackageRaw); 
  })
  .then(function(installedRomPackage) {
    //create filesystem entries pointing to blob segements
    return installFiles(emscripten, installedRomPackage, fileSystemInfo, fileProgress);
  })
    //return our promise for a fully created file system
   return InstalledFileSystem;
}
  
return doLoadRom; //only expose this function
})();
 


var MoaiJS = (function() {


function MoaiJS(canvas, total_memory, onTitleChange, onStatusChange, onError, onPrint, onResolutionChange ) {
	this.canvas = canvas;
	this.onTitleChange = onTitleChange;
	this.onStatusChange = onStatusChange;
	this.onError = onError;
    this.onPrint = onPrint;
	this.onResolutionChange = onResolutionChange;
	this.emscripten = null;
	this.total_memory = total_memory;
	this.loadedFileSystems = []; //array of filesystem urls to load

    console.log("MoaiJS Init");
}

MoaiJS.prototype.getEmscripten = function() {
	if (this.emscripten) return this.emscripten;
    var Opts = { //our required emscripten settings
    		canvas: this.canvas,
    		setStatus: this.onStatusChange,
            print: this.onPrint,
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
	this.AKURunString = function(str) {
    Module.cwrap('AKULoadFuncFromString','number',['string'])(str);
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
	this.emscripten.SetOpenWindowFunc(this.OpenWindowFunc.bind(this));
    this.emscripten.SetSaveFunc(this.SaveFile.bind(this));
    return this.emscripten;
}

MoaiJS.prototype.loadFileSystem = function(romUrl) {
   var that = this;
   function makeProgress(prefix) {
		return function(done,total) {
	   		that.onStatusChange(prefix+": "+done+"/"+total);
		}
  };
  console.log("MoaiJS Load Filesystem "+romUrl);
  this.loadedFileSystems.push(window.LoadRom(this.getEmscripten(),romUrl, makeProgress('Loading Data'), makeProgress('Installing File')));
}

MoaiJS.prototype.runFunc = function(func) {
	var that = this;
    D.all(this.loadedFileSystems).then(function(){
	   console.log("MoaiJS Filesystem Loaded");	
  	   that.emscripten.run();   
		that.hostinit();
       func();

  	}).rethrow();
}

MoaiJS.prototype.run = function(mainLua) {
        var that = this;
        this.runFunc(function() {
            that.runhost(mainLua);
        });
}

MoaiJS.prototype.runString = function(luaStr) {
    var that = this;
    this.runFunc(function() {
        that.AKURunString(luaStr);
    });
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

   //if (width > height) {
  //      $(this.canvas).parent().addClass("portrait");
   // }

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
	canvas.addEventListener("keydown", this.keydown.bind(this), false);
	canvas.addEventListener("keyup", this.keyup.bind(this), false);
	canvas.addEventListener("keypress", this.keypress.bind(this), false);

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

MoaiJS.prototype.hostinit = function() {
	console.log("refreshing context");
	this.RefreshContext();
	console.log("setting working directory");
	this.AKUSetWorkingDirectory('/');
	console.log("setting up canvas");
    this.AKURunString('MOAIEnvironment.horizontalResolution = '+$(this.canvas).parent().width());
    this.AKURunString('MOAIEnvironment.verticalResolution = '+$(this.canvas).parent().height());
}

MoaiJS.prototype.runhost = function(mainLua) {
	console.log("runhost called");
	console.log("restoring save state");
	this.restoreDocumentDirectory();

    this.hostinit();

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

function MoaiPlayer(element, skipTemplate) {
	var el = $(element)
	var template = '<div class="moai-window"> \
                         <div class="moai-header"> \
                            <span class="moai-title">MOAI</span><span class="moai-status"></span> \
                             <div style="clear:both"></div> \
                         </div> \
                            <div class="moai-info"></div>    \
                            <div class="moai-canvas-wrapper" style="display: none;"><canvas class="moai-canvas"  tabindex="1"></canvas></div> \
                        <div class="moai-footer"> \
                         <i id="moai-play" class="fa fa-play">&nbsp;</i> \
                         <i id="moai-stop" class="fa fa-stop" style="display:none">&nbsp;</i> \
                          <i id="moai-pause" class="fa fa-pause">&nbsp;</i>    \
                            <div class="moai-attrib"> \
                            Made with Moai \
                            www.getmoai.com \
                            Copyright (c) 2010-2012 Zipline Games, Inc. All Rights Reserved. \
                            </div> \
                        </div> \
                    </div>';

    //inject our original element contents into the fiddle-inf


    var oldcontent = el.html();
    var infoEl;
    if (!skipTemplate) {
    el.html(template);
         infoEl = el.find(".moai-info");
        infoEl.html(oldcontent);
    } else {
         infoEl = el.find(".moai-info");
    }

	var titleEl = el.find(".moai-title").first();
	var statusEl = el.find(".moai-status").first();
	var canvasEl = el.find(".moai-canvas").first();
	var canvasWrapperEl = el.find(".moai-canvas-wrapper").first();

    var pause = el.find("#moai-pause").first();
	//get settings
	this.url = el.attr('data-url');
	this.script = el.attr('data-script') || 'main.lua';
	var ram = parseInt(el.attr('data-ram') || "48" ,10);
	var title = el.attr('data-title') || 'Moai Player';

    var paused= false;

    pause.on("click",function() {
        paused= !paused;
        if (paused) {
            this.pause();
            pause.addClass("paused")
        } else {
            this.unpause();
            pause.removeClass("paused");
        }
    }.bind(this));

	function onTitleChange(title) {
		titleEl.html(title);
	}

	function onStatusChange(status) {
		statusEl.html(status);
	}

	function onResolutionChange(width, height) {
		console.log("width",width,"height",height,"portrait",(height > width));
		el.find('.moai-canvas-wrapper').first().toggleClass("portrait", (height > width));
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

    this.hideInfo = function() {
        infoEl[0].style.display = "none";
        canvasWrapperEl[0].style.display="table-row";
}





    this.initMoai = function() {
        this.moai = new MoaiJS(canvasEl[0], ram * 1024 * 1024, onTitleChange, onStatusChange, onError.bind(this), onPrint.bind(this), onResolutionChange);
    }
}

MoaiPlayer.prototype.isSupported = function() {
    var gl = false;
    var testCanvas = document.createElement("canvas");
    try { gl = testCanvas.getContext("webgl"); }
    catch (x) { gl = null; }

    if (gl === null) {
        try { gl = canvas.getContext("experimental-webgl");}
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
    this.hideInfo();
    if (!this.moai) { this.initMoai(); }
    this.moai.loadFileSystem(this.url);
    this.moai.run(this.script);
}

MoaiPlayer.prototype.loadRom = function(rom) {
    if (!this.moai) { this.initMoai(); }
    this.moai.loadFileSystem(rom);
}

MoaiPlayer.prototype.runString = function(str) {
    this.hideInfo();
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




