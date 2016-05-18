// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXVERTEXCACHE_H
#define	MOAIGFXVERTEXCACHE_H

#include <moai-sim/MOAIGfxStateCache.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexBuffer.h>

class MOAICamera;
class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIMultiTexture;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAISingleTexture;
class MOAIVertexFormat;
class MOAIViewport;

//================================================================//
// MOAIGfxVertexCache
//================================================================//
class MOAIGfxVertexCache :
	virtual public MOAIGfxStateCacheClient {
protected:

	friend class MOAIGfxGlobalsCache;
	friend class MOAIGfxStateCache;
	
	// Stock OpenGL ES 2.0 has no support for u32 index size in glDrawElements.
	// iOS and many Androids (PowerVR, adreno) support it with GL_OES_element_index_uint extension.
	// We can check extension availability, but using u16 index is fine for the current buffer size (~1000 vertices).
	static const size_t	INDEX_SIZE		= 2;
	
	static const size_t DEFAULT_VERTEX_BUFFER_SIZE	= 0x8000;
	static const size_t DEFAULT_INDEX_BUFFER_SIZE	= 0x1000;

	bool						mIsDrawing;
	bool						mUseIdxBuffer;

	MOAIVertexBuffer*			mVtxBuffer;
	MOAIIndexBuffer*			mIdxBuffer;
	
	u32							mVertexSize;

	u32							mMaxVertices;
	u32							mMaxIndices;
	u32							mMaxPrims;

	u32							mPrimCount;
	u32							mPrimSize;
//	u32							mPrimTopIdx;
//	u32							mPrimTopVtx;
	u32							mPrimType;
	
	u32							mTotalVertices;
	u32							mTotalIndices;
	u16							mIndexBase;

	MOAIVertexFormat*			mVertexFormat;

	bool						mApplyVertexTransform;
	ZLMatrix4x4					mVertexTransform;

	bool						mApplyUVTransform;
	ZLMatrix4x4					mUVTransform;

	ZLColorVec					mVertexColor;
	u32							mVertexColor32;

	//----------------------------------------------------------------//
	void			OnGfxStateWillChange			();
	void			TransformAndWriteQuad			( ZLVec4D* vtx, ZLVec2D* uv );
	void			UpdateLimits					();

public:
	
	//----------------------------------------------------------------//
	void			BeginPrim						();
	void			BeginPrim						( u32 primType, u32 primSize = 0 );
	void			BeginPrimIndexed				( u32 primType, u32 vtxCount, u32 idxCount );
	
	void			BindBufferedDrawing				( MOAIVertexFormat& format );
	void			BindBufferedDrawing				( u32 preset );
	
	void			EndPrim							();
	void			EndPrimIndexed					();
	
	void			FlushBufferedPrims				();
	
	void			InitBuffers						();
	
					MOAIGfxVertexCache				();
					~MOAIGfxVertexCache				();

	void			SetPrimType						( u32 primType, u32 primSize = 0 );

	void			SetUVTransform					();
	void			SetUVTransform					( const ZLMatrix4x4& uvTransform );
	void			SetVertexTransform				();
	void			SetVertexTransform				( const ZLMatrix4x4& vertexTransform );

	//void			UnbindBufferedDrawing			();
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale );
	
	//----------------------------------------------------------------//
//	inline void WriteColor ( float r, float g, float b, float a ) {
//		UNUSED ( r );
//		UNUSED ( g );
//		UNUSED ( b );
//		UNUSED ( a );
//	}
	
	//----------------------------------------------------------------//
	inline void WritePenColor4b () {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < u32 >( this->mVertexColor32 );
	}
	
	//----------------------------------------------------------------//
	inline void WritePenColor4f () {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mVtxBuffer->Write < ZLColorVec >( this->mVertexColor );
	}
		
	//----------------------------------------------------------------//
	inline void WriteIndex ( u16 index ) {
		
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
		this->mIdxBuffer->Write < u16 >( this->mIndexBase + index );
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
