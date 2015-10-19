//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.twitter;

import com.moaisdk.core.Moai;
import com.moaisdk.core.MoaiLog;

import android.app.Activity;
import android.content.Intent;

import twitter4j.AsyncTwitter;
import twitter4j.AsyncTwitterFactory;
import twitter4j.Status;
import twitter4j.Twitter;
import twitter4j.TwitterAdapter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.TwitterMethod;
import twitter4j.auth.AccessToken;

import com.neovisionaries.TwitterOAuthView;

@SuppressWarnings("unused")

//================================================================//
// MoaiTwitter
//================================================================//
public class MoaiTwitter {

    private static enum ActivityResultRequestCode {
        LOGIN;
    }

    private static enum ResultCode {
		RESULT_SUCCESS,
        RESULT_ERROR;
						
        public static ResultCode valueOf ( int index ) {
            ResultCode [] values = ResultCode.values ();
            if (( index < 0 ) || ( index >= values.length )) {
                return RESULT_ERROR;
            }
            return values [ index ];
        }
    }

	private static Activity sActivity = null;
    
    // Used to perform operations that benefit from asynchronous requests. (Login, tweet, etc)
	private static AsyncTwitter	asyncTwitter = getAsyncTwitterInstance();
    // Used to determine if the user is authenticated in a synchronous manner
	private static Twitter syncTwitter = TwitterFactory.getSingleton();

    private static String consumerKey = "";
    private static String consumerSecret = "";
    private static String callbackUrl = "";

	protected static native void	AKUNotifyTwitterLoginComplete	( int statusCode, String token, String tokenSecret);
	protected static native void	AKUNotifyTwitterTweetComplete	( int statusCode );

    //----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
        MoaiLog.d ( "MoaiTwitter: onActivityResult, " + Integer.toString(resultCode));
        String token = "";
        String tokenSecret = "";
        
        if (ResultCode.valueOf(resultCode) == ResultCode.RESULT_SUCCESS) {
                token = data.getExtras().getString("token");
                tokenSecret = data.getExtras().getString("tokenSecret");
                asyncTwitter.setOAuthAccessToken(new AccessToken(token, tokenSecret));
                syncTwitter.setOAuthAccessToken(new AccessToken(token, tokenSecret));
        } 
        synchronized ( Moai.sAkuLock ) {
            AKUNotifyTwitterLoginComplete (resultCode, token, tokenSecret );
        }
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		MoaiLog.i ( "MoaiTwitter onCreate: Initializing Twitter" );
		sActivity = activity;
	}
	
    private static AsyncTwitter getAsyncTwitterInstance() {
        AsyncTwitter twitter = AsyncTwitterFactory.getSingleton();
        twitter.addListener(new TwitterAdapter() {
            @Override
            public void updatedStatus(Status status) {
                AKUNotifyTwitterTweetComplete (ResultCode.RESULT_SUCCESS.ordinal());
            }

            @Override
            public void onException(TwitterException e, TwitterMethod method) {
                if (method == TwitterMethod.UPDATE_STATUS) {
                    MoaiLog.d ( "MoaiTwitter:  Tweet Failed, " + e.getMessage());
                    AKUNotifyTwitterTweetComplete (ResultCode.RESULT_ERROR.ordinal());
                }
            }
        });
        return twitter;
    }

	//================================================================//
	// Twitter JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//	
	public static void init ( String consumerKey, String consumerSecret, String callbackUrl ) {
        asyncTwitter.setOAuthConsumer ( consumerKey, consumerSecret );
        syncTwitter.setOAuthConsumer ( consumerKey, consumerSecret );
        MoaiTwitter.consumerKey = consumerKey;
        MoaiTwitter.consumerSecret = consumerSecret;
        MoaiTwitter.callbackUrl = callbackUrl;
	}

	//----------------------------------------------------------------//	
	public static void login () {
        sActivity.runOnUiThread( new Runnable () {
            public void run () {
                TwitterOAuthView twitterOAuthView = new TwitterOAuthView ( sActivity );
                Intent loginIntent = new Intent(sActivity, TwitterOAuthActivity.class);
                loginIntent.putExtra("consumerKey", consumerKey);
                loginIntent.putExtra("consumerSecret", consumerSecret);
                loginIntent.putExtra("callbackUrl", callbackUrl);
                sActivity.startActivityForResult(loginIntent, ActivityResultRequestCode.LOGIN.ordinal());
            }
        });
    }

	//----------------------------------------------------------------//	
    public static boolean isLoggedIn () {
        boolean isLoggedIn = false;
        try {
            syncTwitter.verifyCredentials();
            isLoggedIn = true; 
        } catch (TwitterException ex) {
            MoaiLog.d ( "MoaiTwitter: Not Authenticated - " + ex.getMessage());
        } catch (IllegalStateException ex) {
            MoaiLog.d ( "MoaiTwitter: Not Authenticated - " + ex.getMessage());
        }

        return isLoggedIn; 
    }

    //----------------------------------------------------------------//	
	public static void setAccessToken (String token, String tokenSecret) {
        asyncTwitter.setOAuthAccessToken(new AccessToken(token, tokenSecret));
        syncTwitter.setOAuthAccessToken(new AccessToken(token, tokenSecret));
	}

	//----------------------------------------------------------------//	
	public static void sendTweet ( String text ) {
        asyncTwitter.updateStatus(text); 
	}
	
}