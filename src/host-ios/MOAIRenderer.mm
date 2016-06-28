//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIContextMgr.h"
#import "MOAIRenderer.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIRenderer ()
//================================================================//
@interface MOAIRenderer () {
    
    GLint				mWidth;
    GLint				mHeight;
    
    EAGLContext*		mEAGLContext;
    
    GLuint				mFramebuffer;
    GLuint				mRenderbuffer;

    GLuint				mMSAAFramebuffer;
    GLuint				mMSAARenderBuffer;
	
    // this gets bound to either mFramebuffer or mMSAAFramebuffer, depending on whether multisampling is enabled
    GLuint				mDepthBuffer;
    
    BOOL                mGCDetected;
    BOOL				mSimStarted;
    int					mMultisample;
    
    BOOL                mFrameIsReady;
}

    @property ( readonly, nonatomic ) int multisample;
    @property ( readonly, nonatomic ) BOOL multisampleEnabled;

	//----------------------------------------------------------------//
    -( void )           bindFramebuffer;
    -( void )           closeGraphicsContext;
    -( void )           createContext               :( CAEAGLLayer* )layer;
    -( BOOL )           createBuffers               :( CAEAGLLayer* )layer :( int )multisample;
    -( void )           deleteBuffers;
    -( void )           openGraphicsContext;
    -( BOOL )           sizeChanged                 :( CAEAGLLayer* )layer;

@end

