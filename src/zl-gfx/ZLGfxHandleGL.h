// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXHANDLEGL_H
#define ZLGFXHANDLEGL_H

#include <zl-util/ZLRefCountedObject.h>

//================================================================//
// ZLGfxResourceGL
//================================================================//
class ZLGfxResourceGL :
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
	
	static ZLGfxResourceGL UNBIND;

private:

	friend class ZLGfx;
	friend class ZLGfxHandleGL;
	friend class ZLGfxImmediate;
	friend class ZLGfxLogger;
	friend class ZLGfxRetained;

	u32		mType;
	u32		mGLID;
	u32		mStatus;

	//----------------------------------------------------------------//
	void				Discard				();
	u32					GLID				() const;
	ZLGfxResourceGL*	RetainForWrite		();
						ZLGfxResourceGL		( ZLGfxResourceGL::Type type, u32 glid, ZLGfxResourceGL::Status status );
						~ZLGfxResourceGL		();
};

//================================================================//
// ZLGfxHandleGL
//================================================================//
class ZLGfxHandleGL {
private:

	friend class ZLGfx;
	friend class ZLGfxImmediate;
	friend class ZLGfxLogger;
	friend class ZLGfxRetained;

	ZLGfxResourceGL*		mResource;

public:
	
	//----------------------------------------------------------------//
	ZLGfxHandleGL&	operator= 			( const ZLGfxHandleGL& handle );
	bool			CanBind				() const;
	u32				GetStatus			() const;
	u32				GetType				() const;
					ZLGfxHandleGL		();
					ZLGfxHandleGL		( const ZLGfxHandleGL& handle );
					ZLGfxHandleGL		( ZLGfxResourceGL::Type type, u32 glid, ZLGfxResourceGL::Status status );
					~ZLGfxHandleGL		();
	
	//----------------------------------------------------------------//
	inline operator bool () const {
	
		return this->mResource != NULL;
	}
	
	//----------------------------------------------------------------//
	operator ZLGfxResourceGL& () const {
	
		return this->mResource ? *this->mResource : ZLGfxResourceGL::UNBIND;
	}
};

#endif
