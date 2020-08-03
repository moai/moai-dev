// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_build.h>
#include <moai_version.h>
#include <moai_version_author.h>
#include <moai-core/MOAIVersion.h>

//================================================================//
// MOAIVersion
//================================================================//

//----------------------------------------------------------------//
STLString MOAIVersion::GetAuthorString () {

	STLString version;
	version.write ( "%s, %s", MOAI_GIT_USER_NAME, MOAI_BUILD_DATE_STR );
	return version;
}

//----------------------------------------------------------------//
STLString MOAIVersion::GetCommitString () {
	
	return MOAI_GIT_COMMIT_STR;
}

//----------------------------------------------------------------//
STLString MOAIVersion::GetVersionString () {

	STLString version;
	
	version.write ( "Moai SDK %d.%d.%d",
		MOAI_SDK_VERSION_MAJOR,
		MOAI_SDK_VERSION_MINOR,
		MOAI_SDK_VERSION_REVISION
	);
		
	return version;
}
