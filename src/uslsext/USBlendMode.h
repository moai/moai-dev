// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USBLENDMODE_H
#define USBLENDMODE_H

//================================================================//
// USBlendMode
//================================================================//
class USBlendMode {
public:
	
	enum {
		BLEND_NORMAL,
		BLEND_ADD,
		BLEND_MULTIPLY,
	};
	
	int				mSourceFactor;
	int				mDestFactor;

	//----------------------------------------------------------------//
	void			Bind				();
	static void		GetBlendFactors		( u32 blend, int& srcFactor, int& dstFactor );
	void			SetBlend			( u32 blend );
	void			SetBlend			( int srcFactor, int dstFactor );
					USBlendMode			();
					~USBlendMode		();
	
	//----------------------------------------------------------------//
	inline bool IsSame ( const USBlendMode& blendMode ) {
		return (( this->mSourceFactor == blendMode.mSourceFactor ) && ( this->mDestFactor == blendMode.mDestFactor ));
	}
};

#endif
