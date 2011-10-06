// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

//================================================================//
// MOAIGfxResource
//================================================================//
class MOAIGfxResource :
	public virtual USLuaObject {
private:

	enum {
		STATE_CLEAR,
		STATE_ERROR,
		STATE_RENEW,
		STATE_WAIT_RENEW,
	};

	u32				mState;
	u32				mLastRenderCount;
	
	USLuaLocal		mOnRenew;

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
	bool			Bind						();
	void			Clear						();
	virtual bool	IsValid						() = 0;
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );
	void			ReleaseGfxResource			();
	void			RenewGfxResource			();
	bool			SoftReleaseGfxResource		( u32 age );
};

#endif
