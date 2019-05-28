// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXRESOURCECLERKGL_H
#define	ZLGFXRESOURCECLERKGL_H

#include <zl-gfx/ZLAbstractGfxMgrGL.h>
#include <zl-gfx/ZLGfxHandleGL.h>

class ZLAbstractGfxResourceGL;
class ZLGfx;
class ZLGfxPipelineClerkGL;
class ZLGfxMgrGL;

//================================================================//
// ZLGfxResourceClerkGL
//================================================================//
class ZLGfxResourceClerkGL :
	virtual public ZLAbstractGfxMgrGL {
protected:
	
	typedef ZLLeanList < ZLAbstractGfxResourceGL* >::Iterator ResourceIt;
	ZLLeanList < ZLAbstractGfxResourceGL* >		mResources;
	
	ZLLeanList < ZLAbstractGfxResourceGL* >		mPendingForLoadList;
	ZLLeanList < ZLAbstractGfxResourceGL* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandleGL, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( ZLAbstractGfxResourceGL& resource );
	void			ProcessDeleters				( ZLGfx& gfx );
	void			ProcessPending				( ZLGfx& gfx, ZLLeanList < ZLAbstractGfxResourceGL* > &list );
	void			RemoveGfxResource			( ZLAbstractGfxResourceGL& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( ZLAbstractGfxResourceGL& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIRenderMgr;
	
	friend class ZLAbstractGfxResourceGL;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard				( const ZLGfxHandleGL& handle, bool shouldDelete );
	void			DiscardResources			();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
					ZLGfxResourceClerkGL		();
					~ZLGfxResourceClerkGL		();
};

#endif
