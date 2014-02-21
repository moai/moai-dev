//package moai;
package com.ziplinegames.moai;

import java.util.concurrent.atomic.AtomicBoolean;

import android.app.Activity;
import android.content.Intent;
import android.provider.MediaStore;

public class MoaiCamera  {

	private static Activity sActivity;
	
	private static final int CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE = 100;
	private static final int CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE = 200;

	protected static native void AKUNotifyPictureTaken( int statusCode, String path );
	protected static native void AKUNotifyPictureTaken(  );
	
	private static AtomicBoolean sPictureTakenFlag = new AtomicBoolean(false);
	
	// codes:
	// -1: RESULT_OK
	//  0: RESULT_CANCELED
	private static int sResultCode = 0;
	private static String sResultPath = "";

	public static AtomicBoolean spFlag = new AtomicBoolean( false );
	
	public static void onCreate( Activity activity ) {
		MoaiLog.d(" MoaiCamera.java::onCreate ");
		sActivity = activity;
	}
	
	public static void onActivityResult( int requestCode, int resultCode, Intent data ) {
		synchronized( Moai.sAkuLock ) {
			MoaiLog.d( "MoaiCamera.java::onActivityResult - running on thread " + Thread.currentThread().getId() );
		    if (requestCode == CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE) {
		    	sResultCode = resultCode;
		        if (resultCode == Activity.RESULT_OK) {
		        	sResultPath = data.getData().getPath();
		        	MoaiLog.d( "data.getData().getPath():        " + data.getData().getPath() );
		        	// http://stackoverflow.com/questions/9027695/android-application-how-to-use-the-camera-and-grab-the-image-bytes
		        } else if (resultCode == Activity.RESULT_CANCELED) {
		            // User cancelled the image capture
		        	MoaiLog.d( "MoaiCamera.java::onACtivityResult(): Image capture canceled!" );
		        } else {
		            // Image capture failed, advise user
		        	MoaiLog.d( "MoaiCamera.java::onACtivityResult(): Image capture failed!" );
		        }
		    } else {
		    	MoaiLog.w( "MoaiCamera.java::onActivityResult(): unexpected requestCode recieved!" );
		    }
		    // tell jni that we are finished
		    MoaiLog.d( "MoaiCamera.java:before notifying ..." );
//		    AKUNotify
		    AKUNotifyPictureTaken( resultCode, sResultPath );
		    
		    spFlag.getAndSet(true);
		}
    }
	
	//================================================================//
	// JNI callback methods
	//================================================================//
	
	public static void takePicture() {
//		synchronized( Moai.sAkuLock ) {
		MoaiLog.d( "MoaiCamera.java::takePicture() - running on thread " + Thread.currentThread().getId() );
		spFlag.getAndSet(false);
//		http://stackoverflow.com/questions/12564112/android-camera-onactivityresult-intent-is-null-if-it-had-extras
		sActivity.runOnUiThread( new Runnable() {
			@Override
			public void run() {
				synchronized( Moai.sAkuLock ) {
				    Intent intent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );
				    sActivity.startActivityForResult(intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE);
				}
			}
		} );
//	    Intent intent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );
//	    // dont pass filename to intent,otherwise the dataintent in onACtivityResult is always NULL
//	    // fileUri = getOutputMediaFileUri(MEDIA_TYPE_IMAGE); // create a file to save the image
//	    // intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri); // set the image file name
//	   sActivity.startActivityForResult( intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE );
//		}
		int foo = 0;
		while( spFlag.get() == false ) {
			try{
				Thread.sleep( 1000);
				MoaiLog.d( "... waiting ... zzZZzzzZZZzzzZZZzz" );
				if( foo > 20 ) {
					spFlag.getAndSet(true);
				}
				foo++;
			}
			catch( Exception e ) {
				MoaiLog.d( "caught exception while waiting: " + e.getMessage() );
			}
		}
	}
	
	public static int getResultCode() {
		return sResultCode;
	}
	
	public static String getResultPath() {
		return sResultPath;
	}
	
}
