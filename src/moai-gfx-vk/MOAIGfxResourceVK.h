// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEVK_H
#define	MOAIGFXRESOURCEVK_H

class MOAIGfxMgrVK;

//================================================================//
// MOAIGfxResourceVK
//================================================================//
/**	@lua	MOAIGfxResourceVK
	@text	Base class for graphics resources owned by OpenVK. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResourceVK :
	public virtual MOAIGfxResource {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	virtual bool		MOAIGfxResource_IsReadyForUse				() const;
	virtual void		MOAIGfxResource_ScheduleForGPUDestroy		();
	virtual bool		MOAIGfxResource_ScheduleForGPUUpdate		();

public:

	//----------------------------------------------------------------//
						MOAIGfxResourceVK				();
	virtual				~MOAIGfxResourceVK				();
};

#endif
