//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.AndroidMarketBillingService.RequestPurchase;
import @PACKAGE@.AndroidMarketBillingService.RestoreTransactions;
import @PACKAGE@.AndroidMarketBillingConstants.PurchaseState;
import @PACKAGE@.AndroidMarketBillingConstants.ResponseCode;
import @PACKAGE@.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
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
import android.view.Display;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

// Tapjoy
import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyVideoNotifier;

// Crittercism
import com.crittercism.app.Crittercism;

// AdColony
import com.jirbo.adcolony.AdColony;
import com.jirbo.adcolony.AdColonyVideoAd;

// OpenGL 2.0
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;

//Facebook
import com.facebook.android.*;
import com.facebook.android.Facebook.*;

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
	private AndroidMarketBillingService		mBillingService;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver;
	private Handler							mHandler;
	private MoaiView						mMoaiView;
    private PurchaseObserver 				mPurchaseObserver;
	private SensorManager 					mSensorManager;
	private boolean							mWaitingToResume;
	private boolean							mWindowFocusLost;
	private Facebook 						mFacebook;
	
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
	protected static native void		AKUNotifyFacebookLogin				( int statusCode );
	protected static native void 		AKUSetConnectionType 				( long connectionType ); // M	
	protected static native void 		AKUSetDocumentDirectory 			( String path ); // M
	protected static native void 		AKUSetWorkingDirectory 				( String path ); // M

   	//----------------------------------------------------------------//
    protected void onCreate ( Bundle savedInstanceState ) {

		MoaiLog.i ( "MoaiActivity onCreate: activity CREATED" );

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

			MoaiLog.e ( "MoaiActivity onCreate: Unable to locate the application bundle" );
		}

		if ( getFilesDir () != null ) {
		 
		 	AKUSetDocumentDirectory ( getFilesDir ().getAbsolutePath ());
		} else {

			MoaiLog.e ( "MoaiActivity onCreate: Unable to set the document directory" );
		}
				
		mHandler = new Handler ();
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );

		mWaitingToResume = false;
		mWindowFocusLost = false;

		mPurchaseObserver = new PurchaseObserver ( null );
        AndroidMarketBillingResponseHandler.register ( mPurchaseObserver );
        
		mBillingService = new AndroidMarketBillingService();
        mBillingService.setContext (this);

 		mFacebook = new Facebook("YOUR_APP_ID");

		startConnectivityReceiver ();

		setContentView ( mMoaiView );
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		MoaiLog.i ( "MoaiActivity onDestroy: activity DESTROYED" );
		
		super.onDestroy ();
		
		stopConnectivityReceiver ();
		
		mBillingService.unbind ();
		
		AKUFinalize();
	}

	//----------------------------------------------------------------//
	protected void onPause () {

		MoaiLog.i ( "MoaiActivity onPause: activity PAUSED" );
		
		super.onPause ();
		
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
		
		// Sessions are started in MoaiView.
		AKUAppWillEndSession ();
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		MoaiLog.i ( "MoaiActivity onResume: activity RESUMED" );

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
			
			MoaiLog.i ( "MoaiActivity onResume: RESUMING now" );
			mMoaiView.pause ( false );
		}
	}

	//----------------------------------------------------------------//
	protected void onStart () {

		MoaiLog.i ( "MoaiActivity onStart: activity STARTED" );

		super.onStart ();
		
        AndroidMarketBillingResponseHandler.register ( mPurchaseObserver );
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		MoaiLog.i ( "MoaiActivity onStop: activity STOPPED" );

		super.onStop ();

        AndroidMarketBillingResponseHandler.unregister ( mPurchaseObserver );
	}
	
	@Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        mFacebook.authorizeCallback(requestCode, resultCode, data);
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
	// Facebook JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	
	public void login ( String [] permissions ) {
		mFacebook.authorize(this, permissions, new DialogListener() {
	        @Override
	        public void onComplete(Bundle values) {
				AKUNotifyFacebookLogin ( 1 );
	        }

	        @Override
	        public void onFacebookError(FacebookError error) {}

	        @Override
	        public void onError(DialogError e) {
				AKUNotifyFacebookLogin ( 0 );
			}

	        @Override
	        public void onCancel() {
				AKUNotifyFacebookLogin ( 0 );
			}
	     });
	}
	
	//================================================================//
	// AdColony JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void initAdColony ( String appId, String [] zoneIds ) {

		String appVersion;
		try {
			
			appVersion = getApplicationContext ().getPackageManager ().getPackageInfo ( appId, 0 ).versionName;
		}
		catch ( Exception e ) {
			
			appVersion = "UNKNOWN";
		}

		ArrayList < String > parameters = new ArrayList < String > ( Arrays.asList ( zoneIds ));
		parameters.add ( 0, appId );
		
		AdColony.configure ( this, appVersion, parameters.toArray ( new String [ parameters.size ()]));
	}

	//----------------------------------------------------------------//
	public boolean isAdColonyVideoReady ( String zoneId ) {

		return new AdColonyVideoAd ( zoneId ).isReady ();
	}
	
	//----------------------------------------------------------------//
	public void playAdColonyVideo ( String zoneId, boolean showPrompt, boolean showConfirmation ) {
		
		AdColonyVideoAd ad = new AdColonyVideoAd ( zoneId );
		if ( showPrompt ) {
			
			ad.offerV4VC ( null, showConfirmation );
		} else {
			
			ad.showV4VC ( null, showConfirmation );
		}
		
		// TODO: Add listener to allow callbacks into Lua
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
	
		AndroidMarketBillingSecurity.setPublicKey ( key );
	}
	
	//================================================================//
	// Google C2DM JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void registerForRemoteNotifications ( String alias ) {
	
		Intent intent = new Intent ( "com.google.android.c2dm.intent.REGISTER" );
		intent.putExtra( "app", PendingIntent.getBroadcast ( getApplicationContext (), 0, new Intent (), 0 ));
		intent.putExtra( "sender", alias );
		getApplicationContext ().startService ( intent );
	}
	
	public void unregisterForRemoteNotifications ( ) {
	
		Intent intent = new Intent ( "com.google.android.c2dm.intent.UNREGISTER" );
		intent.putExtra( "app", PendingIntent.getBroadcast ( getApplicationContext (), 0, new Intent (), 0 ));
		getApplicationContext ().startService ( intent );
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

	private class PurchaseObserver extends AndroidMarketBillingPurchaseObserver {
	    
		//----------------------------------------------------------------//
		public PurchaseObserver ( Handler handler ) {
			
	        super ( MoaiActivity.this, handler );
	    }

		//----------------------------------------------------------------//
		@Override
	    public void onBillingSupported ( boolean supported ) {
	
			AKUNotifyBillingSupported ( supported );
	    }

		//----------------------------------------------------------------//
	    @Override
	    public void onPurchaseStateChange ( PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload ) {
	        
			AKUNotifyPurchaseStateChanged ( itemId, purchaseState.ordinal(), orderId, notificationId, developerPayload );
	    }

		//----------------------------------------------------------------//
		@Override
	    public void onRequestPurchaseResponse ( RequestPurchase request, ResponseCode responseCode ) {
	
			AKUNotifyPurchaseResponseReceived ( request.mProductId, responseCode.ordinal() );
	    }

		//----------------------------------------------------------------//
	    @Override
	    public void onRestoreTransactionsResponse ( RestoreTransactions request, ResponseCode responseCode ) {

			AKUNotifyRestoreResponseReceived ( responseCode.ordinal() );
		}
	}
}