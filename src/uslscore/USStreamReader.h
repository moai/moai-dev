// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSTREAMREADER_H
#define USSTREAMREADER_H

#include <uslscore/USStream.h>

//================================================================//
// USStreamReader
//================================================================//
class USStreamReader :
	public USStream {
public:

	//----------------------------------------------------------------//
	virtual void	Close					() = 0;
	virtual bool	Open					( USStream* stream ) = 0;
					USStreamReader			();
					~USStreamReader			();
};

#endif
