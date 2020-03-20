// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWABLE_H
#define	MOAIDRAWABLE_H

//================================================================//
// MOAIAbstractDrawable
//================================================================//
class MOAIAbstractDrawable :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
private:

	//----------------------------------------------------------------//
	virtual void	MOAIDrawable_Draw			( int subPrimID ) = 0;
	virtual void	MOAIDrawable_DrawDebug		( int subPrimID ) = 0;

public:

	//----------------------------------------------------------------//
					MOAIAbstractDrawable		();
	virtual 		~MOAIAbstractDrawable		();
	static void		Draw						( MOAILuaMemberRef& ref, bool debug = false );
	static void		Draw						( MOAILuaState& state, int idx, bool debug = false );
	void			Draw						( int subPrimID = ( int )-1 ) { MOAIDrawable_Draw ( subPrimID ); }
	void			DrawDebug					( int subPrimID = ( int )-1 ) { MOAIDrawable_DrawDebug ( subPrimID ); }
};

#endif
