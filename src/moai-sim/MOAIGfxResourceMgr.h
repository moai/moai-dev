// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEMGR_H
#define	MOAIGFXRESOURCEMGR_H

class MOAIGfxResource;

//================================================================//
// MOAIGfxResourceMgr
//================================================================//
class MOAIGfxResourceMgr {
private:
	
	typedef ZLLeanList < MOAIGfxResource* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResource* >		mResources;
	
	ZLLeanList < MOAIGfxResource* >		mPendingForLoadList;
	ZLLeanList < MOAIGfxResource* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle*, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource		( MOAIGfxResource& resource );
	void			ProcessDeleters			();
	void			ProcessPending			( ZLLeanList < MOAIGfxResource* > &list );
	void			RemoveGfxResource		( MOAIGfxResource& resource );
	void			RenewResources			();
	void			ScheduleGPUAffirm		( MOAIGfxResource& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIGfxResource;
	friend class MOAIRenderMgr;
	
	//----------------------------------------------------------------//
					MOAIGfxResourceMgr		();
					~MOAIGfxResourceMgr		();
	void			PurgeResources			( u32 age = 0 );
	void			PushDeleter				( ZLGfxHandle* handle );
	void			Update					();
};

#endif
