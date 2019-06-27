// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRGL_RESOURCECLERKGL_H
#define	MOAIGFXMGRGL_RESOURCECLERKGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>

class MOAIGfxResourceGL;
class MOAIGfxMgrGL_DisplayListClerkGL;
class MOAIGfxMgrGL;

//================================================================//
// MOAIGfxMgrGL_ResourceClerkGL
//================================================================//
class MOAIGfxMgrGL_ResourceClerkGL :
	public virtual MOAIGfxMgrGLComponents {
protected:
	
	typedef ZLLeanList < MOAIGfxResourceGL* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResourceGL* >		mResources;
	
	ZLLeanList < MOAIGfxResourceGL* >		mPendingForLoadList;
	ZLLeanList < MOAIGfxResourceGL* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( MOAIGfxResourceGL& resource );
	void			ProcessDeleters				( ZLGfx& gfx );
	void			ProcessPending				( ZLGfx& gfx, ZLLeanList < MOAIGfxResourceGL* > &list );
	void			RemoveGfxResource			( MOAIGfxResourceGL& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( MOAIGfxResourceGL& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIGfxMgrGL_RenderTreeGL;
	
	friend class MOAIGfxResourceGL;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard						( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources					();
	void			PurgeResources						( u32 age = 0 );
	void			Update								();
					MOAIGfxMgrGL_ResourceClerkGL		();
					~MOAIGfxMgrGL_ResourceClerkGL		();
};

#endif
