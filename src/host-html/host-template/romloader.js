/* Promise lib (c) Jonathan Gotti - licence: https://github.com/malko/d.js/LICENCE.txt @version 0.6.0*/
!function(a){"use strict";function b(a){j(function(){throw a})}function c(b){return this.then(b,a)}function d(b){return this.then(a,b)}function e(b,c){return this.then(function(a){return k(b)?b.apply(null,l(a)?a:[a]):t.onlyFuncs?a:b},c||a)}function f(a){function b(){a()}return this.then(b,b),this}function g(a){return this.then(function(b){return k(a)?a.apply(null,l(b)?b.splice(0,0,void 0)&&b:[void 0,b]):t.onlyFuncs?b:a},function(b){return a(b)})}function h(c){return this.then(a,c?function(a){throw c(a),a}:b)}function i(a,b){var c=o(a);if(1===c.length&&l(c[0])){if(!c[0].length)return t.fulfilled([]);c=c[0]}var d=[],e=t(),f=c.length;if(f)for(var g=(function(a){c[a]=t.promisify(c[a]),c[a].then(function(g){a in d||(d[a]=b?c[a]:g,--f||e.resolve(d))},function(g){a in d||(b?(d[a]=c[a],--f||e.resolve(d)):e.reject(g))})}),h=0,i=f;i>h;h++)g(h);else e.resolve(d);return e.promise}var j,k=function(a){return"function"==typeof a},l=function(a){return Array.isArray?Array.isArray(a):a instanceof Array},m=function(a){return!(!a||!(typeof a).match(/function|object/))},n=function(b){return b===!1||b===a||null===b},o=function(a,b){return[].slice.call(a,b)},p="undefined",q=typeof TypeError===p?Error:TypeError;if(typeof process!==p&&process.nextTick)j=process.nextTick;else if(typeof MessageChannel!==p){var r=new MessageChannel,s=[];r.port1.onmessage=function(){s.length&&s.shift()()},j=function(a){s.push(a),r.port2.postMessage(0)}}else j=function(a){setTimeout(a,0)};var t=function(b){function i(){if(0!==s){var a,b=u,c=0,d=b.length,e=~s?0:1;for(u=[];d>c;c++)(a=b[c][e])&&a(p)}}function l(a){function b(a){return function(b){return c?void 0:(c=!0,a(b))}}var c=!1;if(s)return this;try{var d=m(a)&&a.then;if(k(d)){if(a===v)throw new q("Promise can't resolve itself");return d.call(a,b(l),b(o)),this}}catch(e){return b(o)(e),this}return r(function(){p=a,s=1,i()}),this}function o(a){return s||r(function(){try{throw a}catch(b){p=b}s=-1,i()}),this}var p,r=(a!==b?b:t.alwaysAsync)?j:function(a){a()},s=0,u=[],v={then:function(a,b){var c=t();return u.push([function(b){try{n(a)?c.resolve(b):c.resolve(k(a)?a(b):t.onlyFuncs?b:a)}catch(d){c.reject(d)}},function(a){if((n(b)||!k(b)&&t.onlyFuncs)&&c.reject(a),b)try{c.resolve(k(b)?b(a):b)}catch(d){c.reject(d)}}]),0!==s&&r(i),c.promise},success:c,error:d,otherwise:d,apply:e,spread:e,ensure:f,nodify:g,rethrow:h,isPending:function(){return!(0!==s)},getStatus:function(){return s}};return v.toSource=v.toString=v.valueOf=function(){return p===a?this:p},{promise:v,resolve:l,fulfill:l,reject:o}};t.deferred=t.defer=t,t.nextTick=j,t.alwaysAsync=!0,t.onlyFuncs=!0,t.resolved=t.fulfilled=function(a){return t(!0).resolve(a).promise},t.rejected=function(a){return t(!0).reject(a).promise},t.wait=function(a){var b=t();return setTimeout(b.resolve,a||0),b.promise},t.delay=function(a,b){var c=t();return setTimeout(function(){try{c.resolve(a.apply(null))}catch(b){c.reject(b)}},b||0),c.promise},t.promisify=function(a){return a&&k(a.then)?a:t.resolved(a)},t.all=function(){return i(arguments,!1)},t.resolveAll=function(){return i(arguments,!0)},t.nodeCapsule=function(a,b){return b||(b=a,a=void 0),function(){var c=t(),d=o(arguments);d.push(function(a,b){a?c.reject(a):c.resolve(arguments.length>2?o(arguments,1):b)});try{b.apply(a,d)}catch(e){c.reject(e)}return c.promise}},typeof window!==p&&(window.D=t),typeof module!==p&&module.exports&&(module.exports=t)}();


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
        var d = D.defer();
        try {
          var openRequest = this.indexedDB.open(this.DB_NAME, this.DB_VERSION);
        } catch (e) {
          d.reject(e);
        }
        openRequest.onupgradeneeded = function(event) {
          var db = event.target.result;

          if(db.objectStoreNames.contains(this.PACKAGE_STORE_NAME)) {
            db.deleteObjectStore(this.PACKAGE_STORE_NAME);
          }
          var packages = db.createObjectStore(this.PACKAGE_STORE_NAME);

          if(db.objectStoreNames.contains(this.METADATA_STORE_NAME)) {
            db.deleteObjectStore(this.METADATA_STORE_NAME);
          }
          var metadata = db.createObjectStore(this.METADATA_STORE_NAME);
        };
        openRequest.onsuccess = function(event) {
          var db = event.target.result;
          d.resolve(db);
        };
        openRequest.onerror = function(error) {
          d.reject(error);
        };

        return d;
};

