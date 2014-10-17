//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "RefPtr.h"

class USTexture;

//================================================================//
// OpenGLView
//================================================================//
@interface OpenGLView : UIView {
@protected

	GLint mWidth;
	GLint mHeight;
    
	RefPtr < EAGLContext > mContext;
    
	GLuint mFramebuffer;
	GLuint mRenderbuffer;
	GLuint mDepthbuffer;
}

	PROPERTY_READONLY ( GLuint, framebuffer );

	//----------------------------------------------------------------//
	-( void )	beginDrawing;
	-( void )	closeContext;
	-( void )	drawView;
	-( void )	endDrawing;
	-( void )	openContext;

@end
