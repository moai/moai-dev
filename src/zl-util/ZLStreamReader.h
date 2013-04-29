// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAMREADER_H
#define ZLSTREAMREADER_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLStreamReader
//================================================================//
class ZLStreamReader :
	public ZLStream {
public:

	//----------------------------------------------------------------//
	virtual void	Close					() = 0;
	virtual bool	Open					( ZLStream& stream ) = 0;
					ZLStreamReader			();
					~ZLStreamReader			();
};

#endif
