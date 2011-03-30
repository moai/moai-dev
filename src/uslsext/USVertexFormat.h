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
	
	enum {
		BIND			= 0x00000001,
		STAY_BOUND		= 0x00000002,
		UNBIND			= 0x00000004,
		STAY_UNBOUND	= 0x00000008,
		
		IS_BOUND		= 0x00000003,
		US_UNBOUND		= 0x0000000C,
	};
	
	GLint		mSize;
	GLenum		mType;		// type of the element
	GLenum		mUse;		// type of gl array (i.e. GL_COLOR_ARRAY, GL_NORMAL_ARRAY, etc.)
	u32			mOffset;
 	u32			mIsEnabled;
 	
	//----------------------------------------------------------------//
	void	Bind				( void* buffer, u32 stride ) const;
	void	Init				( GLenum use );
	bool	IsMatch				( const USVertexFormatElem& elem ) const;
			USVertexFormatElem	();
	void	Set					( GLint size, GLenum type, u32 offset );
	void	Unbind				( u32 type );
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

	//----------------------------------------------------------------//
	u32			GetSize					( GLint size, GLenum type );
	
public:
	
	GET ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	void		Bind					( void* buffer ) const;
	void		Clear					();
	void		DeclareColorArray		( GLenum type );
	void		DeclareNormalArray		( GLenum type );
	void		DeclareTexCoordArray	( GLint size, GLenum type );
	void		DeclareVertexArray		( GLint size, GLenum type );
	GLenum		GetColorType			() const;
	bool		IsMatch					( const USVertexFormat& format ) const;
				USVertexFormat			();
	virtual		~USVertexFormat			();
};

#endif
