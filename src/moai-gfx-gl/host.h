//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_GFX_GL_HOST_H
#define MOAI_GFX_GL_HOST_H

#include <moai-core/host.h>

// setup
AKU_API void			AKUGfxGLAppFinalize				();
AKU_API void			AKUGfxGLAppInitialize			();
AKU_API void			AKUGfxGLContextInitialize		( AKUContextID contextID );

AKU_API void			AKUDetectFramebuffer			( AKUContextID contextID );
AKU_API void			AKUDetectGfxContext				( AKUContextID contextID );
AKU_API void			AKUDiscardGfxResources			( AKUContextID contextID );

AKU_API void			AKUDisplayListBeginPhase		( AKUContextID contextID, int phase );
AKU_API void			AKUDisplayListEnable			( AKUContextID contextID, int list );
AKU_API void			AKUDisplayListEndPhase			( AKUContextID contextID, int phase );
AKU_API bool			AKUDisplayListHasContent		( AKUContextID contextID, int list );
AKU_API bool			AKUDisplayListIsEnabled			( AKUContextID contextID, int list );
AKU_API void			AKUDisplayListProcess			( AKUContextID contextID, int list );
AKU_API void			AKUDisplayListPublishAndReset	( AKUContextID contextID );

#endif
