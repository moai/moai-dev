// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_version_author.h>
#include <moai-core/MOAIVersion.h>
#include <moai-core/moai_version.h>

//================================================================//
// MOAIVersion
//================================================================//

//----------------------------------------------------------------//
STLString MOAIVersion::GetVersionString () {

	STLString version;
	
	version.write ( "Moai SDK %d.%d",
		MOAI_SDK_VERSION_MAJOR,
		MOAI_SDK_VERSION_MINOR
	);
	
	int revision = MOAI_SDK_VERSION_REVISION; // vs2015 C4127
	if ( revision > 0 ) {
		version.write ( ".%d", MOAI_SDK_VERSION_REVISION );
	}
	
	if ( strlen ( MOAI_SDK_VERSION_AUTHOR ) > 0 ) {
		version.write ( " (ad hoc build by %s)", MOAI_SDK_VERSION_AUTHOR );
	}
	
	#ifdef MOAI_SDK_COMMIT
		version.write ( " %s", MOAI_SDK_COMMIT );
	#endif
	
	return version;
}