RomCache.prototype.getDb = function() {
  if (!this.openedDatabase) {
    this.openedDatabase = this.openDatabase();
  }
  return this.openedDatabase;
}

RomCache.prototype.checkCachedPackage = function( packageName, uuid) {
   var that = this;
   return this.getDb.then(function(db) {
      return that.dbCheckCachedPackage(db, packageName, uuid)
   });
}

      /* Check if there's a cached package, and if so whether it's the latest available */
RomCache.prototype.dbCheckCachedPackage = function(db, packageName, uuid) {
        var p = D.defer();
        var transaction = db.transaction([this.METADATA_STORE_NAME], this.IDB_RO);
        var metadata = transaction.objectStore(this.METADATA_STORE_NAME);

        var getRequest = metadata.get(packageName);
        getRequest.onsuccess = function(event) {
          var result = event.target.result;
          if (!result) {
            return p.resolve(false);
          } else {
            return p.resolve(uuid=== result.uuid);
          }
        };
        getRequest.onerror = function(error) {
          p.reject(error);
        };
        return p;
};

RomCache.prototype.fetchCachedPackage = function(packageName) {
   var that = this;
   return this.getDb.then(function(db) {
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
        return d;
};

RomCache.prototype.cacheRemotePackage = function(packageName, packageData, packageMeta) {
   var that = this;
   return this.getDb.then(function(db) {
      return that.dCachedRemotePackage(db, packageName, packageData, packageMeta);
   });
};

RomCache.prototype.dbCacheRemotePackage = function(db, packageName, packageData, packageMeta) {
        var d = D.defer();
        var transaction = db.transaction([this.PACKAGE_STORE_NAME, this.METADATA_STORE_NAME], this.IDB_RW);
        var packages = transaction.objectStore(this.PACKAGE_STORE_NAME);
        var metadata = transaction.objectStore(this.METADATA_STORE_NAME);

//UP TO HERE!!!

        var putPackageRequest = packages.put(packageData, packageName);
        putPackageRequest.onsuccess = function(event) {
          var putMetadataRequest = metadata.put(packageMeta, packageName);
          putMetadataRequest.onsuccess = function(event) {
            callback(packageData);
          };
          putMetadataRequest.onerror = function(error) {
            errback(error);
          };
        };
        putPackageRequest.onerror = function(error) {
          errback(error);
        };
      };




/* rom loader */

/**
 	Load Json
 	then Read Rom 
 	then wait for FS
 	then install Files
 */

function LoadFileSystemJson(fsJsonUrl) {
  	var promise = D.defer();
 	var xhr = new XMLHttpRequest();
    xhr.open('GET', fsJsonUrl, true);
    xhr.responseType = "json";
    xhr.onload = function(event) {
        var packageInfo = xhr.response;
        promise.resolve(packageInfo);
    };
    xhr.onerror = function(event) {
    	promise.reject("Failed to download "+fsJsonUrl+":"+xhr.statusText);
    };
    xhr.send(null);
    return promise;
}

function LoadFileSystemData(romPackageUrl, progress) {
	var promise = D.defer();
	var xhr = new XMLHttpRequest();
    xhr.open('GET', romPackageUrl, true);
    xhr.responseType = 'arraybuffer';
    xhr.onload = function(event) {
        var packageData = xhr.response;
        promise.resolve(packageData);
    };
    xhr.onprogress = function(event) {
    	progress(event.loaded, event.total);
    }
    xhr.onerror = function(event) {
    	promise.reject("Failed to download FS data"+romPackageUrl+":"+xhr.statusText);
    };
    xhr.send(null);
    return promise;
}


function installFileSystemData(emscripten, romPackageRaw) {
      var byteArray = new Uint8Array(romPackageRaw);
    
      // copy the entire loaded file into a spot in the heap. Files will refer to slices in that. They cannot be freed though.
      var ptr = emscripten['_malloc'](byteArray.length);
      emscripten['HEAPU8'].set(byteArray, ptr);
      return emscripten['HEAPU8'].subarray(ptr, ptr+byteArray.length);
}

function installFile(emscripten, romPackage, start, end, name) {
  var byteArray = romPackage.subarray(this.start, this.end);
  var promise = D.defer() 
  emscripten['FS_createPreloadedFile'](name, null, byteArray, true, true, function() {
    promise.resolve(true)
  }, function() {
    promise.reject( 'Preloading file ' + name + ' failed');
  }, false, true); 
  return promise;
}

function installFiles(emscripten, romPackage, fsInfo, fileProgress) {
  var fileLoads = [];
  var fileCount = fsInfo.files.length;
  var loaded = 0;
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

function LoadRom(emscripten, romUrl, romProgress, fileProgress ) {
   var romJsonUrl = romUrl + ".json";


   var LoadFileSystem = LoadFileSystemData(romUrl, romProgress)
          .then(function(romPackageRaw) { 
            return installFileSystemData(emscripten,romPackageRaw) 
          });

   var InstalledFileSystem = D.all(
        LoadFileSystem,
        LoadFileSystemJson(romJsonUrl)
      ).spread(function (romPackage, fsInfo) {
        return installFiles(emscripten, romPackage, fsInfo, fileProgress)
      });


   return InstalledFileSystem;
}

  
 
