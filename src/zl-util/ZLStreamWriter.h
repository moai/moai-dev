// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAMWRITER_H
#define ZLSTREAMWRITER_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLStreamWriter
//================================================================//
class ZLStreamWriter :
	public ZLStream {
public:

	//----------------------------------------------------------------//
	virtual void	Close					() = 0;
	virtual bool	Open					( ZLStream& stream ) = 0;
					ZLStreamWriter			();
					~ZLStreamWriter			();
};

#endif
