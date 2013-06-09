// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERENTRYBASE_H
#define	MOAIPROFILERENTRYBASE_H

//================================================================//
// MOAIProfilerEntryBase
//================================================================//
class MOAIProfilerEntryBase {
protected:

	//----------------------------------------------------------------//
	virtual void			CopyFrom				( const MOAIProfilerEntryBase& other );
							MOAIProfilerEntryBase	( MOAIProfilerEntryBase* parent, const ZLHashedString& name );
	virtual void			SwapWith				( MOAIProfilerEntryBase& other ) = 0;

private:
	
	//----------------------------------------------------------------//
	void						DebugChildren	( std::string& childNames ) const;
	static void					SwapChildren	( MOAIProfilerEntryBase& left, MOAIProfilerEntryBase& right );
	
public:

	MOAIProfilerEntryBase*		mNext;
	ZLHashedString				mName;
	u16							mDepth;
	MOAIProfilerEntryBase*		mParent;
	MOAIProfilerEntryBase*		mFirstChild;
	MOAIProfilerEntryBase*		mLastChild;
	u32							mNumChildren;
	
	//----------------------------------------------------------------//
	void						AddChildEntry			( MOAIProfilerEntryBase* child );
	virtual bool				IsLessThan				( const MOAIProfilerEntryBase& other ) const = 0;
	virtual						~MOAIProfilerEntryBase	() {}
	virtual void				Reset					();
	void						SortChildren			();
};

#endif
