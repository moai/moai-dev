// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTMATERIALGLOBALSCONTEXT_H
#define	MOAIABSTRACTMATERIALGLOBALSCONTEXT_H

class MOAILight;
class MOAITexture;

//================================================================//
// MOAIMaterialGlobals
//================================================================//
class MOAIMaterialGlobals {
public:

	static const u32 MOAI_UNKNOWN_MATERIAL_GLOBAL	= 0x80000000;
	static const u32 MAX_GLOBAL_LIGHTS				= 256;
	static const u32 MAX_GLOBAL_TEXTURES			= 32;

	//----------------------------------------------------------------//
	static u32			GetNamedGlobalID						( MOAILuaState& state, int idx );
};

//================================================================//
// MOAIMaterial
//================================================================//
// TODO: doxygen
class MOAIAbstractMaterialGlobalsContext :
	public ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual void			MOAIAbstractMaterialGlobalsContext_Apply			( MOAIAbstractMaterialGlobalsContext& dest ) = 0;
	virtual MOAILight*		MOAIAbstractMaterialGlobalsContext_Clear			() = 0;
	virtual MOAILight*		MOAIAbstractMaterialGlobalsContext_GetLight			( u32 name ) = 0;
	virtual MOAITexture*	MOAIAbstractMaterialGlobalsContext_GetTexture		( u32 name ) = 0;
	virtual void			MOAIAbstractMaterialGlobalsContext_SetLight			( u32 name, MOAILight* light ) = 0;
	virtual void			MOAIAbstractMaterialGlobalsContext_SetTexture		( u32 name, MOAITexture* light ) = 0;

public:

	

	//----------------------------------------------------------------//
	void				Apply									( MOAIAbstractMaterialGlobalsContext& dest );
	void				Clear									();
	MOAILight*			GetLight								( u32 name );
	MOAITexture*		GetTexture								( u32 name );
						MOAIAbstractMaterialGlobalsContext		();
	virtual				~MOAIAbstractMaterialGlobalsContext		();
	void				SetLight								( u32 name );
	void				SetLight								( u32 name, MOAILight* light );
	void				SetTexture								( u32 name );
	void				SetTexture								( u32 name, MOAITexture* texture );
};

#endif
