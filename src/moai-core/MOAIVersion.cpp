// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_version_author.h>
#include <moai_version_major.h>
#include <moai_version_minor.h>
#include <moai-core/MOAIVersion.h>

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
	
	if ( MOAI_SDK_VERSION_REVISION > 0 ) {
		version.write ( ".%d", MOAI_SDK_VERSION_REVISION );
	}
	
	if ( strlen ( MOAI_SDK_VERSION_AUTHOR ) > 0 ) {
		version.write ( " (ad hoc build by %s)", MOAI_SDK_VERSION_AUTHOR );
	}
	return version;
}
