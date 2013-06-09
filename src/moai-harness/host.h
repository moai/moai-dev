//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_DEBUGGER_H
#define AKU_DEBUGGER_H

#include <moai-core/host.h>

AKU_API void	AKUFinalizeDebugger			();
AKU_API void    AKUHandleError				( const char* message, struct lua_State* L, int level );
AKU_API void	AKUInitializeDebugger		();
AKU_API void	AKUUpdateDebugger			();

#endif
