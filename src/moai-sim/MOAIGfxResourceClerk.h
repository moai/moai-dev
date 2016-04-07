// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCECLERK_H
#define	MOAIGFXRESOURCECLERK_H

class MOAIGfxResource;

//================================================================//
// MOAIGfxResourceClerk
//================================================================//
class MOAIGfxResourceClerk {
private:
	
	typedef ZLLeanList < MOAIGfxResource* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResource* >		mResources;
	
	ZLLeanList < MOAIGfxResource* >		mPendingForLoadList;
	ZLLeanList < MOAIGfxResource* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle*, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( MOAIGfxResource& resource );
	void			ProcessDeleters				();
	void			ProcessPending				( ZLLeanList < MOAIGfxResource* > &list );
	void			RemoveGfxResource			( MOAIGfxResource& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( MOAIGfxResource& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIGfxResource;
	friend class MOAIRenderMgr;
	
	//----------------------------------------------------------------//
	static void		DeleteOrDiscardHandle		( ZLGfxHandle*& handle, bool shouldDelete );
	void			DiscardResources			();
					MOAIGfxResourceClerk		();
					~MOAIGfxResourceClerk		();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
};

#endif
