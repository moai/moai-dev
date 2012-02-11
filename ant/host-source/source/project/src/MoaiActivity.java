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
	
	private boolean							mMarketBillingSupported;
	private boolean							mMarketNotificationsConfirmed;
	private boolean							mMarketPurchaseRequested;
	private boolean							mMarketTransactionsRestored;
	private String							mTapjoyUserId;
	
	protected static native void 		AKUAppDidStartSession 				();
	protected static native void 		AKUAppWillEndSession 				();
	protected static native void 		AKUEnqueueLevelEvent 				( int deviceId, int sensorId, float x, float y, float z );
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
    protected void onCreate ( Bundle savedInstanceState ) {

		log ( "MoaiActivity onCreate called" );

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
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );

		mWaitingToResume = false;
		mWindowFocusLost = false;

		mPurchaseObserver = new PurchaseObserver ( null );
        MoaiBillingResponseHandler.register ( mPurchaseObserver );
        
		mBillingService = new MoaiBillingService();
        mBillingService.setContext (this);

		startConnectivityReceiver ();
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
		
		log ( "onPause: Start" );
		
		// Call pause on the main thread's next run loop to avoid flicker
		// that occurs when we pause the Open GL surface.
		runOnMainThread ( new Runnable () {

			public void run () {
					
				log ( "Resuming now" );
						
				mMoaiView.pause ( true );
			}
		});

		log ( "onPause: Done" );
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
			
			log ( "onResume: Start" );

			// In some circumstances, resuming appears to cause the main thread
			// to block while the render thread performs a render. If that render
			// loop causes the lua to call back into the Java host, we can deadlock
			// if we proxy the call to the main thread and wait. So, to avoid that
			// possibility, we'll perform the resume on the main thread's next run
			// loop. 
			runOnMainThread ( new Runnable () {

				public void run () {
					
					log ( "Resuming now" );
						
					mMoaiView.pause ( false );
				}
			}, 100 );

			log ( "onResume: Done" );
		}
	}

	//----------------------------------------------------------------//
	protected void onStart () {

		log ( "MoaiActivity onStart called" );

		super.onStart ();
		
        MoaiBillingResponseHandler.register ( mPurchaseObserver );

		// TODO: If a session is stopped, then restarted, we do not resume the session...
	}
	
	//----------------------------------------------------------------//
	protected void onStop () {

		log ( "MoaiActivity onStop called" );

		super.onStop ();

        MoaiBillingResponseHandler.unregister ( mPurchaseObserver );

		AKUAppWillEndSession ();
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
	
	//----------------------------------------------------------------//
	public void startSession () {

		AKUAppDidStartSession ();
	}
	
	//================================================================//
	// Private methods
	//================================================================//

	//----------------------------------------------------------------//
	private void runOnMainThread ( final Runnable runnable ) {

		runOnMainThread ( runnable, false, 0 );
	}
	
	//----------------------------------------------------------------//
	private void runOnMainThread ( final Runnable runnable, boolean wait ) {
	
		runOnMainThread ( runnable, wait, 0 );
	}

	//----------------------------------------------------------------//
	private void runOnMainThread ( final Runnable runnable, long delay ) {
	
		runOnMainThread ( runnable, false, delay );
	}
	
	//----------------------------------------------------------------//
	private void runOnMainThread ( final Runnable runnable, boolean wait, long delay ) {

		log ( "runOnMainThread: Start (Thread: " + Thread.currentThread ().toString () + ")" );
		
		if ( wait && ( Thread.currentThread () == Looper.getMainLooper ().getThread () ))
		{
			log ( "runOnMainThread: Already on MAIN thread, continuing" );

			// NOTE: If the caller has asked that we wait for the runnable to complete
			// before returning, but we're already on the main thread, we IGNORE the
			// delay parameter rather than forcing the main thread to sleep.
			runnable.run ();
			
			log ( "runOnMainThread: Done running on MAIN thread" );
		} else if ( wait ) {
						
			final CountDownLatch signal = new CountDownLatch ( 1 );

			log ( "runOnMainThread: Going to wait" );

			mHandler.postDelayed ( new Runnable () {

				public void run () {

					log ( "runOnMainThread: MAIN Running on main thread" );
				
					runnable.run ();
					
					log ( "runOnMainThread: MAIN Done running, signaling" );
					
					signal.countDown ();
					
					log ( "runOnMainThread: MAIN Done signaling" );
				}
			}, delay );

			try {

				log ( "runOnMainThread: Waiting" );

				signal.await ();
				
				log ( "runOnMainThread: Done waiting" );
			} catch ( InterruptedException e ) {

			}
		} else {
			
			log ( "runOnMainThread: Not waiting" );
			
			mHandler.postDelayed ( runnable, delay );
			
			log ( "runOnMainThread: Kicked off to main thread, done" );
		}
		
		log ( "runOnMainThread: Done" );
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
			
			log ( "onWindowFocusChanged: Start" );
			
			// In some circumstances, resuming appears to cause the main thread
			// to block while the render thread performs a render. If that render
			// loop causes the lua to call back into the Java host, we can deadlock
			// if we proxy the call to the main thread and wait. So, to avoid that
			// possibility, we'll perform the resume on the main thread's next run
			// loop. 
			runOnMainThread ( new Runnable () {

				public void run () {
						
					log ( "Resuming now" );

					mMoaiView.pause ( false );
				}
			}, 100 );
			
			log ( "onWindowFocusChanged: Done" );
		}
	}

	//================================================================//
	// Misc JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void showDialog ( final String title, final String message, final String positiveButton, final String neutralButton, final String negativeButton, final boolean cancelable ) {

		log ( "showDialog: Start" );

	 	runOnMainThread ( new Runnable () {

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
		
		log ( "showDialog: Done" );
	}
	
	//----------------------------------------------------------------//
	public void share ( final String prompt, final String subject, final String text ) {

		log ( "share: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
				
				Intent intent = new Intent ( Intent.ACTION_SEND );
				intent.setType ( "text/plain" );
		
				if ( subject != null ) intent.putExtra ( Intent.EXTRA_SUBJECT, subject );
				if ( text != null ) intent.putExtra ( Intent.EXTRA_TEXT, text );
		
				MoaiActivity.this.startActivity ( Intent.createChooser ( intent, prompt ));
			}
		});

		log ( "share: Done" );
	}

	//================================================================//
	// Open Url JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void openURL ( final String url ) {

		log ( "openURL: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {

				Uri uri = Uri.parse ( url );
				Intent intent = new Intent ( Intent.ACTION_VIEW, uri );
				MoaiActivity.this.startActivity ( intent );
			}
		});

		log ( "openURL: Done" );
	}
	
	//================================================================//
	// Crittercism JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public void initCrittercism ( final String appId ) {

		log ( "initCrittercism: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
		
				Crittercism.init ( getApplicationContext(), appId );
			}
		});

		log ( "initCrittercism: Done" );
	}
	
	//================================================================//
	// Tapjoy JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public String getUserId () {

		log ( "getUserId: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {

 				mTapjoyUserId = TapjoyConnect.getTapjoyConnectInstance ().getUserID ();
			}
		}, true );
		
		log ( "getUserId: Done" );
		
		return mTapjoyUserId;
	}
	 
	//----------------------------------------------------------------//
	public void initVideoAds () {

		log ( "initVideoAds: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
		
				TapjoyConnect.getTapjoyConnectInstance ().initVideoAd ( MoaiActivity.this );
			}
		});

		log ( "initVideoAds: Done" );
	}

	//----------------------------------------------------------------//
	public void requestTapjoyConnect ( final String appId, final String appSecret ) {

		log ( "requestTapjoyConnect: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
				
				TapjoyConnect.requestTapjoyConnect ( MoaiActivity.this, appId, appSecret );
			}
		});

		log ( "requestTapjoyConnect: Done" );
	}
		
	//----------------------------------------------------------------//
	public void setVideoAdCacheCount ( final int count ) {

		log ( "setVideoAdCacheCount: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
		
				TapjoyConnect.getTapjoyConnectInstance ().setVideoCacheCount ( count );
			}
		});

		log ( "setVideoAdCacheCount: Done" );
	}
	
	//----------------------------------------------------------------//
	public void showOffers () {

		log ( "showOffers: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
		
				TapjoyConnect.getTapjoyConnectInstance ().showOffers ();
			}
		});

		log ( "showOffers: Done" );
	}
		
	//================================================================//
	// In-App Billing JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public boolean checkBillingSupported () {
		
		log ( "checkBillingSupported: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
				
				mMarketBillingSupported = mBillingService.checkBillingSupported ();				
			}
		}, true);

		log ( "checkBillingSupported: Done" );

		return mMarketBillingSupported;
	}

	//----------------------------------------------------------------//
	public boolean confirmNotification ( final String notificationId ) {
		
		log ( "confirmNotification: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
		
				ArrayList<String> notifyList = new ArrayList<String> ();
   				notifyList.add ( notificationId );

				String[] notifyIds = notifyList.toArray ( new String[notifyList.size ()] );

				mMarketNotificationsConfirmed = mBillingService.confirmNotifications ( notifyIds );
			}
		}, true );
		
		log ( "confirmNotification: Done" );
		
		return mMarketNotificationsConfirmed;
	}
		
	//----------------------------------------------------------------//
	public boolean requestPurchase ( final String productId, final String developerPayload ) {
	
		log ( "requestPurchase: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
					
				mMarketPurchaseRequested = mBillingService.requestPurchase ( productId, developerPayload );
			}
		}, true );
		
		log ( "requestPurchase: Done" );
		
		return mMarketPurchaseRequested;
	}
	
	//----------------------------------------------------------------//
	public boolean restoreTransactions () {

		log ( "restoreTransactions: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
					
				mMarketTransactionsRestored = mBillingService.restoreTransactions ();
			}
		}, true );
		
		log ( "restoreTransactions: Done" );
		
		return mMarketTransactionsRestored;
	}
	
	//----------------------------------------------------------------//
	public void setMarketPublicKey ( final String key ) {
	
		log ( "setMarketPublicKey: Start" );

		runOnMainThread ( new Runnable () {

			public void run () {
					
				MoaiBillingSecurity.setPublicKey ( key );
			}
		});

		log ( "setMarketPublicKey: Done" );
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