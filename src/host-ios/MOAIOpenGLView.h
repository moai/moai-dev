//----------------------------------------------------------------//
// Copyright (c) 2014 Plumzi, Inc.
// All Rights Reserved.
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>


//================================================================//
// PZOpenGLView
//================================================================//
@interface MOAIOpenGLView : UIView {
@protected
}

    //----------------------------------------------------------------//

    -( void )   beginDrawing;
    -( void )	createContext;
    -( void )   endDrawing;

@end
