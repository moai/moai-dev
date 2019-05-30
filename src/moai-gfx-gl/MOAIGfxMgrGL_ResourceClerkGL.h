// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRGL_RESOURCECLERKGL_H
#define	MOAIGFXMGRGL_RESOURCECLERKGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>

class MOAIAbstractGfxResourceGL;
class MOAIGfxMgrGL_PipelineClerkGL;
class MOAIGfxMgrGL;

//================================================================//
// MOAIGfxMgrGL_ResourceClerkGL
//================================================================//
class MOAIGfxMgrGL_ResourceClerkGL :
	virtual public MOAIGfxMgrGLComponents {
protected:
	
	typedef ZLLeanList < MOAIAbstractGfxResourceGL* >::Iterator ResourceIt;
	ZLLeanList < MOAIAbstractGfxResourceGL* >		mResources;
	
	ZLLeanList < MOAIAbstractGfxResourceGL* >		mPendingForLoadList;
	ZLLeanList < MOAIAbstractGfxResourceGL* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( MOAIAbstractGfxResourceGL& resource );
	void			ProcessDeleters				( ZLGfx& gfx );
	void			ProcessPending				( ZLGfx& gfx, ZLLeanList < MOAIAbstractGfxResourceGL* > &list );
	void			RemoveGfxResource			( MOAIAbstractGfxResourceGL& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( MOAIAbstractGfxResourceGL& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIGfxMgrGL_RenderTreeGL;
	
	friend class MOAIAbstractGfxResourceGL;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard				( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources			();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
					MOAIGfxMgrGL_ResourceClerkGL		();
					~MOAIGfxMgrGL_ResourceClerkGL		();
};

#endif
