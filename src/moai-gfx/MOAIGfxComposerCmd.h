// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERCMD_H
#define	MOAIGFXCOMPOSERCMD_H

class MOAIAbstractGfxComposerCallable;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIGfxComposerAddrModeEnum
//================================================================//
struct MOAIGfxComposerAddrModeEnum {
	enum _ {
		NONE,
		FROM_MATERIAL_MGR,
		FROM_OBJECT,
		FROM_PIPELINE_GLOBALS,
		TO_GFX_STATE,
		TO_MATERIAL_MGR,
		TO_SHADER_BODY,
		UNKNOWN,
	};
};

//================================================================//
// MOAIGfxComposerCmdEnum
//================================================================//
struct MOAIGfxComposerCmdEnum {
	enum _ {
		NONE,
		CALL,
		CALL_FROM_SHADER,
		SHADER,
		TEXTURE,
		UNIFORM,
	};
};

//================================================================//
// MOAIGfxComposerCmd
//================================================================//
// TODO: doxygen
class MOAIGfxComposerCmd :
	public virtual ZLRefCountedObject {
protected:

	friend class MOAIGfxComposer;
	friend class MOAIGfxComposerInterface;

	// this is a nasty omnibus object because I am too lazy to piece it out right now.
	// or devise yet another ad hoc bytecode command scheme.

	// TODO: devise yet another ad hoc bytecode command scheme.

	ZLStrongPtr < MOAIShader > 		mShader;
	ZLStrongPtr < MOAITexture > 	mTexture;
	ZLIndex 						mTextureUnit;

	ZLIndex							mPipelineGlobalID;
	ZLIndex							mTargetUniformID;
	ZLIndex							mTargetUniformIndex;

	MOAIGfxComposerCmdEnum::_		mType;
	MOAIGfxComposerAddrModeEnum::_ 	mAddressingModeFrom;
	MOAIGfxComposerAddrModeEnum::_ 	mAddressingModeTo;

	//----------------------------------------------------------------//
	static void 	ExecuteCall 					( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
	void			ExecuteUniform 					();

public:

	//----------------------------------------------------------------//
	bool			Execute							( MOAIAbstractGfxComposerCallable& callable );
					MOAIGfxComposerCmd 				();
					MOAIGfxComposerCmd 				( MOAIGfxComposerCmdEnum::_ type, MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to );
					~MOAIGfxComposerCmd 			();
};

#endif
