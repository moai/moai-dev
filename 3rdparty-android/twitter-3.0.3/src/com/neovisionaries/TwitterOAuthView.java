/*
 * Copyright (C) 2012 Neo Visionaries Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.neovisionaries;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.auth.AccessToken;
import twitter4j.auth.RequestToken;
import android.content.Context;
import android.graphics.Bitmap;
import android.net.Uri;
import android.net.http.SslError;
import android.os.AsyncTask;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.webkit.SslErrorHandler;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;


/**
 * A {@link WebView} subclass dedicated to Twitter OAuth on Android,
 * using <a href="http://twitter4j.org/">twitter4j</a>.
 *
 * <p>
 * As this class is implemented as a subclass of View, it can be
 * integrated into the Android layout system seamlessly. This fact
 * makes this class an easily-reusable UI component.
 * </p>
 *
 * <p>
 * To use this class, it is not necessary to review the flow of
 * OAuth handshake. Just implement {@link TwitterOAuthView.Listener}
 * and call {@link #start(String, String, String, boolean, Listener)
 * start()} method. The result of OAuth handshake is reported via
 * either of the listener's methods, {@link
 * Listener#onSuccess(TwitterOAuthView, AccessToken) onSuccess()} or
 * {@link Listener#onFailure(TwitterOAuthView, TwitterOAuthView.Result)
 * onFailure()}.
 * </p>
 *
 * <p>
 * Below is an example Activity implementation using TwitterOAuthView.
 * </p>
 *
 * <pre style="border: 1px solid black; margin: 1em; padding: 0.5em;">
 * 
 * package twitteroauthview.sample;
 * 
 * 
 * import twitter4j.auth.AccessToken;
 * import com.neovisionaries.android.twitter.{@link TwitterOAuthView};
 * import com.neovisionaries.android.twitter.{@link TwitterOAuthView.Result};
 * import android.app.Activity;
 * import android.os.Bundle;
 * import android.widget.Toast;
 *
 *
 * public class TwitterOAuthActivity extends Activity implements {@link TwitterOAuthView.Listener}
 * {
 *     <span style="color: darkgreen;">// Replace values of the parameters below with your own.</span> 
 *     private static final String CONSUMER_KEY = "YOUR CONSUMER KEY HERE";
 *     private static final String CONSUMER_SECRET = "YOUR CONSUMER SECRET HERE";
 *     private static final String CALLBACK_URL = "YOUR CALLBACK URL HERE";
 *     private static final boolean DUMMY_CALLBACK_URL = true;
 * 
 * 
 *     private {@link TwitterOAuthView} view;
 *     private boolean oauthStarted;
 * 
 * 
 *     &#x0040;Override
 *     public void onCreate(Bundle savedInstanceState)
 *     {
 *         super.onCreate(savedInstanceState);
 * 
 *         <span style="color: darkgreen;">// Create an instance of TwitterOAuthView.</span>
 *         view = new {@link TwitterOAuthView#TwitterOAuthView(Context) TwitterOAuthView}(this);
 * 
 *         setContentView(view);
 * 
 *         oauthStarted = false;
 *     }
 * 
 * 
 *     &#x0040;Override
 *     protected void onResume()
 *     {
 *         super.onResume();
 * 
 *         if (oauthStarted)
 *         {
 *             return;
 *         }
 * 
 *         oauthStarted = true;
 * 
 *         <span style="color: darkgreen;">// Start Twitter OAuth process. Its result will be notified via
 *         // TwitterOAuthView.Listener interface.</span>
 *         view.{@link #start(String, String, String, boolean, Listener)
 *         start}(CONSUMER_KEY, CONSUMER_SECRET, CALLBACK_URL, DUMMY_CALLBACK_URL, this);
 *     }
 * 
 * 
 *     public void {@link TwitterOAuthView.Listener#onSuccess(TwitterOAuthView, AccessToken)
 *     onSuccess}({@link TwitterOAuthView} view, {@link AccessToken} accessToken)
 *     {
 *         <span style="color: darkgreen;">// The application has been authorized and an access token
 *         // has been obtained successfully. Save the access token
 *         // for later use.</span>
 *         showMessage("Authorized by " + accessToken.{@link AccessToken#getScreenName() getScreenName()});
 *     }
 * 
 * 
 *     public void {@link TwitterOAuthView.Listener#onFailure(TwitterOAuthView, TwitterOAuthView.Result)
 *     onFailure}({@link TwitterOAuthView} view, {@link TwitterOAuthView.Result Result} result)
 *     {
 *         <span style="color: darkgreen;">// Failed to get an access token.</span>
 *         showMessage("Failed due to " + result);
 *     }
 * 
 * 
 *     private void showMessage(String message)
 *     {
 *         <span style="color: darkgreen;">// Show a popup message.</span>
 *         Toast.makeText(this, message, Toast.LENGTH_LONG).show();
 *     }
 * }
 * </pre>
 *
 * @author Takahiko Kawasaki
 */
