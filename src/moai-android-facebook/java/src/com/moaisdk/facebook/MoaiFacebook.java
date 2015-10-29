//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.facebook;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Collection;
import com.moaisdk.core.*;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphResponse;
import com.facebook.GraphRequest;
import com.facebook.HttpMethod;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.widget.AppInviteDialog;
import com.facebook.share.model.AppInviteContent;

import java.net.URL;
import java.util.HashMap;

import org.json.*;

@SuppressWarnings("unused")

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

	private static Activity sActivity                                   = null;
    private static CallbackManager sCallbackManager                     = null;
	private static String sAppId                                        = null;
    private static String sUserEmail                                    = null;
    private static String sUserID                                       = null;
    private static String sUserName                                     = null;
    private static AccessToken  sLoginAccessToken                       = null;

    protected static native void	AKUNotifyFacebookLoginSuccess	();
    protected static native void	AKUNotifyFacebookLoginDismissed	();
    protected static native void	AKUNotifyFacebookLoginError 	();

    private static FacebookCallback sLoginCallback = new FacebookCallback<LoginResult>() {
        @Override
        public void onSuccess ( LoginResult loginResult ) {

            MoaiLog.i ( "MoaiFacebook onSuccess" );
            sLoginAccessToken = loginResult.getAccessToken ();

            GraphRequest.newMeRequest(
                    sLoginAccessToken, new GraphRequest.GraphJSONObjectCallback () {

                        @Override
                        public void onCompleted ( JSONObject json, GraphResponse response ) {
                            if (response.getError () != null) {

                                // handle error
                                System.out.println ( "ERROR" );
                                AKUNotifyFacebookLoginError ();

                            } else {

                                System.out.println ( "Success" );
                                try {

                                    sUserID         = json.getString ( "id" );
                                    sUserName       = json.getString ( "name" );

                                    AKUNotifyFacebookLoginSuccess ();

                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }
                            }
                        }

                    }).executeAsync();
        }

        @Override
        public void onCancel() {

            MoaiLog.i ( "MoaiFacebook onCancel" );
            sUserEmail      = null;
            sUserID         = null;
            sUserName       = null;
            AKUNotifyFacebookLoginDismissed ();
        }

        @Override
        public void onError(FacebookException exception) {

            MoaiLog.i ( "MoaiFacebook onError" );
            sUserEmail      = null;
            sUserID         = null;
            sUserName       = null;
            AKUNotifyFacebookLoginError ();
        }
    };

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {

		MoaiLog.i ( "MoaiFacebook onActivityResult: Calling Session onActivityResult ()" );
        sCallbackManager.onActivityResult(requestCode, resultCode, data);

//		Session session = Session.getActiveSession ();
//		if ( session != null ) {
//		      session.onActivityResult ( sActivity, requestCode, resultCode, data );
//		}
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiFacebook onCreate: Initializing Facebook" );

		sActivity = activity;
        sCallbackManager = CallbackManager.Factory.create();

        FacebookSdk.sdkInitialize ( sActivity );
        LoginManager.getInstance().registerCallback ( sCallbackManager, sLoginCallback );
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i("MoaiFacebook on resume");
//		if(sAppId != null) {
//			com.facebook.AppEventsLogger.activateApp(sActivity, sAppId);
//		}
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

    //----------------------------------------------------------------//
    public static String getToken () {

        MoaiLog.i ( "MoaiFacebook: getToken" );
        return sLoginAccessToken.getToken ();
    }

    //----------------------------------------------------------------//
    public static String getUserID () {

        //MoaiLog.i ( "MoaiFacebook: getUserID" );
        return sUserID;
    }

	//----------------------------------------------------------------//
	public static void init ( String appId ) {

		sAppId = appId;
	}

	//----------------------------------------------------------------//
	public static boolean isSessionValid () {

		MoaiLog.i ( "MoaiFacebook: isSessionValid" );
//		Session session = Session.getActiveSession ();
//		if (( session != null ) && session.isOpened ()) {
//			MoaiLog.i ( "MoaiFacebook: session is valid" );
//			return true;
//		}
		MoaiLog.i ( "MoaiFacebook: SESSION IS INVALID" );
		return false;
	}

	//----------------------------------------------------------------//
	public static void login ( String [] p ) {

		MoaiLog.i("MoaiFacebook: login");

        // TODO pass permissions via argument
        ArrayList<String> permissions = new ArrayList<String>();
        permissions.add ( "user_friends" );
        permissions.add ( "user_photos" );
        permissions.add ( "public_profile" );
        permissions.add ( "email" );
        LoginManager.getInstance().logInWithReadPermissions ( sActivity, permissions );
	}

	//----------------------------------------------------------------//
	public static void logout () {

        MoaiLog.i("MoaiFacebook: logout");
        LoginManager.getInstance ().logOut ();
	}

	//----------------------------------------------------------------//
	public static void postToFeed ( String link, String picture, String name, String caption, String description, String message ) {

	}

	//----------------------------------------------------------------//
	public static boolean restoreSession () {

		MoaiLog.i ( "MoaiFacebook: restoreSession" );

        return false;
	}

    //----------------------------------------------------------------//
    public static void showInviteDialog ( String linkURL, String imgURL ) {

        MoaiLog.i ( "MoaiFacebook: showInviteDialog" );

        if ( AppInviteDialog.canShow()) {
            AppInviteContent content = new AppInviteContent.Builder ()
                    .setApplinkUrl ( linkURL )
                    .setPreviewImageUrl ( imgURL )
                    .build ();
            AppInviteDialog.show ( sActivity, content );
        }
    }
}