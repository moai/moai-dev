//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_TEST_H
#define AKU_MOAIEXT_TEST_H

#include <aku/AKU.h>

AKU_API void	AKUTestInit					();
AKU_API void	AKUTestRunScript			( const char* filename );
AKU_API void	AKUTestSetResultsFile		( const char* filename );
AKU_API void	AKUTestSetStaging			();

#endif
