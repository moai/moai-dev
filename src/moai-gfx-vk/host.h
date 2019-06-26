//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_GFX_VK_HOST_H
#define MOAI_GFX_VK_HOST_H

#include <moai-core/host.h>

// setup
AKU_API void			AKUGfxVKAppFinalize				();
AKU_API void			AKUGfxVKAppInitialize			();
AKU_API void			AKUGfxVKContextInitialize		();

AKU_API void			AKUDetectFramebuffer			();
AKU_API void			AKUDetectGfxContext				();
AKU_API void			AKUDiscardGfxResources			();

AKU_API void			AKUDisplayListBeginPhase		( int phase );
AKU_API void			AKUDisplayListEnable			( int list );
AKU_API void			AKUDisplayListEndPhase			( int phase );
AKU_API bool			AKUDisplayListHasContent		( int list );
AKU_API bool			AKUDisplayListIsEnabled			( int list );
AKU_API void			AKUDisplayListProcess			( int list );
AKU_API void			AKUDisplayListPublishAndReset	();

#endif
