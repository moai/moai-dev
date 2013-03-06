// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	void			Bind				();
	static void		GetBlendFactors		( u32 blend, int& srcFactor, int& dstFactor );
	void			SetBlend			( u32 blend );
	void			SetBlend			( int srcFactor, int dstFactor );
	void			SetBlendEquation	( int equation );
					MOAIBlendMode		();
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
