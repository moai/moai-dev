// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONRESULTMGR_H
#define	MOAIPARTITIONRESULTMGR_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIPartitionResultBuffer.h>

//================================================================//
// MOAIPartitionResultMgr
//================================================================//
class MOAIPartitionResultMgr :
	public MOAIGlobalClass < MOAIPartitionResultMgr > {
private:
	
	MOAIPartitionResultBuffer	mBuffer;
	
public:
	
	GET ( MOAIPartitionResultBuffer&, Buffer, mBuffer )
	
	//----------------------------------------------------------------//
			MOAIPartitionResultMgr		();
			~MOAIPartitionResultMgr		();
};

#endif
