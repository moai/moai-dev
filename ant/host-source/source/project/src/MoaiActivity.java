//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.MoaiBillingService.RequestPurchase;
import @PACKAGE@.MoaiBillingService.RestoreTransactions;
import @PACKAGE@.MoaiBillingConstants.PurchaseState;
import @PACKAGE@.MoaiBillingConstants.ResponseCode;

import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

import @PACKAGE@.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

// Tapjoy
import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyVideoNotifier;

// Crittercism
import com.crittercism.app.Crittercism;

// OpenGL 2.0
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;

enum DIALOG_RESULT {
	POSITIVE,
	NEUTRAL,
	NEGATIVE,
	CANCEL,
	TOTAL,
};

enum CONNECTION_TYPE {
	NONE,
	WIFI,
	WWAN,
	TOTAL,
};

enum INPUT_DEVICE {
	DEVICE,
	TOTAL,
};

enum INPUT_SENSOR {
	COMPASS,
	LEVEL,
	LOCATION,
	TOUCH,
	TOTAL,
};

// README: See README in MoaiView.java regarding thread safety in Java and Aku.

//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements TapjoyVideoNotifier {

	private AccelerometerEventListener		mAccelerometerListener;
	private Sensor 							mAccelerometerSensor;
	private MoaiBillingService 				mBillingService;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver;
	private Handler							mHandler;
	private MoaiView						mMoaiView;
    private PurchaseObserver 				mPurchaseObserver;
	private SensorManager 					mSensorManager;
	private boolean							mWaitingToResume;
	private boolean							mWindowFocusLost;
	
	// Threading indications; M = Runs on Main thread, R = Runs on GL thread,
	// ? = Runs on arbitrary thread.
	// NOTE that this is based on the current Android host setup; if you move
	// any one of these calls elesewhere in the host, you may alter what thread
	// it executes on. 
	protected static native void 		AKUAppWillEndSession 				(); // M
	protected static native void 		AKUEnqueueLevelEvent 				( int deviceId, int sensorId, float x, float y, float z ); // M
	protected static native void 		AKUFinalize 						(); // M
	protected static native void 		AKUMountVirtualDirectory 			( String virtualPath, String archive ); // M
	protected static native boolean 	AKUNotifyBackButtonPressed			(); // M
	protected static native void 		AKUNotifyBillingSupported			( boolean supported ); // M
	protected static native void 		AKUNotifyDialogDismissed			( int dialogResult ); // M
	protected static native void 		AKUNotifyPurchaseResponseReceived	( String productId, int responseCode ); // M
	protected static native void 		AKUNotifyPurchaseStateChanged		( String productId, int purchaseState, String orderId, String notificationId, String developerPayload ); // M
	protected static native void 		AKUNotifyRestoreResponseReceived	( int responseCode ); // M
	protected static native void		AKUNotifyVideoAdReady				(); // ? TBD
	protected static native void		AKUNotifyVideoAdError				( int statusCode ); // ? TBD
	protected static native void		AKUNotifyVideoAdClose				(); // ? TBD
	protected static native void 		AKUSetConnectionType 				( long connectionType ); // M	
	protected static native void 		AKUSetDocumentDirectory 			( String path ); // M
	protected static native void 		AKUSetWorkingDirectory 				( String path ); // M

   	//----------------------------------------------------------------//
    protected void onCreate ( Bundle savedInstanceState ) {

		log ( "MoaiActivity onCreate called" );

    	super.onCreate ( savedInstanceState );

       	System.load ( "/data/data/@PACKAGE@/lib/libmoai.so" ); 

        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_FULLSCREEN ); 
	    getWindow ().addFlags ( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay ();
		ConfigurationInfo info = (( ActivityManager ) getSystemService ( Context.ACTIVITY_SERVICE )).getDeviceConfigurationInfo ();

	    mMoaiView = new MoaiView ( this, this, display.getWidth (), display.getHeight (), info.reqGlEsVersion );

		// TODO: Reorder AKU initialization so that the virtual directory can be
		// mounted BEFORE creating the MoaiView.
		try {
			
			ApplicationInfo myApp = getPackageManager ().getApplicationInfo ( getPackageName (), 0 );

			AKUMountVirtualDirectory ( "bundle", myApp.publicSourceDir );
			AKUSetWorkingDirectory ( "bundle/assets/@WORKING_DIR@" );				
		} catch ( NameNotFoundException e ) {

			log ( "Unable to locate the application bundle" );
		}

		AKUSetDocumentDirectory ( getFilesDir ().getAbsolutePath ());
				
		mHandler = new Handler ();
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );

		mWaitingToResume = false;
		mWindowFocusLost = false;

		mPurchaseObserver = new PurchaseObserver ( null );
        MoaiBillingResponseHandler.register ( mPurchaseObserver );
        
		mBillingService = new MoaiBillingService();
        mBillingService.setContext (this);

		startConnectivityReceiver ();

		setContentView ( mMoaiView );
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		log ( "MoaiActivity onDestroy called" );
		
		super.onDestroy ();
		
		stopConnectivityReceiver ();
		
		mBillingService.unbind ();
		
		AKUFinalize();
	}

	//----------------------------------------------------------------//
	protected void onPause () {

		log ( "MoaiActivity onPause called" );
		
		super.onPause ();
		
		if ( mAccelerometerListener != null ) {
			
			mSensorManager.unregisterListener ( mAccelerometerListener );
		}
		
		// If we've been paused, then we're assuming we've lost focus. 
		// This handles the case where the user presses the lock button
		// very quickly twice, in which case we do not receive the 
		// expected windows focus events.
		mWindowFocusLost = true;

		log ( "Pausing now" );
		mMoaiView.pause ( true );
		
		// Sessions are started in MoaiView.
		AKUAppWillEndSession ();
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		log ( "MoaiActivity onResume called" );

		super.onResume ();
		
		if ( mAccelerometerListener != null ) {
			
			mSensorManager.registerListener ( mAccelerometerListener, mAccelerometerSensor, SensorManager.SENSOR_DELAY_NORMAL );
		}
		
		// If we have not lost Window focus, then resume immediately; 
		// otherwise, wait to regain focus before we resume. All of 
		// this nonsense is to prevent audio from playing while the
		// screen is locked.
		mWaitingToResume = mWindowFocusLost;
		if ( !mWindowFocusLost ) {
			
			log ( "Resuming now" );
			mMoaiView.pause ( false );
		}
	}

	//----------------------------------------------------------------//
	protected void onStart () {

		log ( "MoaiActivity onStart called" );

		super.onStart ();
		
        MoaiBillingResponseHandler.register ( mPurchaseObserver );
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		log ( "MoaiActivity onStop called" );

		super.onStop ();

        MoaiBillingResponseHandler.unregister ( mPurchaseObserver );
	}
	
	//================================================================//
	// Public methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void enableAccelerometerEvents ( boolean enabled ) {
		
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
	public static void log ( String message ) {
		
		Log.i ( "MoaiLog", message );
	}
	
	//================================================================//
	// Private methods
	//================================================================//

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
	        
			if ( AKUNotifyBackButtonPressed () ) {
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
		
		log ( "MoaiActivity onWindowFocusChanged called" );
		
		super.onWindowFocusChanged ( hasFocus );
				
		// If we are waiting to resume and just got the window focus back, 
		// it's time to resume. All of this nonsense is to prevent audio 
		// from playing while the screen is locked.
		mWindowFocusLost = !hasFocus;
		if ( mWaitingToResume && hasFocus ) {
		
			mWaitingToResume = false;

			log ( "Resuming now" );
			mMoaiView.pause ( false );
		}
	}

	//================================================================//
	// Misc JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void showDialog ( String title, String message, String positiveButton, String neutralButton, String negativeButton, boolean cancelable ) {

		AlertDialog.Builder builder = new AlertDialog.Builder ( MoaiActivity.this );

		if ( title != null ) {
			builder.setTitle ( title );
		}

		if ( message != null ) {
			builder.setMessage ( message );
		}

		if ( positiveButton != null ) {
			builder.setPositiveButton ( positiveButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) 
				{
					AKUNotifyDialogDismissed ( DIALOG_RESULT.POSITIVE.ordinal () );
					}
				});
		}

		if ( neutralButton != null ) {
			builder.setNeutralButton ( neutralButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) 
				{
					AKUNotifyDialogDismissed ( DIALOG_RESULT.NEUTRAL.ordinal () );
				}
			});
		}

		if ( negativeButton != null ) {
			builder.setNegativeButton ( negativeButton, new DialogInterface.OnClickListener () {
				public void onClick ( DialogInterface arg0, int arg1 ) 
				{
					AKUNotifyDialogDismissed ( DIALOG_RESULT.NEGATIVE.ordinal () );
				}
			});
		}

		builder.setCancelable ( cancelable );
		if ( cancelable ) {
			builder.setOnCancelListener ( new DialogInterface.OnCancelListener () {
				public void onCancel ( DialogInterface arg0 ) 
				{
					AKUNotifyDialogDismissed ( DIALOG_RESULT.CANCEL.ordinal () );
				}
			});
		}

		builder.create().show();			
	}
	
	//----------------------------------------------------------------//
	public void share ( String prompt, String subject, String text ) {

		Intent intent = new Intent ( Intent.ACTION_SEND );
		intent.setType ( "text/plain" );
		
		if ( subject != null ) intent.putExtra ( Intent.EXTRA_SUBJECT, subject );
		if ( text != null ) intent.putExtra ( Intent.EXTRA_TEXT, text );
	
		MoaiActivity.this.startActivity ( Intent.createChooser ( intent, prompt ));
	}

	//================================================================//
	// Open Url JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void openURL ( String url ) {

		Uri uri = Uri.parse ( url );
		Intent intent = new Intent ( Intent.ACTION_VIEW, uri );
		MoaiActivity.this.startActivity ( intent );
	}
	
	//================================================================//
	// Crittercism JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void initCrittercism ( String appId ) {

		Crittercism.init ( getApplicationContext(), appId );
	}
	
	//================================================================//
	// Tapjoy JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public String getUserId () {

		return TapjoyConnect.getTapjoyConnectInstance ().getUserID ();
	}
	 
	//----------------------------------------------------------------//
	public void initVideoAds () {

		TapjoyConnect.getTapjoyConnectInstance ().initVideoAd ( MoaiActivity.this );
	}

	//----------------------------------------------------------------//
	public void requestTapjoyConnect ( String appId, String appSecret ) {

		TapjoyConnect.requestTapjoyConnect ( MoaiActivity.this, appId, appSecret );
	}
		
	//----------------------------------------------------------------//
	public void setVideoAdCacheCount ( int count ) {

		TapjoyConnect.getTapjoyConnectInstance ().setVideoCacheCount ( count );
	}
	
	//----------------------------------------------------------------//
	public void showOffers () {

		TapjoyConnect.getTapjoyConnectInstance ().showOffers ();
	}
		
	//================================================================//
	// In-App Billing JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public boolean checkBillingSupported () {
		
		return mBillingService.checkBillingSupported ();				
	}

	//----------------------------------------------------------------//
	public boolean confirmNotification ( String notificationId ) {

		ArrayList<String> notifyList = new ArrayList<String> ();
	   	notifyList.add ( notificationId );

		String[] notifyIds = notifyList.toArray ( new String[notifyList.size ()] );
					
		return mBillingService.confirmNotifications ( notifyIds );
	}
		
	//----------------------------------------------------------------//
	public boolean requestPurchase ( String productId, String developerPayload ) {

		return mBillingService.requestPurchase ( productId, developerPayload );
	}
	
	//----------------------------------------------------------------//
	public boolean restoreTransactions () {

		return mBillingService.restoreTransactions ();
	}
	
	//----------------------------------------------------------------//
	public void setMarketPublicKey ( String key ) {
	
		MoaiBillingSecurity.setPublicKey ( key );
	}
	
	//================================================================//
	// TapjoyVideoNotifier methods
	//================================================================//	

	//----------------------------------------------------------------//
	public void videoComplete () {

		AKUNotifyVideoAdClose ();
	}

	//----------------------------------------------------------------//
	public void videoError ( int statusCode ) {

		AKUNotifyVideoAdError ( statusCode );
	}

	//----------------------------------------------------------------//
	public void videoReady () {

		AKUNotifyVideoAdReady ();
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
					
			CONNECTION_TYPE connectionType = CONNECTION_TYPE.NONE;
					
			if ( networkInfo != null ) {
				
				 switch ( networkInfo.getType () ) {
					 								
				 	case ConnectivityManager.TYPE_MOBILE: {
					
				 		connectionType = CONNECTION_TYPE.WWAN;
				 		break;
				 	}
					 									
				 	case ConnectivityManager.TYPE_WIFI: {
					
				 		connectionType = CONNECTION_TYPE.WIFI;
				 		break;
				 	}
				 }
			}
							 
			AKUSetConnectionType (( long )connectionType.ordinal ());
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

			int deviceId = INPUT_DEVICE.DEVICE.ordinal ();
			int sensorId = INPUT_SENSOR.LEVEL.ordinal ();

			AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
		}
	};

	//================================================================//
	// PurchaseObserver
	//================================================================//

	private class PurchaseObserver extends MoaiBillingPurchaseObserver {
	    
		private static final String TAG = "MoaiBillingPurchaseObserver";

		//----------------------------------------------------------------//
		public PurchaseObserver ( Handler handler ) {
			
	        super ( MoaiActivity.this, handler );
	    }

		//----------------------------------------------------------------//
		@Override
	    public void onBillingSupported ( boolean supported ) {
	        
			if ( MoaiBillingConstants.DEBUG ) {
				
	            Log.i ( TAG, "onBillingSupported: " + supported );
	        }
	
			AKUNotifyBillingSupported ( supported );
	    }

		//----------------------------------------------------------------//
	    @Override
	    public void onPurchaseStateChange ( PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload ) {
        	
			if ( MoaiBillingConstants.DEBUG ) {
				
	            Log.i ( TAG, "onPurchaseStateChange: " + itemId + ", " + purchaseState );
	        }
	        
			AKUNotifyPurchaseStateChanged ( itemId, purchaseState.ordinal(), orderId, notificationId, developerPayload );
	    }

		//----------------------------------------------------------------//
		@Override
	    public void onRequestPurchaseResponse ( RequestPurchase request, ResponseCode responseCode ) {
	        
			if ( MoaiBillingConstants.DEBUG ) {
				
	            Log.d ( TAG, "onRequestPurchaseResponse: " + request.mProductId + ", " + responseCode );
	        }
	
			AKUNotifyPurchaseResponseReceived ( request.mProductId, responseCode.ordinal() );
	    }

		//----------------------------------------------------------------//
	    @Override
	    public void onRestoreTransactionsResponse ( RestoreTransactions request, ResponseCode responseCode ) {

	        if ( MoaiBillingConstants.DEBUG ) {
		
	            Log.d ( TAG, "onRestoreTransactionsResponse: " + responseCode );
	        }

			AKUNotifyRestoreResponseReceived ( responseCode.ordinal() );
		}
	}
}