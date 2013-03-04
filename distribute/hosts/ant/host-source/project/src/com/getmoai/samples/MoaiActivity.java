//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

// Moai
import com.ziplinegames.moai.*;

import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import java.net.URI;
import android.content.Context;
import android.os.AsyncTask;
import android.net.Uri;
import android.provider.Settings.Secure;

//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity {

	private AccelerometerEventListener		mAccelerometerListener = null;
	private Sensor 							mAccelerometerSensor = null;
	private Sensor							mMagnetometerSensor = null;
	private LocationEventListener			mLocationListener = null;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver = null;
	private MoaiView						mMoaiView = null;
	private SensorManager 					mSensorManager = null;
	private LocationManager					mLocationManager = null;
	private boolean							mWaitingToResume = false;
	private boolean							mWindowFocusLost = false;
	private float []						mAccelerometerData = null;
	
	//----------------------------------------------------------------//
	static {
		
		MoaiLog.i ( "Loading libmoai.so" );
		
		System.loadLibrary ( "moai" );
	}

	//----------------------------------------------------------------//
    public void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
        super.onActivityResult ( requestCode, resultCode, data );
		Moai.onActivityResult ( requestCode, resultCode, data );
    }

   	//----------------------------------------------------------------//
    protected void onCreate ( Bundle savedInstanceState ) {

		MoaiLog.i ( "MoaiActivity onCreate: activity CREATED" );

		mAccelerometerData = new float[3];
		
    	super.onCreate ( savedInstanceState );
		Moai.onCreate ( this );
		
		Moai.createContext ();
		Moai.init ();
		
        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_FULLSCREEN );
	    //getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

		try {
			
			ApplicationInfo myApp = getPackageManager ().getApplicationInfo ( getPackageName (), 0 );

			Moai.mount ( "bundle", myApp.publicSourceDir );
			Moai.setWorkingDirectory ( "bundle/assets/@WORKING_DIR@" );				
		} catch ( NameNotFoundException e ) {

			MoaiLog.e ( "MoaiActivity onCreate: Unable to locate the application bundle" );
		}

		if ( getFilesDir () != null ) {
		 
		 	Moai.setDocumentDirectory ( getFilesDir ().getAbsolutePath ());
		} else {

			MoaiLog.e ( "MoaiActivity onCreate: Unable to set the document directory" );
		}
				
		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay ();
		ConfigurationInfo info = (( ActivityManager ) getSystemService ( Context.ACTIVITY_SERVICE )).getDeviceConfigurationInfo ();

	    mMoaiView = new MoaiView ( this, display.getWidth (), display.getHeight (), info.reqGlEsVersion );
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );

		mLocationManager = (LocationManager) getSystemService ( Context.LOCATION_SERVICE );

		startConnectivityReceiver ();
		enableAccelerometerEvents ( false );
		enableLocationEvents ( false );

		setContentView ( mMoaiView );
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		MoaiLog.i ( "MoaiActivity onDestroy: activity DESTROYED" );
		
		super.onDestroy ();	
		Moai.onDestroy ();
		
		stopConnectivityReceiver ();
				
		Moai.finish ();
	}
	
	//----------------------------------------------------------------//
	protected void onNewIntent ( Intent intent ) {
		
		MoaiLog.i ( "MoaiActivity onNewIntent: application started from NEW INTENT" );
		
		setIntent ( intent );
	}

	//----------------------------------------------------------------//
	protected void onPause () {

		MoaiLog.i ( "MoaiActivity onPause: activity PAUSED" );
		
		super.onPause ();	
		Moai.onPause ();
		
		if ( mAccelerometerListener != null ) {
			
			mSensorManager.unregisterListener ( mAccelerometerListener );
		}
		
		if ( mLocationListener != null ) {

			mLocationManager.removeUpdates( mLocationListener );
		}

		// If we've been paused, then we're assuming we've lost focus. 
		// This handles the case where the user presses the lock button
		// very quickly twice, in which case we do not receive the 
		// expected windows focus events.
		//mWindowFocusLost = true;

		MoaiLog.i ( "MoaiActivity onPause: PAUSING now" );
		mMoaiView.pause ( true );
		
		Moai.setApplicationState ( Moai.ApplicationState.APPLICATION_PAUSED );
		
		Moai.endSession ();		
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		MoaiLog.i ( "MoaiActivity onResume: activity RESUMED" );

		super.onResume ();
		Moai.onResume ();
		
		if ( mAccelerometerListener != null ) {
			
			mSensorManager.registerListener ( mAccelerometerListener, mAccelerometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
			mSensorManager.registerListener ( mAccelerometerListener, mMagnetometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
		}

		if ( mLocationListener != null ) {

			mLocationManager.requestLocationUpdates ( LocationManager.NETWORK_PROVIDER, 0, 0, mLocationListener );
			mLocationManager.requestLocationUpdates ( LocationManager.GPS_PROVIDER, 0, 0, mLocationListener );
		}

		// If we have not lost Window focus, then resume immediately; 
		// otherwise, wait to regain focus before we resume. All of 
		// this nonsense is to prevent audio from playing while the
		// screen is locked.
		mWaitingToResume = mWindowFocusLost;
		if ( !mWindowFocusLost ) {
			
			MoaiLog.i ( "MoaiActivity onResume: RESUMING now" );
			mMoaiView.pause ( false );
		}
	}

	//----------------------------------------------------------------//
	protected void onStart () {

		MoaiLog.i ( "MoaiActivity onStart: activity STARTED" );

		super.onStart ();
		Moai.onStart ();
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		MoaiLog.i ( "MoaiActivity onStop: activity STOPPED" );

		super.onStop ();
		Moai.onStop ();
	}
	
	//================================================================//
	// Private methods
	//================================================================//

	//----------------------------------------------------------------//
	private void enableAccelerometerEvents ( boolean enabled ) {
		
		if ( !enabled ) {
			
			if ( mAccelerometerListener != null ) {

				mSensorManager.unregisterListener ( mAccelerometerListener );
				mAccelerometerListener = null;
			}

			if ( mAccelerometerSensor != null ) {
				
				mAccelerometerSensor = null;
			}
		} else if ( enabled ) {
			
			if ( mAccelerometerSensor == null ) {
				
				mAccelerometerSensor = mSensorManager.getDefaultSensor ( Sensor.TYPE_ACCELEROMETER );
				mMagnetometerSensor = mSensorManager.getDefaultSensor ( Sensor.TYPE_MAGNETIC_FIELD );
			}
			
			if ( mAccelerometerListener == null ) {

				mAccelerometerListener = new AccelerometerEventListener ();
				mSensorManager.registerListener ( mAccelerometerListener, mAccelerometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
				mSensorManager.registerListener ( mAccelerometerListener, mMagnetometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
			}
		}
	}

	//----------------------------------------------------------------//
	private void enableLocationEvents ( boolean enabled ) {
		
		if ( !enabled ) {
			
			if ( mLocationListener != null ) {

				mLocationManager.removeUpdates( mLocationListener );
				mLocationListener = null;
			}
		} else if ( enabled ) {
			
			if ( mLocationListener == null ) {

				mLocationListener = new LocationEventListener ();
				mLocationManager.requestLocationUpdates ( LocationManager.NETWORK_PROVIDER, 0, 0, mLocationListener );
				mLocationManager.requestLocationUpdates ( LocationManager.GPS_PROVIDER, 0, 0, mLocationListener );

			}
		}
	}
		
	//----------------------------------------------------------------//
	private void startConnectivityReceiver () {
		
		if ( mConnectivityReceiver == null ) {
			
			mConnectivityReceiver = new ConnectivityBroadcastReceiver ();

			IntentFilter filter = new IntentFilter ();
			filter.addAction ( ConnectivityManager.CONNECTIVITY_ACTION );
			this.registerReceiver ( mConnectivityReceiver, filter );
		}
	}
	
	//----------------------------------------------------------------//
	private void stopConnectivityReceiver () {
		
		this.unregisterReceiver ( mConnectivityReceiver );
		mConnectivityReceiver = null;
	}
	
	//================================================================//
	// KeyEvent methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public boolean onKeyDown ( int keyCode, KeyEvent event ) {

	    if ( keyCode == KeyEvent.KEYCODE_BACK ) {
	        
			if ( Moai.backButtonPressed ()) {
				
				return true;
			}
	    }
	    
	    return super.onKeyDown ( keyCode, event );
	}
	
	//================================================================//
	// WindowEvent methods
	//================================================================//

	//----------------------------------------------------------------//
	public void onWindowFocusChanged ( boolean hasFocus ) {
		
		MoaiLog.i ( "MoaiActivity onWindowFocusChanged: activity FOCUS CHANGED" );
		
		super.onWindowFocusChanged ( hasFocus );
				
		// If we are waiting to resume and just got the window focus back, 
		// it's time to resume. All of this nonsense is to prevent audio 
		// from playing while the screen is locked.
		mWindowFocusLost = !hasFocus;
	
		if ( mWaitingToResume && hasFocus ) {
		
			mWaitingToResume = false;

			MoaiLog.i ( "MoaiActivity onWindowFocusChanged: RESUMING now" );
			mMoaiView.pause ( false );
		}
	}

	//================================================================//
	// ConnectivityBroadcastReceiver
	//================================================================//

	private class ConnectivityBroadcastReceiver extends BroadcastReceiver {

		//----------------------------------------------------------------//
		@Override
		public void onReceive ( Context context, Intent intent ) {
			
			ConnectivityManager manager = ( ConnectivityManager )context.getSystemService ( Context.CONNECTIVITY_SERVICE );
			NetworkInfo networkInfo = manager.getActiveNetworkInfo ();
					
			Moai.ConnectionType connectionType = Moai.ConnectionType.CONNECTION_NONE;
					
			if ( networkInfo != null ) {
				
				 switch ( networkInfo.getType () ) {
					 								
				 	case ConnectivityManager.TYPE_MOBILE: {
					
				 		connectionType = Moai.ConnectionType.CONNECTION_WWAN;
				 		break;
				 	}
					 									
				 	case ConnectivityManager.TYPE_WIFI: {
					
				 		connectionType = Moai.ConnectionType.CONNECTION_WIFI;
				 		break;
				 	}
				 }
			}
			
			MoaiLog.i ( "ConnectivityBroadcastReceiver onReceive: Connection = " + connectionType );
			
			Moai.setConnectionType (( long )connectionType.ordinal ());
		}
	};
	
	//================================================================//
	// AccelerometerEventListener
	//================================================================//

	private class AccelerometerEventListener implements SensorEventListener {
		
		private float [] mGravity;
		private float [] mGeomagnetic;
		private float mRotationMatrixA [] = new float [ 9 ];
		private float mRotationMatrixB [] = new float [ 9 ];
		private float orientation [] = new float [ 3 ];

		//----------------------------------------------------------------//
		public void onAccuracyChanged ( Sensor sensor, int accuracy ) {
			
		}

		// Thanks to NVIDIA for this useful canonical-to-screen orientation function.
		public void canonicalOrientationToScreenOrientation ( int displayRotation, float[] canVec, float[] screenVec ) { 
				
			 final int axisSwap[][] = {
				 { 1,-1, 1, 0 },   // ROTATION_0
				 { 1, 1, 0, 1 },   // ROTATION_90
				 {-1,-1, 1, 0 },   // ROTATION_180
				 { 1,-1, 0, 1 } }; // ROTATION_270

			 final int[] as = axisSwap[displayRotation];
			 screenVec[0] = (float)as[0] * canVec[ as[2] ];
			 screenVec[1] = (float)as[1] * canVec[ as[3] ];
			 screenVec[2] = canVec[2];
		}
		
		//----------------------------------------------------------------//
		public void onSensorChanged ( SensorEvent event ) {

			if ( event.sensor.getType () == Sensor.TYPE_ACCELEROMETER ) {

				Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay ();
                canonicalOrientationToScreenOrientation ( display.getRotation (), event.values, mAccelerometerData );
                
                float x = mAccelerometerData [ 0 ];
                float y = mAccelerometerData [ 1 ];
                float z = mAccelerometerData [ 2 ];
                
				mGravity = mAccelerometerData;

				int deviceId = Moai.InputDevice.INPUT_DEVICE.ordinal ();
				int sensorId = Moai.InputSensor.SENSOR_LEVEL.ordinal ();
                
                // normalize the vector
                double mag = Math.sqrt ( x * x + y * y + z * z );
                x = x / ( float ) mag;
                y = y / ( float ) mag;
                z = z / ( float ) mag;

				Moai.enqueueLevelEvent ( deviceId, sensorId, x, y, z );
			}
			else if ( event.sensor.getType () == Sensor.TYPE_MAGNETIC_FIELD )
				mGeomagnetic = event.values;

			if ( mGravity != null && mGeomagnetic != null && SensorManager.getRotationMatrix( mRotationMatrixA, null, mGravity, mGeomagnetic ) ) {

				int deviceId = Moai.InputDevice.INPUT_DEVICE.ordinal ();
				int sensorId = Moai.InputSensor.SENSOR_COMPASS.ordinal ();

				SensorManager.remapCoordinateSystem (
					mRotationMatrixA,
					SensorManager.AXIS_X,
					SensorManager.AXIS_Z,
					mRotationMatrixB );

				SensorManager.getOrientation ( mRotationMatrixB, orientation );
				float heading = orientation [0] * 57.2957795f; 
				if ( heading < 0 ) heading += 360;

				Moai.enqueueCompassEvent ( deviceId, sensorId, heading );
			}
		}
	};

	//================================================================//
	// LocationEventListener
	//================================================================//

	private class LocationEventListener implements LocationListener {
		
		//----------------------------------------------------------------//
		public void onLocationChanged ( Location location ) {

			double longitude = location.getLongitude ();
			double latitude = location.getLatitude ();
			double altitude = location.getAltitude ();
			float hAccuracy = location.getAccuracy ();
			float vAccuracy = location.getAccuracy ();
			float speed = location.getSpeed ();

			int deviceId = Moai.InputDevice.INPUT_DEVICE.ordinal ();
			int sensorId = Moai.InputSensor.SENSOR_LOCATION.ordinal ();

			Moai.enqueueLocationEvent ( deviceId, sensorId, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
		}

		//----------------------------------------------------------------//
		public void onStatusChanged ( String provider, int status, Bundle extras ) {}

		//----------------------------------------------------------------//
		public void onProviderEnabled ( String provider ) {}

		//----------------------------------------------------------------//
		public void onProviderDisabled ( String provider ) {}
	};
}
