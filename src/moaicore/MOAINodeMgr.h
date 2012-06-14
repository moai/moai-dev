// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAINODEMGR_H
#define MOAINODEMGR_H

class MOAINode;

//================================================================//
// MOAINodeMgr
//================================================================//
class MOAINodeMgr :
	public MOAIGlobalClass < MOAINodeMgr > {
private:

	MOAINode* mUpdateListHead;
	MOAINode* mUpdateListTail;

	//----------------------------------------------------------------//
	void			InsertAfter			( MOAINode& cursor, MOAINode& node );
	void			InsertBefore		( MOAINode& cursor, MOAINode& node );
	void			PushBack			( MOAINode& node );
	void			PushFront			( MOAINode& node );
	void			Remove				( MOAINode& node );

public:

	friend class MOAINode;

	//----------------------------------------------------------------//
	void			Update				();
					MOAINodeMgr			();
					~MOAINodeMgr		();
};

#endif
