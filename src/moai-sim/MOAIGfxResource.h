// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

//================================================================//
// MOAIGfxState
//================================================================//
/**	@lua	MOAIGfxState
	@text	Abstract base class for objects that represent changes
			to graphics state.
*/
class MOAIGfxState :
	public virtual MOAILuaObject {
private:

	friend class MOAIGfxDevice;

	//----------------------------------------------------------------//
	// this is for binding via the gfx device's cache; we need this since
	// Bind () is supposed to be ignorant of
	virtual bool		LoadGfxState			() { return false; }

public:

	//----------------------------------------------------------------//
						MOAIGfxState			();
	virtual				~MOAIGfxState			();
};

//================================================================//
// MOAIGfxResource
//================================================================//
/**	@lua	MOAIGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResource :
	public MOAIGfxState {
private:

	enum {
		STATE_READY,
		STATE_ERROR,
		STATE_PRECREATE,
		STATE_PRELOAD,
	};

	u32				mState;
	u32				mLastRenderCount;
	
	MOAILuaMemberRef	mOnRenew;

	ZLLeanLink < MOAIGfxResource* > mLink;

	//----------------------------------------------------------------//
	static int		_getAge						( lua_State* L );
	static int		_softRelease				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	bool			Affirm						();
	bool			HasLoadScript				();
	virtual bool	IsRenewable					() = 0; // return 'true' if resource has sufficient information to create GPU-side resource - MAIN THREAD
	virtual void	OnBind						() = 0; // select GPU-side resource on device for use - GRAPHICS THREAD
	virtual void	OnClear						() = 0; // clear any CPU-side resources used by class - MAIN THREAD
	virtual void	OnCreate					() = 0; // create GPU-side resource - GRAPHICS THREAD
	virtual void	OnDestroy					() = 0; // destroy GPU-side resource - MAIN THREAD
	virtual void	OnInvalidate				() = 0; // clear any handles or references to GPU-side resource - MAIN THREAD
	virtual void	OnLoad						() = 0; // load or initialize any CPU-side resources required to create device resource - MAIN THREAD
	virtual void	OnUnbind					() = 0; // unbind GPU-side resource - GRAPHICS THREAD

public:

	friend class MOAIGfxDevice;

	GET ( u32, State, mState );

	//----------------------------------------------------------------//
	bool			Bind						();
	void			Clear						();
	void			Destroy						();
	void			Invalidate					();
	virtual bool	IsValid						() = 0; // return 'true' if the handle to the GPU-side resource is valid (or initialized) - GRAPHICS THREAD
	void			Load						();
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	bool			SoftRelease					( u32 age );
	void			Unbind						();
};

#endif
