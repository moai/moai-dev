//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIView.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

enum {
    MOAI_INPUT_DEVICE,
    TOTAL_INPUT_DEVICES,
};

enum {
    MOAI_TOUCH_SENSOR,
    TOTAL_SENSORS,
};

//================================================================//
// MOAIView ()
//================================================================//
@interface MOAIView () {

    AKUContextID        mAKUContext;
    NSTimeInterval      mAnimInterval;
    CADisplayLink*      mDisplayLink;
    
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
    -( void )       beginDrawing;
    -( void )       closeGraphicsContext;
    -( bool )       createBuffers;
    -( void )       createContext;
    -( void )       deleteBuffers;
    -( void )       drawView;
    -( void )       dummyFunc;
    -( void )       endDrawing;
	-( void )       handleTouches           :( NSSet* )touches :( BOOL )down;
    -( void )       openGraphicsContext;
    -( void )       onUpdateAnim;
    -( void )       releaseContext;
    -( void )       renewContext;
    -( BOOL )       sizeChanged;
	-( void )       startAnimation;
	-( void )       stopAnimation;
@end

//================================================================//
// MOAIView
//================================================================//
@implementation MOAIView

    @dynamic multisample;
    @dynamic multisampleEnabled;

    //----------------------------------------------------------------//
    +( void ) appInitialize {

        AKUAppInitialize ();

        static const int length = 255;
		char version [ length ];
		AKUGetMoaiVersion ( version, length );
		printf ( "%s\n", version );
    }

    //----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {
        ( void )application;
    
