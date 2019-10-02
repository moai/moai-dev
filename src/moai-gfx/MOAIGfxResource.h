// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

#include <moai-gfx/MOAIReloadable.h>

//================================================================//
// MOAIGfxResource
//================================================================//
class MOAIGfxResource :
	public virtual MOAIReloadable {
public:

	enum PipelineHint {
		LOADER_PIPELINE,
		RENDER_PIPELINE,
	};

protected:

	//----------------------------------------------------------------//
	static int			_destroy					( lua_State* L );
	static int			_scheduleForGPUUpdate		( lua_State* L );

	//----------------------------------------------------------------//
	virtual bool		MOAIGfxResource_DoCPUCreate					() = 0;
	virtual bool		MOAIGfxResource_IsReady						() const = 0;
	virtual bool		MOAIGfxResource_OnCPUCreate					(); // load or initialize any CPU-side resources required to create the GPU-side resource
	virtual void		MOAIGfxResource_OnCPUDestroy				();
	virtual void		MOAIGfxResource_OnCPUPurgeRecoverable		(); // clear any recoverable CPU-side memory used by class
	virtual void		MOAIGfxResource_ScheduleForGPUDestroy		() = 0;
	virtual bool		MOAIGfxResource_ScheduleForGPUUpdate		( PipelineHint hint ) = 0;
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	void				Destroy						();
	bool				DoCPUCreate					();
	bool				IsReady						() const;
						MOAIGfxResource				();
						~MOAIGfxResource			();
	bool				ScheduleForGPUUpdate		( PipelineHint hint = RENDER_PIPELINE );
};

#endif
