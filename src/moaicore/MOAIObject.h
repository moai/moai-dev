// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIOBJECT_H
#define MOAIOBJECT_H

#include <moaicore/MOAIRtti.h>

class MOAICanary;

//================================================================//
// MOAIObject
//================================================================//
class MOAIObject :
	public RTTIBase {
private:

	MOAICanary*		mCanary;

	//----------------------------------------------------------------//
	virtual void	OnRelease			( u32 refCount );
	virtual void	OnRetain			( u32 refCount );
			
public:

	friend class MOAICanary;

	//----------------------------------------------------------------//
	MOAICanary*		AffirmCanary		();
	u32				GetRefCount			();
	bool			IsInScope			();
	void			Release				();
	void			Retain				();
					MOAIObject			();
	virtual			~MOAIObject			();
};

#endif
