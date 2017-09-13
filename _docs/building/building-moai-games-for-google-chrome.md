---
title: "Building Moai Games for Google Chrome"
---

Moai SDK now supports running games with [Native Client](http://en.wikipedia.org/wiki/Google_Native_Client) in Google's Chrome browser, as announced [recently](http://getmoai.com/2011/10/big-news-moai-for-chrome/) on the Moai blog.

Prerequisites
-------------

- Moai SDK 0.9 (Revision 8) or later.

- Chrome 15 or later.

- An FMOD license. More information about licensing is available [here](http://www.fmod.org/index.php/sales). If you do not want sound in your game, you will need to remove the FMOD library dependencies from the build and re-build Moai from source.

Running Native Client as an unpacked extension
----------------------------------------------

For an unpacked extension:

- Create a manifest in the format specified at <http://code.google.com/chrome/extensions/manifest.html> and place it in your extension directory.

- Run Chrome.

- Click on the wrench icon in the upper right corner and go to Tools-\>Extensions.

- Check the "Developer mode" box - the option to "Load upacked extension…" should appear.

- Click "Load upacked extension…" and navigate to the extension directory that contains your .nexe binaries, .nmf and webpage and click "Select".

- Open a new tab and your new extension should appear.

Alternatively you can host the directory containing your extension on a webserver and simply access that from Chrome. In any case, be sure to clear the cache each time you make changes. You can also run Chrome in incognito mode to prevent it from being cached.

Notes
-----

- As of Moai SDK release 0.95, a sample Native Client host is available in hosts/chrome/.

- Direct socket communication to outside domains is not supported (some examples for how to get around this will come in the future - see [Same Origin Policy](http://en.wikipedia.org/wiki/Same_origin_policy/)).

- The Native Client binary will load 'main.lua' in the main extension directory when the extension is run.

- File I/O is a work in progress. You can read and write from the "NaClFileSys" directory, but don't use any additional sub-directories inside. You will also have to set the appropriate permissions to allow local file I/O. The Moai SDK Native Client sample includes a sample manifest.json that enables unlimited storage (there is a way to do this in javascript on your page as well).

For questions, please see this [forum thread](http://getmoai.com/forums/post1176.html).
