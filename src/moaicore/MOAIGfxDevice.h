// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICE_H
#define	MOAIGFXDEVICE_H

class MOAITexture;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxDevice
//================================================================//
class MOAIGfxDevice :
	public USGlobalClass < MOAIGfxDevice > {
private:
	
	static const u32 DEFAULT_BUFFER_SIZE	= 0x8000;
	
	const MOAIVertexFormat*	mVertexFormat;
	GLenum					mVertexColorType;
	
	void*			mBuffer;
	u32				mSize;
	u32				mTop;
	u32				mPrimTop;
	
	GLenum			mPrimType;
	u32				mPrimSize;
	u32				mPrimCount;
	u32				mMaxPrims;
	
	MOAITexture*	mTexture;
	
	USAffine2D		mCameraTransform;
	USAffine2D		mVtxTransform;
	USAffine2D		mUVTransform;
	
	USColorVec		mPenColor;
	u32				mPackedColor;
	float			mPenWidth;
	float			mPointSize;
	
	USBlendMode		mBlendMode;
	bool			mBlendEnabled;
	
	USRect			mScissorRect;

	u32				mWidth;
	u32				mHeight;

	//----------------------------------------------------------------//
	void					ClearBuffer				();
	void					DrawPrims				();
	void					Rebind					();
	
public:
	
	enum {
		GL_PIPELINE_FIXED,
		GL_PIPELINE_PROGRAMMABLE,
	};
	
	//----------------------------------------------------------------//
	void					BeginPrim				();
	void					BeginPrim				( u32 primType );
	void					Clear					();
	void					ClearErrors				();
	u32						CountErrors				();
	void					DrawPrims				( const MOAIVertexFormat& format, GLenum primType, void* buffer, u32 size ); 
	void					EndPrim					();
	void					Flush					();
	const USAffine2D&		GetCameraTransform		();
	cc8*					GetErrorString			( int error );
	u32						GetHeight				();
	const USColorVec&		GetPenColor				();
	u32						GetPipelineMode			();
	USRect					GetRect					();
	const USAffine2D&		GetUVTransform			();
	const USAffine2D&		GetVtxTransform			();
	u32						GetWidth				();
	u32						LogErrors				();
	u32						PrintErrors				();
	void					Reserve					( u32 size );
	void					Reset					();
	void					SetBlendMode			();
	void					SetBlendMode			( const USBlendMode& blendMode );
	void					SetBlendMode			( int srcFactor, int dstFactor );
	void					SetCameraTransform		();
	void					SetCameraTransform		( const USAffine2D& cameraTransform );
	void					SetPenColor				( u32 color );
	void					SetPenColor				( const USColorVec& colorVec );
	void					SetPenColor				( float r, float g, float b, float a );
	void					SetPenWidth				( float penWidth );
	void					SetPointSize			( float pointSize );
	void					SetPrimType				( u32 primType );
	void					SetScissorRect			();
	void					SetScissorRect			( const USRect& rect );
	void					SetSize					( u32 width, u32 height );
	bool					SetTexture				( MOAITexture* texture = 0 );
	void					SetUVTransform			();
	void					SetUVTransform			( const USAffine2D& uvTransform );
	void					SetVertexFormat			();
	void					SetVertexFormat			( const MOAIVertexFormat& format );
	void					SetVertexPreset			( u32 preset );
	void					SetVtxTransform			();
	void					SetVtxTransform			( const USAffine2D& vtxTransform );
							MOAIGfxDevice			();
							~MOAIGfxDevice			();
	void					WriteQuad				( USVec2D* vtx, USVec2D* uv );
	
	//----------------------------------------------------------------//
	inline void WriteColor ( float r, float g, float b, float a ) {
		UNUSED ( r );
		UNUSED ( g );
		UNUSED ( b );
		UNUSED ( a );
	}
	
	//----------------------------------------------------------------//
	inline void WritePenColor () {
		
		if ( this->mVertexColorType == GL_FLOAT ) {
			this->Write < USColorVec >( this->mPenColor );
		}
		else {
			this->Write < u32 >( this->mPackedColor );
		}
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( USVec2D uv ) {
	
		this->mUVTransform.Transform ( uv );
		this->Write ( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( USVec2D vtx ) {
	
		this->mVtxTransform.Transform ( vtx );		
		this->Write ( vtx );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void Write ( const TYPE& type ) {
		
		*( TYPE* )(( size_t )this->mBuffer + this->mTop ) = type;
		this->mTop += sizeof ( TYPE );
	}
};

#endif
