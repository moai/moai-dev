// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEXAMPLEDSLINTERFACE_H
#define	MOAIEXAMPLEDSLINTERFACE_H

#include <moai-core/MOAIAbstractDSLInterface.h>

//================================================================//
// MOAIExampleDSLInterface
//================================================================//
// TODO: doxygen
class MOAIExampleDSLInterface :
	public virtual MOAIAbstractDSLInterface {
protected:

	enum {
		CMD_FOO,
		CMD_BAR,
		CMD_BAZ,
	};

	//----------------------------------------------------------------//
	static int		_dsl_bar		( lua_State* L );
	static int		_dsl_baz		( lua_State* L );
	static int		_dsl_foo		( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAIAbstractDSLInterface_InitializeBinding		( MOAIAbstractDSLStream& stream ) const;
	void			MOAIAbstractDSLInterface_RegisterLuaAPI			( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	void			Bar				();
	void			Baz				();
	void			Foo				();
};

#endif
