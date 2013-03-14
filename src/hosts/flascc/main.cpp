/*
  
  FlasCC Host for Moai SDK
  Created by Francisco Tufró on 3/5/2013
  © Zipline Games, Inc.
  
*/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <AS3/AS3.h>
#include <Flash++.h>
#include <SDL.h>
#include <SDL_video.h>

#include <aku/AKU.h>
#include <aku/AKU-untz.h>
#include <lua-headers/moai_lua.h>
#include "GL/gl.h"
#include "GL/glu.h"

using namespace std;

namespace GlutInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace GlutInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}

Uint32 frameDelta = (Uint32)(1000.0 / 60.0);
Uint32 lastFrame;

SDL_Surface *surface;
int width, height;

void initializeAKU () {
  AKUCreateContext ();
  // AKUUntzInit ();
  AKUSetInputConfigurationName ( "AKUFlasCC" );
  
  AKUReserveInputDevices     ( GlutInputDeviceID::TOTAL );
  AKUSetInputDevice        ( GlutInputDeviceID::DEVICE, "device" );
  
  AKUReserveInputDeviceSensors ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::TOTAL );
  AKUSetInputDeviceKeyboard    ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD,   "keyboard" );
  AKUSetInputDevicePointer   ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::POINTER,    "pointer" );
  AKUSetInputDeviceButton      ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_LEFT, "mouseLeft" );
  AKUSetInputDeviceButton      ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_MIDDLE, "mouseMiddle" );
  AKUSetInputDeviceButton      ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::MOUSE_RIGHT,  "mouseRight" );
  
  
  AKUDetectGfxContext ();
	
  // int width, height;
  //   
  //   inline_as3(
  //       "%0 = CModule.rootSprite.stage.stageWidth;\n"
  //       "%1 = CModule.rootSprite.stage.stageHeight;\n"
  //       : "=r"(width),"=r"(height) :
  //   );
  
  SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
  AKUSetScreenSize ( width, height );
  AKUSetViewSize ( width, height );
  
  AKURunBytecode ( moai_lua, moai_lua_SIZE );
  
  lastFrame = SDL_GetTicks();
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 )
	    height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return( 1 );
}

/* general OpenGL initialization function */
void initGL( )
{

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    // return( 1 );
}


void inititalizeSDL () {
  
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
   
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    
    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  	{
  	    fprintf( stderr, "Video initialization failed: %s\n",
  		     SDL_GetError( ) );
        // Quit( 1 );
  	}
    
    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
  	{
  	    fprintf( stderr, "Video query failed: %s\n",
  		     SDL_GetError( ) );
        // Quit( 1 );
  	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
    
    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
  	  videoFlags |= SDL_HWSURFACE;
    else
  	  videoFlags |= SDL_SWSURFACE;
    
    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
  	  videoFlags |= SDL_HWACCEL;

    
    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


    /* get a SDL surface */
    int bppc = SDL_VideoModeOK( 100, 100, 16, SDL_OPENGL );
    printf ( "Bpp... %i\n", bppc );
    
    surface = SDL_SetVideoMode( width, height, 0, SDL_OPENGL );
    printf ( "Surface... %i, %i\n", surface->flags, SDL_OPENGL );
    printf ( "videoFlags... %i, %i\n", SDL_OPENGL | SDL_NOFRAME, SDL_OPENGL );
    printf( "Video mode set failed: %s\n", SDL_GetError( ) );
    fflush ( stdout );
    
    /* Verify there is a surface */
    if ( !surface )
  	{
  	  printf( "Video mode set failed: %s\n", SDL_GetError( ) );
  	  fflush ( stdout );
      // Quit( 1 );
  	}
  	
  	/* initialize OpenGL */
    initGL( );
    
    /* resize the initial window */
    resizeWindow( width, height );
}

extern "C" void mainLoopTick(){
  AKUUpdate();

  AKURender();
  SDL_GL_SwapBuffers();

  Uint32 currentFrame = SDL_GetTicks();
  Uint32 delta = currentFrame - lastFrame;
  if(delta < frameDelta)
    SDL_Delay(frameDelta - delta);
  lastFrame = SDL_GetTicks();  
  
  // /* Clear The Screen And The Depth Buffer */
  // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // 
  // /* Move Left 1.5 Units And Into The Screen 6.0 */
  // glLoadIdentity();
  // glTranslatef( -1.5f, 0.0f, -6.0f );
  // 
  // glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
  // glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
  //   glVertex3f(  0.0f,  1.0f, 0.0f ); /* Top */
  //   glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
  //   glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
  // glEnd( );                           /* Finished Drawing The Triangle */
  // 
  // /* Move Right 3 Units */
  // glTranslatef( 3.0f, 0.0f, 0.0f );
  // 
  // glBegin( GL_QUADS );                /* Draw A Quad */
  //   glVertex3f( -1.0f,  1.0f, 0.0f ); /* Top Left */
  //   glVertex3f(  1.0f,  1.0f, 0.0f ); /* Top Right */
  //   glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
  //   glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
  // glEnd( );                           /* Done Drawing The Quad */
  // 
  // /* Draw it to the screen */
  // SDL_GL_SwapBuffers( );
  // 
  // // fprintf( stderr, "Error: %s\n",
  // //    SDL_GetError( ) );
  
}

int main()
{
  glClearColor ( 0, 1, 0, 1);
  // Retrieve width and height
  inline_as3(
      "%0 = CModule.rootSprite.stage.stageWidth;\n"
      "%1 = CModule.rootSprite.stage.stageHeight;\n"
      : "=r"(width),"=r"(height) :
  );
  
  inititalizeSDL ();
    /* AKU */
  initializeAKU ();
    
  // Run main.lua
  AKURunScript ( "main.lua" );
  SDL_GL_SwapBuffers();
      
  AS3_GoAsync();
}