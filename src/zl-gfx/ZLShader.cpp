// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLShader.h>

//================================================================//
// ZLShader
//================================================================//

//----------------------------------------------------------------//
void ZLShader::ApplyUniforms () {

	if ( this->mProgram ) {
		this->mProgram->ApplyUniforms ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
void ZLShader::BindUniforms () {
	
	if ( this->mProgram ) {
		this->mProgram->BindUniforms ();
	}
}

//----------------------------------------------------------------//
bool ZLShader::HasDirtyUniforms () {

	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mPendingUniformBuffer );
}

//----------------------------------------------------------------//
void ZLShader::ScheduleTextures () {

	if ( this->mProgram ) {
		this->mProgram->ScheduleTextures ();
	}
}

//----------------------------------------------------------------//
void ZLShader::SetProgram ( ZLShaderProgram* program ) {
	
	this->mProgram = program;
	
	if ( program ) {
		program->InitUniformBuffer ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
void ZLShader::UpdateUniforms () {

	if ( this->mProgram ) {
		this->mProgram->UpdateUniforms ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
ZLShader::ZLShader () {

	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
ZLShader::~ZLShader () {
}

