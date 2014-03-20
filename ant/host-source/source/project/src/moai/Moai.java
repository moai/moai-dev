//----------------------------------------------------------------//
// Copyright (c) 2010-2013 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Locale;
import java.util.TimeZone;
import java.util.UUID;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings.Secure;
import android.util.DisplayMetrics;

//================================================================//
// Moai
//================================================================//
public class Moai {

	public enum ApplicationState {
		APPLICATION_UNINITIALIZED,
		APPLICATION_RUNNING,
		APPLICATION_PAUSED;

		public static ApplicationState valueOf ( int index ) {
			ApplicationState [] values = ApplicationState.values ();
			if (( index < 0 ) || ( index >= values.length )) {
				return APPLICATION_UNINITIALIZED;
			}
			return values [ index ];
		}
	}

	public enum DialogResult {
		RESULT_POSITIVE,
		RESULT_NEUTRAL,
		RESULT_NEGATIVE,
		RESULT_CANCEL;

		public static DialogResult valueOf ( int index ) {
			DialogResult [] values = DialogResult.values ();
			if (( index < 0 ) || ( index >= values.length )) {
				return RESULT_CANCEL;
			}
			return values [ index ];
		}
	}

	public enum ConnectionType {
		CONNECTION_NONE,
		CONNECTION_WIFI,
		CONNECTION_WWAN;

		public static ConnectionType valueOf ( int index ) {
			ConnectionType [] values = ConnectionType.values ();
			if (( index < 0 ) || ( index >= values.length )) {
				return CONNECTION_NONE;
			}
			return values [ index ];
		}
	}

	public enum InputDevice {
		INPUT_DEVICE;

		public static InputDevice valueOf ( int index ) {
			InputDevice [] values = InputDevice.values ();
			if (( index < 0 ) || ( index >= values.length )) {
				return INPUT_DEVICE;
			}
			return values [ index ];
		}
	}

	public enum InputSensor {
		SENSOR_COMPASS,
		SENSOR_LEVEL,
		SENSOR_LOCATION,
		SENSOR_TOUCH;

		public static InputSensor valueOf ( int index ) {
			InputSensor [] values = InputSensor.values ();
			if (( index < 0 ) || ( index >= values.length )) {
				return SENSOR_TOUCH;
			}
			return values [ index ];
		}
	}

	private static String [] sExternalClasses = {
		"com.ziplinegames.moai.MoaiAdColony",
		"com.ziplinegames.moai.MoaiAmazonBilling",
		"com.ziplinegames.moai.MoaiChartBoost",
		"com.ziplinegames.moai.MoaiCrittercism",
		"com.ziplinegames.moai.MoaiFacebook",
		"com.ziplinegames.moai.MoaiKeyboard",
		"com.ziplinegames.moai.MoaiGoogleBilling",
		"com.ziplinegames.moai.MoaiGooglePush",
		"com.ziplinegames.moai.MoaiTapjoy",
		"com.ziplinegames.moai.MoaiTwitter",
		"com.ziplinegames.moai.MoaiCamera",
	};

	private static Activity 				sActivity = null;
	private static ApplicationState 		sApplicationState = ApplicationState.APPLICATION_UNINITIALIZED;
	private static ArrayList < Class < ? >>	sAvailableClasses = new ArrayList < Class < ? >> ();

	public static final Object		sAkuLock = new Object ();

