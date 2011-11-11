//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import java.io.BufferedInputStream;   
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream; 

import @PACKAGE@.R;

import android.app.Activity; 
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;
 
//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements SensorEventListener {

	private Sensor 				mAccelerometer;
	private MoaiView			mMoaiView;
	private SensorManager 		mSensorManager;
	
	protected static native void AKUEnqueueCompassEvent 	( int heading );
	protected static native void AKUEnqueueLevelEvent 		( int deviceId, int sensorId, float x, float y, float z );
	protected static native void AKUEnqueueLocationEvent 	( int deviceId, int sensorId, int longitude, int latitude, int altitude, float hAccuracy, float vAccuracy, float speed );

	//----------------------------------------------------------------//
	public static void log ( String message ) {
		
		Log.i ( "MoaiLog", message );
	}
	
    //----------------------------------------------------------------//
    protected void onCreate ( Bundle savedInstanceState ) {

		log ( "MoaiActivity onCreate called" );

		// call super
    	super.onCreate ( savedInstanceState );

		// load libmoai
       	System.load ( "/data/data/@PACKAGE@/lib/libmoai.so" ); 

		// configure window
        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_FULLSCREEN ); 
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

		// get display dimenstions
		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay ();
		int displayWidth = display.getWidth ();
		int displayHeight = display.getHeight ();
		
		// create Moai view
	    mMoaiView = new MoaiView ( this, displayWidth, displayHeight );

		// detect OpenGL 2.0
		ActivityManager am = ( ActivityManager ) getSystemService ( Context.ACTIVITY_SERVICE );
        ConfigurationInfo info = am.getDeviceConfigurationInfo ();
	    
		if ( info.reqGlEsVersion >= 0x20000 ) {
			mMoaiView.setEGLContextClientVersion ( 2 );
		}

		// create custom renderer for the Moai view
        mMoaiView.setRenderer ( mMoaiView.new MoaiRenderer () );
        mMoaiView.setRenderMode ( GLSurfaceView.RENDERMODE_CONTINUOUSLY );

		// set activity to use Moai view
		setContentView ( mMoaiView );
		
		// get access to the accelerometer sensor
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );
		mAccelerometer = mSensorManager.getDefaultSensor ( Sensor.TYPE_ACCELEROMETER );
		
		// unpack assets
		File externalFilesDir = new File ( getExternalFilesDir ( null ), "" );
    	unpackAssets ( externalFilesDir );
	    mMoaiView.setDirectory ( externalFilesDir.getAbsolutePath () );
	
		// run Lua scripts
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		log ( "MoaiActivity onDestroy called" );
		
		// call super
		super.onDestroy ();
	}

	//----------------------------------------------------------------//
	protected void onPause () {

		log ( "MoaiActivity onPause called" );
		
		// call super
		super.onPause ();
		
		// unregister for accelerometer events
		mSensorManager.unregisterListener ( this );
	}

	//----------------------------------------------------------------//
	protected void onStart () {

		log ( "MoaiActivity onStart called" );

		// call super
		super.onStart ();
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		log ( "MoaiActivity onStop called" );

		// call super
		super.onStop ();
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		log ( "MoaiActivity onResume called" );

		// call super
		super.onResume ();
		
		// register for accelerometer events
		mSensorManager.registerListener ( this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL );
	}
		
    //----------------------------------------------------------------//
	private void unpackAssets ( File rootDir ) {
		
		log ( "MoaiActivity unpackingAssets . . ." );
		
		InputStream is = getResources ().openRawResource ( R.raw.bundle );
		File extractTo = new File ( rootDir.getAbsolutePath () + "/" );
		ZipInputStream zis;
		
		try {
			
			zis = new ZipInputStream ( new BufferedInputStream ( is ));
			
			ZipEntry ze; 
			byte [] buffer = new byte [ 4096 ];
			
			while (( ze = zis.getNextEntry ()) != null ) {
				
				File file = new File ( extractTo, ze.getName ());
				
				if ( ze.isDirectory () && !file.exists ()) {
					file.mkdirs();
					
					while ( zis.read ( buffer ) != -1 );
				}
				else {
					if ( !file.getParentFile().exists()) {
						file.getParentFile().mkdirs();
					}
					
					BufferedOutputStream out = new BufferedOutputStream ( new FileOutputStream ( file ));
					
					int count;
					while (( count = zis.read ( buffer )) != -1) {
						out.write ( buffer, 0, count ); 
					}
					out.close();
				}
			}
			zis.close();
		}
		catch ( Exception e ) {
			Log.e ( "MoaiActivity", "Unable to read or write to SD card");
		}
		finally {
		}
		
		log ( "MoaiActivity unpackingAssets complete" );
	}

	//================================================================//
	// SensorEventListener methods
	//================================================================//

	//----------------------------------------------------------------//
	public void onAccuracyChanged ( Sensor sensor, int accuracy ) {
	}
	
	//----------------------------------------------------------------//
	public void onSensorChanged ( SensorEvent event ) {
		
		if ( ! mMoaiView.getSensorsEnabled () ) {
			return;
		}
		
		float x = event.values [ 0 ];
		float y = event.values [ 1 ];
		float z = event.values [ 2 ];
		
		int deviceId = MoaiInputDeviceID.DEVICE.ordinal ();
		int sensorId = MoaiInputDeviceSensorID.LEVEL.ordinal ();
		
		AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
	}
}
