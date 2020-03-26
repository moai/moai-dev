// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

class MOAIFont;

//================================================================//
// MOAIDraw
//================================================================//
/**	@lua	MOAIDraw
	@text	Singleton for performing immediate mode drawing operations.
			See MOAIDrawDeck.
*/
class MOAIDraw :
	public ZLContextClass < MOAIDraw >,
	public virtual MOAIAbstractDrawingAPI {
private:
	
	//----------------------------------------------------------------//
	void			MOAIAbstractDrawingAPI_RetainObject			( ZLRefCountedObject* object );
	void			MOAIAbstractDrawingAPI_SubmitCommand		( MOAIDrawingCmdEnum::_ cmd, const void* param, ZLSize size );
	
public:

	//----------------------------------------------------------------//
	void			BeginDrawString			( float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY );
	void			DrawString				( cc8* text, float x, float y, float width, float height );
	void			DrawString				( cc8* text, float x, float y, float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY, float width, float height );
	void			EndDrawString			();
					MOAIDraw				();
					~MOAIDraw				();
};

#endif
