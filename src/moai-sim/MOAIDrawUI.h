// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWUI_H
#define	MOAIDRAWUI_H

class MOAIFont;

//================================================================//
// MOAIDrawUI
//================================================================//
// TODO: doxygen
class MOAIDrawUI :
	public virtual ZLContextClass {
private:
	
//	//----------------------------------------------------------------//
//	void			MOAIAbstractDrawingAPI_RetainObject			( ZLRefCountedObject* object );
//	void			MOAIAbstractDrawingAPI_SubmitCommand		( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	
public:

	//----------------------------------------------------------------//
	void			BeginDrawString			( float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY );
	void			DrawString				( cc8* text, float x, float y, float width, float height );
	void			DrawString				( cc8* text, float x, float y, float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY, float width, float height );
	void			EndDrawString			();
					MOAIDrawUI				();
					~MOAIDrawUI				();
};

#endif
