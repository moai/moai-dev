// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-sim/MOAIMaterialBase.h>

class MOAILight;
class MOAITextureBase;

//================================================================//
// MOAIMaterialNamedGlobal
//================================================================//
class MOAIMaterialNamedGlobal {
private:

	friend class MOAIMaterial;
	friend class MOAIMaterialMgr;
	
	union {
		MOAILight*			mLight;
		MOAITextureBase*	mTexture;
		void*				mPtr;
	};
	
	u32					mName;

public:

	//----------------------------------------------------------------//
	MOAIMaterialNamedGlobal () :
		mPtr ( 0 ),
		mName ( 0 ) {
	}
};

//================================================================//
// MOAILightSet
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public MOAIMaterialBase {
private:

	friend class MOAIMaterialMgr;

	ZLLeanArray < MOAIMaterialNamedGlobal >		mLights;
	ZLLeanArray < MOAIMaterialNamedGlobal >		mTextures;
	
	//----------------------------------------------------------------//
	MOAIMaterialNamedGlobal&	AffirmNamedGlobal		( ZLLeanArray < MOAIMaterialNamedGlobal >& array, u32 name );
	MOAIMaterialNamedGlobal*	FindNamedGlobal			( ZLLeanArray < MOAIMaterialNamedGlobal >& array, u32 name );

public:

	//----------------------------------------------------------------//
	MOAILight*			GetNamedLight				( u32 name );
	MOAITextureBase*	GetNamedTexture				( u32 name );
						MOAIMaterial				();
	virtual				~MOAIMaterial				();
	void				ReserveLights				( u32 n );
	void				ReserveTextures				( u32 n );
	void				SetNamedLight				( u32 name, MOAILight* light );
	void				SetNamedTexture				( u32 name, MOAITextureBase* texture );
};

#endif
