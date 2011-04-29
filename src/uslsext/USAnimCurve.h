// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USANIMCURVE_H
#define USANIMCURVE_H

//================================================================//
// USAnimKey
//================================================================//
class USAnimKey {
public:

	float		mTime;
	float		mValue;
	u32			mMode;
	float		mWeight;
};

//================================================================//
// USAnimCurve
//================================================================//
class USAnimCurve :
	public USLeanArray < USAnimKey > {
public:

	//----------------------------------------------------------------//
	u32				FindKeyID			( float time );
	bool			GetBoolValue		( float time );
	float			GetFloatDelta		( float t0, float t1 );
	float			GetFloatValue		( float time );
	u32				GetIndexValue		( float time );
	int				GetIntValue			( float time );
	float			GetLength			();
	void			SetKey				( u32 id, float time, float value, u32 mode, float weight = 1.0f );
					USAnimCurve			();
					~USAnimCurve		();
};

#endif
