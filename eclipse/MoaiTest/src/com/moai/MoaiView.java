//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.moai;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	int mWidth, mHeight;
	private MoaiRenderer mRenderer;
	private MoaiThread mThread = null;
	private MoaiView mForRen;
	boolean mSurfaceCreated = false;
	boolean mStartRun = false;
	boolean mSetDirectory = false;
	String mLuaThread = null;
	String mAppRoot = null;
	
    //----------------------------------------------------------------//
	public MoaiView ( Context context ) {
		
		super ( context );
		mRenderer = new MoaiRenderer ();
		setRenderer ( mRenderer );
		
		if ( mThread == null ) {
			mThread = new MoaiThread ();
		}
		
		InitializeAku ( this );
	}
	
    //----------------------------------------------------------------//
	public MoaiView ( Context context, int w, int h ) {
		
		super ( context );
		mRenderer = new MoaiRenderer ();
		setRenderer ( mRenderer );
		
		if ( mThread == null ) {
			mThread = new MoaiThread ();
		}
		
		mForRen = this;
		mWidth = w;
		mHeight = h;
	}
	
	//================================================================//
	// AKU callbacks
	//================================================================//

	//----------------------------------------------------------------//
	void AKUEnterFullscreenModeFunc () {
	}

	//----------------------------------------------------------------//
	void AKUExitFullscreenModeFunc () {
	}

	//----------------------------------------------------------------//
	void AKUHideLoadingScreenFunc () {
	}

	//----------------------------------------------------------------//
	void AKUOpenWindowFunc ( char[] title, int width, int height ) {
	}

	//----------------------------------------------------------------//
	void AKUShowLoadingScreenFunc () {
	}

	//----------------------------------------------------------------//
	void AKUShowSoftwareKeyboardFunc () {
	}

	//----------------------------------------------------------------//
	public void AKUStartGameLoopFunc () {
		
		if ( !mThread.isAlive () ) {
			mThread.start ();
		}
	}
	
	
    //----------------------------------------------------------------//
	public void RunScript ( String scriptName ) {
		
		mLuaThread = scriptName;
		
		if ( !mSurfaceCreated ) {
			mStartRun = true;
			return;
		}
		
		Run ( mLuaThread );
	}
	
    //----------------------------------------------------------------//
	public void SetDirectory ( String path ) {
		
		mAppRoot = path;
		
		if ( !mSurfaceCreated ) {
			mSetDirectory = true;
			return;
		}
		
		if ( mAppRoot != null ) {
			setWorkingDirectory ( mAppRoot );
		}
	}

	//================================================================//
	// MoaiRenderer
	//================================================================//
	class MoaiRenderer implements GLSurfaceView.Renderer {

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			if ( mThread.isAlive () ) {
				onDraw ( mWidth, mHeight );
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			gl.glViewport (0, 0, width, height );
			mWidth = width;
			mHeight = height;
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated (GL10 gl, EGLConfig config ) {

			InitializeAku ( mForRen );
			
			mSurfaceCreated = true;
			
			if ( mSetDirectory ) {
				if ( mAppRoot != null ) {
					setWorkingDirectory ( mAppRoot );
				}
			}
			
			if ( mStartRun ) {
				Run( mLuaThread );
			}
		}
	}
	
	//================================================================//
	// MoaiThread
	//================================================================//
	class MoaiThread extends Thread {
		
		protected boolean mRun = true;
		protected long mCurTime = 0;

	    //----------------------------------------------------------------//
		@Override
		public void run () {
			
			while ( mRun ) {
				
				if ( mCurTime == 0 ) {
					mCurTime = SystemClock.uptimeMillis ();
					continue;
				}
				
				if ( SystemClock.uptimeMillis () - mCurTime < 32 ) {
					yield ();
				}
				else {
					mCurTime = SystemClock.uptimeMillis ();
					onUpdateAnim (); 
				}
			}
		}
	}
	
	protected static native void InitializeAku ( MoaiView view );
	protected static native void Run ( String fileName );
	protected static native void FinalizeAku ();
	protected static native void DeinitializeAku ();
	public static native void onDraw ( int width, int height );
	public static native void onUpdateAnim ();
	public static native void onUpdateHeading ( int heading );
	public static native void onUpdateLocation ( int longitude, int latitude, int altitude, float hAccuracy, float vAccuracy, float speed );
	public static native void handleTouches (int touch, boolean down, int locX, int locY, int tapCount );
	public static native void setWorkingDirectory ( String path );
}