public class TwitterOAuthView extends WebView
{
    /**
     * Tag for logging.
     */
    private static final String TAG = "TwitterOAuthView";


    /**
     * Internal flag for debug logging. Change the value to 'true'
     * to turn on debug logging.
     */
    private static final boolean DEBUG = false;


    /**
     * Result code of Twitter OAuth process.
     *
     * @author Takahiko Kawasaki
     */
    public enum Result
    {
        /**
         * The application has been authorized by the user and
         * got an access token successfully.
         */
        SUCCESS,


        /**
         * Twitter OAuth process was cancelled. This result code
         * is generated when the internal {@link AsyncTask}
         * subclass was cancelled for some reasons.
         */
        CANCELLATION,


        /**
         * Twitter OAuth process was not even started due to
         * failure of getting a request token. The pair of
         * consumer key and consumer secret was wrong or some
         * kind of network error occurred.
         */
        REQUEST_TOKEN_ERROR,


        /**
         * The application has not been authorized by the user,
         * or a network error occurred during the OAuth handshake.
         */
        AUTHORIZATION_ERROR,


        /**
         * The application has been authorized by the user but
         * failed to get an access token.
         */
        ACCESS_TOKEN_ERROR
    }


    /**
     * Listener to be notified of Twitter OAuth process result.
     *
     * <p>
     * The methods of this listener are called on the UI thread.
     * </p>
     *
     * @author Takahiko Kawasaki
     */
    public interface Listener
    {
        /**
         * Called when the application has been authorized by the user
         * and got an access token successfully.
         *
         * @param view
         * @param accessToken
         */
        void onSuccess(TwitterOAuthView view, AccessToken accessToken);


        /**
         * Called when the OAuth process was not completed successfully.
         *
         * @param view
         * @param result
         */
        void onFailure(TwitterOAuthView view, Result result);
    }


