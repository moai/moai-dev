var LibraryWebUntz = {
  $UNTZ__deps: ['$FS'],
  $UNTZ: {
    howls: [],
    state: []
  },

  untzsound_createfromfile: function(file,loadIntoMemory) {
      
      var slot = UNTZ.howls.length;
      var filestr = Module.Pointer_stringify(file);
      filestr = FS.absolutePath(filestr, FS.currentPath); //normalise and get full path based on root folder.
    
      //attempt precache from embedded assets
      var precache = null;
      if (loadIntoMemory) {
        var fileobj = FS.findObject(filestr);
        if (fileobj) {
           var arraybuf = new Uint8Array(fileobj.contents);
           precache = arraybuf.buffer;
        }
      }


      UNTZ.howls[slot] = new Howl({
            urls:[filestr],
            buffer: !loadIntoMemory,
            precache: precache,
            onpause: function() {
              UNTZ.state[slot] = "paused";
            },
            onplay: function() {
              UNTZ.state[slot] = "playing";
            },
            onend: function() {
              if (!UNTZ.howls[slot].loop()) {
                UNTZ.state[slot] = "finished";
              }
            }
      });
      return slot; 
  },
  untzsound_destroy: function(handle) { 
      UNTZ.howls[handle] = undefined;
  },
  untzsound_setVolume: function(handle, volume) { 
      UNTZ.howls[handle].volume(volume);
  },
  untzsound_getVolume: function(handle) {
      return UNTZ.howls[handle].volume();
  },
  untzsound_setLooping: function(handle, loop) {
      UNTZ.howls[handle].loop(Boolean(loop));
  },
  untzsound_isLooping: function(handle) { 
      return UNTZ.howls[handle].loop();
  },
  untzsound_getLength: function(handle) {
      return UNTZ.howls[handle]._duration;
  },
  untzsound_setLoopPoints: function(handle,startTime,endTime) {
      var sound = UNTZ.howls[handle];
      sound.sprite({'_default':[startTime*1000,endTime*1000]});
  },
  untzsound_getLoopStart: function(handle) { 
      var sprite =  UNTZ.howls[handle].sprite();
      return sprite[0]/1000;
  },
  untzsound_getLoopEnd: function(handle) { 
      var sprite =  UNTZ.howls[handle].sprite();
      return sprite[1]/1000; 
  },
  untzsound_setPosition: function(handle, seconds) {
     UNTZ.howls[handle].pos(seconds);
  },
  untzsound_getPosition:function(handle) {
     return UNTZ.howls[handle].pos();
  },
  untzsound_play:function(handle) { 
     UNTZ.howls[handle].play();
  },
  untzsound_pause:function(handle) {
     UNTZ.howls[handle].pause();
  },
  untzsound_stop:function(handle) {
     UNTZ.state[handle] = "stopped";
     UNTZ.howls[handle].stop();
  },
  untzsound_isPlaying:function(handle) { 
     return UNTZ.state[handle] == "playing";
  },
  untzsound_isPaused:function(handle) { 
     return UNTZ.state[handle] == "paused";
  },
  untzsound_getNumChannels:function(handle) {
     return 2; 
  },
  untzsound_getSampleRate:function(handle) {
     return 44100;
  },
  untz_setVolume:function(volume) { Howler.volume(volume); },
  untz_getVolume:function() { return Howler.volume(); },
  untz_suspend:function() { Howler.mute() },
  untz_resume:function() { Howler.unmute() }


};

autoAddDeps(LibraryWebUntz, '$UNTZ');
mergeInto(LibraryManager.library, LibraryWebUntz);


  