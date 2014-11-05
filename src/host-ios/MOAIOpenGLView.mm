//----------------------------------------------------------------//
// Copyright (c) 2014 Plumzi, Inc.
// All Rights Reserved.
//----------------------------------------------------------------//

#import "MOAIOpenGLView.h"

#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <moai-core/host.h>
#import <moai-sim/host.h>

//================================================================//
// MOAIOpenGLView ()
//================================================================//
@interface MOAIOpenGLView () {
@protected

    NSInteger           mAnimInterval;
    CADisplayLink*		mDisplayLink;
	
    GLint				mWidth;
    GLint				mHeight;
    
    EAGLContext*		mEAGLContext;
    
    GLuint				mFramebuffer;
    GLuint				mRenderbuffer;
    GLuint              mDepthbuffer;
	
    GLuint				mMSAAFramebuffer;
    GLuint				mMSAARenderBuffer;
	
    GLuint				mDepthBuffer;
	
    BOOL                mGCDetected;
    BOOL				mSimStarted;
    int					mMultisample;
}

    //----------------------------------------------------------------//
    -( void )   closeGraphicsContext;
    -( bool )	createBuffers;
    -( void )	deleteBuffers;
    -( void )   openGraphicsContext;
    -( void )	releaseContext;
    -( void )	renewContext;

@end

