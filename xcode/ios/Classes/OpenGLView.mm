//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <aku/AKU.h>

#import "OpenGLView.h"

//================================================================//
// OpenGLView ()
//================================================================//
@interface OpenGLView ()

	//----------------------------------------------------------------//	
	-( bool )	createBuffers;
	-( void )	createContext;
	-( void )	deleteBuffers;
	-( void )	releaseContext;
	-( void )	renewContext;

@end

//================================================================//
// OpenGLView
//================================================================//
@implementation OpenGLView

	SYNTHESIZE	( GLuint, framebuffer, Framebuffer );

	//----------------------------------------------------------------//
	-( void ) beginDrawing {
	
		[ self openContext ];
		glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mFramebuffer );
	}

	//----------------------------------------------------------------//
	-( void ) closeContext {
	
		if ([ EAGLContext currentContext ] == mContext ) {
			[ EAGLContext setCurrentContext:nil ];
		}
	}

	//----------------------------------------------------------------//
	-( bool ) createBuffers {
		
		mWidth = 0;
		mHeight = 0;
		
		glGenFramebuffersOES ( 1, &mFramebuffer );
		glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mFramebuffer );
		
		glGenRenderbuffersOES ( 1, &mRenderbuffer );
		glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mRenderbuffer );
		
		[ mContext renderbufferStorage :GL_RENDERBUFFER_OES fromDrawable :(CAEAGLLayer*)self.layer ];
		glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &mWidth );
		glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &mHeight );
		
		glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderbuffer );
		
		// set us up the depth buffer
		//glGenRenderbuffersOES ( 1, &mDepthbuffer );
		//glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mDepthbuffer );

		//glRenderbufferStorageOES ( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
		//glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthbuffer );
		
		if ( glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ) != GL_FRAMEBUFFER_COMPLETE_OES ) {
			NSLog ( @"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ));
			return false;
		}

		return true;
	}

	//----------------------------------------------------------------//
	-( void ) createContext {
		
		// Get the layer
		CAEAGLLayer* glLayer = ( CAEAGLLayer* )self.layer;
		
		glLayer.opaque = YES;
		glLayer.drawableProperties = [ NSDictionary
			dictionaryWithObjectsAndKeys :[ NSNumber numberWithBool:NO ],
			kEAGLDrawablePropertyRetainedBacking,
			kEAGLColorFormatRGBA8, // kEAGLColorFormatRGB565 or kEAGLColorFormatRGBA8
			kEAGLDrawablePropertyColorFormat,
			nil
		];
		
		if ([[ UIScreen mainScreen ] respondsToSelector: @selector ( scale )]) {
			CGFloat appContentScaleFactor = [[ UIScreen mainScreen ] scale ];
			
			if ([ glLayer respondsToSelector: @selector ( setContentsScale: )]) {
				glLayer.contentsScale = appContentScaleFactor;
			}
		}
		
		mContext = [[[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 ] autorelease ];
    
		if ( !mContext ) {
			mContext = [[[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES1 ] autorelease ];
		}
	}

	//----------------------------------------------------------------//
	-( void ) dealloc {
		
		[ self openContext ];
		[ self deleteBuffers ];
		[ self closeContext ];
		
		[ super dealloc ];
	}

	//----------------------------------------------------------------//
	-( void ) deleteBuffers {
		
		glDeleteFramebuffersOES ( 1, &mFramebuffer );
		mFramebuffer = 0;
		
		glDeleteRenderbuffersOES ( 1, &mRenderbuffer );
		mRenderbuffer = 0;
		
		if ( mDepthbuffer ) {
			glDeleteRenderbuffersOES ( 1, &mDepthbuffer );
			mDepthbuffer = 0;
		}
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
	}

	//----------------------------------------------------------------//
	-( void ) endDrawing {
			
		// finish & present
		glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mRenderbuffer );
		[ mContext presentRenderbuffer :GL_RENDERBUFFER_OES ];
		[ self closeContext ]; // do not remove this
		
	}

	//----------------------------------------------------------------//
	-( id ) init {
	
		self = [ super init ];
		if ( self ) {
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
	-( id ) initWithCoder :( NSCoder* )coder {
	
		self = [ super initWithCoder :coder ];
		if ( self ) {
			[ self createContext ];
			[ self openContext ];
			[ self createBuffers ];
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( id ) initWithFrame :( CGRect )frame {
	
		self = [ super initWithFrame :frame ];
		if ( self ) {
			[ self createContext ];
			[ self openContext ];
			[ self createBuffers ];
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	// You must implement this method
	+( Class ) layerClass {
		return [ CAEAGLLayer class ];
	}
	
	//----------------------------------------------------------------//
	-( void ) openContext {
		
		if ([ EAGLContext currentContext ] != mContext ) {
			[ EAGLContext setCurrentContext:mContext ];
		}
	}

	//----------------------------------------------------------------//
	-( void ) releaseContext {
	
		[ EAGLContext setCurrentContext:nil ];
		mContext = nil;
	}

	//----------------------------------------------------------------//
	-( void ) renewContext {
		
		if ( !mContext ) {
			[ self createContext ];
			[ self openContext ];
			[ self createBuffers ];
		}
	}

@end
