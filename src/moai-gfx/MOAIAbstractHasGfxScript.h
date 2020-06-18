// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASGFXSCRIPT_H
#define	MOAIABSTRACTHASGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractDrawingObject.h>
#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIDrawingAPI.h>

class MOAIAbstractGfxScript;

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractHasGfxScript :
	public virtual MOAIAbstractDrawingObject {
protected:
	
	//----------------------------------------------------------------//
	void						MOAIAbstractDrawingAPI_RetainObject			( ZLRefCountedObject* object );
	void						MOAIAbstractDrawingAPI_SubmitCommand		( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxScript&		MOAIAbstractHasGfxScript_AffirmGfxScript	() = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript&		AffirmGfxScript				();
								MOAIAbstractHasGfxScript	();
								~MOAIAbstractHasGfxScript	();
	void						RunScript					( MOAIAbstractGfxScriptCallback* callable = NULL, MOAIDrawingAPIEnum::_ callCommand = MOAIDrawingAPIEnum::NONE );

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& AffirmGfxScriptWithType () {
	
		return *MOAICast < TYPE >( &this->AffirmGfxScript ());
	}
};

#endif
