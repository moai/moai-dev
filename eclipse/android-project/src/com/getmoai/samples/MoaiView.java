//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.getmoai.samples;
 
import javax.microedition.khronos.egl.EGLConfig; 
import javax.microedition.khronos.opengles.GL10;
  
import android.content.Context;
import android.opengl.GLSurfaceView; 
import android.os.SystemClock;
import android.view.MotionEvent; 

import android.os.Build;
import android.provider.Settings.Secure;

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	int mWidth;   
	int mHeight;
	private MoaiRenderer mRenderer;
	private MoaiThread mThread = null; 
	private MoaiView mForRen;
	boolean mSurfaceCreated = false;
	String mLuaThread = null;
	String mAppRoot = null;
	private boolean mIsValid = false;
	String mPackageName = null;
	String mUDID = null;
	
	//----------------------------------------------------------------//
	public void cleanup () {
		
		mIsValid = false;
	}
	
    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		if( mIsValid ) {
			
			boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
			isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
			handleTouches ( event.getActionIndex (), isDown, Math.round ( event.getX () ), Math.round ( event.getY () ), 1 );
		}
		return true;
	}
	
	
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
		setRenderMode ( RENDERMODE_WHEN_DIRTY );
		
		if ( mThread == null ) {
			mThread = new MoaiThread ();
		}
		
		mForRen = this;
		mWidth = w;
		mHeight = h;

		mPackageName = context.getPackageName ();
		mUDID = Secure.getString ( context.getContentResolver (), Secure.ANDROID_ID );
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
	public void SetDirectory ( String path ) {
		
		mAppRoot = path;
	}


	private void GetDeviceProperties ( ) {
				
		// App name
		String appName;
		if ( mPackageName != null ) {
			String[] packNameParts = mPackageName. split ( "." );
			if ( packNameParts.length > 1 ) {
				appName = packNameParts[ packNameParts.length - 1 ];
			}
			else {
				appName = mPackageName;
			}
		}
		else {
			appName = "UNKNOWN";
		}
		
		// ARM EABI
		String abi = Build.CPU_ABI;
		
		// Device Brand
		String devBrand = Build.BRAND;
		
		// Device Design Name
		String devDes = Build.DEVICE;
		
		// Device Manufacturer
		String ma;
		try {
			ma = Build.class.getDeclaredField ( "MANUFACTURER" ). get ( Build.class ). toString ();
		} catch ( Throwable e ) {
			ma = "UNKNOWN";
		}		
		
		// Device Model
		String devModel = Build.MODEL;
		
		// Device Product
		String devProduct = Build.PRODUCT;
		
		// OS name
		String osName = "Android";
		
		// OS Version
		String osVersion = Build.VERSION.RELEASE;
		
		// UDID
		if ( mUDID == null ) {
			mUDID = "UNKNOWN";
		}
		
		setDeviceProperties ( appName, abi, devBrand, devDes, ma, devModel, 
			devProduct, osName, osVersion, mUDID );
	}
	
	//================================================================//
	// MoaiRenderer
	//================================================================//
	class MoaiRenderer implements GLSurfaceView.Renderer {

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			if ( mIsValid ) {
				onDraw ( mWidth, mHeight );
				gl.glFlush ();
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			gl.glViewport ( 0, 0, width, height );
			mWidth = width;
			mHeight = height;
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {
   
			InitializeAku ( mForRen ); 
			GetDeviceProperties ();
			
			mSurfaceCreated = true;
			mIsValid = true;

			setWorkingDirectory ( mAppRoot );
			Run ( "config-android.lua", mWidth, mHeight );
			Run ( "main.lua", mWidth, mHeight );
		}
	}
	
	//================================================================//
	// MoaiThread
	//================================================================//
	class MoaiThread extends Thread {

	    //----------------------------------------------------------------//
		@Override
		public void run () {
			
			long time = SystemClock.uptimeMillis ();
			
			while ( mIsValid ) {
				
				long elapsed = SystemClock.uptimeMillis () - time;
				
				if ( elapsed >= 32 ) {
					onUpdateAnim ();
					requestRender ();
					time += elapsed;
				}
				yield ();
			}
			//DeinitializeAku ();
			//FinalizeAku ();
		}
	}
	
	protected static native void InitializeAku ( MoaiView view );
	protected static native void Run ( String fileName, int width, int height );
	protected static native void FinalizeAku ();
	protected static native void DeinitializeAku ();
	public static native void onDraw ( int width, int height );
	public static native void onUpdateAnim ();
	public static native void onUpdateHeading ( int heading );
	public static native void onUpdateLocation ( int longitude, int latitude, int altitude, float hAccuracy, float vAccuracy, float speed );
	public static native void handleTouches (int touch, boolean down, int locX, int locY, int tapCount );
	public static native void setWorkingDirectory ( String path );
	
	//Device properties
	public static native void setDeviceProperties ( String appName, String abi, String devBrand, String devDes, String ma, String devModel, 
			String devProduct, String osName, String osVersion, String UDID );
}
