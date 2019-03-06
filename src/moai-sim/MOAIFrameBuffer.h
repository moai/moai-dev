// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

class MOAIColor;
class MOAIImage;

//================================================================//
// MOAIFrameBuffer
//================================================================//
/**	@lua	MOAIFrameBuffer
	@text	MOAIFrameBuffer is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAIPartitionViewLayer. To use MOAIFrameBuffer
			pass a table of MOAIRenderable objects to setRenderTable ().
			The table will usually be a stack of MOAIPartitionViewLayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIFrameBuffer :
	public virtual MOAILuaObject,
	public virtual ZLFrameBuffer {
protected:
	
	friend class MOAIGfxMgr;

	bool								mGrabNextFrame;
	MOAILuaMemberRef					mOnFrameFinish;
	MOAILuaSharedPtr < MOAIImage >		mFrameImage;

	//----------------------------------------------------------------//
	static int			_getGrabbedImage			( lua_State* L );
	static int			_grabNextFrame				( lua_State* L );
	static int			_isPendingGrab				( lua_State* L );

	//----------------------------------------------------------------//
	void				ZLGfxListener_OnReadPixels				( const ZLCopyOnWrite& buffer, void* userdata );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBuffer )
	
	//----------------------------------------------------------------//
	void				GrabImage					( MOAIImage* image );
						MOAIFrameBuffer				();
						~MOAIFrameBuffer			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
