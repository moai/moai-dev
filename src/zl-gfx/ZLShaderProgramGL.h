// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADERPROGRAMGL_H
#define	ZLSHADERPROGRAMGL_H

#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLAbstractShaderUniformSchemaGL.h>
#include <zl-gfx/ZLGfxHandleGL.h>
#include <zl-gfx/ZLShaderUniformGL.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class ZLShaderGL;
class ZLTextureBaseGL;

//================================================================//
// ZLShaderProgramGlobalGL
//================================================================//
class ZLShaderProgramGlobalGL {
private:

	friend class MOAIShaderGL;
	friend class MOAIShaderGlobals;
	friend class ZLShaderProgramGL;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
		ZLShaderProgramGlobalGL		();
};

//================================================================//
// ZLShaderProgramTextureGL
//================================================================//
class ZLShaderProgramTextureGL {
private:

	friend class ZLShaderProgramGL;

	u32							mName;
	ZLIndex						mUnit;
	ZLStrongPtr < ZLTextureBaseGL >	mTexture;

public:

	//----------------------------------------------------------------//
		ZLShaderProgramTextureGL	();
};


//================================================================//
// ZLShaderProgramGL
//================================================================//
/**	@lua	ZLShaderProgramGL
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShaderGL instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class ZLShaderProgramGL :
	public virtual ZLAbstractGfxResourceGL,
	public virtual ZLAbstractShaderUniformSchemaGL {
protected:

	friend class ZLShaderGL;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandleGL		mProgram;
	ZLGfxHandleGL		mVertexShader;
	ZLGfxHandleGL		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < ZLShaderUniformGL >			mUniforms;
	ZLLeanArray < ZLShaderProgramGlobalGL >		mGlobals;
	ZLLeanArray < ZLShaderProgramTextureGL >	mTextures;

	size_t										mMaxCount;
	size_t										mUniformBufferSize;

	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
	void					AffirmUniforms				();
	void					ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void					BindUniforms				();
	ZLGfxHandleGL			CompileShader				( u32 type, cc8* source );
	ZLShaderUniformGL*		GetUniform					( ZLIndex uniformID );
	void					InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
	void					ScheduleTextures			();
	void					UpdateUniforms				( ZLLeanArray < u8 >& buffer );
	
	//----------------------------------------------------------------//
	bool						ZLAbstractGfxResource_OnCPUCreate					();
	void						ZLAbstractGfxResource_OnCPUDestroy					();
	void						ZLAbstractGfxResource_OnGPUBind						();
	bool						ZLAbstractGfxResource_OnGPUCreate					();
	void						ZLAbstractGfxResource_OnGPUDeleteOrDiscard			( bool shouldDelete );
	void						ZLAbstractGfxResource_OnGPUUnbind					();
	bool						ZLAbstractGfxResource_OnGPUUpdate					();
	ZLShaderUniformHandleGL		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const;
	void						ZLGfxListener_OnUniformLocation						( u32 addr, void* userdata );
	
public:

	static const u32 	MOAI_UNKNOWN_MATERIAL_GLOBAL 	= 0x80000000; // TODO: ZLGfx

	//----------------------------------------------------------------//
	void				Clear						();
	void				DeleteShaders				();
	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	void				Load						( cc8* vshSource, cc8* fshSource );
	void				ReserveAttributes			( ZLSize nAttributes );
	void				ReserveGlobals				( ZLSize nGlobals );
	void				ReserveTextures				( ZLSize nTextures );
	void				ReserveUniforms				( ZLSize nUniforms );
	void				SetGlobal					( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, ZLTextureBaseGL* fallback );
	void				SetTexture					( ZLIndex idx, ZLTextureBaseGL* texture, ZLIndex unit );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						ZLShaderProgramGL			();
						~ZLShaderProgramGL			();
};

#endif
