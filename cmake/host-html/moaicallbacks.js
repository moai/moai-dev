mergeInto(LibraryManager.library, {
  OpenWindowFunc: function(title,width,height) {
    moaijs.OpenWindowFunc(Module.Pointer_stringify(title),width,height);
  },

  //replace fclose to notify us on file change to help with save game handling
  fclose: function(stream) {
    var fileinfo = FS.streams[stream];
    if(fileinfo && fileinfo.isWrite) {
    	moaijs.SaveFile(FS.absolutePath(fileinfo.path), fileinfo.object.contents);
    };
    _fsync(stream);
    return _close(stream);
  },

  //savegame support
  RestoreFile__deps: ['$FS'],
  RestoreFile: function(path, data) {
    //normalize the path
    var path = FS.absolutePath(path);

    var existing = FS.findObject(path);
  	if (existing) {
  		existing.contents = data;
  	} else {
		  var parts = path.split('/');
		  var name = parts.pop();
  		var dir = parts.join('/') || '/';
  	
  	  FS.createPath('/',dir,true,true);
  	  FS.createDataFile(dir,name,data,true,true);
    }
  }
});
