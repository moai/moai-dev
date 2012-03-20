// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIGfxState
//================================================================//
/**	@name	MOAIGfxState
	@text	Abstract base class for objects that represent changes
			to graphics state.
*/
class MOAIGfxState :
	public virtual MOAILuaObject {
public:

	//----------------------------------------------------------------//
	virtual bool		LoadGfxState			() { return false; }
						MOAIGfxState			();
	virtual				~MOAIGfxState			();
};

//================================================================//
// MOAIGfxResource
//================================================================//
/**	@name	MOAIGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResource :
	public MOAIGfxState {
private:

	enum {
		STATE_CLEAR,
		STATE_ERROR,
		STATE_RENEW,
		STATE_WAIT_RENEW,
	};

	u32				mState;
	u32				mLastRenderCount;
	
	MOAILuaLocal	mOnRenew;

	USLeanLink < MOAIGfxResource* > mLink;

	//----------------------------------------------------------------//
	static int		_clear						( lua_State* L );
	static int		_getAge						( lua_State* L );
	static int		_preload					( lua_State* L );
	static int		_setRenewCallback			( lua_State* L );
	static int		_softRelease				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	bool			Affirm						();
	bool			Bind						();
	bool			HasLoadScript				();
	virtual bool	IsRenewable					() = 0;
	virtual void	OnBind						() = 0;
	virtual void	OnClear						() = 0;
	virtual void	OnLoad						() = 0;
	virtual void	OnRenew						() = 0;
	virtual void	OnUnload					() = 0;
	void			SetError					();

public:

	friend class MOAIGfxDevice;

	//----------------------------------------------------------------//
	void			Clear						();
	virtual bool	IsValid						() = 0;
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			ReleaseGfxResource			();
	void			RenewGfxResource			();
	void			ResetGfxResource			();
	bool			SoftReleaseGfxResource		( u32 age );
};

#endif
