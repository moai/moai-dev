// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USURLMGR_H
#define USURLMGR_H

#include <uslsext/USDelegate.h>
#include <uslsext/USHttpTask.h>
#include <uslsext/USTaskThread.h>

class USHttpTask;

//================================================================//
// USUrlMgr
//================================================================//
class USUrlMgr :
	public USGlobalClass < USUrlMgr > {
private:

	USTaskThread	mDataIOThread;

	//----------------------------------------------------------------//
	void			AddHandle			( USHttpTask& task );

public:

	friend class USHttpTask;

	//----------------------------------------------------------------//
	bool			More				();
	void			Process				();
					USUrlMgr			();
					~USUrlMgr			();
};

#endif
