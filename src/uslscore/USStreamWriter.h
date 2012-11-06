// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSTREAMWRITER_H
#define USSTREAMWRITER_H

#include <uslscore/USStream.h>

//================================================================//
// USStreamWriter
//================================================================//
class USStreamWriter :
	public USStream {
public:

	//----------------------------------------------------------------//
	virtual void	Close					() = 0;
	virtual bool	Open					( USStream* stream ) = 0;
					USStreamWriter			();
					~USStreamWriter			();
};

#endif