//================================================================//
// MOAIRenderer
//================================================================//
@implementation MOAIRenderer

    @synthesize eaglContext = mEAGLContext;

	//----------------------------------------------------------------//
	-( void ) bindFramebuffer {

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
	-( BOOL ) createBuffers :( CAEAGLLayer* )layer :( int )multisample {
        
        mMultisample = multisample;
        
		mWidth = 0;
		mHeight = 0;

        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, nil );

		// set us up the frame buffers
	    glGenFramebuffersOES ( 1, &mFramebuffer );
	    glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mFramebuffer );
        
	    glGenRenderbuffersOES ( 1, &mRenderbuffer );
	    glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mRenderbuffer );
	    [ mEAGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:( CAEAGLLayer* )layer ];
        
	    glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &mWidth );
	    glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &mHeight );
        
	    glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderbuffer );

	    // check OK
	    if ( glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ) != GL_FRAMEBUFFER_COMPLETE_OES ) {
	        NSLog ( @"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ));
	        return NO;
	    }

	    // set us up the msaa buffers
	    if ([ self multisampleEnabled ]) {
        
	        glGenFramebuffersOES ( 1 , &mMSAAFramebuffer );
	        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mMSAAFramebuffer );
	    
	        glGenRenderbuffersOES ( 1, &mMSAARenderBuffer );
	        glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mMSAARenderBuffer );

	        glRenderbufferStorageMultisampleAPPLE ( GL_RENDERBUFFER_OES, self.multisample, GL_RGBA4_OES, mWidth, mHeight );
	        glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mMSAARenderBuffer );
	    }

	    glGenRenderbuffersOES ( 1, &mDepthBuffer );
	    glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mDepthBuffer );
	    
        // set us up the depth buffer
	    if ([ self multisampleEnabled ]) {
	        glRenderbufferStorageMultisampleAPPLE ( GL_RENDERBUFFER_OES, self.multisample, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
	    }
	    else {
	        glRenderbufferStorageOES ( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
	    }
	    
	    glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthBuffer );

	    // check OK
	    if ( glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ) != GL_FRAMEBUFFER_COMPLETE_OES ) {
	        NSLog ( @"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ));
	        return NO;
	    }

	    CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
	    CGFloat scale = [[ UIScreen mainScreen ] scale ];
	    CGFloat screenWidth = screenRect.size.width * scale;
	    CGFloat screenHeight = screenRect.size.height * scale;
	    
	    [ self bindFramebuffer ];
	    
        AKUSetScreenSize ( screenWidth, screenHeight );
		AKUSetViewSize ( mWidth, mHeight );
        AKUDetectFramebuffer ();
        
	    return YES;
	}

    //----------------------------------------------------------------//
	-( void ) createContext :( CAEAGLLayer* )layer {
		
        layer.opaque = YES;
		layer.drawableProperties = @{
			kEAGLDrawablePropertyRetainedBacking:[NSNumber numberWithBool:NO],
			kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8, // kEAGLColorFormatRGB565 or kEAGLColorFormatRGBA8
		};
		
		if ([[ UIScreen mainScreen ] respondsToSelector: @selector ( scale )]) {
			CGFloat appContentScaleFactor = [[ UIScreen mainScreen ] scale ];
			if ([ layer respondsToSelector: @selector ( setContentsScale: )]) {
				layer.contentsScale = appContentScaleFactor;
			}
		}
        
        mEAGLContext = [[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 ];
	    assert ( mEAGLContext );

        [ self openGraphicsContext ];
        AKUDetectGfxContext ();
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
	    
	    if ( mMSAARenderBuffer ) {
	        glDeleteRenderbuffersOES ( 1, &mMSAARenderBuffer );
	        mMSAARenderBuffer = 0;
	    }
	    
	    if ( mDepthBuffer ) {
	        glDeleteRenderbuffersOES ( 1, &mDepthBuffer );
	        mDepthBuffer = 0;
	    }
	}

    //----------------------------------------------------------------//
    -( id ) initWithLayer :( CAEAGLLayer* )layer :( int )multisample {
    
        self = [ super init ];
		if ( self != nil ) {
            [ self createContext :layer ];
            [ self createBuffers :layer :multisample ];
        }
        return self;
    }

    //----------------------------------------------------------------//
	-( int ) multisample {
	    return mMultisample;
	}

    //----------------------------------------------------------------//
	-( BOOL ) multisampleEnabled {
	    return mMultisample > 1;
	}

    //----------------------------------------------------------------//
	-( void ) openGraphicsContext {
		
		if ([ EAGLContext currentContext ] != mEAGLContext ) {
			[ EAGLContext setCurrentContext:mEAGLContext ];
		}
	}

    //----------------------------------------------------------------//
    -( void ) presentFrame {
        
        [ self openGraphicsContext ];
        
        if ( mFrameIsReady ) {
            glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mRenderbuffer );
            [ mEAGLContext presentRenderbuffer:GL_RENDERBUFFER_OES ];
            mFrameIsReady = NO;
        }
        
        [ self closeGraphicsContext ];
    }

    //----------------------------------------------------------------//
    -( void ) render {
        
        if ( mFrameIsReady ) return;
        if ( !AKUDisplayListHasContent ( AKU_DISPLAY_LIST_DRAWING )) return;
        
        [ self openGraphicsContext ];
        [ self bindFramebuffer ];
        
        [ MOAIContextMgr displayListBeginPhase:AKU_DISPLAY_LIST_DRAWING_PHASE ];
        
        if ( AKUDisplayListIsEnabled ( AKU_DISPLAY_LIST_DRAWING )) {
            AKUDisplayListProcess ( AKU_DISPLAY_LIST_DRAWING );
        }
        else {
            AKURender ();
        }
        
        [ MOAIContextMgr displayListEndPhase:AKU_DISPLAY_LIST_DRAWING_PHASE ];
        
        if ([ self multisampleEnabled ]) {
            // resolve multisample buffer
            glBindFramebufferOES ( GL_READ_FRAMEBUFFER_APPLE, mMSAAFramebuffer );
            glBindFramebufferOES ( GL_DRAW_FRAMEBUFFER_APPLE, mFramebuffer );
            glResolveMultisampleFramebufferAPPLE ();
                
            GLenum attachments [] = { GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES };
            glDiscardFramebufferEXT ( GL_READ_FRAMEBUFFER_APPLE, 1, attachments );
        }

        //[ NSThread sleepForTimeInterval:( 1.0f / 15.0f )];

        mFrameIsReady = YES;
        
        [ self closeGraphicsContext ];
    }

    //----------------------------------------------------------------//
    -( void ) resize :( CAEAGLLayer* )layer {
            
        if (( mEAGLContext != nil ) && ([ self sizeChanged :layer ])) {
            [ self openGraphicsContext ];
            [ self deleteBuffers ];
            [ self createBuffers :layer :mMultisample ];
            [ self closeGraphicsContext ];
        }
    }

    //----------------------------------------------------------------//
    -( BOOL ) sizeChanged :( CAEAGLLayer* )layer {
    
        float scale = 1;

		if ([[ UIScreen mainScreen ] respondsToSelector:@selector ( scale )]) {
			scale = [[ UIScreen mainScreen ] scale ];
		}
    
        CGSize size = layer.bounds.size;
        return (( mWidth != ( GLint )( size.width * scale )) || ( mHeight != ( GLint )( size.height * scale )));
    }

    //----------------------------------------------------------------//
    -( void ) shutdown {
            
        if ( mEAGLContext != nil ) {
            [ self openGraphicsContext ];
            [ self deleteBuffers ];
            [ self closeGraphicsContext ];
        }
        mEAGLContext = nil;
    }

@end
