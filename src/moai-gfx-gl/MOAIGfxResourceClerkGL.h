// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXRESOURCECLERKGL_H
#define	MOAIGFXRESOURCECLERKGL_H

#include <moai-gfx-gl/MOAIAbstractGfxMgrComponentsGL.h>

class MOAIAbstractGfxResourceGL;
class MOAIGfxPipelineClerkGL;
class MOAIGfxMgrGL;

//================================================================//
// MOAIGfxResourceClerkGL
//================================================================//
class MOAIGfxResourceClerkGL :
	virtual public MOAIAbstractGfxMgrComponentsGL {
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
	friend class MOAIRenderMgrGL;
	
	friend class MOAIAbstractGfxResourceGL;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard				( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources			();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
					MOAIGfxResourceClerkGL		();
					~MOAIGfxResourceClerkGL		();
};

#endif
