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

// OpenGL 2.0
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;
 
//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements SensorEventListener, TapjoyVideoNotifier {

	private Sensor 							mAccelerometer;
	private MoaiBillingService 				mBillingService;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver;
	private Handler							mHandler;
	private MoaiView						mMoaiView;
    private PurchaseObserver 				mPurchaseObserver;
	private SensorManager 					mSensorManager;
	private boolean							mWaitingToResume;
	private boolean							mWindowFocusLost;
	
	static enum DIALOG_RESULT {
		POSITIVE,
		NEUTRAL,
		NEGATIVE,
		CANCEL,
	}

	protected static native void 		AKUAppDidStartSession 				();
	protected static native void 		AKUAppWillEndSession 				();
	protected static native void 		AKUEnqueueCompassEvent 				( int heading );
	protected static native void 		AKUEnqueueLevelEvent 				( int deviceId, int sensorId, float x, float y, float z );
	protected static native void 		AKUEnqueueLocationEvent 			( int deviceId, int sensorId, int longitude, int latitude, int altitude, float hAccuracy, float vAccuracy, float speed );
	protected static native void 		AKUFinalize 						();
	protected static native void 		AKUMountVirtualDirectory 			( String virtualPath, String archive );
	protected static native boolean 	AKUNotifyBackButtonPressed			();
	protected static native void 		AKUNotifyBillingSupported			( boolean supported );
	protected static native void 		AKUNotifyDialogDismissed			( int dialogResult );
	protected static native void 		AKUNotifyPurchaseResponseReceived	( String productId, int responseCode );
	protected static native void 		AKUNotifyPurchaseStateChanged		( String productId, int purchaseState, String orderId, String notificationId, String developerPayload );
	protected static native void 		AKUNotifyRestoreResponseReceived	( int responseCode );
	protected static native void		AKUNotifyVideoAdReady				();
	protected static native void		AKUNotifyVideoAdError				( int statusCode );
	protected static native void		AKUNotifyVideoAdClose				();
	protected static native void 		AKUSetConnectionType 				( long connectionType );
	protected static native void 		AKUSetDocumentDirectory 			( String path );

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
	    mMoaiView = new MoaiView ( this, this, displayWidth, displayHeight );

		// OpenGL 2.0
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

		// set documents directory
		String filesDir = getFilesDir ().getAbsolutePath ();
		AKUSetDocumentDirectory ( filesDir );

		PackageManager  pm = getPackageManager();
		try {
			ApplicationInfo myApp = pm.getApplicationInfo(getPackageName(), 0);

			AKUMountVirtualDirectory ( "bundle", myApp.publicSourceDir );

			mMoaiView.setDirectory ( "bundle/assets" );
		} catch (NameNotFoundException e) {
			// This should obviously never, ever happen.
			log ( "Unable to locate the application bundle" );
		}
				
		mHandler = new Handler ();

		mWaitingToResume = false;
		mWindowFocusLost = false;

		mPurchaseObserver = new PurchaseObserver ( null );
        MoaiBillingResponseHandler.register ( mPurchaseObserver );
        
		mBillingService = new MoaiBillingService();
        mBillingService.setContext(this);

		startConnectivityReceiver ();
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		log ( "MoaiActivity onDestroy called" );
		
		super.onDestroy ();
		
		stopConnectivityReceiver ();
		
		mBillingService.unbind();
		
		AKUFinalize();
	}

	//----------------------------------------------------------------//
	protected void onPause () {

		log ( "MoaiActivity onPause called" );
		
		super.onPause ();
		
		mSensorManager.unregisterListener ( this );
		
		// If we've been paused, then we're assuming we've lost focus. 
		// This handles the case where the user presses the lock button
		// very quickly twice, in which case we do not receive the 
		// expected windows focus events.
		mWindowFocusLost = true;
		
		mMoaiView.pause ( true );
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

		AKUAppWillEndSession ();
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		log ( "MoaiActivity onResume called" );

		super.onResume ();
		
		mSensorManager.registerListener ( this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL );
		
		// If we have not lost Window focus, then resume immediately; 
		// otherwise, wait to regain focus before we resume. All of 
		// this nonsense is to properly handle the lock screen...
		mWaitingToResume = mWindowFocusLost;
		if ( !mWaitingToResume ) {
			
			log ( "Resuming now..." );
		
			mMoaiView.pause ( false );
		}
	}
	
	//================================================================//
	// Public methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public static void startSession () {

		AKUAppDidStartSession ();
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

	public void onWindowFocusChanged ( boolean hasFocus ) {
		
		log ( "MoaiActivity onWindowFocusChanged called" );
		
		super.onWindowFocusChanged ( hasFocus );
				
		// If we are waiting to resume and just got the window focus back, 
		// it's time to resume. All of this nonsense is to properly handle
		// the lock screen...
		mWindowFocusLost = !hasFocus;
		if ( mWaitingToResume && !mWindowFocusLost ) {
		
			log ( "Resuming now..." );
		
			mWaitingToResume = false;
			mMoaiView.pause ( false );
		}
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

		synchronized ( MoaiView.LOCK_OBJECT ) {
		
			float x = event.values [ 0 ];
			float y = event.values [ 1 ];
			float z = event.values [ 2 ];
			
			int deviceId = MoaiInputDeviceID.DEVICE.ordinal ();
			int sensorId = MoaiInputDeviceSensorID.LEVEL.ordinal ();
			
			AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
		}
	}

	//================================================================//
	// Misc JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void showDialog ( final String title, final String message, final String positiveButton, final String neutralButton, final String negativeButton, final boolean cancelable ) {

		mHandler.post ( new Runnable () {

			public void run () {

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
		});
	}
	
	//----------------------------------------------------------------//
	public void share ( String prompt, String subject, String text ) {

		Intent intent = new Intent ( Intent.ACTION_SEND );
		intent.setType ( "text/plain" );
		
		if ( subject != null ) intent.putExtra ( Intent.EXTRA_SUBJECT, subject );
		if ( text != null ) intent.putExtra ( Intent.EXTRA_TEXT, text );
		
		this.startActivity ( Intent.createChooser ( intent, prompt ));
	}

	//================================================================//
	// Open Url JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void openURL ( String url ) {

		Uri uri = Uri.parse ( url );
		Intent intent = new Intent ( Intent.ACTION_VIEW, uri );
		startActivity ( intent );
	}
	
	//================================================================//
	// Tapjoy JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void requestTapjoyConnect ( String appId, String appSecret ) {
	
		TapjoyConnect.requestTapjoyConnect ( this, appId, appSecret );
	}
	
	public String getUserId () {
		
		return TapjoyConnect.getTapjoyConnectInstance ().getUserID ();
	}
	 
	public void initVideoAds () {
		
		TapjoyConnect.getTapjoyConnectInstance ().initVideoAd ( this );
	}
	
	public void setVideoAdCacheCount ( int count ) {
		
		TapjoyConnect.getTapjoyConnectInstance ().setVideoCacheCount ( count );
	}
	
	public void showOffers () {
		
		TapjoyConnect.getTapjoyConnectInstance ().showOffers ();
	}
	
	//================================================================//
	// Tapjoy video delegate callback methods
	//================================================================//	
	public void videoReady () {

		AKUNotifyVideoAdReady ();
	}

	public void videoError ( int statusCode ) {

		AKUNotifyVideoAdError ( statusCode );
	}

	public void videoComplete () {

		AKUNotifyVideoAdClose ();
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
	// ConnectivityBroadcastReceiver
	//================================================================//
	private class ConnectivityBroadcastReceiver extends BroadcastReceiver {

		//----------------------------------------------------------------//
		@Override
		public void onReceive ( Context context, Intent intent ) {
			
			ConnectivityManager conMgr = ( ConnectivityManager )context.getSystemService ( Context.CONNECTIVITY_SERVICE );
			NetworkInfo networkInfo = conMgr.getActiveNetworkInfo ();
					
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
							 
			AKUSetConnectionType ( ( long )connectionType.ordinal () );
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
