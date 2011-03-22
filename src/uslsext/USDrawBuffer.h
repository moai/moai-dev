// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USDRAWBUFFER_H
#define	USDRAWBUFFER_H

#include <uslscore/USAccessors.h>
#include <uslsext/USAffine2D.h>
#include <uslsext/USBlendMode.h>
#include <uslsext/USColor.h>

class USTexture;
class USDrawBuffer;

//================================================================//
// USGfxArrayInfo
//================================================================//
class USGfxArrayInfo {
private:

	friend class USDrawBuffer;
	
	enum {
		BIND			= 0x00000001,
		STAY_BOUND		= 0x00000002,
		UNBIND			= 0x00000004,
		STAY_UNBOUND	= 0x00000008,
		
		
		IS_BOUND		= 0x00000003,
		US_UNBOUND		= 0x0000000C,
	};
	
	GLint		mSize;
	GLenum		mType;
	u32			mOffset;
	
 	u32			mState;
 	u32			mStride;
 	
	//----------------------------------------------------------------//
	void	Bind				( USDrawBuffer& drawBuffer, u32 type, size_t base, u32 stride );
	bool	Compare				( GLint size, GLenum type, u32 offset ) const;
			USGfxArrayInfo		();
	void	Set					();
	void	Set					( GLint size, GLenum type, u32 offset );
	void	Unbind				( u32 type );
};

//================================================================//
// USDrawBuffer
//================================================================//
class USDrawBuffer :
	public USGlobalClass < USDrawBuffer > {
private:
	
	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 DEFAULT_BUFFER_SIZE	= 0x8000;
	
	enum {
		ARRAY_COLOR,
		ARRAY_NORMAL,
		ARRAY_TEX_COORD,
		ARRAY_VERTEX,
		TOTAL_ARRAY_TYPES,
	};
	
	USGfxArrayInfo mArrayInfo [ TOTAL_ARRAY_TYPES ];
	
	void*		mBuffer;
	u32			mSize;
	u32			mTop;
	u32			mPrimTop;
	
	GLenum		mPrimType;
	u32			mPrimSize;
	u32			mPrimCount;
	u32			mMaxPrims;
	
	u32			mVertexSize;
	
	USTexture*	mTexture;
	
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
	void		ClearBuffer				();
	void		DrawPrims				();
	u32			GetSize					( GLint size, GLenum type );
	void		Rebind					();
	
public:
	
	//----------------------------------------------------------------//
	void					BeginFormat				();
	void					BeginPrim				();
	void					BeginPrim				( u32 primType );
	void					BindColorArray			();
	void					BindColorArray			( GLenum type );
	void					BindNormalArray			();
	void					BindNormalArray			( GLenum type );
	void					BindTexCoordArray		();
	void					BindTexCoordArray		( GLint size, GLenum type );
	bool					BindTexture				( USTexture* texture = 0 );
	void					BindVertexArray			();
	void					BindVertexArray			( GLint size, GLenum type );
	void					Clear					();
	void					EndFormat				();
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
	void					SetUVTransform			();
	void					SetUVTransform			( const USAffine2D& uvTransform );
	void					SetVtxTransform			();
	void					SetVtxTransform			( const USAffine2D& vtxTransform );
							USDrawBuffer			();
							~USDrawBuffer			();
	
	//----------------------------------------------------------------//
	inline void WriteColor ( float r, float g, float b, float a ) {
		UNUSED ( r );
		UNUSED ( g );
		UNUSED ( b );
		UNUSED ( a );
	}
	
	//----------------------------------------------------------------//
	inline void WritePenColor () {
	
		USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_COLOR ];
		
		if ( arrayInfo.mType == GL_FLOAT ) {
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
