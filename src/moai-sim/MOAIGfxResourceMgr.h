// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEMGR_H
#define	MOAIGFXRESOURCEMGR_H

//================================================================//
// MOAIGfxDeleter
//================================================================//
class MOAIGfxDeleter {
public:

	enum {
		DELETE_BUFFER,
		DELETE_FRAMEBUFFER,
		DELETE_PROGRAM,
		DELETE_SHADER,
		DELETE_TEXTURE,
		DELETE_RENDERBUFFER,
	};

	u32		mResourceID;
	u32		mType;

	//----------------------------------------------------------------//
	void		Delete			();
};

//================================================================//
// MOAIGfxResourceMgr
//================================================================//
class MOAIGfxResourceMgr :
	public MOAIGlobalClass < MOAIGfxResourceMgr > {
private:
	
	typedef ZLLeanList < MOAIGfxResource* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResource* > mResources;
	ZLLeanList < MOAIGfxResource* > mPending;
	
	ZLLeanStack < MOAIGfxDeleter, 32 > mDeleterStack;

	//----------------------------------------------------------------//
	void			InsertGfxResource		( MOAIGfxResource& resource );
	void			RemoveGfxResource		( MOAIGfxResource& resource );
	void			RenewResources			();
	void			ScheduleGPUAffirm		( MOAIGfxResource& resource );
	void			Update					();
	
public:
	
	friend class MOAIGfxDevice;
	friend class MOAIGfxResource;
	friend class MOAIRenderMgr;
	
	//----------------------------------------------------------------//
					MOAIGfxResourceMgr		();
					~MOAIGfxResourceMgr		();
	void			PurgeResources			( u32 age = 0 );
	void			PushDeleter				( u32 type, u32 id );
};

#endif
