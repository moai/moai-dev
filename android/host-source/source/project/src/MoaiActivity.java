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

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
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

// OpenGL 2.0
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;
 
//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements SensorEventListener {

	private Sensor 							mAccelerometer;
	private MoaiBillingService 				mBillingService;
	private ConnectivityBroadcastReceiver 	mConnectivityReceiver;
	private Handler							mHandler;
	private MoaiView						mMoaiView;
    private PurchaseObserver 				mPurchaseObserver;
	private SensorManager 					mSensorManager;
	
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
	protected static native void 		AKUMountVirtualDirectory 			( String virtualPath, String archive );
	protected static native boolean 	AKUNotifyBackButtonPressed			();
	protected static native void 		AKUNotifyBillingSupported			( boolean supported );
	protected static native void 		AKUNotifyDialogDismissed			( int dialogResult );
	protected static native void 		AKUNotifyPurchaseResponseReceived	( String productId, int responseCode );
	protected static native void 		AKUNotifyPurchaseStateChanged		( String productId, int purchaseState, String orderId, String notificationId, String developerPayload );
	protected static native void 		AKUNotifyRestoreResponseReceived	( int responseCode );
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

		// unpack assets
 		unpackAssets ( filesDir );
 		mMoaiView.setDirectory ( filesDir );

		mHandler = new Handler ();

		mPurchaseObserver = new PurchaseObserver ( null );
        MoaiBillingResponseHandler.register ( mPurchaseObserver );
        
		mBillingService = new MoaiBillingService();
        mBillingService.setContext(this);
    }

	//----------------------------------------------------------------//
	protected void onDestroy () {

		log ( "MoaiActivity onDestroy called" );
		
		// call super
		super.onDestroy ();
		
		// unregister to receive connectivity actions
		stopConnectivityReceiver ();
		
		mBillingService.unbind();
	}

	//----------------------------------------------------------------//
	public boolean onKeyDown ( int keyCode, KeyEvent event ) {

	    if ( keyCode == KeyEvent.KEYCODE_BACK ) {
	        
			if ( AKUNotifyBackButtonPressed () ) {
				return true;
			}
	    }
	    
	    return super.onKeyDown ( keyCode, event );
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
		
        MoaiBillingResponseHandler.register ( mPurchaseObserver );

		//AKUAppDidStartSession ();
	}
	
	public static void startSession () {
		AKUAppDidStartSession ();
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		log ( "MoaiActivity onStop called" );

		// call super
		super.onStop ();

        MoaiBillingResponseHandler.unregister ( mPurchaseObserver );

		AKUAppWillEndSession ();
	}
	
	//----------------------------------------------------------------//
	protected void onResume () {

		log ( "MoaiActivity onResume called" );

		// call super
		super.onResume ();
		
		// register for accelerometer events
		mSensorManager.registerListener ( this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL );
		
		startConnectivityReceiver ();
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
		
    //----------------------------------------------------------------//
	private void unpackAssets ( String rootDir ) {
		
		log ( "MoaiActivity unpackingAssets . . ." );
		
		InputStream is = getResources ().openRawResource ( R.raw.bundle );
		File extractTo = new File ( rootDir + "/" );
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
	public boolean requestPurchase ( String productId )	{
		
		return mBillingService.requestPurchase ( productId, null );
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
