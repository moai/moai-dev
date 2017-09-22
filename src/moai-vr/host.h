//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_VR_HOST_H
#define MOAI_VR_HOST_H

#include <moai-core/host.h>

AKU_API void	AKUVrAppFinalize			();
AKU_API void	AKUVrAppInitialize			();
AKU_API void	AKUVrContextInitialize		();
AKU_API void	AKUVrPause					( bool pause );
AKU_API void	AKUVrUpdate					();

#endif
