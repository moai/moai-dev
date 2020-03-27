// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINED_H
#define	MOAIGFXSCRIPTRETAINED_H

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractDrawingAPIObject.h>

class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

//================================================================//
// MOAIGfxScriptRetained
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetained :
	public virtual MOAIAbstractGfxScript,
	public virtual MOAIAbstractDrawingAPIObject {
protected:

	friend class MOAIAbstractGfxScriptInterface;

	ZLMemStream											mCommandStream;
	ZLLeanArray < u8 > 									mBytecode;
	ZLLeanStack < ZLStrongPtr < ZLRefCountedObject > >	mRetainedObjects;
	
	//----------------------------------------------------------------//
	void 				ExecuteBytecode				( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand );
	void				ExecuteMemStream			( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand );
		
	//----------------------------------------------------------------//
//	void				MOAIAbstractDrawingAPI_Call				();
	void				MOAIAbstractDrawingAPI_RetainObject		( ZLRefCountedObject* object );
	void				MOAIAbstractDrawingAPI_SubmitCommand	( MOAIDrawingCmdEnum::_ cmd, const void* param, ZLSize size );
	void				MOAIAbstractGfxScript_RunScript			( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand );
	virtual void		MOAIGfxScriptRetained_Execute			( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) const;

public:

	//----------------------------------------------------------------//
	bool				HasContent					();
						MOAIGfxScriptRetained		();
						~MOAIGfxScriptRetained		();
	void				Optimize					();
	void				Reset						();
};

#endif
