// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXRESOURCECLERK_H
#define	ZLGFXRESOURCECLERK_H

class ZLAbstractGfxResource;
class ZLGfxResource;

//================================================================//
// ZLGfxResourceClerk
//================================================================//
class ZLGfxResourceClerk {
private:
	
	typedef ZLLeanList < ZLAbstractGfxResource* >::Iterator ResourceIt;
	ZLLeanList < ZLAbstractGfxResource* >		mResources;
	
	ZLLeanList < ZLAbstractGfxResource* >		mPendingForLoadList;
	ZLLeanList < ZLAbstractGfxResource* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource			( ZLAbstractGfxResource& resource );
	void			ProcessDeleters				();
	void			ProcessPending				( ZLLeanList < ZLAbstractGfxResource* > &list );
	void			RemoveGfxResource			( ZLAbstractGfxResource& resource );
	void			RenewResources				();
	void			ScheduleGPUAffirm			( ZLAbstractGfxResource& resource, u32 listID );
	
public:
	
	friend class MOAIGfxMgr;
	friend class MOAIRenderMgr;
	
	friend class ZLAbstractGfxResource;
	
	//----------------------------------------------------------------//
	static void		DeleteOrDiscard				( const ZLGfxHandle& handle, bool shouldDelete );
	void			DiscardResources			();
	void			PurgeResources				( u32 age = 0 );
	void			Update						();
					ZLGfxResourceClerk			();
					~ZLGfxResourceClerk			();
};

#endif
