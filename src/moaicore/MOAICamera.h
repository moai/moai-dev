// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERA_H
#define	MOAICAMERA_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

class MOAIViewport;

//================================================================//
// MOAICamera
//================================================================//
/**	@name	MOAICamera
	@text	Abstract base class for camera objects. Plugs into
			MOAILayer to project geometry being rendered.
*/
class MOAICamera :
	public virtual MOAITransform {
public:
	
	//----------------------------------------------------------------//
	virtual USMatrix4x4		GetProjMtx			( const MOAIViewport& viewport ) const = 0;
	virtual USMatrix4x4		GetProjMtxInv		( const MOAIViewport& viewport ) const = 0;
							MOAICamera			();
							~MOAICamera			();
	void					RegisterLuaClass	( MOAILuaState& state );
	void					RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
