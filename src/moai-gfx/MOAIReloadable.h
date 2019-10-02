// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRELOADABLE_H
#define	MOAIRELOADABLE_H

//================================================================//
// MOAIReloadable
//================================================================//
class MOAIReloadable :
	public virtual MOAILuaObject {
protected:

	// for custom loading function
	MOAILuaMemberRef	mReloader;
	bool				mIsLoaded;

	//----------------------------------------------------------------//
	static int			_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	bool				FinishLoading				();

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	virtual void		MOAIReloadable_ClearRecoverable				();
	virtual bool		MOAIReloadable_FinishLoading				();

public:

	//----------------------------------------------------------------//
	bool				ClearReloadable				();
	bool				HasReloader					() const;
						MOAIReloadable				();
						~MOAIReloadable				();
	bool				Reload						();
};

#endif
