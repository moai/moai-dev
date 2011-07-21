// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USVERTEXFORMAT_H
#define	USVERTEXFORMAT_H

#include <uslscore/USAccessors.h>
#include <uslsext/USAffine2D.h>
#include <uslsext/USBlendMode.h>
#include <uslsext/USColor.h>

class USTexture;
class USDrawBuffer;

//================================================================//
// USVertexFormatElem
//================================================================//
class USVertexFormatElem {
private:

	friend class USVertexFormat;
	
	GLint		mSize;
	GLenum		mType;		// type of the element
	u32			mOffset;
 	bool		mIsEnabled;
 	
	//----------------------------------------------------------------//
	void	Bind				( void* buffer, u32 stride, GLenum use ) const;
	void	Set					();
	void	Set					( GLint size, GLenum type, u32 offset );
	void	Unbind				( GLenum use ) const;
			USVertexFormatElem	();
};

//================================================================//
// USVertexFormatAttr
//================================================================//
class USVertexFormatAttr {
private:

	friend class USVertexFormat;
	
	GLint		mIndex;
	GLint		mSize;
	GLenum		mType;			// type of the element
	GLboolean	mNormalized;
	u32			mOffset;
 	
	//----------------------------------------------------------------//
	void	Bind					( void* buffer, u32 stride ) const;
	void	Set						( GLint index, GLint size, GLenum type, GLboolean normalized, u32 offset );
	void	Unbind					() const;

public:

	//----------------------------------------------------------------//
			USVertexFormatAttr		();
};

//================================================================//
// USVertexFormat
//================================================================//
class USVertexFormat {
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
	
	USVertexFormatElem mElements [ TOTAL_ARRAY_TYPES ];
	u32 mVertexSize;

	USLeanArray < USVertexFormatAttr >	mAttributes;
	u32									mTotalAttributes;

	//----------------------------------------------------------------//
	u32			GetSize					( GLint size, GLenum type );
	
public:
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	void		Bind					( void* buffer ) const;
	void		Clear					();
	bool		ComputeBounds			( void* buffer, u32 size, USRect& bounds );
	void		DeclareAttribute		( GLint index, GLint size, GLenum type, GLboolean normalized );
	void		DeclareColorArray		( GLenum type );
	void		DeclareNormalArray		( GLenum type );
	void		DeclareTexCoordArray	( GLenum type, GLint size );
	void		DeclareVertexArray		( GLenum type, GLint size );
	GLenum		GetColorType			() const;
	void		ReserveAttributes		( u32 total );
	void		Unbind					() const;
				USVertexFormat			();
	virtual		~USVertexFormat			();
};

#endif
