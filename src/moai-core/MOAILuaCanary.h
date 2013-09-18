// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUACANARY_H
#define MOAILUACANARY_H

class MOAILuaObject;

//================================================================//
// MOAILuaCanary
//================================================================//
class MOAILuaCanary {
private:

	friend class MOAILuaObject;

	u32			mRefCount;
	bool		mIsValid;

public:
	
	//----------------------------------------------------------------//
	bool		IsValid				();
				MOAILuaCanary		();
				~MOAILuaCanary		();
	void		Release				();
	void		Retain				();				
};

#endif
