//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;
 
import static android.opengl.GLES20.glFlush;
import static android.opengl.GLES20.glViewport;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.SystemClock;
import android.provider.Settings.Secure;
import android.view.MotionEvent;

//----------------------------------------------------------------//
enum CONNECTION_TYPE {
	NONE,
	WIFI,
	WWAN
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
	private ConnectivityManager 	mConMgr;
	private Context					mContext;
	private int 					mHeight;
	private boolean					mIsValid;
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
	protected static native void AKUInit 						( MoaiView view );
	protected static native void AKUPause 						( boolean paused );
	protected static native void AKURender	 					();
	protected static native void AKUReserveInputDevices			( int total );
	protected static native void AKUReserveInputDeviceSensors	( int deviceId, int total );
	protected static native void AKUResize	 					( int width, int height );
	protected static native void AKURunScript 					( String filename );
	protected static native void AKUSetContext 					( int akuContextId );
	protected static native void AKUSetDeviceProperties 		( String appName, String abi, String devBrand, String devName, String devManufacturer, String devModel, String devProduct, String osBrand, String osVersion, String udid );	
	protected static native void AKUSetInputConfigurationName	( String name );
	protected static native void AKUSetInputDevice		 		( int deviceId, String name );
	protected static native void AKUSetInputDeviceCompass 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceLevel 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceLocation 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetInputDeviceTouch 		( int deviceId, int sensorId, String name );
	protected static native void AKUSetWorkingDirectory 		( String path ); 
	protected static native void AKUUntzInit			 		();
	protected static native void AKUUpdate				 		();

	//----------------------------------------------------------------//
	public String getConnectivity () {
		
		NetworkInfo networkInfo = mConMgr.getActiveNetworkInfo ();
		
		if ( networkInfo != null ) {
			return networkInfo.getTypeName ();
		}
		else {
			return "NOT CONNECTED";
		}
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
	public MoaiView ( Context context, int width, int height ) {

		super ( context );

		mConMgr = ( ConnectivityManager )context.getSystemService ( Context.CONNECTIVITY_SERVICE );
		mContext = context;
		mHeight = height;
		mIsValid = false;
		mSensorsEnabled = false;
		mWidth = width;
		
		AKUInit ( this );
		moaiInit ();
	}

	//----------------------------------------------------------------//
	private void moaiInit () {
	
		mAku = AKUCreateContext ();
		
		// AKUExtLoadLuasql ();
		// AKUExtLoadLuacurl ();
		// AKUExtLoadLuacrypto ();
		// AKUExtLoadLuasocket ();
		
		AKUSetInputConfigurationName 	( "Android" );

		AKUReserveInputDevices			( MoaiInputDeviceID.TOTAL.ordinal () );
		AKUSetInputDevice				( MoaiInputDeviceID.DEVICE.ordinal (), "device" );
		
		AKUReserveInputDeviceSensors	( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.TOTAL.ordinal () );
		AKUSetInputDeviceCompass		( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.COMPASS.ordinal (),		"compass" );
		AKUSetInputDeviceLevel			( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.LEVEL.ordinal (),		"level" );
		AKUSetInputDeviceLocation		( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.LOCATION.ordinal (),		"location" );
		AKUSetInputDeviceTouch			( MoaiInputDeviceID.DEVICE.ordinal (), MoaiInputDeviceSensorID.TOUCH.ordinal (),		"touch" );

		AKUUntzInit ();
		
		mSensorsEnabled = true;

		// set device properties
		String osBrand 			= "Android";
		String appName 			= mContext.getPackageName ();
		String udid 			= Secure.getString ( mContext.getContentResolver (), Secure.ANDROID_ID );
		String abi 				= Build.CPU_ABI;
		String devBrand 		= Build.BRAND;
		String devName			= Build.DEVICE;
		String devManufacturer 	= Build.MANUFACTURER;
		String devModel 		= Build.MODEL;
		String devProduct 		= Build.PRODUCT;
		String osVersion 		= Build.VERSION.RELEASE;
		
		AKUSetDeviceProperties ( appName, abi, devBrand, devName, devManufacturer, devModel, devProduct, osBrand, osVersion, udid );
	}

    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		if ( mIsValid ) {

			boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
			isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
		
			AKUEnqueueTouchEvent (
				MoaiInputDeviceID.DEVICE.ordinal (),
				MoaiInputDeviceSensorID.TOUCH.ordinal (),
				event.getActionIndex (), 
				isDown, 
				Math.round ( event.getX () ), 
				Math.round ( event.getY () ), 
				1 
			);
		}
		
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
		AKUResize ( mWidth, mHeight );
		AKURunScript ( filename );
	}
	
    //----------------------------------------------------------------//
	public void setDirectory ( String path ) {
		
		mAppRoot = path;
	}
	
	//================================================================//
	// MoaiRenderer
	//================================================================//
	class MoaiRenderer implements GLSurfaceView.Renderer {

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			if ( mIsValid ) {
				
				AKUSetContext ( mAku );
				AKUResize ( mWidth, mHeight );
				AKURender ();

				glFlush ();
			
				AKUSetContext ( mAku );
				AKUUpdate ();
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			MoaiActivity.log ( "MoaiRenderer onSurfaceChanged called" );

			glViewport ( 0, 0, width, height );
			mWidth = width;
			mHeight = height;
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {

			MoaiActivity.log ( "MoaiRenderer onSurfaceCreated called" );
   
			AKUDetectGfxContext ();
			mIsValid = true;

			AKUSetWorkingDirectory ( mAppRoot + "/@WORKING_DIR@" );
			run ( "@RUN_INIT_DIR@/init.lua" );
			@RUN@
		}
	}
}
