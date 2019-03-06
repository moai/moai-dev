// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADER_H
#define	ZLSHADER_H

#include <zl-gfx/ZLShaderProgram.h>
#include <zl-gfx/ZLAbstractShaderUniformSchema.h>

//================================================================//
// ZLShader
//================================================================//
/**	@lua	ZLShader
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class ZLShader :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
protected:

	friend class ZLGfxStateGPUCache;
	friend class ZLShaderProgram;

	ZLStrongPtr < ZLShaderProgram >		mProgram;
	ZLLeanArray < u8 >					mPendingUniformBuffer;

	//----------------------------------------------------------------//
	bool					IsDirty					();

public:

	GET ( ZLShaderProgram*, Program, mProgram )

	//----------------------------------------------------------------//
	void					ApplyUniforms			();
	void					BindUniforms			();
	void					Bless					();
	bool					HasDirtyUniforms		();
	void					ResizeUniformArray		( u32 uniformID, u32 count );
	void					ScheduleTextures		();
	void					SetProgram				( ZLShaderProgram* program );
	void					UpdateUniforms			();
							ZLShader				();
							~ZLShader				();
};

#endif
