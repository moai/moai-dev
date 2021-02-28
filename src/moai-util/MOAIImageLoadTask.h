// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIIMAGELOADTASK_H
#define MOAIIMAGELOADTASK_H

#include <moai-util/MOAITask.h>

class MOAIImage;
class MOAIDataBuffer;

//================================================================//
// MOAIImageLoadTask
//================================================================//
class MOAIImageLoadTask :
	public MOAITask {
private:
	
	ZLStrongPtr < MOAIImage >	mImage;

	STLString					mFilename;
	MOAIDataBuffer*				mDataBuffer;
	MOAIImage*					mTarget;
	MOAILuaStrongRef			mOnFinish;
	u32							mTransform;

	//----------------------------------------------------------------//
	void		Execute					();
	void		Publish					();

	//----------------------------------------------------------------//
	void		_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void		_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	void		Init					( cc8* filename, MOAIImage& target, u32 transform );
	void		Init					( MOAIDataBuffer& data, MOAIImage& target, u32 transform );
				MOAIImageLoadTask		();
				~MOAIImageLoadTask		();
	void		SetCallback				( lua_State* L, int idx );
};

#endif
