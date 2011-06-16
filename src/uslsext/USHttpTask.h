// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHTTPTASK_H
#define USHTTPTASK_H

#include <uslsext/USData.h>
#include <uslsext/USDelegate.h>

class USFileCache;
class USHttpTaskInfo;

//================================================================//
// USHttpTask
//================================================================//
class USHttpTask :
	public virtual USObject {
private:

	u32					mTaskID;
	USHttpTaskInfo*		mInfo;

	void*				mBytes;
	u32					mSize;

	bool				mVerbose;
	u32					mResponseCode;

	USCallback < USHttpTask* > mCallback;

	//----------------------------------------------------------------//
	void	CacheFile			();
	void	CacheFileFinish		( USDataIOTask* task );
	void	Clear				();
	void	Finish				();
	void	LoadFile			( cc8* filename );
	void	LoadFileFinish		( USDataIOTask* task );
			USHttpTask			( const USHttpTask& task );

public:

	friend class USUrlMgr;
	
	GET ( u32, ResponseCode, mResponseCode )
	
	//----------------------------------------------------------------//
	void		Cancel				();
	void*		GetData				();
	void		GetData				( void* buffer, u32 size );
	u32			GetSize				();
	void		HttpGet				( cc8* url, cc8* useragent, bool verbose );
	void		HttpPost			( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose );
				USHttpTask			();
				~USHttpTask			();
	
	//----------------------------------------------------------------//
	template < typename TARGET >
	void SetDelegate ( TARGET* target, UNARY_SELECTOR_DECL ( TARGET, USHttpTask*, func )) {
		
		this->mCallback.template Set < TARGET >( target, func );
	}
};

#endif
