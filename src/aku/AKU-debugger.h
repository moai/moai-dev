//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_DEBUGGER_H
#define AKU_MOAIEXT_DEBUGGER_H

#include <aku/AKU.h>

AKU_API void    AKUDebugHarnessHandleError  ( const char* message, struct lua_State* L, int level );
AKU_API void	AKUDebugHarnessInit			();
AKU_API void	AKUDebugHarnessUpdate		();

#endif