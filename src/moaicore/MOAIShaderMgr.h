// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGR_H
#define	MOAISHADERMGR_H

class MOAIShader;

//================================================================//
// MOAIShaderMgr
//================================================================//
class MOAIShaderMgr :
	public USGlobalClass < MOAIShaderMgr > {
public:

	enum {
		MOAI_BASIC_NO_TEXTURE,
		MOAI_BASIC_ONE_TEXTURE,
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
