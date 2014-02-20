//package moai;
package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;
import android.provider.MediaStore;

public class MoaiCamera  {

	private static Activity sActivity;
	
	private static final int CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE = 100;
	private static final int CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE = 200;
	
	protected static native void AKUNotifyPictureTaken( int statusCode, String path );
	
	public static void onCreate( Activity activity ) {
		MoaiLog.d(" MoaiCamera.java::onCreate ");
		sActivity = activity;
		
		
	}
	
	public static void onActivityResult( int requestCode, int resultCode, Intent data ) {
		MoaiLog.d( "MoaiCamera.java::onActivityResult" );
	    if (requestCode == CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE) {
	        if (resultCode == Activity.RESULT_OK) {
//	        	MoaiLog.d( "data.getData().getEncodedPath(): " + data.getData().getEncodedPath() );
	        	MoaiLog.d( "data.getData().getPath():        " + data.getData().getPath() );
	        	
	        	// send location via jni to lua form here
	        	// or read data and send whole data - 
	        	// http://stackoverflow.com/questions/9027695/android-application-how-to-use-the-camera-and-grab-the-image-bytes
	        	
	        	AKUNotifyPictureTaken( resultCode, data.getData().getPath() );
	        	
	        } else if (resultCode == Activity.RESULT_CANCELED) {
	            // User cancelled the image capture
	        	AKUNotifyPictureTaken( resultCode, "" );
	        } else {
	            // Image capture failed, advise user
	        	AKUNotifyPictureTaken( resultCode, "" );
	        }
	    }

    }
	
	//================================================================//
	// JNI callback methods
	//================================================================//
	
	public static void takePicture() {
		MoaiLog.d( "MoaiCamera.java::takePicture()" );
//		http://stackoverflow.com/questions/12564112/android-camera-onactivityresult-intent-is-null-if-it-had-extras
		sActivity.runOnUiThread( new Runnable() {
			@Override
			public void run() {
			    Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			    sActivity.startActivityForResult(intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE);
				
			}
		} );
//		// create Intent to take a picture and return control to the calling application
//	    Intent intent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );
//
//	    // dont pass filename to intent,otherwise the dataintent in onACtivityResult is always NULL
//	    // fileUri = getOutputMediaFileUri(MEDIA_TYPE_IMAGE); // create a file to save the image
//	    // intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri); // set the image file name
//
//	    // start the image capture Intent
//	   sActivity.startActivityForResult( intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE );
	}
	
}
