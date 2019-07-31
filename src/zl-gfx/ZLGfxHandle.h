// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXHANDLE_H
#define ZLGFXHANDLE_H

//================================================================//
// ZLGfxResource
//================================================================//
class ZLGfxResource :
	public ZLRefCountedObject {
public:

	enum Type {
		NONE,
		BUFFER,
		FRAMEBUFFER,
		PROGRAM,
		SHADER,
		TEXTURE,
		RENDERBUFFER,
		VERTEXARRAY,
	};
	
	enum Status {
		NOT_ALLOCATED,
		GPU_ALLOCATED,
		SYSTEM_ALLOCATED,
		PENDING_ALLOCATION,
	};
	
	static ZLGfxResource UNBIND;

private:

	friend class ZLGfx;
	friend class ZLGfxHandle;
	friend class ZLGfxImmediate;
	friend class ZLGfxLogger;
	friend class ZLGfxRetained;

	u32		mType;
	u32		mGLID;
	u32		mStatus;

	//----------------------------------------------------------------//
	void				Discard				();
	u32					GLID				() const;
	ZLGfxResource*		RetainForWrite		();
						ZLGfxResource		( ZLGfxResource::Type type, u32 glid, ZLGfxResource::Status status );
						~ZLGfxResource		();
};

//================================================================//
// ZLGfxHandle
//================================================================//
class ZLGfxHandle {
private:

	friend class ZLGfx;
	friend class ZLGfxImmediate;
	friend class ZLGfxLogger;
	friend class ZLGfxRetained;

	ZLGfxResource*		mResource;

public:
	
	//----------------------------------------------------------------//
	ZLGfxHandle&	operator= 			( const ZLGfxHandle& handle );
	bool			CanBind				() const;
	u32				GetStatus			() const;
	u32				GetType				() const;
					ZLGfxHandle			();
					ZLGfxHandle			( const ZLGfxHandle& handle );
					ZLGfxHandle			( ZLGfxResource::Type type, u32 glid, ZLGfxResource::Status status );
					~ZLGfxHandle		();
	
	//----------------------------------------------------------------//
	inline operator bool () const {
	
		return this->mResource != NULL;
	}
	
	//----------------------------------------------------------------//
	operator ZLGfxResource& () const {
	
		return this->mResource ? *this->mResource : ZLGfxResource::UNBIND;
	}
};

#endif