 	protected static native void    AKUAppInitialize				();
 	protected static native void    AKUModulesRunLuaAPIWrapper      ();
	protected static native boolean	AKUAppBackButtonPressed			();
	protected static native void    AKUModulesContextInitialize     ();
	protected static native void	AKUAppDialogDismissed			( int dialogResult );
	protected static native void	AKUAppDidStartSession			( boolean resumed );
	protected static native void	AKUAppOpenedFromURL			( String url );
	protected static native void	AKUAppWillEndSession 			();
	protected static native int		AKUCreateContext 				();
	protected static native void	AKUDetectGfxContext 			();
	protected static native void	AKUEnqueueLevelEvent 			( int deviceId, int sensorId, float x, float y, float z );
	protected static native void	AKUEnqueueLocationEvent			( int deviceId, int sensorId, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
	protected static native void	AKUEnqueueCompassEvent			( int deviceId, int sensorId, float heading );
	protected static native void	AKUEnqueueTouchEvent 			( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount );
	protected static native void	AKUFinalize 					();
	protected static native void	AKUInit 						();
	protected static native void	AKUMountVirtualDirectory 		( String virtualPath, String archive );
	protected static native void	AKUPause 						( boolean paused );
	protected static native void	AKURender	 					();
	protected static native void	AKUReserveInputDevices			( int total );
	protected static native void	AKUReserveInputDeviceSensors	( int deviceId, int total );
	protected static native void	AKURunScript 					( String filename );
	protected static native void	AKUSetConnectionType 			( long connectionType );
	protected static native void	AKUSetContext 					( int contextId );
	protected static native void	AKUSetDeviceProperties 			( String appName, String appId, String appVersion, String abi, String devBrand, String devName, String devManufacturer, String devModel, String devProduct, int numProcessors, String osBrand, String osVersion, String udid );
	protected static native void	AKUSetDocumentDirectory 		( String path );
	protected static native void	AKUSetInputConfigurationName	( String name );
	protected static native void	AKUSetInputDevice		 		( int deviceId, String name );
	protected static native void	AKUSetInputDeviceCompass 		( int deviceId, int sensorId, String name );
	protected static native void	AKUSetInputDeviceLevel 			( int deviceId, int sensorId, String name );
	protected static native void	AKUSetInputDeviceLocation 		( int deviceId, int sensorId, String name );
	protected static native void	AKUSetInputDeviceTouch 			( int deviceId, int sensorId, String name );
	protected static native void	AKUSetScreenSize				( int width, int height );
	protected static native void	AKUSetScreenDpi					( int dpi );
	protected static native void	AKUSetViewSize					( int width, int height );
	protected static native void	AKUSetWorkingDirectory 			( String path );
	protected static native void	AKUModulesUpdate				 		();
	protected static native void	AKUSetDeviceLocale				( String langCode, String countryCode );

	//----------------------------------------------------------------//
	static {
		for ( String className : sExternalClasses ) {
			Class < ? > theClass = findClass ( className );
			if ( theClass != null ) {
				sAvailableClasses.add ( theClass );
			}
		}
	}

	//----------------------------------------------------------------//
	public static boolean backButtonPressed () {
		boolean result;
		synchronized ( sAkuLock ) {
			result = AKUAppBackButtonPressed ();
		}
		return result;
	}

	//----------------------------------------------------------------//
	public static int createContext () {
		int contextId;
		synchronized ( sAkuLock ) {
			contextId = AKUCreateContext ();
			AKUSetContext ( contextId );
			AKUModulesContextInitialize ();
			AKUModulesRunLuaAPIWrapper ();
		}
		return contextId;
	}

	//----------------------------------------------------------------//
	public static void detectGraphicsContext () {
		synchronized ( sAkuLock ) {
			AKUDetectGfxContext ();
		}
	}

	//----------------------------------------------------------------//
	public static void dialogDismissed ( int dialogResult ) {
		synchronized ( sAkuLock ) {
			AKUAppDialogDismissed ( dialogResult );
		}
	}

	public static void AppOpenedFromURL ( String url ) {
		synchronized ( sAkuLock ) {
			AKUAppOpenedFromURL ( url );
		}
	}

	//----------------------------------------------------------------//
	public static void endSession () {
		synchronized ( sAkuLock ) {
			AKUAppWillEndSession ();
		}
	}

	//----------------------------------------------------------------//
	public static void enqueueLevelEvent ( int deviceId, int sensorId, float x, float y, float z ) {
		synchronized ( sAkuLock ) {
			AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
		}
	}

	//----------------------------------------------------------------//
	public static void enqueueLocationEvent ( int deviceId, int sensorId, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {
		synchronized ( sAkuLock ) {
			AKUEnqueueLocationEvent ( deviceId, sensorId, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
		}
	}

	//----------------------------------------------------------------//
	public static void enqueueCompassEvent ( int deviceId, int sensorId, float heading ) {
		synchronized ( sAkuLock ) {
			AKUEnqueueCompassEvent ( deviceId, sensorId, heading );
		}
	}

	//----------------------------------------------------------------//
	public static void enqueueTouchEvent ( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount ) {
		synchronized ( sAkuLock ) {
			AKUEnqueueTouchEvent ( deviceId, sensorId, touchId, down, x, y, tapCount );
		}
	}

	//----------------------------------------------------------------//
	public static void finish () {
		synchronized ( sAkuLock ) {
			AKUFinalize ();
		}
	}

	//----------------------------------------------------------------//
	public static ApplicationState getApplicationState () {
		return sApplicationState;
	}

	//----------------------------------------------------------------//
	public static void init () {
		synchronized ( sAkuLock ) {

			AKUSetInputConfigurationName ( "Android" );

			AKUReserveInputDevices ( Moai.InputDevice.values ().length );
			AKUSetInputDevice ( Moai.InputDevice.INPUT_DEVICE.ordinal (), "device" );

			AKUReserveInputDeviceSensors ( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.values ().length );
			AKUSetInputDeviceCompass ( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_COMPASS.ordinal (), "compass" );
			AKUSetInputDeviceLevel ( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_LEVEL.ordinal (), "level" );
			AKUSetInputDeviceLocation ( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_LOCATION.ordinal (), "location" );
			AKUSetInputDeviceTouch ( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_TOUCH.ordinal (), "touch" );

			AKUInit ();
			

		

			String appId = sActivity.getPackageName ();
			String appName;
			try {
			    appName = sActivity.getPackageManager ().getApplicationLabel ( sActivity.getPackageManager ().getApplicationInfo ( appId, 0 )).toString ();
			} catch ( Exception e ) {
				appName = "UNKNOWN";
			}

			String appVersion;
			try {
				appVersion = sActivity.getPackageManager ().getPackageInfo ( appId, 0 ).versionName;
			}
			catch ( Exception e ) {
				appVersion = "UNKNOWN";
			}

			String udid = Secure.getString ( sActivity.getContentResolver (), Secure.ANDROID_ID );
			if ( udid == null ) {
				udid = "UNKNOWN";
			}

			AKUSetDeviceProperties ( appName, appId, appVersion, Build.CPU_ABI, Build.BRAND, Build.DEVICE, Build.MANUFACTURER, Build.MODEL, Build.PRODUCT, Runtime.getRuntime ().availableProcessors (), "Android", Build.VERSION.RELEASE, udid );
			AKUSetDeviceLocale(Locale.getDefault().getLanguage(), Locale.getDefault().getCountry());

		}
	}

	//----------------------------------------------------------------//
	public static void mount ( String virtualPath, String archive ) {
		synchronized ( sAkuLock ) {
			AKUMountVirtualDirectory ( virtualPath, archive );
		}
	}

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onActivityResult", new Class < ? > [] { java.lang.Integer.TYPE, java.lang.Integer.TYPE, Intent.class }, new Object [] { new Integer ( requestCode ), new Integer ( resultCode ), data });
		}
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		sActivity = activity;
		AKUAppInitialize();
		MoaiMoviePlayer.onCreate ( activity );
		MoaiBrowser.onCreate ( activity );
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onCreate", new Class < ? > [] { Activity.class }, new Object [] { activity });
		}
	}

