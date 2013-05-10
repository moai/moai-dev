// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_version.h>
#include <moai-core/MOAIVersion.h>

//================================================================//
// MOAIVersion
//================================================================//

//----------------------------------------------------------------//
STLString MOAIVersion::GetVersionString () {

	STLString version;
	
	version.write ( "Moai SDK %g", ( float )MOAI_SDK_VERSION_MAJOR_MINOR );
	
	#if MOAI_SDK_VERSION_REVISION >= 0
		version.write ( " revision %d", MOAI_SDK_VERSION_REVISION );
		
		if ( strlen ( MOAI_SDK_VERSION_AUTHOR ) > 0 ) {
			version.write ( " (%s)", MOAI_SDK_VERSION_AUTHOR );
		}
	#else
		if ( strlen ( MOAI_SDK_VERSION_AUTHOR ) > 0 ) {
			version.write ( " (ad hoc build by %s)", MOAI_SDK_VERSION_AUTHOR );
		}
	#endif
	
	return version;
}
