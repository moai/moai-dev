//  Created by ProbablyInteractive.
//  Copyright 2009 Probably Interactive. All rights reserved.

#ifndef WAX_H
#define WAX_H

#ifdef __cplusplus
	extern "C" {
#endif

#import "lua.h"

#define WAX_VERSION 0.93

void			wax_setup				( lua_State* L );
lua_State*		wax_currentLuaState		();
void			wax_end					();

#ifdef __cplusplus
	}	// extern "C"
#endif

#endif
