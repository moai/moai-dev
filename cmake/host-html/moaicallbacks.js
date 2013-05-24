mergeInto(LibraryManager.library, {
  OpenWindowFunc: function(title,width,height) {
    moaijs.OpenWindowFunc(Module.Pointer_stringify(title),width,height);
  }
});