		AKUIosNotifyRemoteNotificationRegistrationComplete ( nil, error );
	}

    //----------------------------------------------------------------//
    +( BOOL ) application :( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
    
        AKUAppInitialize ();
        if ( AKUModulesIosApplicationDidFinishLaunchingWithOptions ( application, launchOptions ) == NO ) return NO;

        static const int length = 255;
		char version [ length ];
		AKUGetMoaiVersion ( version, length );
		printf ( "%s\n", version );
        
        return YES;
    }

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {
		( void )application;
        
		AKUIosNotifyRemoteNotificationReceived ( pushBundle );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
        ( void )application;
    
    	// TODO: why did we need this? why not do it in Lua?
    	//NSString* strData = [[[[deviceToken description]
		//	stringByReplacingOccurrencesOfString: @"<" withString: @""]
		//	stringByReplacingOccurrencesOfString: @">" withString: @""]
		//	stringByReplacingOccurrencesOfString: @" " withString: @""];
        //NSLog(@"%@", strData);
		AKUIosNotifyRemoteNotificationRegistrationComplete ( deviceToken, nil );
	}
		
    //----------------------------------------------------------------//
    -( BOOL ) application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {

        if ( AKUModulesIosApplicationOpenURL ( application, url, sourceApplication, annotation ) == NO ) {
            AKUIosOpenUrl ( url, sourceApplication );
        }
        return YES;
    }

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

		// set us up the frame buffers
	    glGenFramebuffersOES(1, &mFramebuffer);
	    glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
	    
	    glGenRenderbuffersOES(1, &mRenderbuffer);
	    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
	    
	    [ mEAGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer ];
	    glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &mWidth );
	    glGetRenderbufferParameterivOES ( GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &mHeight );
        
	    glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderbuffer );

	    // check OK
	    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
	        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	        return false;
	    }

	    // set us up the msaa buffers
	    if ([self multisampleEnabled]) {
	        glGenFramebuffersOES ( 1 , &mMSAAFramebuffer );
	        glBindFramebufferOES ( GL_FRAMEBUFFER_OES, mMSAAFramebuffer );
	    
	        glGenRenderbuffersOES ( 1, &mMSAARenderBuffer );
	        glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mMSAARenderBuffer );

	        glRenderbufferStorageMultisampleAPPLE ( GL_RENDERBUFFER_OES, self.multisample, GL_RGBA4_OES, mWidth, mHeight );
	        glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mMSAARenderBuffer);
	    }

	    glGenRenderbuffersOES ( 1, &mDepthBuffer );
	    glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mDepthBuffer );
	    
        // set us up the depth buffer
	    if ([self multisampleEnabled]) {
	        glRenderbufferStorageMultisampleAPPLE ( GL_RENDERBUFFER_OES, self.multisample, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
	    }
	    else {
	        glRenderbufferStorageOES ( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight );
	    }
	    
	    glFramebufferRenderbufferOES ( GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthBuffer );

	    // check OK
	    if ( glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ) != GL_FRAMEBUFFER_COMPLETE_OES ) {
	        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES ( GL_FRAMEBUFFER_OES ));
	        return false;
	    }

	    CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
	    CGFloat scale = [[ UIScreen mainScreen ] scale ];
	    CGFloat screenWidth = screenRect.size.width * scale;
	    CGFloat screenHeight = screenRect.size.height * scale;
	    
	    [ self bindFramebuffer ];
	    
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
		
        AKUDeleteContext ( mAKUContext );
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
        
        self.opaque = AKUIsGfxBufferOpaque () != 0;
		
		[ self beginDrawing ];
		
		AKUSetContext ( mAKUContext );
		AKURender ();

		[ self endDrawing ];
	}
	
    //----------------------------------------------------------------//
    -( void ) dummyFunc {
        //dummy to fix weird input bug
    }

    //----------------------------------------------------------------//
	-( void ) endDrawing {
        
		if ([ self multisampleEnabled ]) {
			// resolve multisample buffer
			glBindFramebufferOES ( GL_READ_FRAMEBUFFER_APPLE, mMSAAFramebuffer );
			glBindFramebufferOES ( GL_DRAW_FRAMEBUFFER_APPLE, mFramebuffer );
			glResolveMultisampleFramebufferAPPLE ();
	            
			GLenum attachments [] = { GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES };
			glDiscardFramebufferEXT ( GL_READ_FRAMEBUFFER_APPLE, 1, attachments );
		}
	        
		// finish & present
		glBindRenderbufferOES ( GL_RENDERBUFFER_OES, mRenderbuffer );
		[ mEAGLContext presentRenderbuffer:GL_RENDERBUFFER_OES ];

	    [ self closeGraphicsContext ]; // do not remove this
	}

    //----------------------------------------------------------------//
    // Since iOS 8.0 both [[UIScreen mainScreen] bounds/applicationFrame] are interface oriented.
	// Before iOS 8.0, only the portrait "bounds" where returned.
	// The function below ensure Portrait bounds are returned.
    +( CGRect ) getScreenBoundsFromCurrentOrientation :( CGRect )bounds {
    
        bool lessThaniOS8 = [ MOAIView isSystemVersionLessThan:@"8.0" ];
        if ( lessThaniOS8 || UIInterfaceOrientationIsPortrait ([ UIApplication sharedApplication ].statusBarOrientation )) {
            return bounds;
        }
        return CGRectMake ( bounds.origin.y, bounds.origin.x, bounds.size.height, bounds.size.width );
    }

    //----------------------------------------------------------------//
	-( int ) guessScreenDpi {
    
		float dpi;
		float scale = 1;

		if ([[ UIScreen mainScreen ] respondsToSelector:@selector ( scale )]) {
			scale = [[ UIScreen mainScreen ] scale];
		}
		if ( UI_USER_INTERFACE_IDIOM () == UIUserInterfaceIdiomPad ) {
			//Not working for iPad Mini, but appropriate solution doesn't exist yet
			dpi = 132 * scale;
		}
        else {
			dpi = 163 * scale;
		}
		return dpi;
	}

	//----------------------------------------------------------------//
	-( void ) handleTouches :( NSSet* )touches :( BOOL )down {
	
		for ( UITouch* touch in touches ) {
			
			CGPoint p = [ touch locationInView:self ];
			
			AKUEnqueueTouchEvent (
				MOAI_INPUT_DEVICE,
				MOAI_TOUCH_SENSOR,
				( int )( long ) touch, // use the address of the touch as a unique id
				down,
				p.x * [[ UIScreen mainScreen ] scale ],
				p.y * [[ UIScreen mainScreen ] scale ]
			);
		}
	}

    //----------------------------------------------------------------//
    -( id ) initWithCoder:(NSCoder*)aDecoder
    {
        // make sure to do self init here
        if ( self = [ self init ]) {
            mMultisample = 1;
        }
        return self;
    }

    //----------------------------------------------------------------//
    +( BOOL ) isSystemVersionLessThan :( NSString* )version {

        return ([[[ UIDevice currentDevice ] systemVersion ] compare:version options:NSNumericSearch ] == NSOrderedAscending );
    }

    //----------------------------------------------------------------//
	// You must implement this method
	+( Class ) layerClass {
		return [ CAEAGLLayer class ];
	}

    //----------------------------------------------------------------//
	-( void ) layoutSubviews {
    
	    // It is necessary to call super here
	    [ super layoutSubviews ];
	    
	    if (( mEAGLContext != nil ) && ([ self sizeChanged ])) {
	        [ self openGraphicsContext ];
	        [ self deleteBuffers ];
	        [ self createBuffers ];
	    }
	}

    //----------------------------------------------------------------//
	-( void ) moaiInit {
        [self moaiInitWithMultisample:1 ];
    }

	//----------------------------------------------------------------//
	-( void ) moaiInitWithMultisample :( int )multisample {
    
        NSAssert ((( multisample == 1 ) || ( multisample == 4 )), @"Invalid multisample" );
    
        mMultisample = multisample;
    
		mAKUContext = AKUCreateContext ();
		AKUSetUserdata (( __bridge void* )self );
		
		AKUModulesContextInitialize ();
		
		AKUSetInputConfigurationName ( "iPhone" );

		AKUReserveInputDevices			( TOTAL_INPUT_DEVICES );
		AKUSetInputDevice				( MOAI_INPUT_DEVICE, "device" );
		
		AKUReserveInputDeviceSensors	( MOAI_INPUT_DEVICE, TOTAL_SENSORS );
		AKUSetInputDeviceTouch			( MOAI_INPUT_DEVICE, MOAI_TOUCH_SENSOR,		"touch" );
		
        CGRect screenRect = [ MOAIView getScreenBoundsFromCurrentOrientation:[[ UIScreen mainScreen ] bounds ]];
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;
		
		AKUSetScreenDpi([ self guessScreenDpi ]);
		
		[ self createContext ];
        [ self openGraphicsContext ];
        [ self createBuffers ];
        
        AKUModulesRunLuaAPIWrapper ();
        
        // start to run the moai thread immediately so it renders the view before returning from here
        // to get a chance to display a splash screen for example while the rest loads
        mAnimInterval = 1; // 1 for 60fps, 2 for 30fps
        
        [ self pause:false ];
        [ self closeGraphicsContext ];
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
	-( void ) onUpdateAnim {
		
		AKUSetContext ( mAKUContext );
		AKUModulesUpdate ();
        
		self.opaque = AKUIsGfxBufferOpaque () != 0;
		
		[ self beginDrawing ];
		AKUSetContext ( mAKUContext );
		AKURender ();
		[ self endDrawing ];
        
        //sometimes the input handler will get 'locked out' by the render, this will allow it to run
        [ self performSelector:@selector( dummyFunc ) withObject:self afterDelay: 0 ];
	}
	
	//----------------------------------------------------------------//
	-( void ) pause :( BOOL )paused {
	
        AKUModulesPause ( paused );
    
		if ( paused ) {
			[ self stopAnimation ];
		}
		else {
			[ self startAnimation ];
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

	//----------------------------------------------------------------//
	-( void ) run :( NSString* )filename {
	
		AKUSetContext ( mAKUContext );
        AKULoadFuncFromFile ([ filename UTF8String ]);
		AKUCallFunc ();
	}

    //----------------------------------------------------------------//
    -( void ) setWorkingDirectory :( NSString* )path {
    
        AKUSetContext ( mAKUContext );
        AKUSetWorkingDirectory ([ path UTF8String ]);
    }

    //----------------------------------------------------------------//
    -( BOOL ) sizeChanged {
    
        float scale = 1;

		if ([[ UIScreen mainScreen ] respondsToSelector:@selector ( scale )]) {
			scale = [[ UIScreen mainScreen ] scale];
		}
    
        CGSize size = self.layer.bounds.size;
        return (( mWidth != ( GLint )( size.width * scale )) || ( mHeight != ( GLint )( size.height * scale )));
    }

	//----------------------------------------------------------------//
	-( void ) startAnimation {
		
		if ( !mDisplayLink ) {
			CADisplayLink* aDisplayLink = [[ UIScreen mainScreen ] displayLinkWithTarget:self selector:@selector( onUpdateAnim )];
			[ aDisplayLink setFrameInterval:mAnimInterval ];
			[ aDisplayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ]; // or NSRunLoopCommonModes
			mDisplayLink = aDisplayLink;
		}
	}

	//----------------------------------------------------------------//
	-( void ) stopAnimation {

        [ mDisplayLink invalidate ];
        mDisplayLink = nil;
	}

	//----------------------------------------------------------------//
	-( void )touchesBegan:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
	//----------------------------------------------------------------//
	-( void )touchesCancelled:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )touches;
		( void )event;
		
		AKUEnqueueTouchEventCancel ( MOAI_INPUT_DEVICE, MOAI_TOUCH_SENSOR );
	}
	
	//----------------------------------------------------------------//
	-( void )touchesEnded:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :NO ];
	}

	//----------------------------------------------------------------//
	-( void )touchesMoved:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
@end
