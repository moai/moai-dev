//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;
 
import javax.microedition.khronos.egl.EGLConfig; 
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.*;  
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.opengl.GLSurfaceView; 
import android.os.SystemClock;
import android.util.Log;
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
	ConnectivityManager mConMan = null;

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

		Log.e( "MoaiLog", "This is the one that should work!");
	
		if ( mThread == null ) {
			mThread = new MoaiThread ();
		}

		InitializeAku ( this );
	}
	
    //----------------------------------------------------------------//
	public MoaiView ( Context context, int w, int h ) {

		super ( context );

		Log.e( "MoaiLog", "This is the one that should FAIL!");
				
		if ( mThread == null ) {
			mThread = new MoaiThread ();
		}
		
		mForRen = this;
		mWidth = w;
		mHeight = h;

		mPackageName = context.getPackageName ();
		mUDID = Secure.getString ( context.getContentResolver (), Secure.ANDROID_ID );
		mConMan = ( ConnectivityManager )context.getSystemService ( Context.CONNECTIVITY_SERVICE );
		
		InitializeAku ( this );
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
	
	//================================================================//
	// Device properties callbacks
	//================================================================//
	
	//----------------------------------------------------------------//
	public String GetConnectivity () {
		
		NetworkInfo ni = mConMan.getActiveNetworkInfo ();
		if ( ni != null )
			return ni.getTypeName ();
		else
			return "NOT CONNECTED";
	}		

   //----------------------------------------------------------------//
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
		
		setDeviceProperties ( appName, abi, devBrand, devDes, ma, devModel, devProduct, osName, osVersion, mUDID );
	}

	//----------------------------------------------------------------//
	public String GenerateGUID () {
	
		return java.util.UUID.randomUUID().toString();
	}
	
    //----------------------------------------------------------------//
	public void SetDirectory ( String path ) {
		
		mAppRoot = path;
	}
	
    //----------------------------------------------------------------//
	public void UpdateAccelerometer ( float axisX, float axisY, float axisZ ) {
		
		onUpdateAccelerometer ( axisX, axisY, axisZ );
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
				glFlush (); 
				onUpdateAnim ();
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			glViewport ( 0, 0, width, height );
			mWidth = width;
			mHeight = height;
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {
   
			InitializeAku ( mForRen );
			GetDeviceProperties ();
			
			DetectAkuContext ();
			mSurfaceCreated = true;
			mIsValid = true; 

			setWorkingDirectory ( mAppRoot + "/@WORKING_DIR@" );
			Run ( "@RUN_INIT_DIR@/init.lua", mWidth, mHeight );
			@RUN@
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
					requestRender ();
					time += elapsed;
				}
				yield ();
			} 
		}
	}
	
	protected static native void InitializeAku ( MoaiView view );
	protected static native void Run ( String fileName, int width, int height );
	protected static native void FinalizeAku ();
	protected static native void DeinitializeAku ();
	protected static native void DetectAkuContext ();
	public static native void onDraw ( int width, int height );
	public static native void onUpdateAnim ();
	public static native void onUpdateAccelerometer ( float axisX, float axisY, float axisZ );
	public static native void onUpdateHeading ( int heading );
	public static native void onUpdateLocation ( int longitude, int latitude, int altitude, float hAccuracy, float vAccuracy, float speed );
	public static native void handleTouches (int touch, boolean down, int locX, int locY, int tapCount );
	public static native void setWorkingDirectory ( String path ); 
	
	//Device properties
	public static native void setDeviceProperties ( String appName, String abi, String devBrand, String devDes, String ma, String devModel, 
			String devProduct, String osName, String osVersion, String UDID );
}
