// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFX_GLES_H
#define ZLGFX_GLES_H

#ifdef MOAI_OS_WINDOWS
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif

#ifdef MOAI_OS_OSX
	//#include <GL/glew.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>

	#define glGenVertexArrays		glGenVertexArraysAPPLE
	#define glBindVertexArray		glBindVertexArrayAPPLE
	#define glDeleteVertexArrays	glDeleteVertexArraysAPPLE

#endif

#ifdef MOAI_OS_IPHONE

	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>

	#define GL_WRITE_ONLY			0x88B9
	#define GL_RGBA8				GL_RGBA8_OES

	// TODO: should not have to do this
	// this is to suppress a false positive error in Xcode
	extern GLvoid*		glMapBufferOES				( GLenum target, GLenum access );
	extern GLboolean	glUnmapBufferOES			( GLenum target );

	#define glMapBuffer				glMapBufferOES
	#define glUnmapBuffer			glUnmapBufferOES

	// TODO: should not have to do this
	// this is to suppress a false positive error in Xcode
	extern GLvoid		glBindVertexArrayOES		( GLuint array );
	extern GLvoid		glGenVertexArraysOES		( GLsizei n, GLuint *arrays );
	extern GLvoid		glDeleteVertexArraysOES		( GLsizei n, const GLuint *arrays );

	#define glGenVertexArrays		glGenVertexArraysOES
	#define glBindVertexArray		glBindVertexArrayOES
	#define glDeleteVertexArrays	glDeleteVertexArraysOES

#endif

#ifdef MOAI_OS_ANDROID

	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	//#include <EGL/egl.h>

	#define GL_WRITE_ONLY			0x88B9

#endif

#ifdef MOAI_OS_LINUX
	#ifndef MOAI_OS_NACL
		#ifndef ANDROID
			#include <GL/glew.h>
		#endif
	#endif
#endif

#ifdef MOAI_OS_NACL
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define GL_RGBA8 GL_RGBA8_OES
#endif

#ifdef MOAI_OS_HTML
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#define MOAI_OS_NACL 1
	#define GL_RGBA8 GL_RGBA8_OES
#endif

#ifdef MOAI_OS_BLACKBERRY
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define GL_RGBA8 GL_RGBA8_OES
#endif

#define REMAP_EXTENSION_PTR(target, ext) target = target ? target : ext;

#endif
