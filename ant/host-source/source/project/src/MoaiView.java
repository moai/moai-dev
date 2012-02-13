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
import android.provider.Settings.Secure;
import android.util.Log;
import android.view.MotionEvent;

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	private MoaiActivity 	mActivity;
	private int 			mAku;
	private Context			mContext;
	private int 		 	mHeight;
	private int 		 	mWidth;
	
	protected static native int	 AKUCreateContext 				();
	protected static native void AKUDeleteContext 				( int akuContextId );
	protected static native void AKUDetectGfxContext 			();
	protected static native void AKUAppDidStartSession			();
	protected static native void AKUEnqueueTouchEvent 			( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount );
	protected static native void AKUExtLoadLuacrypto			();
	protected static native void AKUExtLoadLuacurl				();
	protected static native void AKUExtLoadLuasocket			();
	protected static native void AKUExtLoadLuasql				();
	protected static native void AKUInit 						( MoaiView view, MoaiActivity activity );
	protected static native void AKUPause 						( boolean paused );
	protected static native void AKUReleaseGfxContext			();
	protected static native void AKURender	 					();
	protected static native void AKUReserveInputDevices			( int total );
	protected static native void AKUReserveInputDeviceSensors	( int deviceId, int total );
	protected static native void AKURunScript 					( String filename );
	protected static native void AKUSetContext 					( int akuContextId );
	protected static native void AKUSetDeviceProperties 		( String appName, String appId, String appVersion, String abi, String devBrand, String devName, String devManufacturer, String devModel, String devProduct, String osBrand, String osVersion, String udid );	
	protected static native void AKUSetInputConfigurationName	( String name );
	protected static native void AKUSetInputDevice		 		( int deviceId, String name );
	protected static native void AKUSetInputDeviceCompass 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceLevel 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceLocation 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceTouch 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetScreenSize				( int width, int height );
	protected static native void AKUSetViewSize					( int width, int height );
	protected static native void AKUUntzInit			 		();
	protected static native void AKUUpdate				 		();

    //----------------------------------------------------------------//
	public MoaiView ( Context context, MoaiActivity activity, int width, int height, int glesVersion ) {

		super ( context );
		
		mContext = context;
		mActivity = activity;
		
		// If the screen is locked when the view is created, the orientation 
		// will initially be portrait, so we need to compensate.
		if ( height > width ) {
			mWidth = height;
			mHeight = width;
		}
		else {
			mWidth = width;
			mHeight = height;
		}
		
		mAku = AKUCreateContext ();
		
		AKUExtLoadLuasql ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuasocket ();
		
		AKUSetInputConfigurationName ( "Android" );

		AKUReserveInputDevices			( INPUT_DEVICE.TOTAL.ordinal () );
		AKUSetInputDevice				( INPUT_DEVICE.DEVICE.ordinal (), "device" );
		
		AKUReserveInputDeviceSensors	( INPUT_DEVICE.DEVICE.ordinal (), INPUT_SENSOR.TOTAL.ordinal () );
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

        setRenderer ( new MoaiRenderer ());
		onPause (); // Pause rendering until restarted by the activity lifecycle.
		
		// Enqueue an initialization event on the GL thread. This will run as soon as
		// the GL thread is active. NOTE: This method cannot be called until the 
		// renderer has been set.
		queueEvent ( new Runnable () {

			public void run () {
		
				log ( "MoaiRenderer initializing" );
		
				AKUInit ( MoaiView.this, mActivity );
				
				// TODO: At the moment, enabling accelerometer events before AKU is initialized
				// can cause a crash, so we wait until here. Once the bug is fixed, this line 
				// of code can be moved anywhere.
				mActivity.enableAccelerometerEvents ( false );
			}
		});
	}		
	
	//================================================================//
	// Public methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public String getGUID () {
	
		return java.util.UUID.randomUUID ().toString ();
	}

	//----------------------------------------------------------------//
	public static void log ( String message ) {
		
		Log.i ( "MoaiLog", message );
	}
		
	//----------------------------------------------------------------//
	public void pauseAku ( boolean paused ) {
	
		if ( paused ) {
			AKUPause ( true );
		}
		else {
			AKUPause ( false );
		}
	}

	//----------------------------------------------------------------//
	public void pauseSurface ( boolean paused ) {

		if ( paused ) {
			setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );
			onPause ();
		}
		else {
			onResume ();
			setRenderMode ( GLSurfaceView.RENDERMODE_CONTINUOUSLY );
		}
	}

	//================================================================//
	// Private methods
	//================================================================//

    //----------------------------------------------------------------//
	private void runScript ( String filename ) {

		AKUSetContext ( mAku );
		AKUSetViewSize ( mWidth, mHeight );
		AKURunScript ( filename );
	}	

	//================================================================//
	// Touch methods
	//================================================================//

    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
		isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
			
		final int pointerCount = event.getPointerCount();
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
	public class MoaiRenderer implements GLSurfaceView.Renderer {

		private boolean mRunScriptsExecuted = false;

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			AKUSetContext ( mAku );
			AKUSetViewSize ( mWidth, mHeight );
			AKUUpdate ();
			AKURender ();
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			log ( "MoaiRenderer onSurfaceChanged called" );

			mWidth = width;
			mHeight = height;
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {

			log ( "MoaiRenderer onSurfaceCreated called" );

			AKUReleaseGfxContext ();
			AKUDetectGfxContext ();
			
			if ( !mRunScriptsExecuted ) {
				
				log ( "Running game scripts" );
				
				// TODO: Move this init script to the initialization routine in the constructor
				// as soon as the bug is fixed that will allow the virutal directory to be 
				// mounted before the MoaiView is created.
				runScript ( "@RUN_INIT_DIR@/init.lua" );
			
				@RUN_COMMAND@
				
				mRunScriptsExecuted = true;
			}

			// Since we are pausing and resuming the GL surface along with the
			// activity lifecycle, we know that the GL surface will be destroyed
			// and recreated every time through. So, because we want to ensure
			// that the lua scripts have been run before notifying that the 
			// session has started, this is the appropriate location to do so.
			AKUAppDidStartSession ();
		}		
	}
}
