// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVERTEXARRAYGL_H
#define	ZLVERTEXARRAYGL_H

#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLGfxHandleGL.h>

class ZLVertexBufferGL;
class ZLVertexFormatGL;

//================================================================//
// ZLVertexBufferWithFormatGL
//================================================================//
class ZLVertexBufferWithFormatGL {
public:

	ZLStrongPtr < ZLVertexBufferGL >				mBuffer;
	ZLStrongPtr < ZLVertexFormatGL >				mFormat;

	//----------------------------------------------------------------//
				ZLVertexBufferWithFormatGL		();
				~ZLVertexBufferWithFormatGL		();
	void		SetBufferAndFormat				( ZLVertexBufferGL* buffer, ZLVertexFormatGL* format );
	
};

//================================================================//
// MOAIVertexArrayGL
//================================================================//
// TODO: doxygen
class ZLVertexArrayGL :
	public ZLAbstractGfxResourceGL {
protected:

	ZLLeanArray < ZLGfxHandleGL >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	ZLIndex											mCurrentVAO;

	ZLLeanArray < ZLVertexBufferWithFormatGL >		mVertexBuffers;

	bool				mUseVAOs;

	//----------------------------------------------------------------//
	bool				AffirmVertexBuffers			( u32 idx );
	void				BindVertexArrayItems		();
	void				UnbindVertexArrayItems		();
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void				ZLAbstractGfxResource_OnCPUDestroy				(); // clear any CPU-side memory used by class
	void				ZLAbstractGfxResource_OnGPUBind					(); // select GPU-side resource on device for use
	bool				ZLAbstractGfxResource_OnGPUCreate				(); // create GPU-side resource
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void				ZLAbstractGfxResource_OnGPUUnbind				(); // unbind GPU-side resource
	bool				ZLAbstractGfxResource_OnGPUUpdate				();

public:
		
	IS ( UsingVAOs, mUseVAOs, true )
	
	//----------------------------------------------------------------//
	ZLVertexBufferGL*		GetVertexBuffer				( ZLIndex idx );
	ZLVertexFormatGL*		GetVertexFormat				( ZLIndex idx );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SetVertexBuffer				( ZLIndex idx, ZLVertexBufferGL* vtxBuffer, ZLVertexFormatGL* vtxFormat );
						ZLVertexArrayGL				();
						~ZLVertexArrayGL			();
};

#endif
