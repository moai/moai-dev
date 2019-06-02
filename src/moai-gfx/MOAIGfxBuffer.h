// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-gfx/MOAIGfxResource.h>

//================================================================//
// MOAIGfxBuffer
//================================================================//
class MOAIGfxBuffer :
	public virtual MOAIGfxResource,
	public virtual MOAIStream,
	public virtual ZLCopyOnWrite {
protected:

	//----------------------------------------------------------------//
	static int			_copyFromStream				( lua_State* L );
	static int			_reserve					( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAIGfxResource_OnCPUDestroy	();
	
public:

	//----------------------------------------------------------------//
	void				CopyFromStream				( ZLStream& stream, size_t size );
						MOAIGfxBuffer				();
						~MOAIGfxBuffer				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Reserve						( ZLSize size );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
