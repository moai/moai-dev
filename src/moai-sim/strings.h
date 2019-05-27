// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAI_SIM_STRINGS_H
#define	MOAI_SIM_STRINGS_H

#define MOAISTRING_MOAIAction_Profile_PSFF				"MOAIAction::Update(%p: %s [%s]) step %.2f ms took %.2f ms\n"
#define MOAISTRING_MOAIBox2DBody_InvalidVertexCount_D	"BOX2D ERROR: Vertex count %d is invalid (less than 3 or greater than max)\n"
#define MOAISTRING_MOAIBox2DBody_MissingInstance		"BOX2D ERROR: Attempt to access missing Box2D body instance\n"
#define MOAISTRING_MOAIBox2DFixture_MissingInstance		"BOX2D ERROR: Attempt to access missing Box2D fixture instance\n"
#define MOAISTRING_MOAIBox2DJoint_MissingInstance		"BOX2D ERROR: Attempt to access missing Box2D joint instance\n"
#define MOAISTRING_MOAIBox2DWorld_IsLocked				"BOX2D ERROR: Attempt to perform illegal operation during collision update\n"
#define MOAISTRING_MOAIGfxDevice_OpenGLError_S			"OPENGL ERROR: %s\n"
#define MOAISTRING_MOAIGfxResource_MissingDevice		"Unable to bind graphics resource - missing graphics device\n"
#define MOAISTRING_MOAIShader_ShaderInfoLog_S			"%s\n"
#define MOAISTRING_MOAITexture_MemoryUse_SDFS			"TEXTURE: '%s' %10luk = %6.2fMB < %s\n"
#define MOAISTRING_MOAITexture_NoFramebuffer			"TEXTURE: OpenGL framebuffer object is unsupported on this device\n"
#define MOAISTRING_MOAITexture_NonPowerOfTwo_SDD		"TEXTURE: '%s' is not a power of two (w:%d, h:%d)\n"

#endif
