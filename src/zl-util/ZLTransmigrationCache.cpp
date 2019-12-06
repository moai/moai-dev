// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLTransmigrationCache.h>

//================================================================//
// ZLTransmigrationCache
//================================================================//
	
//----------------------------------------------------------------//
void ZLTransmigrationCache::BeginTransmigration () {

	this->mActiveCount++;
}

//----------------------------------------------------------------//
void ZLTransmigrationCache::EndTransmigration () {

	--this->mActiveCount;

	if ( this->mActiveCount == 0 ) {
		STLSet < ZLAbstractTransmigrationMemoMap* >::iterator cleanupSetIt = this->mCleanupSet.begin ();
		for ( ; cleanupSetIt != this->mCleanupSet.end (); ++cleanupSetIt ) {
			( *cleanupSetIt )->Clear ();
		}
		this->mCleanupSet.clear ();
	}
}

//----------------------------------------------------------------//
ZLTransmigrationCache& ZLTransmigrationCache::Get () {

	return ZLThreadLocalPtr < ZLTransmigrationCache >::Singleton ();
}

//----------------------------------------------------------------//
bool ZLTransmigrationCache::IsActive () const {

	return ( this->mActiveCount > 0 );
}

//----------------------------------------------------------------//
ZLTransmigrationCache::ZLTransmigrationCache () :
	mActiveCount ( 0 ) {
}
