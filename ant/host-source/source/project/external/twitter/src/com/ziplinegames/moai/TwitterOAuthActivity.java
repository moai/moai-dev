package com.ziplinegames.moai;

import twitter4j.auth.AccessToken;
import com.neovisionaries.TwitterOAuthView;
import com.neovisionaries.TwitterOAuthView.Result;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.ziplinegames.moai.MoaiTwitter;

//================================================================//
// TwitterOAuthActivity 
//================================================================//
public class TwitterOAuthActivity extends Activity implements TwitterOAuthView.Listener {
    private static final boolean DONT_CALL_CALLBACK_URL = true;

    private TwitterOAuthView view;
    private boolean oauthStarted;

    String consumerKey = "";
    String consumerSecret = "";
    String callbackUrl = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        consumerKey = getIntent().getExtras().getString("consumerKey");
        consumerSecret = getIntent().getExtras().getString("consumerSecret");
        callbackUrl = getIntent().getExtras().getString("callbackUrl");

        view = new TwitterOAuthView(this);
        setContentView(view);

        oauthStarted = false;
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (oauthStarted) {
            return;
        }

        oauthStarted = true;

        view.start(consumerKey, consumerSecret, callbackUrl, DONT_CALL_CALLBACK_URL, this);
    }

    @Override
    public void onSuccess(TwitterOAuthView view, AccessToken accessToken) {
        Intent data = new Intent();
        data.putExtra("token", accessToken.getToken());
        data.putExtra("tokenSecret", accessToken.getTokenSecret());
        setResult(Result.SUCCESS.ordinal(), data);
        finish(); 
    }
 
    @Override
    public void onFailure(TwitterOAuthView view, Result result) {
        MoaiLog.d ( "MoaiTwitter: Login Failed, " + result.name());
        setResult(result.ordinal());
        finish(); 
    }
}
