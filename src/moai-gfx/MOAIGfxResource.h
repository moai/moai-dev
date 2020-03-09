// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

//================================================================//
// MOAIGfxResource
//================================================================//
class MOAIGfxResource :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	// for custom loading function
	MOAILuaMemberRef	mReloader;

	//----------------------------------------------------------------//
	static int			_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	bool				FinishLoading				();

	//----------------------------------------------------------------//
	virtual void		MOAIGfxResource_ClearReloadable				();
	virtual bool		MOAIGfxResource_FinishLoading				();
	virtual bool		MOAIGfxResource_IsReadyForUse				() const;
	virtual void		MOAIGfxResource_ScheduleForGPUDestroy		();
	virtual bool		MOAIGfxResource_ScheduleForGPUUpdate		();
	void				MOAILuaObject_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	bool				ClearReloadable				();
	bool				IsReadyForUse				() const;
	bool				HasReloader					() const;
						MOAIGfxResource				();
						~MOAIGfxResource			();
	bool				Reload						();
	void				ScheduleForGPUDestroy		();
	bool				ScheduleForGPUUpdate		();
};

#endif
