// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNIFORMCOMPOSER_H
#define	MOAIUNIFORMCOMPOSER_H

#include <moai-gfx/MOAIUniformComposerInterface.h>
#include <moai-gfx/MOAITexture.h>

class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

//================================================================//
// MOAIUniformComposerGlobalRef
//================================================================//
class MOAIUniformComposerGlobalRef {
private:

	friend class MOAIUniformComposer;
	friend class MOAIUniformComposerInterface;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
			MOAIUniformComposerGlobalRef		();
};

//================================================================//
// MOAIUniformComposerTextureRef
//================================================================//
class MOAIUniformComposerTextureRef {
private:

	friend class MOAIUniformComposer;
	friend class MOAIUniformComposerInterface;

	u32								mName;
	ZLIndex							mUnit;
	ZLStrongPtr < MOAITexture >		mTexture;

public:

	//----------------------------------------------------------------//
			MOAIUniformComposerTextureRef		();
};

//================================================================//
// MOAIUniformComposer
//================================================================//
// TODO: doxygen
class MOAIUniformComposer :
	public virtual MOAIUniformComposerInterface {
protected:

	friend class MOAIUniformComposerInterface;

	ZLLeanArray < MOAIUniformComposerGlobalRef >	mGlobals;
	ZLLeanArray < MOAIUniformComposerTextureRef >	mTextures;

	//----------------------------------------------------------------//
	void						MOAILuaObject_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIUniformComposer&		MOAIUniformComposerInterface_AffirmComposer		();
	MOAIUniformComposer*		MOAIUniformComposerInterface_GetComposer		();

public:

	DECL_LUA_FACTORY ( MOAIUniformComposer )

	//----------------------------------------------------------------//
	void			ComposeUniforms				( const MOAIUniformSchema& schema, MOAIAbstractUniformBuffer& buffer ) const;
					MOAIUniformComposer			();
					~MOAIUniformComposer		();
	void			SelectTextures				();
};

#endif
