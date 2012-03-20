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
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

// Moai
import com.ziplinegames.moai.*;

//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity {

	private AccelerometerEventListener		mAccelerometerListener = null;
	private Sensor 							mAccelerometerSensor = null;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver = null;
	private MoaiView						mMoaiView = null;
	private SensorManager 					mSensorManager = null;
	private boolean							mWaitingToResume = false;
	private boolean							mWindowFocusLost = false;

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

    	super.onCreate ( savedInstanceState );
		Moai.onCreate ( this );
		
		Moai.createContext ();
		Moai.init ();
		
        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_FULLSCREEN );
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

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

		startConnectivityReceiver ();
		enableAccelerometerEvents ( false );	

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
		
		// If we've been paused, then we're assuming we've lost focus. 
		// This handles the case where the user presses the lock button
		// very quickly twice, in which case we do not receive the 
		// expected windows focus events.
		mWindowFocusLost = true;

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
			}
			
			if ( mAccelerometerListener == null ) {

				mAccelerometerListener = new AccelerometerEventListener ();
				mSensorManager.registerListener ( mAccelerometerListener, mAccelerometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
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
		
		//----------------------------------------------------------------//
		public void onAccuracyChanged ( Sensor sensor, int accuracy ) {
			
		}

		//----------------------------------------------------------------//
		public void onSensorChanged ( SensorEvent event ) {

			float x = event.values [ 0 ];
			float y = event.values [ 1 ];
			float z = event.values [ 2 ];

			int deviceId = Moai.InputDevice.INPUT_DEVICE.ordinal ();
			int sensorId = Moai.InputSensor.SENSOR_LEVEL.ordinal ();

			Moai.enqueueLevelEvent ( deviceId, sensorId, x, y, z );
		}
	};
}