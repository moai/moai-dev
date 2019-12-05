// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBLENDMODE_H
#define MOAIBLENDMODE_H

#include <moai-gfx/MOAIGfxConsts.h>

// TODO: replace this with ZLColorBlendFunc

//================================================================//
// MOAIBlendMode
//================================================================//
class MOAIBlendMode {
public:
	
	MOAIBlendFunc::Type				mEquation;
	MOAIBlendFactor::Type				mSourceFactor;
	MOAIBlendFactor::Type				mDestFactor;

	//----------------------------------------------------------------//
	void			SetBlend			( MOAIBlendFunc::Type equation, MOAIBlendFactor::Type srcFactor, MOAIBlendFactor::Type dstFactor );
					MOAIBlendMode		();
					MOAIBlendMode		( MOAIBlendFunc::Type equation, MOAIBlendFactor::Type srcFactor, MOAIBlendFactor::Type dstFactor );
					~MOAIBlendMode		();
	
	//----------------------------------------------------------------//
	inline bool IsSame ( const MOAIBlendMode& blendMode ) {
		return (
			( this->mSourceFactor == blendMode.mSourceFactor ) &&
			( this->mDestFactor == blendMode.mDestFactor ) &&
			( this->mEquation == blendMode.mEquation )
		);
	}
};

#endif
