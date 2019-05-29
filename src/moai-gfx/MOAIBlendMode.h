// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBLENDMODE_H
#define MOAIBLENDMODE_H

// TODO: replace this with ZLColorBlendFunc

//================================================================//
// MOAIBlendMode
//================================================================//
class MOAIBlendMode {
public:
	
	int				mEquation;
	int				mSourceFactor;
	int				mDestFactor;

	//----------------------------------------------------------------//
//	void			Init				( MOAILuaState& state, int idx );
	void			SetBlend			( int equation, int srcFactor, int dstFactor );
//	int				Push				( MOAILuaState& state ) const;
					MOAIBlendMode		();
					MOAIBlendMode		( int equation, int srcFactor, int dstFactor );
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
