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
import android.widget.LinearLayout;

// Moai
import com.ziplinegames.moai.*;

import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import java.net.URI;
import android.content.Context;
import android.os.AsyncTask;
import android.net.Uri;
import android.provider.Settings.Secure;

// OUYA CODE
import tv.ouya.console.api.OuyaController;
import tv.ouya.console.api.OuyaFacade;
import android.view.MotionEvent;
import android.os.Handler;
import android.os.Looper;
// END OUYA CODE

// MOGA
import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;

//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements ControllerListener {

	private AccelerometerEventListener		mAccelerometerListener = null;
	private Sensor							mAccelerometerSensor = null;
	private Sensor							mMagnetometerSensor = null;
	private LocationEventListener			mLocationListener = null;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver = null;
	private MoaiView						mMoaiView = null;
	private SensorManager 					mSensorManager = null;
	private LocationManager					mLocationManager = null;
	private boolean							mWaitingToResume = false;
	private boolean							mWindowFocusLost = false;
	private float []						mAccelerometerData = null;
	private float[]							mLS = new float[2];
	private float[] 						mRS = new float[2];
	private float[]							mTrigg = new float[2];
	private Controller						mController = null;
	private Handler 						mButtonHandler = null;
	private Runnable 						mMenuButtonDown = null;

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
    	// OUYA CODE
    	OuyaFacade.getInstance().init(this, "@OUYA_DEV_ID@");
    	if (OuyaFacade.getInstance().isRunningOnOUYAHardware()) {
  			OuyaController.init(this);
  			mButtonHandler = new Handler ( Looper.getMainLooper ());
  			mMenuButtonDown = new Runnable () {
  				public void run () {
  					MoaiLog.i("ButtonUp");
  					Moai.AKUEnqueueKeyboardEvent(1,3,6, false);
  				}
  			};
  			try {
  				java.io.InputStream inputStream = getResources().openRawResource(R.raw.ouyakey);
  				byte[] applicationKey = new byte[inputStream.available()];
  				inputStream.read(applicationKey);
  				inputStream.close();
  				java.security.spec.X509EncodedKeySpec keySpec = new java.security.spec.X509EncodedKeySpec(applicationKey);
  				java.security.KeyFactory keyFactory = java.security.KeyFactory.getInstance("RSA");
  				java.security.PublicKey	publicKey = keyFactory.generatePublic(keySpec);
  				MoaiOuyaBilling.setPublicKey(publicKey);
  			} catch (Exception e) {
  				MoaiLog.e("Unable to create encryption key", e);
  			}
      	// END OUYA CODE
    	} else {
    	 §mController = Controller.getInstance(this);
			 mController.init();
			 mController.setListener(this, new Handler());
    	}

      MoaiLog.i ( "MoaiActivity onCreate: activity CREATED" );
      mAccelerometerData = new float[3];
		super.onCreate ( savedInstanceState );
		Moai.onCreate ( this );

		Moai.createContext ();
		Moai.init ();
		requestWindowFeature ( Window.FEATURE_NO_TITLE );
		getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_FULLSCREEN );
		getWindow ().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		//getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

		try {

			ApplicationInfo myApp = getPackageManager ().getApplicationInfo ( getPackageName (), 0 );

			Moai.mount ( "bundle", myApp.sourceDir );
			Moai.setWorkingDirectory ( "bundle/assets/@WORKING_DIR@" );
		} catch ( NameNotFoundException e ) {

			MoaiLog.e ( "MoaiActivity onCreate: Unable to locate the application bundle" );
		}

		if ( getFilesDir () != null ) {
			java.io.File f = new java.io.File(getFilesDir() + "/documents");
			f.mkdirs();
			Moai.setDocumentDirectory (f.getAbsolutePath());
			f = new java.io.File(this.getApplicationContext().getFilesDir() + "/cache");
			f.mkdirs();
			Moai.setCacheDirectory(f.getAbsolutePath());
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

		LinearLayoutIMETrap con = MoaiKeyboard.getContainer ();
		setContentView ( con );
		con.addView ( mMoaiView );
		con.addView ( MoaiKeyboard.getEditText ());
	}

	//----------------------------------------------------------------//
	protected void onDestroy () {

		MoaiLog.i ( "MoaiActivity onDestroy: activity DESTROYED" );

		super.onDestroy ();
		Moai.onDestroy ();

		stopConnectivityReceiver ();
		if (mController != null) {
			mController.exit();
		}

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

		if (mController != null) {
			mController.onPause();
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

		if (mController != null) {
			mController.onResume();
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
		boolean handled = false;

		switch(keyCode){
			// OUYA CODE
			case OuyaController.BUTTON_O:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 0, true);
				handled = true;
				break;
			case OuyaController.BUTTON_U:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 2, true);
				handled = true;
				break;
			case OuyaController.BUTTON_Y:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 3, true);
				handled = true;
				break;
			case OuyaController.BUTTON_A:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 1, true);
				handled = true;
				break;
			case OuyaController.BUTTON_MENU:
				if (OuyaFacade.getInstance().isRunningOnOUYAHardware()) {
					Moai.AKUEnqueueKeyboardEvent(1, 3, 6, true);
					mButtonHandler.removeCallbacks(mMenuButtonDown);
					mButtonHandler.postDelayed ( mMenuButtonDown , 100 );
					handled = true;
				}
				break;
			case OuyaController.BUTTON_L3:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 7, true);
				handled = true;
				break;
			case OuyaController.BUTTON_R3:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 8, true);
				handled = true;
				break;
			case OuyaController.BUTTON_L1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 9, true);
				handled = true;
				break;
			case OuyaController.BUTTON_R1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 10, true);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_UP:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 11, true);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_DOWN:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 12, true);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_LEFT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 13, true);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_RIGHT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 14, true);
				handled = true;
				break;
			// END OUYA CODE
			case KeyEvent.KEYCODE_BACK:
				handled = Moai.backButtonPressed();
				break;
		}

		return handled || super.onKeyDown(keyCode, event);
	}

	public boolean onKeyUp ( int keyCode, KeyEvent event ) {
		boolean handled = false;

		switch(keyCode){
			// OUYA CODE
			case OuyaController.BUTTON_O:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 0, false);
				handled = true;
				break;
			case OuyaController.BUTTON_U:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 2, false);
				handled = true;
				break;
			case OuyaController.BUTTON_Y:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 3, false);
				handled = true;
				break;
			case OuyaController.BUTTON_A:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 1, false);
				handled = true;
				break;
			case OuyaController.BUTTON_L3:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 7, false);
				handled = true;
				break;
			case OuyaController.BUTTON_R3:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 8, false);
				handled = true;
				break;
			case OuyaController.BUTTON_L1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 9, false);
				handled = true;
				break;
			case OuyaController.BUTTON_R1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 10, false);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_UP:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 11, false);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_DOWN:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 12, false);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_LEFT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 13, false);
				handled = true;
				break;
			case OuyaController.BUTTON_DPAD_RIGHT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 14, false);
				handled = true;
				break;
			// END OUYA CODE
			case KeyEvent.KEYCODE_BACK:
				handled = Moai.backButtonPressed();
				break;
		}

		return handled || super.onKeyDown(keyCode, event);
	}


	// OUYA CODE
	public boolean onGenericMotionEvent(MotionEvent event) {
		//Get the player #
		int odid = event.getDeviceId();
		boolean handled = OuyaController.onGenericMotionEvent(event);

		if (handled) {

			OuyaController c = OuyaController.getControllerByDeviceId(odid);

			//Get all the axis for the event
			float temp = c.getAxisValue(OuyaController.AXIS_LS_X);
			boolean update = false;

			if (temp != mLS[0]) {
				mLS[0] = temp;
				update = true;
			}

			temp = c.getAxisValue(OuyaController.AXIS_LS_Y);

			if (temp != mLS[1]) {
				mLS[1] = temp;
				update = true;
			}

			if (update) {
				Moai.AKUEnqueueJoystickEvent(1, 0, mLS[0], mLS[1]);
			}

			temp = c.getAxisValue(OuyaController.AXIS_RS_X);
			update = false;

			if (temp != mRS[0]) {
				mRS[0] = temp;
				update = true;
			}

			temp = c.getAxisValue(OuyaController.AXIS_RS_Y);

			if (temp != mRS[1]) {
				mRS[1] = temp;
				update = true;
			}

			if (update) {
				Moai.AKUEnqueueJoystickEvent(1, 1, mRS[0], mRS[1]);
			}

			temp = c.getAxisValue(OuyaController.AXIS_L2);
			update = false;

			if (temp != mTrigg[0]) {
				mTrigg[0] = temp;
				update = true;
			}

			temp = c.getAxisValue(OuyaController.AXIS_R2);

			if (temp != mTrigg[1]) {
				mTrigg[1] = temp;
				update = true;
			}

			if (update) {
				Moai.AKUEnqueueJoystickEvent(1, 2, mTrigg[0], mTrigg[1]);
			}
		}
		return handled;
	}

	// END OUYA CODE

	// MOGA LISTENER CODE
	public void onKeyEvent(com.bda.controller.KeyEvent event) {
		boolean down = event.getAction() == com.bda.controller.KeyEvent.ACTION_DOWN;
		switch(event.getKeyCode()) {
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_A:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 0, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_B:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 1, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_Y:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 2, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_X:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 3, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_SELECT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 4, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_START:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 6, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_THUMBL:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 7, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_THUMBR:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 8, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_L1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 9, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_BUTTON_R1:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 10, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_DPAD_UP:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 11, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_DPAD_DOWN:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 12, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_DPAD_LEFT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 13, down);
				break;
			case com.bda.controller.KeyEvent.KEYCODE_DPAD_RIGHT:
				Moai.AKUEnqueueKeyboardEvent(1, 3, 14, down);
				break;
		}
	}

	public void onMotionEvent(com.bda.controller.MotionEvent event) {
		//Get all the axis for the event
		float temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_X);
		boolean update = false;

		if (temp != mLS[0]) {
			mLS[0] = temp;
			update = true;
		}

		temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_Y);

		if (temp != mLS[1]) {
			mLS[1] = temp;
			update = true;
		}

		if (update) {
			Moai.AKUEnqueueJoystickEvent(1, 0, mLS[0], mLS[1]);
		}

		temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_Z);
		update = false;

		if (temp != mRS[0]) {
			mRS[0] = temp;
			update = true;
		}

		temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_RZ);

		if (temp != mRS[1]) {
			mRS[1] = temp;
			update = true;
		}

		if (update) {
			Moai.AKUEnqueueJoystickEvent(1, 1, mRS[0], mRS[1]);
		}

		temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_LTRIGGER);
		update = false;

		if (temp != mTrigg[0]) {
			mTrigg[0] = temp;
			update = true;
		}

		temp = event.getAxisValue(com.bda.controller.MotionEvent.AXIS_RTRIGGER);

		if (temp != mTrigg[1]) {
			mTrigg[1] = temp;
			update = true;
		}

		if (update) {
			Moai.AKUEnqueueJoystickEvent(1, 2, mTrigg[0], mTrigg[1]);
		}
	}

	public void onStateEvent(com.bda.controller.StateEvent event) {
		switch(event.getState()) {
			case com.bda.controller.StateEvent.STATE_CONNECTION:
				switch(event.getAction()) {
					case com.bda.controller.StateEvent.ACTION_DISCONNECTED:
						MoaiLog.i("MOGA DISCONNECTED");
						Moai.AKUSetInputDeviceActive(1, false);
						break;
					case com.bda.controller.StateEvent.ACTION_CONNECTED:
						MoaiLog.i("MOGA CONNECTED");
						Moai.AKUSetInputDeviceActive(1, true);
						break;
					case com.bda.controller.StateEvent.ACTION_CONNECTING:
						MoaiLog.i("MOGA CONNECTING");
						// TODO: add connecting information
						break;
			}
			break;
			case com.bda.controller.StateEvent.STATE_POWER_LOW:
				if(event.getAction() == com.bda.controller.StateEvent.ACTION_TRUE) {
					MoaiLog.i("MOGA POWER LOW");
				} else {
					MoaiLog.i("MOGA POWER NORMAL");
				}
			break;
		}
	}
	// END MOGA LISTENER CODE

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
