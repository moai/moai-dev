// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBLENDMODE_H
#define MOAIBLENDMODE_H

//================================================================//
// MOAIBlendMode
//================================================================//
class MOAIBlendMode {
public:
	
	enum {
		BLEND_NORMAL,
		BLEND_ADD,
		BLEND_MULTIPLY,
	};
	
	int				mEquation;
	int				mSourceFactor;
	int				mDestFactor;

	//----------------------------------------------------------------//
	void			Init				( MOAILuaState& state, int idx );
	void			SetBlend			( u32 blend );
	void			SetBlend			( int equation, int srcFactor, int dstFactor );
					MOAIBlendMode		();
					MOAIBlendMode		( int equation, int srcFactor, int dstFactor );
					~MOAIBlendMode		();
	int				Push				( MOAILuaState& state ) const;
	
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