    /**
     * A constructor that calls {@link WebView#WebView(Context, AttributeSet, int)
     * super}(context, attrs, defStyle).
     *
     * @param context
     * @param attrs
     * @param defStyle
     */
    public TwitterOAuthView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);

        // Additional initialization.
        init();
    }


    /**
     * A constructor that calls {@link WebView#WebView(Context, AttributeSet)
     * super}(context, attrs).
     * 
     * @param context
     * @param attrs
     */
    public TwitterOAuthView(Context context, AttributeSet attrs)
    {
        super(context, attrs);

        // Additional initialization.
        init();
    }


    /**
     * A constructor that calls {@link WebView#WebView(Context) super}(context).
     * 
     * @param context
     */
    public TwitterOAuthView(Context context)
    {
        super(context);

        // Additional initialization.
        init();
    }


    private void init()
    {
        WebSettings settings = getSettings();

        // Not use cache.
        settings.setCacheMode(WebSettings.LOAD_NO_CACHE);

        // Enable JavaScript.
        settings.setJavaScriptEnabled(true);

        // Enable zoom control.
        settings.setBuiltInZoomControls(true);

        // Scroll bar
        setScrollBarStyle(WebView.SCROLLBARS_INSIDE_OVERLAY);
    }


    /**
     * Start Twitter OAuth process.
     *
     * <p>
     * This method does the following in the background.
     * </p>
     *
     * <ol>
     * <li>Get a request token using the given pair of consumer key
     *     and consumer secret.
     * <li>Load the authorization URL that the obtained request token
     *     points to into this TwitterOAuthView instance.
     * <li>Wait for the user to finish the authorization process at
     *     Twitter's authorization site. This TwitterOAuthView
     *     instance is redirected to the callback URL as a result.
     * <li>Detect the redirection to the callback URL and retrieve
     *     the value of the oauth_verifier parameter from the URL.
     *     If and only if dummyCallbackUrl is false, the callback
     *     URL is actually accessed.
     * <li>Get an access token using the oauth_verifier.
     * <li>Call {@link Listener#onSuccess(TwitterOAuthView, AccessToken)
     *     onSuccess()} method of the {@link Listener listener} on the
     *     UI thread.
     * </ol>
     *
     * <p>
     * If an error occurred during the above steps, {@link
     * Listener#onFailure(TwitterOAuthView, TwitterOAuthView.Result)
     * onFailure()} of the {@link Listener listener} is called.
     * </p>
     *
     * @param consumerKey
     * @param consumerSecret
     * @param callbackUrl
     * @param dummyCallbackUrl
     * @param listener
     *
     * @throws IllegalArgumentException
     *         At least one of 'consumerKey', 'consumerSecret' or
     *         'callbackUrl' is null.
     */
    public void start(String consumerKey, String consumerSecret, String callbackUrl, boolean dummyCallbackUrl,
            Listener listener)
    {
        if (consumerKey == null || consumerSecret == null || callbackUrl == null || listener == null)
        {
            throw new IllegalArgumentException();
        }

        Boolean dummy = Boolean.valueOf(dummyCallbackUrl);

        new TwitterOAuthTask().execute(consumerKey, consumerSecret, callbackUrl, dummy, listener);
    }


    private class TwitterOAuthTask extends AsyncTask<Object, Void, Result>
    {
        private String callbackUrl;
        private boolean dummyCallbackUrl;
        private Listener listener;
        private Twitter twitter;
        private RequestToken requestToken;
        private volatile boolean authorizationDone;
        private volatile String verifier;
        private AccessToken accessToken;


        @Override
        protected void onPreExecute()
        {
            // Set up a WebViewClient on the UI thread.
            TwitterOAuthView.this.setWebViewClient(new LocalWebViewClient());
        }


        @Override
        protected Result doInBackground(Object... args)
        {
            String consumerKey = (String)args[0];
            String consumerSecret = (String)args[1];

            // Callback URL.
            callbackUrl = (String)args[2];
            dummyCallbackUrl = (Boolean)args[3];

            // Listener
            listener = (Listener)args[4];

            if (DEBUG)
            {
                Log.d(TAG, "CONSUMER KEY = " + consumerKey);
                Log.d(TAG, "CONSUMER SECRET = " + consumerSecret);
                Log.d(TAG, "CALLBACK URL = " + callbackUrl);
                Log.d(TAG, "DUMMY CALLBACK URL = " + dummyCallbackUrl);
            }

            if (DEBUG)
                System.setProperty("twitter4j.debug", "true");

            // Create a Twitter instance with the given pair of
            // consumer key and consumer secret.
            twitter = new TwitterFactory().getInstance();
            twitter.setOAuthConsumer(consumerKey, consumerSecret);

            // Get a request token. This triggers network access.
            requestToken = getRequestToken();
            if (requestToken == null)
            {
                // Failed to get a request token.
                return Result.REQUEST_TOKEN_ERROR;
            }

            // Access Twitter's authorization page. After the user's
            // operation, this web view is redirected to the callback
            // URL, which is caught by shouldOverrideUrlLoading() of
            // LocalWebViewClient.
            authorize();

            // Wait until the authorization step is done.
            waitForAuthorization();

            // If the authorization has succeeded, 'verifier' is not null.
            if (verifier == null)
            {
                // The authorization failed.
                return Result.AUTHORIZATION_ERROR;
            }

            // The authorization succeeded. The last step is to get
            // an access token using the verifier.
            accessToken = getAccessToken();
            if (accessToken == null)
            {
                // Failed to get an access token.
                return Result.ACCESS_TOKEN_ERROR;
            }

            // All the steps were done successfully.
            return Result.SUCCESS;
        }


        @Override
        protected void onProgressUpdate(Void... values)
        {
            // In this implementation, onProgressUpdate() is called
            // only from authorize().

            // The authorization URL.
            String url = requestToken.getAuthorizationURL();

            if (DEBUG)
                Log.d(TAG, "Loading the authorization URL: " + url);

            // Load the authorization URL on the UI thread.
            TwitterOAuthView.this.loadUrl(url);
        }


        @Override
        protected void onPostExecute(Result result)
        {
            if (DEBUG)
                Log.d(TAG, "onPostExecute: result = " + result);

            if (result == null)
            {
                // Probably cancelled.
                result = Result.CANCELLATION;
            }

            if (result == Result.SUCCESS)
            {
                // Call onSuccess() method of the listener.
                listener.onSuccess(TwitterOAuthView.this, accessToken);
            }
            else
            {
                // Call onFailure() method of the listener.
                listener.onFailure(TwitterOAuthView.this, result);
            }
        }


        private RequestToken getRequestToken()
        {
            try
            {
                // Get a request token. This triggers network access.
                RequestToken token = twitter.getOAuthRequestToken();

                if (DEBUG)
                    Log.d(TAG, "Got a request token.");

                return token;
            }
            catch (TwitterException e)
            {
                // Failed to get a request token.
                e.printStackTrace();
                Log.e(TAG, "Failed to get a request token.", e);

                // No request token.
                return null;
            }
        }


        private void authorize()
        {
            // WebView.loadUrl() needs to be called on the UI thread,
            // so trigger onProgressUpdate().
            publishProgress();
        }


        private void waitForAuthorization()
        {
            while (authorizationDone == false)
            {
                synchronized (this)
                {
                    try
                    {
                        if (DEBUG)
                            Log.d(TAG, "Waiting for the authorization step to be done.");

                        this.wait();
                    }
                    catch (InterruptedException e)
                    {
                    }
                }
            }

            if (DEBUG)
                Log.d(TAG, "Finished waiting for the authorization step to be done.");
        }


        private void notifyAuthorization()
        {
            // The authorization step was done.
            authorizationDone = true;

            synchronized (this)
            {
                if (DEBUG)
                    Log.d(TAG, "Notifying that the authorization step was done.");

                this.notify();
            }
        }


        private class LocalWebViewClient extends WebViewClient
        {

            @Override
            public void onReceivedError(WebView view, int errorCode, String description, String failingUrl)
            {
                // Something wrong happened during the authorization step.
                Log.e(TAG, "onReceivedError: [" + errorCode + "] " + description);

                // Stop the authorization step.
                notifyAuthorization();
            }


            @Override
            public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error)
            {
                handler.proceed();
            }


            @Override
            public void onPageStarted(WebView view, String url, Bitmap favicon)
            {
                super.onPageStarted(view, url, favicon);

                // 11 = Build.VERSION_CODES.HONEYCOMB (Android 3.0)
                if (Build.VERSION.SDK_INT < 11)
                {
                    // According to this page:
                    //
                    // http://www.catchingtales.com/android-webview-shouldoverrideurlloading-and-redirect/416/
                    //
                    // shouldOverrideUrlLoading() is not called for redirects on
                    // Android earlier than 3.0, so call the method manually.
                    //
                    // The implementation of shouldOverrideUrlLoading() returns
                    // true only when the URL starts with the callback URL and
                    // dummyCallbackUrl is true.
                    boolean stop = shouldOverrideUrlLoading(view, url);

                    if (stop)
                    {
                        // Stop loading the current page.
                        stopLoading();
                    }
                }
            }


            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                // Check if the given URL is the callback URL.
                if (url.startsWith(callbackUrl) == false)
                {
                    // The URL is not the callback URL.
                    return false;
                }

                // This web view is about to be redirected to the callback URL.
                if (DEBUG)
                    Log.d(TAG, "Detected the callback URL: " + url);

                // Convert String to Uri.
                Uri uri = Uri.parse(url);

                // Get the value of the query parameter "oauth_verifier".
                // A successful response should contain the parameter.
                verifier = uri.getQueryParameter("oauth_verifier");

                if (DEBUG)
                    Log.d(TAG, "oauth_verifier = " + verifier);
                
                // Notify that the the authorization step was done.
                notifyAuthorization();

                // Whether the callback URL is actually accessed or not
                // depends on the value of dummyCallbackUrl. If the
                // value of dummyCallbackUrl is true, the callback URL
                // is not accessed.
                return dummyCallbackUrl;
            }
        }


        private AccessToken getAccessToken()
        {
            try
            {
                // Get an access token. This triggers network access.
                AccessToken token = twitter.getOAuthAccessToken(requestToken, verifier);

                if (DEBUG)
                    Log.d(TAG, "Got an access token for " + token.getScreenName());

                return token;
            }
            catch (TwitterException e)
            {
                // Failed to get an access token.
                e.printStackTrace();
                Log.e(TAG, "Failed to get an access token.", e);

                // No access token.
                return null;
            }
        }
    }
}
