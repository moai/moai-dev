#ifndef USLSEXT_PCH_H
#define	USLSEXT_PCH_H

#include <uslscore/uslscore.h>

#ifndef TIXML_USE_STL
	#define TIXML_USE_STL
#endif

// glew
#ifdef _WIN32
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif

// FIXME The following doesn't distinguish between Mac OS X and iPhone builds.
//	 We'll explicitly set MAC_OS_X in compiler options for now.

//#if defined( __APPLE__ ) && defined( __MACH__ )
//	#define MAC_OS_X
//#endif

#ifdef MAC_OS_X
	#include <GL/glew.h>
#endif

#ifdef __linux
	#ifndef ANDROID
		#include <GL/glew.h>
	#endif
#endif

#if defined( _WIN32 ) || defined( MAC_OS_X ) || defined( __linux )
	#ifndef ANDROID
		#define USING_GLUT
	#endif
#endif

#if !defined( USING_GLUT )
	//#import <OpenGLES/EAGL.h>
	#ifndef ANDROID
		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
	#else
		#include <GLES/gl.h>
		#include <GLES/glext.h>
	#endif
#endif

#endif
