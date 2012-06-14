#include <uslscore/uslscore.h>
#include <moaicore-config.h>

#ifdef MOAI_OS_WINDOWS
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif

#ifdef MOAI_OS_OSX
	//#include <GL/glew.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
#endif

#ifdef MOAI_OS_IPHONE
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
	
	// TODO: replace this w/ runtime ogl extension checks
	#define MOAI_TEST_PVR
	
	#define GL_RGBA8 GL_RGBA8_OES
	
#endif

#ifdef MOAI_OS_ANDROID
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
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

#ifdef _ARM_ARCH_7
	#define MOAI_ARM7
#endif