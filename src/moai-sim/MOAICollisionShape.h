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
		char mRectBuffer [ sizeof ( ZLRect )];
		char mQuadBuffer [ sizeof ( ZLQuad )];
	};

	u32 mType;

	//----------------------------------------------------------------//
	const ZLRect&	GetRect					() const;
	const ZLQuad&	GetQuad					() const;
	static bool		Overlap					( const ZLRect& s0, const ZLRect& s1 );
	static bool		Overlap					( const ZLQuad& s0, const ZLRect& s1 );
	static bool		Overlap					( const ZLQuad& s0, const ZLQuad& s1 );

public:
	
	//----------------------------------------------------------------//
					MOAICollisionShape		();
					~MOAICollisionShape		();
	bool			Overlap					( const MOAICollisionShape& shape ) const;
	void			Set						( const ZLRect& rect );
	void			Set						( const ZLQuad& quad );
};

#endif
