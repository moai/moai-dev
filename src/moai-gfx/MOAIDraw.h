// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

#include <moai-gfx/MOAIDrawAPI.h>

class MOAIGfxMgr;

//================================================================//
// MOAIDraw
//================================================================//
// TODO: doxygen
class MOAIDraw :
	public virtual MOAIAbstractCmdHandlerWithAPI < MOAIDrawAPI >,
	public virtual ZLContextClass {
protected:

	//----------------------------------------------------------------//
	void			ExecuteCompose					( MOAIGfxMgr& gfxMgr, MOAIShader* shader );
	void			ExecuteDebugTri					( MOAIGfxMgr& gfxMgr );
	void			ExecuteDrawAnimCurve			( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAnimCurve& param );
	void 			ExecuteDrawAxis2D 				( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAxis2D& param );
	void 			ExecuteDrawAxisGrid2D			( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawAxisGrid2D& param );
	void 			ExecuteDrawLine 				( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawLine& param );
	void 			ExecuteDrawPoint 				( MOAIGfxMgr& gfxMgr, const ZLVec3D& param );
	void 			ExecuteDrawRay 					( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawRay& param );
	void 			ExecuteDrawTriangle				( MOAIGfxMgr& gfxMgr, const MOAIDrawAPIParam::DrawTriangle& param );

	//----------------------------------------------------------------//
	void			MOAIAbstractCmdHandler_HandleCommand		( u32 cmd, const void* param );

public:

	DECL_LUA_SINGLETON ( MOAIDraw )

	//----------------------------------------------------------------//
					MOAIDraw						();
					~MOAIDraw						();
};

#endif
