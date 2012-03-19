//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.view.View;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.UUID;

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
		"com.ziplinegames.moai.MoaiCrittercism",
		"com.ziplinegames.moai.MoaiFacebook",
		"com.ziplinegames.moai.MoaiGoogleBilling",
		"com.ziplinegames.moai.MoaiGooglePush",
		"com.ziplinegames.moai.MoaiTapjoy",
	};
	
	private static Activity 			sActivity = null;
	private static ApplicationState 	sApplicationState = ApplicationState.APPLICATION_UNINITIALIZED;
	private static ArrayList < Class > 	sAvailableClasses = new ArrayList < Class > ();

	protected static native boolean	AKUAppBackButtonPressed			();
	protected static native void 	AKUAppDialogDismissed			( int dialogResult );
	protected static native void 	AKUAppDidStartSession			( boolean resumed );
	protected static native void 	AKUAppWillEndSession 			();
	protected static native int	 	AKUCreateContext 				();
	protected static native void 	AKUDetectGfxContext 			();
	protected static native void 	AKUEnqueueLevelEvent 			( int deviceId, int sensorId, float x, float y, float z );
	protected static native void 	AKUEnqueueTouchEvent 			( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount );
	protected static native void 	AKUExtLoadLuacrypto				();
	protected static native void 	AKUExtLoadLuacurl				();
	protected static native void 	AKUExtLoadLuasocket				();
	protected static native void 	AKUExtLoadLuasql				();
	protected static native void 	AKUFinalize 					();
	protected static native void 	AKUInit 						();
	protected static native void 	AKUMountVirtualDirectory 		( String virtualPath, String archive );
	protected static native void 	AKUPause 						( boolean paused );
	protected static native void 	AKURender	 					();
	protected static native void 	AKUReserveInputDevices			( int total );
	protected static native void 	AKUReserveInputDeviceSensors	( int deviceId, int total );
	protected static native void	AKURunScript 					( String filename );
	protected static native void	AKUSetConnectionType 			( long connectionType );
	protected static native void 	AKUSetContext 					( int contextId );
	protected static native void 	AKUSetDeviceProperties 			( String appName, String appId, String appVersion, String abi, String devBrand, String devName, String devManufacturer, String devModel, String devProduct, String osBrand, String osVersion, String udid );
	protected static native void 	AKUSetDocumentDirectory 		( String path );
	protected static native void 	AKUSetInputConfigurationName	( String name );
	protected static native void 	AKUSetInputDevice		 		( int deviceId, String name );
	protected static native void 	AKUSetInputDeviceCompass 		( int deviceId, int sensorId, String name );
	protected static native void 	AKUSetInputDeviceLevel 			( int deviceId, int sensorId, String name );
	protected static native void 	AKUSetInputDeviceLocation 		( int deviceId, int sensorId, String name );
	protected static native void	AKUSetInputDeviceTouch 			( int deviceId, int sensorId, String name );
	protected static native void 	AKUSetScreenSize				( int width, int height );
	protected static native void 	AKUSetViewSize					( int width, int height );
	protected static native void 	AKUSetWorkingDirectory 			( String path );
	protected static native void 	AKUUntzInit			 			();
	protected static native void 	AKUUpdate				 		();

	//----------------------------------------------------------------//
	static {
		
		for ( String className : sExternalClasses )
		{
			Class theClass = findClass ( className );
			if ( theClass != null ) {
				
				sAvailableClasses.add ( theClass );
			}
		}
	}
	
	//----------------------------------------------------------------//
	public static boolean backButtonPressed () {

		return AKUAppBackButtonPressed ();
	}

	//----------------------------------------------------------------//
	public static int createContext () {

		int contextId = AKUCreateContext ();
		AKUSetContext ( contextId );
		
		return contextId;
	}

	//----------------------------------------------------------------//
	public static void detectGraphicsContext () {

		AKUDetectGfxContext ();
	}
	
	//----------------------------------------------------------------//
	public static void dialogDismissed ( int dialogResult ) {

		AKUAppDialogDismissed ( dialogResult );
	}	
	
	//----------------------------------------------------------------//
	public static void endSession () {

		AKUAppWillEndSession ();
	}	
	
	//----------------------------------------------------------------//
	public static void enqueueLevelEvent ( int deviceId, int sensorId, float x, float y, float z ) {
		
		AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
	}

	//----------------------------------------------------------------//
	public static void enqueueTouchEvent ( int deviceId, int sensorId, int touchId, boolean down, int x, int y, int tapCount ) {
		
		AKUEnqueueTouchEvent ( deviceId, sensorId, touchId, down, x, y, tapCount );
	}

	//----------------------------------------------------------------//
	public static void finish () {

		AKUFinalize ();
	}

	//----------------------------------------------------------------//
	public static ApplicationState getApplicationState () {

		return sApplicationState;
	}

	//----------------------------------------------------------------//
	public static void init () {

		AKUSetInputConfigurationName 	( "Android" );

		AKUReserveInputDevices			( Moai.InputDevice.values ().length );
		AKUSetInputDevice				( Moai.InputDevice.INPUT_DEVICE.ordinal (), "device" );
		
		AKUReserveInputDeviceSensors	( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.values ().length );
		AKUSetInputDeviceCompass		( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_COMPASS.ordinal (), "compass" );
		AKUSetInputDeviceLevel			( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_LEVEL.ordinal (), "level" );
		AKUSetInputDeviceLocation		( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_LOCATION.ordinal (), "location" );
		AKUSetInputDeviceTouch			( Moai.InputDevice.INPUT_DEVICE.ordinal (), Moai.InputSensor.SENSOR_TOUCH.ordinal (), "touch" );

		AKUExtLoadLuasql ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuasocket ();

		AKUInit ();
		AKUUntzInit ();
		
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
		
		String udid	= Secure.getString ( sActivity.getContentResolver (), Secure.ANDROID_ID );
		if ( udid == null ) {
			
			udid = "UNKNOWN";
		}
		
		AKUSetDeviceProperties ( appName, appId, appVersion, Build.CPU_ABI, Build.BRAND, Build.DEVICE, Build.MANUFACTURER, Build.MODEL, Build.PRODUCT, "Android", Build.VERSION.RELEASE, udid );
	}	

	//----------------------------------------------------------------//
	public static void mount ( String virtualPath, String archive ) {

		AKUMountVirtualDirectory ( virtualPath, archive );
	}	

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onActivityResult", new Class [] { java.lang.Integer.TYPE, java.lang.Integer.TYPE, Intent.class }, new Object [] { new Integer ( requestCode ), new Integer ( resultCode ), data });
		}	
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		sActivity = activity;

		for ( Class theClass : sAvailableClasses ) {
			
			executeMethod ( theClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
		}
	}
	
	//----------------------------------------------------------------//
	public static void onDestroy () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onDestroy", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onPause () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onPause", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onResume () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onResume", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onStart", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onStop", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void pause ( boolean paused ) {

		AKUPause ( paused );
	}

	//----------------------------------------------------------------//
	public static void render () {

		AKURender ();
	}

	//----------------------------------------------------------------//
	public static void runScript ( String filename ) {

		AKURunScript ( filename );
	}
	
	//----------------------------------------------------------------//
	public static void setApplicationState ( ApplicationState state ) {

		if ( state != sApplicationState ) {
			
			sApplicationState = state;
		
			for ( Class theClass : sAvailableClasses ) {
			
				executeMethod ( theClass, null, "onApplicationStateChanged", new Class [] { ApplicationState.class }, new Object [] { sApplicationState });
			}
		}
	}

	//----------------------------------------------------------------//
	public static void setConnectionType ( long connectionType ) {

		AKUSetConnectionType ( connectionType );
	}	
	
	//----------------------------------------------------------------//
	public static void setDocumentDirectory ( String path ) {

		AKUSetDocumentDirectory ( path );
	}	
	
	//----------------------------------------------------------------//
	public static void setScreenSize ( int width, int height ) {

		AKUSetScreenSize ( width, height );
	}	

	//----------------------------------------------------------------//
	public static void setViewSize ( int width, int height ) {

		AKUSetViewSize ( width, height );
	}	

	//----------------------------------------------------------------//
	public static void setWorkingDirectory ( String path ) {

		AKUSetWorkingDirectory ( path );
	}	
	
	//----------------------------------------------------------------//
	public static void startSession ( boolean resumed ) {

		AKUAppDidStartSession ( resumed );
	}	

	//----------------------------------------------------------------//
	public static void update () {

		AKUUpdate ();
	}	
	
	//================================================================//
	// Private methods
	//================================================================//
	
	//----------------------------------------------------------------//
	private static Class findClass ( String className ) {
		
		Class theClass = null;
		try {

			theClass = Class.forName ( className );
		} catch ( Throwable e ) {

		}
		
		return theClass;
	}
	
	//----------------------------------------------------------------//
	private static Object executeMethod ( Class theClass, Object theInstance, String methodName, Class [] parameterTypes, Object [] parameterValues ) {
		
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
	public static void openURL ( String url ) {

		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, Uri.parse ( url )));
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

		AlertDialog.Builder builder = new AlertDialog.Builder ( sActivity );

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

		builder.create ().show ();			
	}
}