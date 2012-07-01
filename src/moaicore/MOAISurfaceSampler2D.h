#ifndef MOAISURFACESAMPLER2D_H
#define MOAISURFACESAMPLER2D_H

#include <moaicore/MOAISurface2D.h>

class MOAIProp;

//================================================================//
// MOAISurfaceBuffer2D
//================================================================//
class MOAISurfaceBuffer2D {
private:

	friend class MOAISurfaceSampler2D;

	static const u32 MAX_SURFACES = 256;

	MOAISurface2D mSurfaces [ MAX_SURFACES ];
	u32 mTop;

public:

	GET ( u32, Top, mTop )

	//----------------------------------------------------------------//
	const MOAISurface2D&	GetSurface					( u32 id ) const;
							MOAISurfaceBuffer2D			();
							~MOAISurfaceBuffer2D		();
};

//================================================================//
// MOAISurfaceSampler2D
//================================================================//
class MOAISurfaceSampler2D {
private:

	MOAISurfaceBuffer2D*	mBuffer;
	
	USRect					mSampleRect; // local space
	MOAIProp*				mSourceProp;
	
	USAffine2D				mWorldToSampler;
	USAffine2D				mTransform;

public:

	//----------------------------------------------------------------//
	void		Init						( MOAISurfaceBuffer2D& buffer, const USRect& sampleRect, const USAffine2D& worldToSampler );
				MOAISurfaceSampler2D		();
				~MOAISurfaceSampler2D		();
	void		PushSurface					( const MOAISurface2D& surface );
	void		SetSourceProp				( MOAIProp* prop );
};

#endif
