// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USDRAWBUFFER_H
#define	USDRAWBUFFER_H

#include <uslscore/USAccessors.h>
#include <uslsext/USAffine2D.h>
#include <uslsext/USBlendMode.h>
#include <uslsext/USColor.h>
#include <uslsext/USVertexFormat.h>
#include <uslsext/USVertexFormatMgr.h>

class USTexture;
class USDrawBuffer;
class USVertexFormat;

//================================================================//
// USDrawBuffer
//================================================================//
class USDrawBuffer :
	public USGlobalClass < USDrawBuffer > {
private:
	
	static const u32 DEFAULT_BUFFER_SIZE	= 0x8000;
	
	USVertexFormat	mVertexFormat;
	u32				mVertexPreset;
	GLenum			mVertexColorType;
	
	void*			mBuffer;
	u32				mSize;
	u32				mTop;
	u32				mPrimTop;
	
	GLenum			mPrimType;
	u32				mPrimSize;
	u32				mPrimCount;
	u32				mMaxPrims;
	
	USTexture*		mTexture;
	
	USAffine2D		mVtxTransform;
	USAffine2D		mUVTransform;
	
	USColorVec		mPenColor;
	u32				mPackedColor;
	float			mPenWidth;
	float			mPointSize;
	
	USBlendMode		mBlendMode;
	bool			mBlendEnabled;
	
	USRect			mScissorRect;

	//----------------------------------------------------------------//
	void					ClearBuffer				();
	void					DrawPrims				();
	void					Rebind					();
	
public:
	
	//----------------------------------------------------------------//
	void					BeginPrim				();
	void					BeginPrim				( u32 primType );
	void					Clear					();
	void					DrawPrims				( const USVertexFormat& format, GLenum primType, void* buffer, u32 size ); 
	void					EndPrim					();
	void					Flush					();
	const USColorVec&		GetPenColor				();
	const USAffine2D&		GetUVTransform			();
	const USAffine2D&		GetVtxTransform			();
	void					Reserve					( u32 size );
	void					Reset					();
	void					SetBlendMode			();
	void					SetBlendMode			( const USBlendMode& blendMode );
	void					SetBlendMode			( int srcFactor, int dstFactor );
	void					SetPenColor				( u32 color );
	void					SetPenColor				( const USColorVec& colorVec );
	void					SetPenColor				( float r, float g, float b, float a );
	void					SetPenWidth				( float penWidth );
	void					SetPointSize			( float pointSize );
	void					SetPrimType				( u32 primType );
	void					SetScissorRect			();
	void					SetScissorRect			( const USRect& rect );
	bool					SetTexture				( USTexture* texture = 0 );
	void					SetUVTransform			();
	void					SetUVTransform			( const USAffine2D& uvTransform );
	void					SetVertexFormat			();
	void					SetVertexFormat			( const USVertexFormat& format );
	void					SetVertexPreset			( u32 presetID );
	void					SetVtxTransform			();
	void					SetVtxTransform			( const USAffine2D& vtxTransform );
							USDrawBuffer			();
							~USDrawBuffer			();
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
