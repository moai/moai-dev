mergeInto(LibraryManager.library, {
  OpenWindowFunc: function(title,width,height) {
    moaijs.OpenWindowFunc(Module.Pointer_stringify(title),width,height);
  },

  //replace fclose to notify us on file change to help with save game handling
  fclose: function(stream) {
    var fileinfo = FS.streams[stream];
    if(fileinfo && fileinfo.isWrite) {
    	moaijs.OnFileModified(fileinfo);
    };
    _fsync(stream);
    return _close(stream);
  },

  RestoreFile:function(path, data) {
  	/*
    UpdateSave()

  data = JSON.parse(filedata)
	path=absolutepath(path)
	existing = FS.findObject(path)
	if !exiting
		parts = path.split('/')
		name = parts.pop()
		dir = parts.join('/')
		createPath('/',dir,true,true)
		createDataFile(dir,name,data,true,true)
        else
		existing.contents = data
	end
end
    */
    
    var existing = FS.findObject(path);
  	if (existing) {
  		existing.contents = data;
  	} else {
  		var info = fs.analyzePath(path);
  		var parent= info.parentPath;
  		if (!info.parentExists) {
  			parent=FS.createPath("/",info,parentPath,true,true);
  		}


  		TODO more stuff.
  	}
  }
});
