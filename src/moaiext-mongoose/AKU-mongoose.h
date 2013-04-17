//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_FMOD_DESIGNER_H
#define AKU_MOAIEXT_FMOD_DESIGNER_H

#include <aku/AKU.h>

AKU_API void	AKUFmodDesignerInit			        ();
AKU_API void	AKUFmodDesignerMuteSystem			( bool mute );
AKU_API void	AKUFmodDesignerRelease			    ();
AKU_API void	AKUFmodDesignerRestoreSession		();
AKU_API void	AKUFmodDesignerUpdate				( float fDeltaTime );

#endif
