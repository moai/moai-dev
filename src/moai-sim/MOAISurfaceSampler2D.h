#ifndef MOAISURFACESAMPLER2D_H
#define MOAISURFACESAMPLER2D_H

class MOAITransform;

//================================================================//
// MOAISurface2D
//================================================================//
class MOAISurface2D :
	public USSurface2D {
public:

	MOAITransform*	mSourcePrim;
};

//================================================================//
// MOAISurfaceSampler2D
//================================================================//
class MOAISurfaceSampler2D {
private:

	ZLRect				mWorldRect;
	ZLRect				mLocalRect;

	ZLAffine3D			mWorldToSampleMtx;
	ZLAffine3D			mLocalToSampleMtx;

	MOAITransform*	mSourcePrim;

public:

	enum {
		kMaxSurfaces = 256,
	};

	MOAISurface2D mSurfaces [ kMaxSurfaces ];
	u32 mTop;

	//----------------------------------------------------------------//
	void		AddSurfaceFromLocal			( ZLVec2D v0, ZLVec2D v1 );
	void		AddSurfaceFromWorld			( ZLVec2D v0, ZLVec2D v1 );
	void		Clear						();
	ZLRect		GetLocalRect				();
	void		Init						( const ZLAffine3D& worldToSampleMtx, const ZLRect& worldRect );
				MOAISurfaceSampler2D		();
				~MOAISurfaceSampler2D		();
	void		SetObjectMtx				();
	void		SetObjectMtx				( const ZLAffine3D& localToWorld, const ZLAffine3D& worldToLocal );
	void		SetSourcePrim				( MOAITransform* sourcePrim );
};

#endif
