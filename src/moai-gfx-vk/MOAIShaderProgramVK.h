// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMVK_H
#define	MOAISHADERPROGRAMVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>
#include <moai-gfx-vk/MOAIShaderUniformVK.h>

#define		OPENVK_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENVK_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBVK_PREPROC		"precision mediump int;\n precision mediump float;\n"

class MOAIShaderVK;
class MOAITextureVK;

////================================================================//
//// MOAIShaderProgramGlobalVK
////================================================================//
//class MOAIShaderProgramGlobalVK {
//private:
//
//	friend class MOAIShaderVK;
//	friend class MOAIShaderGlobals;
//	friend class MOAIShaderProgramVK;
//
//	u32			mGlobalID;
//	ZLIndex		mUniformID;
//	ZLIndex		mIndex;
//
//public:
//
//	//----------------------------------------------------------------//
//		MOAIShaderProgramGlobalVK		();
//};
//
////================================================================//
//// MOAIShaderProgramTextureVK
////================================================================//
//class MOAIShaderProgramTextureVK {
//private:
//
//	friend class MOAIShaderProgramVK;
//
//	u32							mName;
//	ZLIndex						mUnit;
//	ZLStrongPtr < MOAITextureVK >	mTexture;
//
//public:
//
//	//----------------------------------------------------------------//
//		MOAIShaderProgramTextureVK		();
//};

//================================================================//
// MOAIShaderProgramVK
//================================================================//
/**	@lua	MOAIShaderProgramVK
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShaderVK instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class MOAIShaderProgramVK :
	public virtual MOAIGfxResourceVK,
	public virtual MOAIUniformSchema {
protected:

//	friend class MOAIShaderVK;
//
//	STLString		mVertexShaderSource;
//	STLString		mFragmentShaderSource;
//
//	ZLGfxHandle		mProgram;
//	ZLGfxHandle		mVertexShader;
//	ZLGfxHandle		mFragmentShader;
//
//	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
//	STLMap < u32, STLString > mAttributeMap;
//
//	ZLLeanArray < MOAIShaderUniformVK >			mUniforms;
//	ZLLeanArray < MOAIShaderProgramGlobalVK >	mGlobals;
//	ZLLeanArray < MOAIShaderProgramTextureVK >	mTextures;
//
//	size_t										mMaxCount;
//	size_t										mUniformBufferSize;
//
//	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
//	static int					_declareUniform				( lua_State* L );
//	static int					_load						( lua_State* L );
//	static int					_reserveGlobals				( lua_State* L );
//	static int					_reserveTextures			( lua_State* L );
//	static int					_reserveUniforms			( lua_State* L );
//	static int					_setGlobal					( lua_State* L );
//	static int					_setTexture					( lua_State* L );
//	static int					_setVertexAttribute			( lua_State* L );

	//----------------------------------------------------------------//
//	void						AffirmUniforms				();
//	void						ApplyUniforms				( ZLLeanArray < u8 >& buffer );
//	void						BindUniforms				();
//	ZLGfxHandle					CompileShader				( u32 type, cc8* source );
//	MOAIShaderUniformVK*		GetUniform					( ZLIndex uniformID );
//	void						InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
//	int							ReserveGlobals 				( lua_State* L, int idx );
	
	//----------------------------------------------------------------//
	bool						MOAIGfxResource_OnCPUCreate							();
	void						MOAIGfxResource_OnCPUPurgeRecoverable						();
	void						MOAIGfxResourceVK_OnGPUBind							();
	bool						MOAIGfxResourceVK_OnGPUCreate						();
	void						MOAIGfxResourceVK_OnGPUDeleteOrDiscard				( bool shouldDelete );
	void						MOAIGfxResourceVK_OnGPUUnbind						();
	bool						MOAIGfxResourceVK_OnGPUUpdate						();
	void						MOAILuaObject_RegisterLuaClass						( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs						( MOAIComposer& composer, MOAILuaState& state );
	MOAIUniformHandle		MOAIAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const;
//	void						ZLGfxListener_OnUniformLocation						( u32 addr, void* userdata );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramVK )

	//----------------------------------------------------------------//
//	void				Clear						();
//	void				DeleteShaders				();
//	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
//	void				Load						( cc8* vshSource, cc8* fshSource );
//	void				ReserveAttributes			( ZLSize nAttributes );
//	void				ReserveGlobals				( ZLSize nGlobals );
//	void				ReserveTextures				( ZLSize nTextures );
//	void				ReserveUniforms				( ZLSize nUniforms );
//	void				SetGlobal					( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
//	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureVK* fallback );
//	void				SetTexture					( ZLIndex idx, MOAITextureVK* texture, ZLIndex unit );
//	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						MOAIShaderProgramVK			();
						~MOAIShaderProgramVK		();
};

#endif
