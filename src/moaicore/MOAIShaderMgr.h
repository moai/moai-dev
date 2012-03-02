// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGR_H
#define	MOAISHADERMGR_H

class MOAIShader;

//================================================================//
// MOAIShaderMgr
//================================================================//
class MOAIShaderMgr :
	public MOAIGlobalClass < MOAIShaderMgr > {
public:

	enum {
		DECK2D_SHADER,
		FONT_SHADER,
		FONT_RGB_SHADER,
		LINE_SHADER,
		MESH_SHADER,
		TOTAL_SHADERS,
	};

private:
	
	MOAIShader* mShaders [ TOTAL_SHADERS ];
	
public:
	
	//----------------------------------------------------------------//
	void				BindShader			( u32 shaderID );
	MOAIShader&			GetShader			( u32 shaderID );
						MOAIShaderMgr		();
						~MOAIShaderMgr		();
};

#endif
