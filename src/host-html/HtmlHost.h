// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	HTMLHOST
#define	HTMLHOST

//----------------------------------------------------------------//
extern "C" {
  void RefreshContext();
  void Cleanup();

//----------------------------------------------------------------//
void onKeyDown ( unsigned char key);

//----------------------------------------------------------------//
void onKeyUp ( unsigned char key );

//----------------------------------------------------------------//
void onMouseButton ( int button, int state  );

//----------------------------------------------------------------//
void onMouseDrag ( int x, int y )             ;
//----------------------------------------------------------------//
void onMouseMove ( int x, int y )              ;

//----------------------------------------------------------------//
void onPaint () ;
//----------------------------------------------------------------//
void onReshape( int w, int h );

//----------------------------------------------------------------//
void onTimer ( )               ;


  //js callbacks
  void EnterFullScreen();
  void ExitFullScreen();
  void OpenWindowFunc(const char* title, int width, int height);
  void RestoreFile(const char* path, int data);
}

#endif
