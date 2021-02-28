// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIURLMGRNACL_H
#define MOAIURLMGRNACL_H

#ifdef MOAI_OS_NACL

class MOAIHttpTaskNaCl;
class MOAIUrlMgrOpaque;

//================================================================//
// MOAIUrlMgrNaCl
//================================================================//
class MOAIUrlMgrNaCl :
	public virtual ZLContextClass {
private:

	STLList < MOAIHttpTaskNaCl* > mTasks;

public:

	friend class MOAIHttpTask;

	//----------------------------------------------------------------//
	void			AddHandle				( MOAIHttpTaskNaCl& task );
					MOAIUrlMgrNaCl			();
					~MOAIUrlMgrNaCl			();
	void			Process					();
};

#endif
#endif
