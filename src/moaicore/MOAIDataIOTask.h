// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATAIOTASK_H
#define MOAIDATAIOTASK_H

#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAITask.h>

//================================================================//
// MOAIDataIOTask
//================================================================//
class MOAIDataIOTask : 
	public MOAITask < MOAIDataIOTask > {
private:

	enum {
		IDLE,
		LOADING,
		SAVING,
	};

	STLString			mFilename;
	MOAIDataBuffer*		mData;
	u32					mState;

	//----------------------------------------------------------------//
	void		Execute				();

public:

	SET	( cc8*, Filename, mFilename )
	GET_SET ( MOAIDataBuffer*, Data, mData )

	//----------------------------------------------------------------//
	void		LoadData			( cc8* filename, MOAIDataBuffer& target );
	void		SaveData			( cc8* filename, MOAIDataBuffer& target );
				MOAIDataIOTask		();
				~MOAIDataIOTask		();
};

#endif
