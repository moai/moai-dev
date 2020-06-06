// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRVK_RESOURCECLERKVK_H
#define	MOAIGFXMGRVK_RESOURCECLERKVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>

class MOAIGfxResourceVK;
class MOAIGfxMgrVK_PipelineClerkVK;
class MOAIGfxMgrVK;

//================================================================//
// MOAIGfxMgrVK_ResourceClerkVK
//================================================================//
class MOAIGfxMgrVK_ResourceClerkVK :
	public virtual MOAIGfxMgrVKComponents {
protected:
	
	typedef ZLLeanList < MOAIGfxResourceVK* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResourceVK* >		mResources;
	
	ZLLeanList < MOAIGfxResourceVK* >		mPendingForLoadList;
	ZLLeanList < MOAIGfxResourceVK* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( MOAIGfxResourceVK& resource );
	void			ProcessDeleters				( ZLGfx& gfx );
	void			ProcessPending				( ZLGfx& gfx, ZLLeanList < MOAIGfxResourceVK* > &list );
	void			RemoveGfxResource			( MOAIGfxResourceVK& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( MOAIGfxResourceVK& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	
	friend class MOAIGfxResourceVK;
	
	//----------------------------------------------------------------//
	void			DeleteOrDiscard						( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources					();
	void			PurgeResources						( u32 age = 0 );
	void			Update								();
					MOAIGfxMgrVK_ResourceClerkVK		();
					~MOAIGfxMgrVK_ResourceClerkVK		();
};

#endif
