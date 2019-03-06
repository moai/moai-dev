// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLATTRID_H
#define	ZLATTRID_H

#include <zl-util/ZLAttribute.h>

//================================================================//
// ZLAttrID
//================================================================//
class ZLAttrID {
private:
	friend class MOAINode;
	friend class MOAIDepLink;
	
	u32		mPackedID;

public:

	//----------------------------------------------------------------//
	static ZLAttrID FromRaw ( u32 packedID ) {
		ZLAttrID attrID;
		attrID.mPackedID = packedID;
		return attrID;
	}
	
	//----------------------------------------------------------------//
	u32 ToRaw () const {
		return this->mPackedID;
	}

	//----------------------------------------------------------------//
	u32 Unpack () const {
		return this->mPackedID & ZLAttribute::ATTR_ID_MASK;
	}
};

#endif
