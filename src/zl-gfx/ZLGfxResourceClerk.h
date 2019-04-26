// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXRESOURCECLERK_H
#define	ZLGFXRESOURCECLERK_H

#include <zl-gfx/ZLAbstractGfxMgr.h>
#include <zl-gfx/ZLGfxHandle.h>

class ZLAbstractGfxResource;
class ZLGfx;
class ZLGfxPipelineClerk;
class ZLGfxMgrGL;

//================================================================//
// ZLGfxResourceClerk
//================================================================//
class ZLGfxResourceClerk :
	virtual public ZLAbstractGfxMgr {
protected:
	
	typedef ZLLeanList < ZLAbstractGfxResource* >::Iterator ResourceIt;
	ZLLeanList < ZLAbstractGfxResource* >		mResources;
	
	ZLLeanList < ZLAbstractGfxResource* >		mPendingForLoadList;
	ZLLeanList < ZLAbstractGfxResource* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( ZLAbstractGfxResource& resource );
	void			ProcessDeleters				( ZLGfx& gfx );
	void			ProcessPending				( ZLGfx& gfx, ZLLeanList < ZLAbstractGfxResource* > &list );
	void			RemoveGfxResource			( ZLAbstractGfxResource& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( ZLAbstractGfxResource& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIRenderMgr;
	
	friend class ZLAbstractGfxResource;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard				( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources			();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
					ZLGfxResourceClerk			();
					~ZLGfxResourceClerk			();
};

#endif
