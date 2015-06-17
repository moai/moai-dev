// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICEVERTEXWRITER_H
#define	MOAIGFXDEVICEVERTEXWRITER_H

#include <moai-sim/MOAIGfxDeviceMtxCache.h>
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
// MOAIGfxDeviceVertexWriter
//================================================================//
class MOAIGfxDeviceVertexWriter :
	public MOAIGfxDeviceMtxCache {
protected:

	static const size_t	INDEX_SIZE		= 4;
	
	static const size_t DEFAULT_VERTEX_BUFFER_SIZE	= 0x8000;
	static const size_t DEFAULT_INDEX_BUFFER_SIZE	= 0x1000;

	bool						mIsDrawing;

	ZLColorVec					mAmbientColor;
	ZLColorVec					mPenColor;
	
	ZLColorVec					mFinalColor;
	u32							mFinalColor32;

	MOAIVertexBuffer			mVtxBuffer;
	MOAIIndexBuffer				mIdxBuffer;
	
	u32							mVertexSize;

	u32							mMaxVertices;
	u32							mMaxPrims;

	u32							mPrimCount;
	u32							mPrimSize;
	u32							mPrimTopIdx;
	u32							mPrimTopVtx;
	u32							mPrimType;

	MOAIVertexFormat*			mVertexFormat;

	//----------------------------------------------------------------//
	void			OnGfxStateWillChange			();
	void			TransformAndWriteQuad			( ZLVec4D* vtx, ZLVec2D* uv );
	void			UpdateFinalColor				();

public:
	
	GET ( ZLColorVec, AmbientColor, mAmbientColor )
	GET ( ZLColorVec, FinalColor, mFinalColor )
	GET ( ZLColorVec, PenColor, mPenColor )
	
	//----------------------------------------------------------------//
	void			BeginPrim						();
	void			BeginPrim						( u32 primType, u32 primSize = 0 );
	
	void			BindBufferedDrawing				( MOAIVertexFormat& format );
	void			BindBufferedDrawing				( u32 preset );
	
	void			EndPrim							();
	
	void			FlushBufferedPrims				();
	
					MOAIGfxDeviceVertexWriter		();
					~MOAIGfxDeviceVertexWriter		();
	void			SetAmbientColor					( u32 color );
	void			SetAmbientColor					( const ZLColorVec& colorVec );
	void			SetAmbientColor					( float r, float g, float b, float a );
	
	void			SetPenColor						( u32 color );
	void			SetPenColor						( const ZLColorVec& colorVec );
	void			SetPenColor						( float r, float g, float b, float a );

	void			SetPrimType						( u32 primType, u32 primSize = 0 );

	void			UnbindBufferedDrawing			();
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale );
	void			WriteQuad						( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale );
	
	//----------------------------------------------------------------//
//	template < typename TYPE >
//	inline void Write ( const TYPE& type ) {
//		
//		size_t top = this->mTop + sizeof ( TYPE );
//		if ( top < this->mSize ) {
//			*( TYPE* )(( size_t )this->mBuffer + this->mTop ) = type;
//			this->mTop = top;
//		}
//	}
	
	//----------------------------------------------------------------//
	inline void WriteColor ( float r, float g, float b, float a ) {
		UNUSED ( r );
		UNUSED ( g );
		UNUSED ( b );
		UNUSED ( a );
	}
	
	//----------------------------------------------------------------//
	inline void WriteFinalColor4b () {
		
		this->mVtxBuffer.Write < u32 >( this->mFinalColor32 );
	}
	
	//----------------------------------------------------------------//
	inline void WriteFinalColor4f () {
		
		this->mVtxBuffer.Write < ZLColorVec >( this->mFinalColor );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( float u, float v ) {
	
		ZLVec2D uv;
		uv.mX = u;
		uv.mY = v;
	
		if ( this->mCpuUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		this->mVtxBuffer.Write < ZLVec2D >( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( ZLVec2D uv ) {
	
		if ( this->mCpuUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		this->mVtxBuffer.Write < ZLVec2D >( uv );
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
		
		if ( this->mCpuVertexTransform ) {
			this->mCpuVertexTransformMtx.Transform ( vtx );
		}
		this->mVtxBuffer.Write < ZLVec4D >( vtx );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( ZLVec2D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, 0.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( ZLVec3D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, vtx.mZ );
	}
};

#endif
