//[MOAI LICENSE]

#ifndef	MOAIMULTISHAPE2D_H
#define	MOAIMULTISHAPE2D_H

//================================================================//
// MOAIMultiShape2D
//================================================================//
class MOAIMultiShape2D {
private:
	
	enum {
		QUAD,
		RECT,
		NONE,
	};
	
	USQuad			mQuad;
	USRect			mRect;
	
	u32				mType;

public:
	
	//----------------------------------------------------------------//
	void		Clear				();
	bool		Intersect			( const MOAIMultiShape2D& shape, USRect& result ) const;
				MOAIMultiShape2D		();
				~MOAIMultiShape2D		();
	bool		Overlap				( const MOAIMultiShape2D& shape ) const;
	void		SetQuad				( const USQuad& quad );
	void		SetRect				( const USRect& rect );
};

#endif
