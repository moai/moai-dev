// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADERPROGRAM_H
#define	ZLSHADERPROGRAM_H

#include <zl-gfx/ZLAbstractGfxResource.h>
#include <zl-gfx/ZLAbstractShaderUniformSchema.h>
#include <zl-gfx/ZLGfxHandle.h>
#include <zl-gfx/ZLShaderUniform.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class ZLShader;
class ZLTexture;

//================================================================//
// ZLShaderProgramGlobal
//================================================================//
class ZLShaderProgramGlobal {
private:

	friend class MOAIShader;
	friend class MOAIShaderGlobals;
	friend class ZLShaderProgram;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
		ZLShaderProgramGlobal		();
};

//================================================================//
// ZLShaderProgramTexture
//================================================================//
class ZLShaderProgramTexture {
private:

	friend class ZLShaderProgram;

	u32							mName;
	ZLIndex						mUnit;
	ZLStrongPtr < ZLTexture >	mTexture;

public:

	//----------------------------------------------------------------//
		ZLShaderProgramTexture	();
};


//================================================================//
// ZLShaderProgram
//================================================================//
/**	@lua	ZLShaderProgram
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShader instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class ZLShaderProgram :
	public virtual ZLAbstractGfxResource,
	public virtual ZLAbstractShaderUniformSchema {
protected:

	friend class ZLShader;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle		mProgram;
	ZLGfxHandle		mVertexShader;
	ZLGfxHandle		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < ZLShaderUniform >				mUniforms;
	ZLLeanArray < ZLShaderProgramGlobal >		mGlobals;
	ZLLeanArray < ZLShaderProgramTexture >		mTextures;

	size_t										mMaxCount;
	size_t										mUniformBufferSize;

	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
	void					AffirmUniforms				();
	void					ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void					BindUniforms				();
	ZLGfxHandle				CompileShader				( u32 type, cc8* source );
	ZLShaderUniform*		GetUniform					( ZLIndex uniformID );
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
	ZLShaderUniformHandle		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const;
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
	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, ZLTexture* fallback );
	void				SetTexture					( ZLIndex idx, ZLTexture* texture, ZLIndex unit );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						ZLShaderProgram				();
						~ZLShaderProgram			();
};

#endif
