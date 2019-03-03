// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBLENDMODE_H
#define ZLBLENDMODE_H

//================================================================//
// ZLBlendMode
//================================================================//
class ZLBlendMode {
public:
	
	int				mEquation;
	int				mSourceFactor;
	int				mDestFactor;

	//----------------------------------------------------------------//
	void			Init				( MOAILuaState& state, int idx );
	void			SetBlend			( int equation, int srcFactor, int dstFactor );
	int				Push				( MOAILuaState& state ) const;
					ZLBlendMode			();
					ZLBlendMode			( int equation, int srcFactor, int dstFactor );
					~ZLBlendMode		();
	
	//----------------------------------------------------------------//
	inline bool IsSame ( const ZLBlendMode& blendMode ) {
		return (
			( this->mSourceFactor == blendMode.mSourceFactor ) &&
			( this->mDestFactor == blendMode.mDestFactor ) &&
			( this->mEquation == blendMode.mEquation )
		);
	}
};

#endif
