//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_FMOD_EX_H
#define AKU_MOAIEXT_FMOD_EX_H

#include <aku/AKU.h>

AKU_API void	AKUFmodExInit				();
AKU_API void	AKUFmodExMuteSystem			( bool mute );
AKU_API void	AKUFmodExRelease			();
AKU_API void	AKUFmodExRestoreSession		();
AKU_API void	AKUFmodExUpdate				();

#endif
