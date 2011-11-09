// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIURLMGR_H
#define MOAIURLMGR_H

class MOAIHttpTask;
class MOAIUrlMgrOpaque;

//================================================================//
// MOAIUrlMgr
//================================================================//
class MOAIUrlMgr :
	public MOAIGlobalClass < MOAIUrlMgr > {
private:

	MOAIUrlMgrOpaque*	mOpaque;

	//----------------------------------------------------------------//
	void			AddHandle			( MOAIHttpTask& task );

public:

	friend class MOAIHttpTask;

	//----------------------------------------------------------------//
	bool			More				();
	void			Process				();
					MOAIUrlMgr			();
					~MOAIUrlMgr			();
};

#endif
