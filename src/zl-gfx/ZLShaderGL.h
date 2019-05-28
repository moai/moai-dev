// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADERGL_H
#define	ZLSHADERGL_H

#include <zl-gfx/ZLAbstractShader.h>
#include <zl-gfx/ZLShaderProgramGL.h>
#include <zl-gfx/ZLAbstractShaderUniformSchemaGL.h>

//================================================================//
// ZLShaderGL
//================================================================//
/**	@lua	ZLShaderGL
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class ZLShaderGL :
	public virtual ZLAbstractShader,
	public virtual ZLRefCountedObject {
protected:

	friend class ZLGfxStateGPUCacheGL;
	friend class ZLShaderProgramGL;

	ZLStrongPtr < ZLShaderProgramGL >		mProgram;
	ZLLeanArray < u8 >					mPendingUniformBuffer;

	//----------------------------------------------------------------//
	bool					IsDirty					();

public:

	GET ( ZLShaderProgramGL*, Program, mProgram )

	//----------------------------------------------------------------//
	void					ApplyUniforms			();
	void					BindUniforms			();
	void					Bless					();
	bool					HasDirtyUniforms		();
	void					ResizeUniformArray		( u32 uniformID, u32 count );
	void					ScheduleTextures		();
	void					SetProgram				( ZLShaderProgramGL* program );
	void					UpdateUniforms			();
							ZLShaderGL				();
							~ZLShaderGL				();
};

#endif
