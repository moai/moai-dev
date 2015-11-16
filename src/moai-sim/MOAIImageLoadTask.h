// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIIMAGELOADTASK_H
#define MOAIIMAGELOADTASK_H

#include <moai-sim/MOAIImage.h>

class MOAIDataBuffer;

//================================================================//
// MOAIImageLoadTask
//================================================================//
class MOAIImageLoadTask :
	public MOAITask {
private:
	
	MOAIImage				mImage;

	STLString				mFilename;
	MOAIDataBuffer*			mDataBuffer;
	MOAIImage*				mTarget;
	MOAILuaStrongRef		mOnFinish;
	u32						mTransform;

	//----------------------------------------------------------------//
	void		Execute				();
	void		Publish				();

public:

	//----------------------------------------------------------------//
	void		Init				( cc8* filename, MOAIImage& target, u32 transform );
	void		Init				( MOAIDataBuffer& data, MOAIImage& target, u32 transform );
				MOAIImageLoadTask	();
				~MOAIImageLoadTask	();
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SetCallback			( lua_State* L, int idx );
};

#endif
