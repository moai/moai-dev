//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;
 
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings.Secure;
import android.view.MotionEvent;

// README: Aku is a thread-unaware platform, by design. Therefore, it is important
// for hosts in a mult-threaded environment such as Android ensure that Aku calls
// are made on the appropriate thread. Furthermore, because some Aku callbacks
// that occur during application run-time need to be run on the Android main (UI)
// thread, we choose to run ALL Aku callbacks that CAN be run on the main thread 
// on the main thread. Notable exceptions include creating and release the graphics
// context and the call to render the scene. Otherwise, all threading decisions 
// regarding Aku in this, and the activity class, are precisely determined in order
// to guarantee reliable execution. WARNING: Moving ANY call to Aku MAY result in
// its execution on a different thread and MAY affect execution and/or result in
// thead deadlocking.

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	private MoaiActivity 	mActivity;
	private int 			mAku;
	private Context			mContext;
	private Handler			mHandler;
	private int 		 	mHeight;
	private Runnable		mUpdateRunnable;
	private int 		 	mWidth;
	
	private static final Object		sAkuLock = new Object ();
	private static final long		AKU_UPDATE_FREQUENCY = 1000 / 60; // 60 Hz, in milliseconds
	
	// Threading indications; M = Runs on Main thread, R = Runs on GL thread,
	// ? = Runs on arbitrary thread.
	// NOTE that this is based on the current Android host setup; if you move
	// any one of these calls elesewhere in the host, you may alter what thread
	// it executes on. 
	protected static native int	 AKUCreateContext 				(); // M
	protected static native void AKUDetectGfxContext 			(); // R
	protected static native void AKUAppDidStartSession			( boolean resumed ); // M
	protected static native void AKUEnqueueTouchEvent 			( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount ); // M
	protected static native void AKUExtLoadLuacrypto			(); // M
	protected static native void AKUExtLoadLuacurl				(); // M
	protected static native void AKUExtLoadLuasocket			(); // M
	protected static native void AKUExtLoadLuasql				(); // M
	protected static native void AKUInit 						( MoaiView view, MoaiActivity activity ); // M
	protected static native void AKUPause 						( boolean paused ); // M
	protected static native void AKURender	 					(); // R
	protected static native void AKUReserveInputDevices			( int total ); // M 
	protected static native void AKUReserveInputDeviceSensors	( int deviceId, int total ); // M
	protected static native void AKURunScript 					( String filename ); // M
	protected static native void AKUSetContext 					( int akuContextId ); // M
	protected static native void AKUSetDeviceProperties 		( String appName, String appId, String appVersion, String abi, String devBrand, String devName, String devManufacturer, String devModel, String devProduct, String osBrand, String osVersion, String udid ); // M
	protected static native void AKUSetInputConfigurationName	( String name ); // M
	protected static native void AKUSetInputDevice		 		( int deviceId, String name ); // M
	protected static native void AKUSetInputDeviceCompass 		( int deviceId, int sensorId, String name ); // M
	protected static native void AKUSetInputDeviceLevel 		( int deviceId, int sensorId, String name ); // M
	protected static native void AKUSetInputDeviceLocation 		( int deviceId, int sensorId, String name ); // M
	protected static native void AKUSetInputDeviceTouch 		( int deviceId, int sensorId, String name ); // M
	protected static native void AKUSetScreenSize				( int width, int height ); // M
	protected static native void AKUSetViewSize					( int width, int height ); // R
	protected static native void AKUUntzInit			 		(); // M
	protected static native void AKUUpdate				 		(); // M

    //----------------------------------------------------------------//
	public MoaiView ( Context context, MoaiActivity activity, int width, int height, int glesVersion ) {

		super ( context );
		
		mContext = context;
		mActivity = activity;
		
		// If the screen is locked when the view is created, the orientation 
		// will initially be portrait, so we need to compensate. This, of 
		// course, assumes that you're locking your application to landscape
		// mode in the manifest.
		if ( height > width ) {
			
			mWidth = height;
			mHeight = width;
		}
		else {
			
			mWidth = width;
			mHeight = height;
		}
		
		mAku = AKUCreateContext ();
		AKUSetContext ( mAku );
		
		AKUExtLoadLuasql ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuasocket ();
		
		AKUSetInputConfigurationName ( "Android" );

		AKUReserveInputDevices			( INPUT_DEVICE.TOTAL.ordinal ());
		AKUSetInputDevice				( INPUT_DEVICE.DEVICE.ordinal (), "device" );
		
		AKUReserveInputDeviceSensors	( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.TOTAL.ordinal ());
		AKUSetInputDeviceCompass		( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.COMPASS.ordinal (),		"compass" );
		AKUSetInputDeviceLevel			( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.LEVEL.ordinal (),		"level" );
		AKUSetInputDeviceLocation		( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.LOCATION.ordinal (),		"location" );
		AKUSetInputDeviceTouch			( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.TOUCH.ordinal (),		"touch" );

		AKUUntzInit ();
		AKUSetScreenSize ( mWidth, mHeight );

		String appId = mContext.getPackageName ();
		
		String appName;
		try {
			
		    appName = mContext.getPackageManager ().getApplicationLabel ( mContext.getPackageManager ().getApplicationInfo ( appId, 0 )).toString ();
		} catch ( Exception e ) {
			
			appName = "UNKNOWN";
		}
		
		String appVersion;
		try {
			
			appVersion = mContext.getPackageManager ().getPackageInfo ( appId, 0 ).versionName;
		}
		catch ( Exception e ) {
			
			appVersion = "UNKNOWN";
		}
		
		String udid	= Secure.getString ( mContext.getContentResolver (), Secure.ANDROID_ID );
		if ( udid == null ) {
			
			udid = "UNKNOWN";
		}

		String osBrand 			= "Android";
		String abi 				= Build.CPU_ABI;
		String devBrand 		= Build.BRAND;
		String devName			= Build.DEVICE;
		String devManufacturer 	= Build.MANUFACTURER;
		String devModel 		= Build.MODEL;
		String devProduct 		= Build.PRODUCT;
		String osVersion 		= Build.VERSION.RELEASE;
		
		AKUSetDeviceProperties ( appName, appId, appVersion, abi, devBrand, devName, devManufacturer, devModel, devProduct, osBrand, osVersion, udid );
		
		if ( glesVersion >= 0x20000 ) {
			
			// NOTE: Must be set before the renderer is set.
			setEGLContextClientVersion ( 2 );
		}
		
		// Create a handler that we can use to post to the main thread and a pseudo-
		// periodic runnable that will handle calling AKUUpdate on the main thread.
		mHandler = new Handler ( Looper.getMainLooper ());
		mUpdateRunnable = new Runnable () {

			public void run () {
			
				synchronized ( sAkuLock ) {
				
					AKUUpdate ();
				}

				mHandler.postDelayed ( mUpdateRunnable , AKU_UPDATE_FREQUENCY );
			}
		};

        setRenderer ( new MoaiRenderer ());
		onPause (); // Pause rendering until restarted by the activity lifecycle.
		
		AKUInit ( this, mActivity );
		
		// TODO: At the moment, enabling accelerometer events before AKU is initialized
		// can cause a crash, so we wait until here. Once the bug is fixed, this line 
		// of code can be moved anywhere.
		mActivity.enableAccelerometerEvents ( false );
	}		
	
	//================================================================//
	// Public methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public String getGUID () {
	
		return java.util.UUID.randomUUID ().toString ();
	}

	//----------------------------------------------------------------//
	public void pause ( boolean paused ) {
	
		if ( paused ) {

			mHandler.removeCallbacks ( mUpdateRunnable );
			
			AKUPause ( true );
			setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );
			onPause ();			
		}
		else {

			onResume ();
			setRenderMode ( GLSurfaceView.RENDERMODE_CONTINUOUSLY );
			AKUPause ( false );

			mHandler.postDelayed ( mUpdateRunnable , AKU_UPDATE_FREQUENCY );
		}
	}

	//================================================================//
	// Touch methods
	//================================================================//

    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
		isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
			
		final int pointerCount = event.getPointerCount ();
		for ( int pointerIndex = 0; pointerIndex < pointerCount; ++pointerIndex ) {
				
			int pointerId = event.getPointerId ( pointerIndex );
								
			AKUEnqueueTouchEvent (
				INPUT_DEVICE.DEVICE.ordinal (),
				INPUT_SENSOR.TOUCH.ordinal (),
				pointerId, 
				isDown, 
				Math.round ( event.getX ( pointerIndex )), 
				Math.round ( event.getY ( pointerIndex )), 
				1
			);
		}			
		
		return true;
	}
	
	//================================================================//
	// MoaiRenderer
	//================================================================//
	private class MoaiRenderer implements GLSurfaceView.Renderer {

		private boolean mRunScriptsExecuted = false;

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			synchronized ( sAkuLock ) {
				
				AKURender ();
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			MoaiLog.i ( "MoaiRenderer onSurfaceChanged: surface CHANGED" );

			mWidth = width;
			mHeight = height;
			
			AKUSetViewSize ( mWidth, mHeight );
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {

			MoaiLog.i ( "MoaiRenderer onSurfaceCreated: surface CREATED" );

			synchronized ( sAkuLock ) {
				
				AKUDetectGfxContext ();
			}
			
			if ( !mRunScriptsExecuted ) {

				mRunScriptsExecuted = true;

				mHandler.postAtFrontOfQueue ( new Runnable () {

					public void run () {
				
						MoaiLog.i ( "MoaiRenderer onSurfaceCreated: Running game scripts" );
				
						@RUN_COMMAND@

						synchronized ( sAkuLock ) {

							// Sessions are ended in MoaiActivity.
							AKUAppDidStartSession ( false );
						}
					}
				});
			} else {
				
				mHandler.post ( new Runnable () {

					public void run () {
				
						synchronized ( sAkuLock ) {

							// Sessions are ended in MoaiActivity.
							AKUAppDidStartSession ( true );
						}
					}
				});
			}
		}	
		
	    //----------------------------------------------------------------//
		private void runScripts ( String [] filenames ) {

			for ( String file : filenames ) {
				
				synchronized ( sAkuLock ) {
					
					MoaiLog.i ( "MoaiRenderer runScripts: Running " + file + " script" );
					
					AKURunScript ( file );
				}
			}
		}	
	}
}
