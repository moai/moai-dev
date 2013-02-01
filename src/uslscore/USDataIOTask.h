// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDATAIOTASK_H
#define USDATAIOTASK_H

#include <uslscore/STLString.h>
#include <uslscore/USTask.h>
#include <uslscore/USData.h>

//================================================================//
// USDataIOTask
//================================================================//
class USDataIOTask : 
	public USTask < USDataIOTask > {
private:

	enum {
		IDLE,
		LOADING,
		SAVING,
	};

	STLString		mFilename;
	USData*			mData;
	u32				mState;

	//----------------------------------------------------------------//
	void	Execute		();

public:

	SET	( cc8*, Filename, mFilename )
	GET_SET ( USData*, Data, mData )

	//----------------------------------------------------------------//
	void	LoadData		( cc8* filename, USData& target );
	void	SaveData		( cc8* filename, USData& target );
			USDataIOTask	();
			~USDataIOTask	();
};

#endif
