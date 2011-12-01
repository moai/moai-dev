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
import android.view.MotionEvent;

//----------------------------------------------------------------//
enum CONNECTION_TYPE {
	NONE,
	WIFI,
	WWAN,
};

//----------------------------------------------------------------//
enum MoaiInputDeviceID {
	DEVICE,
	TOTAL,
};

//----------------------------------------------------------------//
enum MoaiInputDeviceSensorID {
	COMPASS,
	LEVEL,
	LOCATION,
	TOUCH,
	TOTAL,
};

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	private int 					mAku;
	private String 					mAppRoot;
	private Context					mContext;
	private int 					mHeight;
	private MoaiRenderer 			mRenderer;
	private boolean					mSensorsEnabled;
	private int 					mWidth; 
	
	protected static native int	 AKUCreateContext 				();
	protected static native void AKUDeleteContext 				( int akuContextId );
	protected static native void AKUDetectGfxContext 			();
	protected static native void AKUEnqueueTouchEvent 			( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount );
	protected static native void AKUExtLoadLuacrypto			();
	protected static native void AKUExtLoadLuacurl				();
	protected static native void AKUExtLoadLuasocket			();
	protected static native void AKUExtLoadLuasql				();
	protected static native void AKUFinalize 					();
	protected static native void AKUInit 						( MoaiView view, MoaiActivity activity );
	protected static native void AKUPause 						( boolean paused );
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
	protected static native void AKUSetWorkingDirectory 		( String path ); 
	protected static native void AKUUntzInit			 		();
	protected static native void AKUUpdate				 		();

    //----------------------------------------------------------------//
	public MoaiView ( Context context, MoaiActivity activity, int width, int height ) {

		super ( context );

		mContext = context;
		mHeight = height;
		mSensorsEnabled = false;
		mWidth = width;
		
		initMoai ( activity );
	}
	
	//----------------------------------------------------------------//
	public String getGUID () {
	
		return java.util.UUID.randomUUID ().toString ();
	}

    //----------------------------------------------------------------//
	public boolean getSensorsEnabled () {
	
		return mSensorsEnabled;
	}

	//----------------------------------------------------------------//
	private void initDeviceProperties () {

		// get appId
		String appId = mContext.getPackageName ();
		
		// get appName
		String appName = appId;

		String[] packNameParts = appId.split ( "." );

		if ( packNameParts.length > 1 ) {
			appName = packNameParts [ packNameParts.length - 1 ];
		}
		
		// get appVersion
		String appVersion;
		try {
			appVersion = mContext.getPackageManager ().getPackageInfo ( appId, 0 ).versionName;
		}
		catch ( Exception e ) {
			appVersion = "UNKNOWN";
		}
		
		// get udid
		String udid	= Secure.getString ( mContext.getContentResolver (), Secure.ANDROID_ID );
		if ( udid == null ) {
			udid = "UNKNOWN";
		}

		// get other
		String osBrand 			= "Android";
		String abi 				= Build.CPU_ABI;
		String devBrand 		= Build.BRAND;
		String devName			= Build.DEVICE;
		String devManufacturer 	= Build.MANUFACTURER;
		String devModel 		= Build.MODEL;
		String devProduct 		= Build.PRODUCT;
		String osVersion 		= Build.VERSION.RELEASE;
		
		// tell Moai the device properties
		AKUSetDeviceProperties ( appName, appId, appVersion, abi, devBrand, devName, devManufacturer, devModel, devProduct, osBrand, osVersion, udid );
	}
		
	//----------------------------------------------------------------//
	private void initMoai ( MoaiActivity activity ) {
	
		mAku = AKUCreateContext ();
		
		AKUExtLoadLuasql ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuasocket ();
		
		AKUSetInputConfigurationName 	( "Android" );

		AKUReserveInputDevices			( MoaiInputDeviceID.TOTAL.ordinal () );
		AKUSetInputDevice				( MoaiInputDeviceID.DEVICE.ordinal (), "device" );
		
		AKUReserveInputDeviceSensors	( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.TOTAL.ordinal () );
		AKUSetInputDeviceCompass		( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.COMPASS.ordinal (),		"compass" );
		AKUSetInputDeviceLevel			( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.LEVEL.ordinal (),		"level" );
		AKUSetInputDeviceLocation		( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.LOCATION.ordinal (),		"location" );
		AKUSetInputDeviceTouch			( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.TOUCH.ordinal (),		"touch" );

		AKUUntzInit ();
		AKUSetScreenSize ( mWidth, mHeight );

		initDeviceProperties ();
		
		AKUInit ( this, activity );
		mSensorsEnabled = true;
	}

    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
		isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
		int pointerId = ( event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT ) + 1;
		
		AKUEnqueueTouchEvent (
			MoaiInputDeviceID.DEVICE.ordinal (),
			MoaiInputDeviceSensorID.TOUCH.ordinal (),
			pointerId, 
			isDown, 
			Math.round ( event.getX () ), 
			Math.round ( event.getY () ), 
			1
		);
		
		return true;
	}
		
	//----------------------------------------------------------------//
	public void pause ( boolean paused ) {
	
		if ( paused ) {
			AKUPause ( true );
			setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );
		}
		else {
			setRenderMode ( GLSurfaceView.RENDERMODE_CONTINUOUSLY );
			AKUPause ( false );
		}
	}
	
	//----------------------------------------------------------------//
	public void run ( String filename ) {
	
		AKUSetContext ( mAku );
		AKUSetViewSize ( mWidth, mHeight );
		AKURunScript ( filename );
	}
	
    //----------------------------------------------------------------//
	public void setDirectory ( String path ) {
		
		mAppRoot = path;
	}
	
	//================================================================//
	// MoaiRenderer
	//================================================================//
	public class MoaiRenderer implements GLSurfaceView.Renderer {

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			AKUSetContext ( mAku );
			AKUSetViewSize ( mWidth, mHeight );
			AKURender ();

			AKUSetContext ( mAku );
			AKUUpdate ();
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			MoaiActivity.log ( "MoaiRenderer onSurfaceChanged called" );

			mWidth = width;
			mHeight = height;
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {

			MoaiActivity.log ( "MoaiRenderer onSurfaceCreated called" );
   
			AKUDetectGfxContext ();

			AKUSetWorkingDirectory ( mAppRoot + "/@WORKING_DIR@" );
			run ( "@RUN_INIT_DIR@/init.lua" );
			@RUN@
			
			MoaiActivity.startSession ();
		}
	}
}
