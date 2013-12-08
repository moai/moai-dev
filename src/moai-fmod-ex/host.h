//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_FMOD_EX_H
#define AKU_FMOD_EX_H

#include <moai-core/host.h>

AKU_API void	AKUFmodExInit				();
AKU_API void	AKUFmodExMuteSystem			( bool mute );
AKU_API void	AKUFmodExRelease			();
AKU_API void	AKUFmodExRestoreSession		();
AKU_API void	AKUFmodExUpdate				();

#endif
