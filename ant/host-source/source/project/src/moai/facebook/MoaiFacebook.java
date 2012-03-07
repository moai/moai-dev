//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.android.FacebookError;

//================================================================//
// MoaiFacebook
//================================================================//
public class MoaiFacebook {

	public enum DialogResultCode {
		
		RESULT_SUCCESS,
	    RESULT_CANCEL,
        RESULT_ERROR;
						
        public static DialogResultCode valueOf ( int index ) {
	
            DialogResultCode [] values = DialogResultCode.values ();
            if (( index < 0 ) || ( index >= values.length )) {
	
                return RESULT_ERROR;
            }

            return values [ index ];
        }
    }

	private static Activity sActivity = null;
	private static Facebook	sFacebook = null;

	protected static native void	AKUNotifyFacebookLoginComplete	( int statusCode );
	protected static native void	AKUNotifyFacebookPostComplete	( int statusCode );

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "onCreate: Initializing Facebook" );
		
		sActivity = activity;
	}
	
	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
//		sFacebook.authorizeCallback ( requestCode, resultCode, data );
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//	
	public static String getToken () {

		return sFacebook.getAccessToken (); 
	}
	
	//----------------------------------------------------------------//	
	public static void login ( String [] permissions ) {
		
		sFacebook.authorize ( sActivity, permissions, new DialogListener () {
			
	        @Override
	        public void onComplete ( Bundle values ) {
		
				AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_SUCCESS.ordinal() );
	        }
	
	        @Override
	        public void onFacebookError ( FacebookError error ) {
		
				AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
			}
	
	        @Override
	        public void onError ( DialogError e ) {
		
				AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
			}
	
	        @Override
	        public void onCancel () {
		
				AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
			}
		});
	}

	//----------------------------------------------------------------//	
	public static void logout () {
		
		try {
			
			sFacebook.logout ( sActivity );
		}
		catch ( Throwable  e ) {
			
		}
	}
	
	//----------------------------------------------------------------//	
	public static void init ( String appId ) {
		
		sFacebook = new Facebook ( appId ); 
	}

	//----------------------------------------------------------------//	
	public static boolean isSessionValid () {

		return sFacebook.isSessionValid ();
	}
	
	//----------------------------------------------------------------//	
	public static void postToFeed () {
		
		sFacebook.dialog ( sActivity, "feed", new DialogListener () {
			
	        @Override
	        public void onComplete ( Bundle values ) {
		
				AKUNotifyFacebookPostComplete ( DialogResultCode.RESULT_SUCCESS.ordinal() );
	        }
	
	        @Override
	        public void onFacebookError ( FacebookError error ) {
		
				AKUNotifyFacebookPostComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
			}
	
	        @Override
	        public void onError ( DialogError e ) {
		
				AKUNotifyFacebookPostComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
			}
	
	        @Override
	        public void onCancel () {
		
				AKUNotifyFacebookPostComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
			}
		});
	}
	
	//----------------------------------------------------------------//	
	public static void setToken ( String token ) {

		sFacebook.setAccessToken ( token ); 
	}
}