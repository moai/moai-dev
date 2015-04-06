---
title: "Breaking Changes in Moai SDK 1.0"
---

This is an incomplete list of breaking changes, deprecated methods and new implementations. We hope to complete it soon.

moai.lua
--------

There is a backwards compatibility wrapper located in src/lua-headers/moai.lua. It is converted to a C++ header and loaded into the Moai runtime as bytecode by the host application using AKURunBytecode.

MOAIAnim, MOAIAnimCurve
-----------------------

When applied as a delta to an attribute by MOAIAnim the attribute will 'sawtooth' unless set to use the MOAIAnimCurve.APPEND wrapping mode and the anim is set to use the MOAITimer.CONTINUE loop mode.

MOAIBox2D (all classes)
-----------------------

The unitsToMeters is now more consistently applied across the API. Applications using old versions of the API will need to adjust their physics coefficients.

MOAIEaseDriver
--------------

MOAIEaseDriver now inherits from MOAITimer and, as such, should be able to use timer looping modes. MOAIEaseDriver setLength () has been replaced by setSpan ().

MOAIEnvironment
---------------

The MOAIEnvironment accessors have been replaced with key/value pairs. MOAIEnvironment.getScreenSize () has been replaced by MOAIEnvironment.screenWidth and MOAIEnvironment.screenHeight. MOAIEnvironment.setListener () has been temporarily disabled. It will be restored in the next release. Uninitialized keys will now be 'nil' instead of 'UNKNOWN.'

MOAIFont
--------

MOAIFont getScale () is no longer supported as fonts may now contain multiple sizes of glyph. If a font has been initialized with any size then it is not necessary to call MOAITextBox setTextSize (); the font's default size will be used.

MOAILayer, MOAILayer2D
----------------------

MOAILayer setCamera () no longer accepts a transform. Use a MOAICamera or MOAICamera2D. It was pointed out that moai.lua does not initialize MOAILayer2D to use a 2D transform interface. This will be corrected in the next release.

MOAIPartition
-------------

The term 'layer' was changed to 'level' to avoid confusion with MOAILayer. So reserveLayers () and setLayer () have become reserveLevels () and setLevel ().

MOAIRenderMgr
-------------

MOAIRenderMgr implements a new way of defining a layer stack for rendering. Simply pass in a Lua table of layers, indexed numerically starting from 1. The layers will be rendered in order until a 'nil' is encountered. It is also OK to nest tables; they will be rendered recursively.

MOAISim
-------

All render management methods have been moved to MOAIRenderMgr. moai.lua provides an extension to MOAISim that restores them; this will be removed in a future release.

MOAITextBox
-----------

MOAITextBox setFont () and setTextSize () are now implemented in moai.lua as wrappers to set the font's default style. If no default style exists, calling these methods will create one.\
setStringColor() has been removed and is replaced with setHighlight() or in-string tags -- see **samples/fonts/font-colors**.
