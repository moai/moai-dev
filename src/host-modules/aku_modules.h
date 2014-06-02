// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_H
#define	AKU_MODULES_H

#include <host-modules/aku_modules_config.h>

#if AKU_WITH_BOX2D
	#include <moai-box2d/host.h>
#endif

#if AKU_WITH_CHIPMUNK
	#include <moai-chipmunk/host.h>
#endif

#if AKU_WITH_FMOD_DESIGNER
	#include <moai-fmod-designer/host.h>
#endif

#if AKU_WITH_FMOD_EX
	#include <moai-fmod-ex/host.h>
#endif

#if AKU_WITH_HARNESS
	#include <moai-harness/host.h>
#endif

#if AKU_WITH_HTTP_CLIENT
	#include <moai-http-client/host.h>
#endif

#if AKU_WITH_HTTP_SERVER
	#include <moai-http-server/host.h>
#endif

#if AKU_WITH_LUAEXT
	#include <moai-luaext/host.h>
#endif

#if AKU_WITH_SIM
	#include <moai-sim/host.h>
#endif

#if AKU_WITH_TEST
	#include <moai-test/host.h>
#endif

#if AKU_WITH_UNTZ
	#include <moai-untz/host.h>
#endif

#if AKU_WITH_UTIL
	#include <moai-util/host.h>
#endif

#if AKU_WITH_PLUGINS
	#include <moai-plugins/host.h>
#endif
//----------------------------------------------------------------//
void		AKUModulesAppFinalize					();
void		AKUModulesAppInitialize					();
void		AKUModulesContextInitialize				();
void		AKUModulesParseArgs						( int argc, char** argv );
void		AKUModulesRunLuaAPIWrapper				();
void		AKUModulesUpdate						();
void		AKUModulesPause							();
void		AKUModulesResume						();

#endif
