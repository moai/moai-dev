// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINVALIDHEADER_H
#define MOAIINVALIDHEADER_H

#include <moai-sim/MOAITextureHeader.h>

//================================================================//
// MOAIInvalidHeader
//================================================================//
class MOAIInvalidHeader :
	public MOAITextureHeader {
public:
	
	//----------------------------------------------------------------//
	virtual u32				GetDataSize				();
	virtual u32				GetHeaderSize			();
	virtual void*			GetHeaderStart			();
	virtual u32				GetHeight				();
	virtual u32				GetWidth				();
	virtual bool			IsValid					();
	virtual void			Invalidate				();
							MOAIInvalidHeader		();
	virtual					~MOAIInvalidHeader		();
};

#endif
