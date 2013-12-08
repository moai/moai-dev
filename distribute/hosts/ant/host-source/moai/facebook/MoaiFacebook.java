//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import java.io.IOException;
import java.net.MalformedURLException;

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
	protected static native void	AKUNotifyFacebookDialogComplete	( int statusCode );

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
		MoaiLog.i ( "MoaiFacebook onActivityResult: Calling authorize callback" );

		sFacebook.authorizeCallback ( requestCode, resultCode, data );
	}
	
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiFacebook onCreate: Initializing Facebook" );
		
		sActivity = activity;
	}
	
	//================================================================//
	// Facebook JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//	
	public static void extendToken () {

		sFacebook.extendAccessTokenIfNeeded ( sActivity, null ); 
	}
	
	public static long getExpirationDate () {
		return sFacebook.getAccessExpires ();
	}

	//----------------------------------------------------------------//	
	public static String getToken () {

		return sFacebook.getAccessToken (); 
	}
	
	//----------------------------------------------------------------//	
	public static String graphRequest ( String path ) {

		String jsonResult;
		try {
			
			jsonResult = sFacebook.request ( path );
		} catch ( MalformedURLException urle ) {
			
			jsonResult = "Invalid URL";
		} catch ( IOException ioe ) {
			
			jsonResult = "Network Error";
		}
		
		return jsonResult;
	}
	
	//----------------------------------------------------------------//	
	public static void init ( String appId ) {
		
		sFacebook = new Facebook ( appId ); 
		sFacebook.extendAccessTokenIfNeeded ( sActivity, null );
	}

	//----------------------------------------------------------------//	
	public static boolean isSessionValid () {

		return sFacebook.isSessionValid ();
	}
	
	//----------------------------------------------------------------//	
	public static void login ( String [] permissions ) {
		
		sFacebook.authorize ( sActivity, permissions, new DialogListener () {
			
	        @Override
	        public void onComplete ( Bundle values ) {
						
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_SUCCESS.ordinal() );
				}
	        }
	
	        @Override
	        public void onFacebookError ( FacebookError error ) {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onError ( DialogError e ) {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onCancel () {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
				}
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
	public static void postToFeed ( String link, String picture, String name, String caption, String description, String message ) {

		Bundle parameters = new Bundle ();
		
		if ( link != null )	parameters.putString ( "link", link );
		if ( picture != null )	parameters.putString ( "picture", picture );
		if ( name != null )	parameters.putString ( "name", name );
		if ( caption != null )	parameters.putString ( "caption", caption );
		if ( description != null )	parameters.putString ( "description", description );
		if ( message != null )	parameters.putString ( "message", message );
		
		sFacebook.dialog ( sActivity, "feed", parameters, new DialogListener () {
			
	        @Override
	        public void onComplete ( Bundle values ) {
		
				if ( values.containsKey ( "post_id" )) {
					
					
					synchronized ( Moai.sAkuLock ) {
						AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_SUCCESS.ordinal() );
					}
				} else {
					
					synchronized ( Moai.sAkuLock ) {
						AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
					}
				}
	        }
	
	        @Override
	        public void onFacebookError ( FacebookError error ) {
		
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onError ( DialogError e ) {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onCancel () {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
				}
			}
		});
	}
	
	//----------------------------------------------------------------//	
	public static void sendRequest ( String message ) {

		Bundle parameters = new Bundle ();
		
		if ( message != null )	parameters.putString ( "message", message );
		
		sFacebook.dialog ( sActivity, "apprequests", parameters, new DialogListener () {
			
	        @Override
	        public void onComplete ( Bundle values ) {
		
				if ( values.containsKey ( "request" )) {
					
					
					synchronized ( Moai.sAkuLock ) {
						AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_SUCCESS.ordinal() );
					}
				} else {
					
					synchronized ( Moai.sAkuLock ) {
						AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
					}
				}
	        }
	
	        @Override
	        public void onFacebookError ( FacebookError error ) {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onError ( DialogError e ) {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_ERROR.ordinal() );
				}
			}
	
	        @Override
	        public void onCancel () {
				
				synchronized ( Moai.sAkuLock ) {
					AKUNotifyFacebookDialogComplete ( DialogResultCode.RESULT_CANCEL.ordinal() );
				}
			}
		});
	}
	
	//----------------------------------------------------------------//	
	public static void setToken ( String token ) {

		sFacebook.setAccessToken ( token ); 
	}
	
	public static void setExpirationDate ( long expires ) {

		sFacebook.setAccessExpires ( expires );
	}
}