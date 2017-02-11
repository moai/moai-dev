// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALHOLDERMIXIN_H
#define	MOAIMATERIALHOLDERMIXIN_H

#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAITexture.h>

//================================================================//
// MOAIMaterialHolderMixIn
//================================================================//
// TODO: doxygen
template < typename TYPE >
class MOAIMaterialHolderMixIn :
	public virtual MOAILuaObject {
protected:
	
	MOAIMaterial mMaterial;

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getBlendMode ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		return self->mMaterial.GetBlendMode ().Push ( state );
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getCullMode ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		state.Push ( self->mMaterial.GetCullMode ());
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getDepthMask ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		state.Push ( self->mMaterial.GetDepthMask ());
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getDepthTest ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		state.Push ( self->mMaterial.GetDepthTest ());
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getLight ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "UN" )
		
		u32 globalID = state.GetValue < u32 >( 2,  MOAI_UNKNOWN_MATERIAL_GLOBAL );
		state.Push ( self->mMaterial.GetLight ( globalID ));
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getShader ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		state.Push ( self->mMaterial.GetShader ());
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _getTexture ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		
		u32 globalID = state.GetValue < u32 >( 2,  MOAI_UNKNOWN_MATERIAL_GLOBAL );
		state.Push ( self->mMaterial.GetTexture ( globalID ));
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setBlendMode ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )

		MOAIBlendMode blendMode;
		blendMode.Init ( state, 2 );
		
		self->mMaterial.SetBlendMode ( blendMode );
		return 0;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setCullMode ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		self->mMaterial.SetCullMode ( state.GetValue < int >( 2, true ));
		return 0;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setDepthMask ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		self->mMaterial.SetDepthTest ( state.GetValue < bool >( 2, true ));
		return 0;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setDepthTest ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		self->mMaterial.SetDepthTest ( state.GetValue < int >( 2, 0 ));
		return 0;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setLight ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		
		u32 globalID = state.GetValue < u32 >( 2, MOAI_UNKNOWN_MATERIAL_GLOBAL );
		MOAILight* light = state.GetLuaObject < MOAILight >( 3, true );
		self->mMaterial.SetLight ( globalID, light );
		state.Push ( light );
		
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setShader ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		
		MOAIShader* shader = MOAIShader::AffirmShader ( state, 2 );
		self->mMaterial.SetShader ( shader );
		state.Push ( shader );
		
		return 1;
	}

	//----------------------------------------------------------------//
	// TODO: doxygen
	static int _setTexture ( lua_State* L ) {
		MOAI_LUA_SETUP ( TYPE, "U" )
		
		u32 idx = 2;
		u32 globalID = MOAI_UNKNOWN_MATERIAL_GLOBAL;
		
		if ( state.IsType ( 2, LUA_TNUMBER )) {
			globalID = state.GetValue < u32 >( idx++, globalID );
		}
		
		MOAITextureBase* texture = MOAITexture::AffirmTexture ( state, idx );
		self->mMaterial.SetTexture ( globalID, texture );
		state.Push ( texture );
		
		return 1;
	}

	//----------------------------------------------------------------//
	MOAIMaterialHolderMixIn () {
	}
	
	//----------------------------------------------------------------//
	~MOAIMaterialHolderMixIn () {
	}

	//----------------------------------------------------------------//
	void RegisterLuaFuncs ( MOAILuaState& state ) {

		luaL_Reg regTable [] = {
			{ "getBlendMode",			_getBlendMode },
			{ "getCullMode",			_getCullMode },
			{ "getDepthMask",			_getDepthMask },
			{ "getDepthTest",			_getDepthTest },
			{ "getLight",				_getLight },
			{ "getShader",				_getShader },
			{ "getTexture",				_getTexture },
			{ "setBlendMode",			_setBlendMode },
			{ "setCullMode",			_setCullMode },
			{ "setDepthMask",			_setDepthMask },
			{ "setLight",				_setLight },
			{ "setShader",				_setShader },
			{ "setTexture",				_setTexture },
			{ NULL, NULL }
		};
		
		luaL_register ( state, 0, regTable );
	}

public:

	GET_CONST ( MOAIMaterial*, Material, &mMaterial )
};

#endif