//================================================================//
// MOAIOpenGLView
//================================================================//
@implementation MOAIOpenGLView

	//@synthesize framebuffer     = mFramebuffer;
    //@synthesize width           = mWidth;
    //@synthesize height          = mHeight;

	//----------------------------------------------------------------//
	-( void ) beginDrawing {
	
		[self openGraphicsContext];
    	[self bindFramebuffer];
	}

	//----------------------------------------------------------------//
	- (void) bindFramebuffer {

	    if ([ self multisampleEnabled ]) {
	        // draw into multisample buffer
	        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mMSAAFramebuffer );
	    }
	    else {
	        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mFramebuffer );
	    }
	}

	//----------------------------------------------------------------//
	-( void ) closeGraphicsContext {
	
		if ([ EAGLContext currentContext ] == mEAGLContext ) {
			[ EAGLContext setCurrentContext:nil ];
		}
	}

	//----------------------------------------------------------------//
	-( bool ) createBuffers {
		
		mWidth = 0;
		mHeight = 0;

		// set us up the depth buffer
		glGenRenderbuffersOES ( 1, &mDepthbuffer );
		glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mDepthbuffer );

		glRenderbufferStorageOES ( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
		glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthbuffer );

        // check OK
	    if ( glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES ) {
	        NSLog ( @"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ));
	        return false;
	    }

		// set us up the frame buffers
	    glGenFramebuffersOES(1, &mFramebuffer);
	    glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
	    
	    glGenRenderbuffersOES(1, &mRenderbuffer);
	    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
	    
	    [mEAGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &mWidth);
	    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &mHeight);
	    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderbuffer);

	    // check OK
	    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
	        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	        return false;
	    }

	    // set us up the msaa buffers
	    if ([self multisampleEnabled]) {
	        glGenFramebuffersOES(1 , &mMSAAFramebuffer);
	        glBindFramebufferOES(GL_FRAMEBUFFER_OES, mMSAAFramebuffer);
	    
	        glGenRenderbuffersOES(1, &mMSAARenderBuffer);
	        glBindRenderbufferOES(GL_RENDERBUFFER_OES, mMSAARenderBuffer);

	        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, mMultisample, GL_RGBA4_OES, mWidth, mHeight );
	        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mMSAARenderBuffer);
	    }

	    glGenRenderbuffersOES(1, &mDepthBuffer);
	    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthBuffer);
	    
	    if ([self multisampleEnabled]) {
	        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, mMultisample, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight);
	    }
	    else {
	        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight);
	    }
	    
	    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthBuffer);

	    // check OK
	    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
	        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	        return false;
	    }

	    CGRect screenRect = [[UIScreen mainScreen] bounds];
	    CGFloat scale = [[UIScreen mainScreen] scale];
	    CGFloat screenWidth = screenRect.size.width * scale;
	    CGFloat screenHeight = screenRect.size.height * scale;
	    
	    [self bindFramebuffer];
	    
        AKUSetScreenSize ( screenWidth, screenHeight );
		AKUSetViewSize ( mWidth, mHeight );
        AKUDetectFramebuffer ();
        
	    return true;
	}

	//----------------------------------------------------------------//
	-( void ) createContext {
		
		// Get the layer
		CAEAGLLayer* glLayer = ( CAEAGLLayer* )self.layer;
		
		glLayer.opaque = YES;
		glLayer.drawableProperties = @{
			kEAGLDrawablePropertyRetainedBacking:[NSNumber numberWithBool:NO],
			kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8, // kEAGLColorFormatRGB565 or kEAGLColorFormatRGBA8
		};
		
		if ([[ UIScreen mainScreen ] respondsToSelector: @selector ( scale )]) {
			CGFloat appContentScaleFactor = [[ UIScreen mainScreen ] scale ];
			if ([ glLayer respondsToSelector: @selector ( setContentsScale: )]) {
				glLayer.contentsScale = appContentScaleFactor;
			}
		}
		
        mEAGLContext = [[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 ];
	    assert ( mEAGLContext );

        [ self openGraphicsContext ];
        AKUDetectGfxContext ();
	}

	//----------------------------------------------------------------//
	-( void ) dealloc {
		
        [ self releaseContext ];
	}

	//----------------------------------------------------------------//
	- ( void ) deleteBuffers {

	    if ( mFramebuffer ) {
	        glDeleteFramebuffersOES ( 1, &mFramebuffer );
	        mFramebuffer = 0;
	    }
	    
	    if ( mRenderbuffer ) {
	        glDeleteRenderbuffersOES ( 1, &mRenderbuffer );
	        mRenderbuffer = 0;
	    }
	    
	    if ( mMSAAFramebuffer ) {
	        glDeleteFramebuffersOES ( 1, &mMSAAFramebuffer );
	        mMSAAFramebuffer = 0;
	    }
	    
	    if (mMSAARenderBuffer ) {
	        glDeleteRenderbuffersOES ( 1, &mMSAARenderBuffer );
	        mMSAARenderBuffer = 0;
	    }
	    
	    if ( mDepthBuffer ) {
	        glDeleteRenderbuffersOES ( 1, &mDepthBuffer );
	        mDepthBuffer = 0;
	    }
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
	}

	//----------------------------------------------------------------//
	-( void ) endDrawing {
			
		
		if ([self multisampleEnabled]) {
			// resolve multisample buffer
			glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, mMSAAFramebuffer);
			glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, mFramebuffer);
			glResolveMultisampleFramebufferAPPLE();
	            
			GLenum attachments [] = { GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES };
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
		}
	        
		// finish & present
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
		[mEAGLContext presentRenderbuffer:GL_RENDERBUFFER_OES];

	    [self closeGraphicsContext]; // do not remove this
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
			[ self openGraphicsContext ];
			[ self createBuffers ];
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( id ) initWithFrame :( CGRect )frame {
	
		self = [ super initWithFrame :frame ];
		if ( self ) {
			[ self createContext ];
			[ self openGraphicsContext ];
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
	- (void) layoutSubviews {
    
	    // It is necessary to call super here
	    [ super layoutSubviews ];
	    
	    if ( mEAGLContext != nil ) {
	        [ self openGraphicsContext ];
	        [ self deleteBuffers ];
	        [ self createBuffers ];
	    }
	}

    //----------------------------------------------------------------//
	- (BOOL) multisampleEnabled {
	    return mMultisample > 1;
	}
	
	//----------------------------------------------------------------//
	-( void ) openGraphicsContext {
		
		if ([ EAGLContext currentContext ] != mEAGLContext ) {
			[ EAGLContext setCurrentContext:mEAGLContext ];
		}
	}

	//----------------------------------------------------------------//
	-( void ) releaseContext {
	
        [ self closeGraphicsContext ];
    
        if ( mEAGLContext != nil ) {
            [ self openGraphicsContext ];
            [ self deleteBuffers ];
            [ self closeGraphicsContext ];
        }
        mEAGLContext = nil;
	}

	//----------------------------------------------------------------//
	-( void ) renewContext {
		
		if ( !mEAGLContext ) {
			[ self createContext ];
			[ self openGraphicsContext ];
			[ self createBuffers ];
		}
	}

@end
