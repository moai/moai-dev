// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLShaderGL.h>

//================================================================//
// ZLShaderGL
//================================================================//

//----------------------------------------------------------------//
void ZLShaderGL::ApplyUniforms () {

	if ( this->mProgram ) {
		this->mProgram->ApplyUniforms ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
void ZLShaderGL::BindUniforms () {
	
	if ( this->mProgram ) {
		this->mProgram->BindUniforms ();
	}
}

//----------------------------------------------------------------//
bool ZLShaderGL::HasDirtyUniforms () {

	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mPendingUniformBuffer );
}

//----------------------------------------------------------------//
void ZLShaderGL::ScheduleTextures () {

	if ( this->mProgram ) {
		this->mProgram->ScheduleTextures ();
	}
}

//----------------------------------------------------------------//
void ZLShaderGL::SetProgram ( ZLShaderProgramGL* program ) {
	
	this->mProgram = program;
	
	if ( program ) {
		program->InitUniformBuffer ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
void ZLShaderGL::UpdateUniforms () {

	if ( this->mProgram ) {
		this->mProgram->UpdateUniforms ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
ZLShaderGL::ZLShaderGL () {

	RTTI_SINGLE ( ZLAbstractShader )
}

//----------------------------------------------------------------//
ZLShaderGL::~ZLShaderGL () {
}

