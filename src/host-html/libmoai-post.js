    Module.OpenWindowCallback=  null;
    Module.SaveCallback = null;
    Module['SetOpenWindowFunc'] = function(openwindowfunc) {
      Module.OpenWindowCallback = openwindowfunc;
    }

    Module['SetSaveFunc'] = function(savefunc) {
      Module.SaveCallback = savefunc;
    }

    Module['FS_mount'] = FS.mount;
    Module['FS_unmount'] = FS.unmount;
    Module['FS_mkdir'] = FS.mkdir;
    Module['FS_filesystems'] = FS.filesystems;
    Module['FS_chdir'] = FS.chdir;
    Module['FS_root'] = FS.root;
    Module['FS_lookupPath'] = FS.lookupPath;
	return Module;	
}