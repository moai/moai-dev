// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUAREFTABLE_H
#define MOAILUAREFTABLE_H

class MOAILuaState;

//================================================================//
// MOAILuaRefTable
//================================================================//
class MOAILuaRefTable {
private:

	friend class MOAILuaRef;

	int		mTableID;
	
	static const u32		MAX_REF_ID = 0xffffffff;
	static const u32		NO_REF = 0xffffffff;

	static const u32		REFID_CHUNK_SIZE = 1024;
	ZLLeanArray < u32 >		mRefIDStack;
	u32						mRefIDStackTop;

	//----------------------------------------------------------------//
	void		ReleaseRefID		( int refID );
	int			ReserveRefID		();

public:

	//----------------------------------------------------------------//
	void		Clear				();
	void		InitStrong			();
	void		InitWeak			();
				MOAILuaRefTable		();
				~MOAILuaRefTable	();
	void		PushRef				( MOAILuaState& state, int refID );
	void		PushRefTable		( MOAILuaState& state );
	int			Ref					( MOAILuaState& state, int idx );
	void		Unref				( int refID );
};

#endif
