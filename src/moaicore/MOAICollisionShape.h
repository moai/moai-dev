// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONSHAPE_H
#define	MOAICOLLISIONSHAPE_H

//================================================================//
// MOAICollisionShape
//================================================================//
class MOAICollisionShape {
private:

	enum {
		QUAD,
		RECT,
		NONE,
	};

	union {
		char mRectBuffer [ sizeof ( USRect )];
		char mQuadBuffer [ sizeof ( USQuad )];
	};

	u32 mType;

	//----------------------------------------------------------------//
	const USRect&	GetRect					() const;
	const USQuad&	GetQuad					() const;
	static bool		Overlap					( const USRect& s0, const USRect& s1 );
	static bool		Overlap					( const USQuad& s0, const USRect& s1 );
	static bool		Overlap					( const USQuad& s0, const USQuad& s1 );

public:
	
	//----------------------------------------------------------------//
					MOAICollisionShape		();
					~MOAICollisionShape		();
	bool			Overlap					( const MOAICollisionShape& shape ) const;
	void			Set						( const USRect& rect );
	void			Set						( const USQuad& quad );
};

#endif
