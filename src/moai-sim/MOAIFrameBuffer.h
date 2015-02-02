// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

class MOAIColor;
class MOAIImage;

//================================================================//
// MOAIClearableView
//================================================================//
class MOAIClearableView :
	public virtual MOAILuaObject {
private:

	u32				mClearFlags;
	u32				mClearColor;
	MOAIColor*		mClearColorNode;

	//----------------------------------------------------------------//
	static int		_setClearColor			( lua_State* L );
	static int		_setClearDepth			( lua_State* L );

	//----------------------------------------------------------------//
	

public:

	GET_SET ( u32, ClearFlags, mClearFlags );

	//----------------------------------------------------------------//
	void			ClearSurface			();
	bool			IsOpaque				() const;
					MOAIClearableView		();
					~MOAIClearableView		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetClearColor			( MOAIColor* color );
};

//================================================================//
// MOAIFrameBuffer
//================================================================//
/**	@lua	MOAIFrameBuffer
	@text	MOAIFrameBuffer is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAILayer. To use MOAIFrameBuffer
			pass a table of MOAIRenderable objects to setRenderTable ().
			The table will usually be a stack of MOAILayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIFrameBuffer :
	public MOAIClearableView {
protected:
	
	u32					mBufferWidth;
	u32					mBufferHeight;
	float				mBufferScale;
	bool				mLandscape;
	
	u32					mGLFrameBufferID;

	bool				mGrabNextFrame;
	MOAILuaMemberRef	mOnFrameFinish;
	MOAILuaSharedPtr < MOAIImage > mFrameImage;

	u32					mRenderCounter;	// increments every render
	u32					mLastDrawCount;
	MOAILuaStrongRef	mRenderTable;

	//----------------------------------------------------------------//
	static int			_grabNextFrame				( lua_State* L );
	static int			_getPerformanceDrawCount    ( lua_State* L );
	static int			_getRenderTable				( lua_State* L );
	static int			_setRenderTable				( lua_State* L );

	//----------------------------------------------------------------//
	void				RenderTable					( MOAILuaState& state, int idx );

public:
	
	friend class MOAIGfxDevice;
	
	DECL_LUA_FACTORY ( MOAIFrameBuffer )
	
	GET			( u32, BufferWidth, mBufferWidth )
	GET			( u32, BufferHeight, mBufferHeight )
	GET_SET		( float, BufferScale, mBufferScale )
	GET_SET		( bool, Landscape, mLandscape )
	GET			( u32, RenderCounter, mRenderCounter )
	
	//----------------------------------------------------------------//
	void				DetectGLFrameBufferID		();
	ZLRect				GetBufferRect				() const;
	void				GrabImage					( MOAIImage* image );
						MOAIFrameBuffer				();
						~MOAIFrameBuffer			();
	void				SetBufferSize				( u32 width, u32 height );
	void				SetGLFrameBufferID			( u32 frameBufferID );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	virtual void		Render						();
	ZLRect				WndRectToDevice				( ZLRect rect ) const;
};

#endif
