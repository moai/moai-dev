//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_MOAIEXT_DEBUGGER_H
#define AKU_MOAIEXT_DEBUGGER_H

SUPPRESS_EMPTY_FILE_WARNING
#if DEBUG

#include <aku/AKU.h>

AKU_API void    AKUDebugHarnessHandleError  ( const char* message, struct lua_State* L, int level );
AKU_API void	AKUDebugHarnessInit			();
AKU_API void	AKUDebugHarnessUpdate		();

#endif

#endif