	//----------------------------------------------------------------//
	public static void onDestroy () {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onDestroy", new Class < ? > [] { }, new Object [] { });
		}
	}

	//----------------------------------------------------------------//
	public static void onPause () {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onPause", new Class < ? > [] { }, new Object [] { });
		}
	}

	//----------------------------------------------------------------//
	public static void onResume () {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onResume", new Class < ? > [] { }, new Object [] { });
		}
	}

	//----------------------------------------------------------------//
	public static void onStart () {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onStart", new Class < ? > [] { }, new Object [] { });
		}
	}

	//----------------------------------------------------------------//
	public static void onStop () {
		for ( Class < ? > theClass : sAvailableClasses ) {
			executeMethod ( theClass, null, "onStop", new Class < ? > [] { }, new Object [] { });
		}
	}

	//----------------------------------------------------------------//
	public static void pause ( boolean paused ) {
		MoaiLog.d( "Moai.java::pause( " + paused + " )" );
		synchronized ( sAkuLock ) {
			AKUPause ( paused );
		}
	}

	//----------------------------------------------------------------//
	public static void render () {
		synchronized ( sAkuLock ) {
			AKURender ();
		}
	}

	//----------------------------------------------------------------//
	public static void runScript ( String filename ) {
		synchronized ( sAkuLock ) {
			AKURunScript ( filename );
		}
	}

	//----------------------------------------------------------------//
	public static void setApplicationState ( ApplicationState state ) {
		if ( state != sApplicationState ) {
			sApplicationState = state;
			for ( Class < ? > theClass : sAvailableClasses ) {
				executeMethod ( theClass, null, "onApplicationStateChanged", new Class < ? > [] { ApplicationState.class }, new Object [] { sApplicationState });
			}
		}
	}

	//----------------------------------------------------------------//
	public static void setConnectionType ( long connectionType ) {
		synchronized ( sAkuLock ) {
			AKUSetConnectionType ( connectionType );
		}
	}

	//----------------------------------------------------------------//
	public static void setDocumentDirectory ( String path ) {
		synchronized ( sAkuLock ) {
			AKUSetDocumentDirectory ( path );
		}
	}

	//----------------------------------------------------------------//
	public static void setScreenSize ( int width, int height ) {
		synchronized ( sAkuLock ) {
			AKUSetScreenSize ( width, height );
		}
	}

	//----------------------------------------------------------------//
	public static void setScreenDpi ( int dpi ) {
		synchronized ( sAkuLock ) {
			AKUSetScreenDpi ( dpi );
		}
	}

	//----------------------------------------------------------------//
	public static void setViewSize ( int width, int height ) {
		synchronized ( sAkuLock ) {
			AKUSetViewSize ( width, height );
		}
	}

	//----------------------------------------------------------------//
	public static void setWorkingDirectory ( String path ) {
		synchronized ( sAkuLock ) {
			AKUSetWorkingDirectory ( path );
		}
	}

	//----------------------------------------------------------------//
	public static void startSession ( boolean resumed ) {
		synchronized ( sAkuLock ) {
			AKUAppDidStartSession ( resumed );
		}
	}

	//----------------------------------------------------------------//
	public static void update () {
		synchronized ( sAkuLock ) {
			MoaiKeyboard.update ();
			AKUModulesUpdate ();
		}
	}

	//================================================================//
	// Private methods
	//================================================================//

	//----------------------------------------------------------------//
	private static Class < ? > findClass ( String className ) {
		Class < ? > theClass = null;
		try {
			theClass = Class.forName ( className );
		} catch ( Throwable e ) {
		}
		return theClass;
	}

	//----------------------------------------------------------------//
	private static Object executeMethod ( Class < ? > theClass, Object theInstance, String methodName, Class < ? > [] parameterTypes, Object [] parameterValues ) {
		Object result = null;
		if ( theClass != null ) {
			try {
				Method theMethod = theClass.getMethod ( methodName, parameterTypes );
				result = theMethod.invoke ( theInstance, parameterValues );
			} catch ( Throwable e ) {
			}
		}
		return result;
	}

	//================================================================//
	// Miscellaneous JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static String getGUID () {
		return UUID.randomUUID ().toString ();
	}

	//----------------------------------------------------------------//
	public static int getStatusBarHeight () {
		int myHeight = 0;
		switch ( sActivity.getResources ().getDisplayMetrics ().densityDpi ) {
		case DisplayMetrics.DENSITY_HIGH:
			myHeight = 54;
			break;
		case DisplayMetrics.DENSITY_MEDIUM:
			myHeight = 36;
			break;
		case DisplayMetrics.DENSITY_LOW:
			myHeight = 26;
			break;
		default:
			myHeight = 0;
			break;
		}
		return myHeight;
	}

	//----------------------------------------------------------------//
	public static long getUTCTime () {
		Calendar cal = Calendar.getInstance ( TimeZone.getTimeZone ( "UTC" ));
		long inSeconds = cal.getTimeInMillis () / 1000;
		return inSeconds;
	}

	//----------------------------------------------------------------//
	public static void localNotificationInSeconds ( int seconds, String message, String [] keys, String [] values ) {
		MoaiLog.i ( "Moai localNotificationInSeconds: Adding notification alarm" );

		Calendar cal = Calendar.getInstance (); 	// get a Calendar object with current time
		cal.setTimeInMillis ( System.currentTimeMillis ());
		cal.add ( Calendar.SECOND, seconds );		// add desired time to the calendar object

		Intent intent = new Intent ( sActivity, MoaiLocalNotificationReceiver.class );
		for ( int i = 0; i < keys.length; ++i ) {
			intent.putExtra ( keys [ i ], values [ i ]);
		}

		PendingIntent sender = PendingIntent.getBroadcast ( sActivity, 0, intent, 0 );

		AlarmManager am = ( AlarmManager ) sActivity.getSystemService ( Context.ALARM_SERVICE );
		am.set ( AlarmManager.RTC_WAKEUP, cal.getTimeInMillis (), sender );
	}

	//----------------------------------------------------------------//
	public static void openURL ( String url ) {
		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, Uri.parse ( url )));
	}

    //----------------------------------------------------------------//
	public static void sendMail ( String recipient, String subject, String message ) {

		Intent intent = new Intent ( Intent.ACTION_SEND ).setType ( "message/rfc822" );

		if ( recipient != null ) intent.putExtra ( Intent.EXTRA_EMAIL, new String[] {recipient} );
		if ( subject != null ) intent.putExtra ( Intent.EXTRA_SUBJECT, subject );
		if ( message != null ) intent.putExtra ( Intent.EXTRA_TEXT, message );

		sActivity.startActivity ( Intent.createChooser ( intent, "Send E-mail" ));
	}

	//----------------------------------------------------------------//
	public static void share ( String prompt, String subject, String text ) {
		Intent intent = new Intent ( Intent.ACTION_SEND ).setType ( "text/plain" );

		if ( subject != null ) intent.putExtra ( Intent.EXTRA_SUBJECT, subject );
		if ( text != null ) intent.putExtra ( Intent.EXTRA_TEXT, text );

		sActivity.startActivity ( Intent.createChooser ( intent, prompt ));
	}

	//----------------------------------------------------------------//
	public static void showDialog ( String title, String message, String positiveButton, String neutralButton, String negativeButton, boolean cancelable ) {
		final AlertDialog.Builder builder = new AlertDialog.Builder ( sActivity );

		if ( title != null ) builder.setTitle ( title );
		if ( message != null ) builder.setMessage ( message );

		if ( positiveButton != null ) {
			builder.setPositiveButton ( positiveButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) {
					Moai.dialogDismissed ( Moai.DialogResult.RESULT_POSITIVE.ordinal ());
				}
			});
		}

		if ( neutralButton != null ) {
			builder.setNeutralButton ( neutralButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) {
					Moai.dialogDismissed ( Moai.DialogResult.RESULT_NEUTRAL.ordinal ());
				}
			});
		}

		if ( negativeButton != null ) {
			builder.setNegativeButton ( negativeButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) {
					Moai.dialogDismissed ( Moai.DialogResult.RESULT_NEGATIVE.ordinal ());
				}
			});
		}

		builder.setCancelable ( cancelable );
		if ( cancelable ) {
			builder.setOnCancelListener ( new DialogInterface.OnCancelListener () {
				public void onCancel ( DialogInterface arg0 ) {
					Moai.dialogDismissed ( Moai.DialogResult.RESULT_CANCEL.ordinal ());
				}
			});
		}


		sActivity.runOnUiThread( new Runnable () {
			public void run () {
				builder.create ().show ();
			}
		});
	}	
}
