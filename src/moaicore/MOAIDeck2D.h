// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2D_H
#define	MOAIDECK2D_H

#include <moaicore/MOAIDeck.h>

class MOAIGrid;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeck2D
//================================================================//
/**	@name MOAIDeck2D
	@text Base class for 2D decks.
*/
class MOAIDeck2D :
	public MOAIDeck {
public:
	
	//----------------------------------------------------------------//
	USBox				GetBounds			();
	USBox				GetBounds			( u32 idx, MOAIDeckRemapper* remapper );
	virtual USRect		GetRect				( ) = 0;
	virtual USRect		GetRect				( u32 idx, MOAIDeckRemapper* remapper ) = 0;
						MOAIDeck2D			();
						~MOAIDeck2D			();
	void				RegisterLuaClass	( MOAILuaState& state );
	void				RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
