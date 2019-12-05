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
	
	MOAIBlendFuncEnum::_				mEquation;
	MOAIBlendFactorEnum::_				mSourceFactor;
	MOAIBlendFactorEnum::_				mDestFactor;

	//----------------------------------------------------------------//
	void			SetBlend			( MOAIBlendFuncEnum::_ equation, MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor );
					MOAIBlendMode		();
					MOAIBlendMode		( MOAIBlendFuncEnum::_ equation, MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor );
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
