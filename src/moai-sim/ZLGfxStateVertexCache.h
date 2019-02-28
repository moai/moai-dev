// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXSTATEVERTEXCACHE_H
#define	ZLGFXSTATEVERTEXCACHE_H

#include <moai-sim/ZLAbstractGfxStateCache.h>
#include <moai-sim/ZLGfxStateCPUCache.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexBuffer.h>

class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAITextureBase;
class MOAIVertexFormat;

//================================================================//
// ZLGfxStateVertexCache
//================================================================//
class ZLGfxStateVertexCache :
	virtual public ZLAbstractGfxStateCache,
	public ZLGfxStateCPUCache {
protected:

	friend class MOAIGfxMgr; // for now
	friend class ZLGfxStateCache;
	friend class ZLGfxStateCPUCache;
	friend class ZLGfxStateGPUCache;
	
	// Stock OpenGL ES 2.0 has no support for u32 index size in glDrawElements.
	// iOS and many Androids (PowerVR, adreno) support it with GL_OES_element_index_uint extension.
	// We can check extension availability, but using u16 index is fine for the current buffer size (~1000 vertices).
	static const size_t	INDEX_SIZE		= 2;
	
	static const size_t DEFAULT_VERTEX_BUFFER_SIZE	= 0x8000;
	static const size_t DEFAULT_INDEX_BUFFER_SIZE	= 0x1000;

	static const size_t UNIFORM_BUFFER_CHUNK_SIZE	= 1024;

	bool						mIsDrawing;
	
	u32							mVtxBase; // offsets the vertex in drawprims *or* the index when writing indexed prims
	u32							mIdxBase; // this is the offset to the first index for the next call to draw prims
	
	u32							mVtxSize;

	u32							mPrimType;
	bool						mFlushOnPrimEnd;
	bool						mUseIdxBuffer;
	u32							mPrimCount;

	bool						mApplyVertexTransform;
	ZLMatrix4x4					mVertexTransform;

	bool						mApplyUVTransform;
	ZLMatrix4x4					mUVTransform;
	
	ZLStrongPtr < MOAIVertexBuffer >	mVtxBuffer;
	ZLStrongPtr < MOAIIndexBuffer >		mIdxBuffer;
	
	//----------------------------------------------------------------//
	u32				CountPrims						();
	void			FlushVertexCache				();
	void			TransformAndWriteQuad			( ZLVec4D* vtx, ZLVec2D* uv );

public:
	
	enum {
		CONTINUE_OK,
		CONTINUE_ROLLOVER,
		CONTINUE_FAIL,
	};
	
	//----------------------------------------------------------------//
	bool			BeginPrim						( u32 primType, u32 vtxCount, u32 idxCount = 0 );
	u32				ContinuePrim					( u32 vtxCount, u32 idxCount = 0 );
	void			EndPrim							();
	
	void			InitBuffers						();
	
					ZLGfxStateVertexCache			();
					~ZLGfxStateVertexCache			();

	void			Reset							();

	void			SetUVTransform					();
	void			SetUVTransform					( u32 mtxID );
	void			SetUVTransform					( const ZLMatrix4x4& uvTransform );
	
	void			SetVertexTransform				();
	void			SetVertexTransform				( u32 mtxID );
	void			SetVertexTransform				( const ZLMatrix4x4& vertexTransform );

	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale );
	
	//----------------------------------------------------------------//
	inline void WritePenColor4b () {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < u32 >( this->mStateFrameCPU.mFinalColor32 );
	}
	
	//----------------------------------------------------------------//
	inline void WritePenColor4f () {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLColorVec >( this->mStateFrameCPU.mFinalColor );
	}
		
	//----------------------------------------------------------------//
	inline void WriteIndex ( u16 index ) {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mIdxBuffer->Write < u16 >( (u16) this->mVtxBase + index );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( float u, float v ) {
	
		ZLVec2D uv;
		uv.mX = u;
		uv.mY = v;
	
		if ( this->mApplyUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLVec2D >( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( ZLVec2D uv ) {
	
		if ( this->mApplyUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLVec2D >( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( float x, float y ) {
		
		this->WriteVtx ( x, y, 0.0f, 1.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( float x, float y, float z ) {
		
		this->WriteVtx ( x, y, z, 1.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( float x, float y, float z, float w ) {
		
		ZLVec4D vtx;
		vtx.mX = x;
		vtx.mY = y;
		vtx.mZ = z;
		vtx.mW = w;
		
		if ( this->mApplyVertexTransform ) {
			this->mVertexTransform.Transform ( vtx );
		}
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLVec4D >( vtx );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( ZLVec2D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, 0.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( ZLVec3D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, vtx.mZ );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtxRaw ( float x, float y, float z, float w ) {
		
		ZLVec4D vtx;
		vtx.mX = x;
		vtx.mY = y;
		vtx.mZ = z;
		vtx.mW = w;
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLVec4D >( vtx );
	}
};

#endif
