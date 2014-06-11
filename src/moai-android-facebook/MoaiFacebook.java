//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.Arrays;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;

import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.android.FacebookError;
import com.facebook.AccessToken;
import com.facebook.AccessTokenSource;
import com.facebook.NonCachingTokenCachingStrategy;
import com.facebook.Session;
import com.facebook.SessionState;

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
	private static String sAppId = null;

	protected static native void	AKUNotifyFacebookLoginComplete	( int statusCode );
	protected static native void	AKUNotifyFacebookDialogComplete	( int statusCode );

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {

		MoaiLog.i ( "MoaiFacebook onActivityResult: Calling Session onActivityResult ()" );

		Session session = Session.getActiveSession ();
		if ( session != null ) {
		      session.onActivityResult ( sActivity, requestCode, resultCode, data );
		  }
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiFacebook onCreate: Initializing Facebook" );

		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i("MoaiFacebook on resume");
		if(sAppId != null) {
			com.facebook.AppEventsLogger.activateApp(sActivity, sAppId);
		}
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static String getToken () {

		MoaiLog.i ( "MoaiFacebook: getToken" );
		Session session = Session.getActiveSession ();
		if (( session != null ) && session.isOpened ()) {
			String token =  session.getAccessToken ();
			MoaiLog.i ( token );
			return token;
		}
		MoaiLog.i ( "MoaiFacebook: NO ACTIVE SESSION OR TOKEN" );
		return null;
	}

	//----------------------------------------------------------------//
	public static String graphRequest ( String path ) {

		// String jsonResult;
		// try {
		// 	jsonResult = sFacebook.request ( path );
		// } catch ( MalformedURLException urle ) {
		// 	jsonResult = "Invalid URL";
		// } catch ( IOException ioe ) {
		// 	jsonResult = "Network Error";
		// }
		// return jsonResult;
		return null;
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {

		sAppId = appId;
		//sFacebook = new Facebook ( appId );
		//sFacebook.extendAccessTokenIfNeeded ( sActivity, null );
	}

	//----------------------------------------------------------------//
	public static boolean isSessionValid () {

		MoaiLog.i ( "MoaiFacebook: isSessionValid" );
		Session session = Session.getActiveSession ();
		if (( session != null ) && session.isOpened ()) {
			MoaiLog.i ( "MoaiFacebook: session is valid" );
			return true;
		}
		MoaiLog.i ( "MoaiFacebook: SESSION IS INVALID" );
		return false;
	}

	//----------------------------------------------------------------//
	public static void login ( String [] permissions ) {

		MoaiLog.i ( "MoaiFacebook: login" );

		openActiveSession ( sActivity, true, Arrays.asList ( permissions ), new Session.StatusCallback () {
			
			@Override
			public void call ( Session session, SessionState state, Exception exception ) {
			
				switch ( state ) {
					
					case CLOSED:
					case CLOSED_LOGIN_FAILED:
						MoaiLog.i ( "MoaiFacebook: LOGIN FAILED" );
						synchronized ( Moai.sAkuLock ) {
							AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_ERROR.ordinal ());
						}
						break;
					
					case OPENED:
					case OPENED_TOKEN_UPDATED:
						MoaiLog.i ( "MoaiFacebook: LOGIN SUCCEEDED" );
						synchronized ( Moai.sAkuLock ) {
							AKUNotifyFacebookLoginComplete ( DialogResultCode.RESULT_SUCCESS.ordinal ());
						}
						break;
						
					default:
				}
			}
		});

		/*
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
		*/
	}

	//----------------------------------------------------------------//
	public static void logout () {

		Session session = Session.getActiveSession ();
		if ( session != null ) {
			session.closeAndClearTokenInformation ();
		}
	}

	//----------------------------------------------------------------//
	private static Session newSession () {
		Session.Builder builder = new Session.Builder ( sActivity );
		builder.setApplicationId ( sAppId );
		builder.setTokenCachingStrategy ( new NonCachingTokenCachingStrategy ());
		return builder.build ();
	}

	//----------------------------------------------------------------//
	private static Session openActiveSession ( Activity activity, boolean allowLoginUI, List < String > permissions, Session.StatusCallback callback ) {
	    Session.OpenRequest openRequest = new Session.OpenRequest ( activity ).setPermissions ( permissions ).setCallback ( callback );
	    Session session = new Session.Builder ( activity ).build ();
	    if ( SessionState.CREATED_TOKEN_LOADED.equals ( session.getState ()) || allowLoginUI ) {
	        Session.setActiveSession ( session );
	        session.openForRead ( openRequest );
	        return session;
	    }
	    return null;
	}

	//----------------------------------------------------------------//
	public static void postToFeed ( String link, String picture, String name, String caption, String description, String message ) {

		/*
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
		*/
	}

	//----------------------------------------------------------------//
	public static boolean restoreSession () {

		MoaiLog.i ( "MoaiFacebook: restoreSession" );

		Session session = Session.openActiveSessionFromCache ( sActivity );
		return session != null;

		/*
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences ( sActivity ); 
		String accessTokenString = prefs.getString ( "fb_access_token", null );

		//accessTokenString = "CAAFPzY0JN4wBALiWmxfHHEizTo47s0XJezYLFVOASw3yGzpuizgnCbEZA5i3sZBJQ1jUJIyjxneZCs6qD9ZA6Rjgh7FOIIpAbJwny2kjRI4Xx9gPYGN4Sh325X9abVAi5z6JqZA5NSlf3uj1gaYx15TdGcBWZAzgLy9iCO2o9Hdy8wh1ZBG6VMI";
		accessTokenString = "AAAFPzY0JN4wBALiWmxfHHEizTo47s0XJezYLFVOASw3yGzpuizgnCbEZA5i3sZBJQ1jUJIyjxneZCs6qD9ZA6Rjgh7FOIIpAbJwny2kjRI4Xx9gPYGN4Sh325X9abVAi5z6JqZA5NSlf3uj1gaYx15TdGcBWZAzgLy9iCO2o9Hdy8wh1ZBG6VMI";

		MoaiLog.i ( String.format ( "MoaiFacebook: restoreSession access token %s", accessTokenString ));
		
		AccessToken accessToken = AccessToken.createFromExistingAccessToken ( accessTokenString, null, null, AccessTokenSource.WEB_VIEW, null );
		
		Session session = newSession ();
		session.open ( accessToken, null );
		if ( session.isOpened ()) {
			Session.setActiveSession ( session );
			MoaiLog.i ( "MoaiFacebook: session restored and activated" );
			return true;
		}
		
		MoaiLog.i ( "MoaiFacebook: SESSION NOT RESTORED" );
		return false;
		*/
	}

	//----------------------------------------------------------------//
	public static void sendRequest ( String message ) {

		/*
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
		*/
	}
}