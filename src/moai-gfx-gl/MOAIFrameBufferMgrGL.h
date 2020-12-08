// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERMGRGL_H
#define	MOAIFRAMEBUFFERMGRGL_H

class MOAIFrameBufferGL;
class MOAIFrameBufferAttachmentGL;

//================================================================//
// MOAIFrameBufferAttachmentFactory
//================================================================//
// TODO: doxygen
class MOAIFrameBufferAttachmentFactory :
	public virtual ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual MOAIFrameBufferAttachmentGL*	MOAIFrameBufferAttachmentFactory_MakeAttachment		( u32 factoryID, u32 width, u32 height );

public:
	
	//----------------------------------------------------------------//
	MOAIFrameBufferAttachmentGL*		MakeAttachment							( u32 factoryID, u32 width, u32 height );
										MOAIFrameBufferAttachmentFactory		();
	virtual								~MOAIFrameBufferAttachmentFactory 		();
};

//================================================================//
// MOAIFrameBufferAttachmentKey
//================================================================//
// TODO: doxygen
class MOAIFrameBufferAttachmentKey {
public:

	u32				mWidth;
	u32				mHeight;
	u32				mFactoryID;
	
	//----------------------------------------------------------------//
	bool operator < ( const MOAIFrameBufferAttachmentKey& compare ) const {
		return ( memcmp ( this, &compare, sizeof ( MOAIFrameBufferAttachmentKey )) < 0 );
	}
};

//================================================================//
// MOAIFrameBufferMgrGL
//================================================================//
// TODO: doxygen
class MOAIFrameBufferMgrGL :
	public ZLContextClass < MOAIFrameBufferMgrGL >,
	public virtual MOAILuaObject {
private:

//	enum MOAIShaderPresetEnum {
//		DECK2D_SHADER,
//		DECK2D_SNAPPING_SHADER,
//		FONT_SHADER,
//		FONT_SNAPPING_SHADER,
//		FONT_EFFECTS_SHADER,
//		LINE_SHADER,
//		LINE_SHADER_3D,
//		MESH_SHADER,
//		ONETRI_SHADER,
//		TOTAL_SHADERS,
//		
//		UNKNOWN_SHADER = 0xffffffff,
//	};
	
	STLMap < u32, ZLStrongPtr < MOAIFrameBufferAttachmentFactory > > 		mAttachmentFactories;
	
	STLMap < MOAIFrameBufferAttachmentKey, MOAIFrameBufferAttachmentGL* > 	mAttachmentPool;
	STLSet < MOAIFrameBuffer* >												mFrameBufferPool;
	
	//----------------------------------------------------------------//
	static int		_getAttachment				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIFrameBufferMgrGL )
	
	//----------------------------------------------------------------//
					MOAIFrameBufferMgrGL		();
					~MOAIFrameBufferMgrGL		();
};

#endif
