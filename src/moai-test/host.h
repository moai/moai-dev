//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_TEST_HOST_H
#define MOAI_TEST_HOST_H

#include <moai-core/host.h>

AKU_API void	AKUTestAppFinalize					();
AKU_API void	AKUTestAppInitialize				();
AKU_API void	AKUTestContextInitialize			();

AKU_API void	AKUTestRunScript					( const char* filename );
AKU_API void	AKUTestRunTest						( const char* testname );
AKU_API void	AKUTestSetFilter					( const char* filter );
AKU_API void	AKUTestSetFilterFile				( const char* filename );
AKU_API void	AKUTestSetResultsFile				( const char* filename );
AKU_API void	AKUTestSetXmlResultsFile			( const char* filename );
AKU_API void	AKUTestSetStaging					();

#endif