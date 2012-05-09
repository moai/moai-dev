// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup.LayoutParams;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;

public class TapjoyFeaturedAppWebView extends Activity
{
	private WebView webView = null;
	private ProgressBar progressBar;

//	private String clientPackage = "";													// Package of the application using TapjoyConnect
	private String fullScreenAdURL = "";
	private String userID = "";
	private String urlParams = "";
	
	final String TAPJOY_FEATURED_APP = "Featured App";
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{	
		// Get the full screen ad URL from the extras.
		Bundle extras = getIntent().getExtras();
		
		userID = extras.getString(TapjoyConstants.EXTRA_USER_ID);
//		clientPackage = TapjoyConnectCore.getClientPackage();
		urlParams = extras.getString(TapjoyConstants.EXTRA_URL_PARAMS);
		fullScreenAdURL = extras.getString(TapjoyConstants.EXTRA_FEATURED_APP_FULLSCREEN_AD_URL);
		fullScreenAdURL = fullScreenAdURL.replaceAll(" ", "%20");
		
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		//setContentView( getApplicationContext().getResources().getIdentifier("tapjoy_featured_app_web_view", "layout", clientPackage));
		
		RelativeLayout layout = new RelativeLayout(this);
		
		// Load and display the Offers WebView.
		//webView = (WebView) findViewById(this.getResources().getIdentifier("offersWebView", "id", clientPackage));
		webView = new WebView(this);
		webView.setWebViewClient(new TapjoyWebViewClient());
		
		WebSettings webSettings = webView.getSettings();
		webSettings.setJavaScriptEnabled(true);
		
		// Create our progress spinner.
		//progressBar = (ProgressBar)findViewById(getResources().getIdentifier("OfferProgressBar", "id", clientPackage));
		progressBar = new ProgressBar(this, null, android.R.attr.progressBarStyleLarge);
		progressBar.setVisibility(View.VISIBLE);
		
		// Center progress bar.
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);
        progressBar.setLayoutParams(layoutParams);
        
        // Add relative layout to our content view.
        layout.addView(webView, LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);		
		layout.addView(progressBar);
		setContentView(layout);
		
		//TapjoyLog.i(TAPJOY_OFFERS, "load web url: " + System.currentTimeMillis());
		
		webView.loadUrl(fullScreenAdURL);
		
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Opening Full Screen AD URL = [" + fullScreenAdURL + "]");
	}
	

	/**
	 * Handles screen rotation.
	 */
	public void onConfigurationChanged (Configuration newConfig)
	{		
		super.onConfigurationChanged(newConfig);
		
		if (webView != null)
		{
			// Calling javascript "window.onorientationchange();" does not work here.
			// Delaying and then calling it does work so launch a task to do this for us.
			RefreshTask refreshTask = new RefreshTask();
			refreshTask.execute();
		}	
	}

	
	/**
	 * 
	 * This task will delay for a brief amount of time and then call
	 * javascript function "window.onorientationchange();" because
	 * calling this within the onConfigurationChanged method does
	 * not work correctly.
	 *
	 */
	private class RefreshTask extends AsyncTask<Void, Void, Boolean>
	{
		@Override
		protected Boolean doInBackground(Void... params)
		{
			try
			{
				Thread.sleep(200);
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
			
			return true;
		}
		
		protected void onPostExecute(Boolean result)
		{
			if (webView != null)
			{
				// This javascript function will resize the page based on the window dimensions.
				webView.loadUrl("javascript:window.onorientationchange();");
			}
		}
	}
	
	
	/**
	 * This class handle all events of tapjoy customized view, like to open link, test and verify url then load it in browser or market depends upon the url
	 */
	private class TapjoyWebViewClient extends WebViewClient
	{
		public void onPageStarted(WebView view, String url, Bitmap favicon)
		{
			progressBar.setVisibility(View.VISIBLE);
			progressBar.bringToFront();
		}
		
		public void onPageFinished(WebView view, String url)
		{
			// Dismiss the progress wheel once the page is done loading.
			progressBar.setVisibility(View.GONE);
		}
		
		/**
		 * When any user hits any url from tapjoy custom view then this function is called before opening any user.
		 */
		public boolean shouldOverrideUrlLoading(WebView view, String url)
		{
			TapjoyLog.i(TAPJOY_FEATURED_APP, "URL = ["+url+"]");
			
			// Show the offer wall.
			if (url.contains("showOffers"))
			{
				TapjoyLog.i(TAPJOY_FEATURED_APP, "show offers");
				showOffers();
			}
			else
			// This is the "SKIP" button, terminate this activity.
			if (url.contains("dismiss"))
			{
				TapjoyLog.i(TAPJOY_FEATURED_APP, "dismiss");
				finishActivity();
			}
//			else
//			if (url.indexOf("market") > -1)
//			{
//				TapjoyLog.i(TAPJOY_FEATURED_APP, "Market URL = ["+url+"]");
//				
//				try
//				{
//					String str[] = url.split("q=");
//					String composeedURL = "http://market.android.com/details?id="+str[1]+"&referrer="+clientPackage;
//					Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(composeedURL));
//					startActivity(intent);
//					TapjoyLog.i(TAPJOY_FEATURED_APP, "Open URL of application = ["+composeedURL+"]");
//				}
//				catch(Exception e)
//				{
//					TapjoyLog.i(TAPJOY_FEATURED_APP, "Android market is unavailable at this device. To view this link install market.");
//				}
//			}
			else
			// Keep redirecting URLs in the webview.
			if (url.contains(TapjoyConstants.TJC_BASE_REDIRECT_DOMAIN))
			{
				TapjoyLog.i(TAPJOY_FEATURED_APP, "Open redirecting URL = ["+url+"]");
				view.loadUrl(url);
			}
			else
			// Open all other items in a new window.
			{
				TapjoyLog.i(TAPJOY_FEATURED_APP, "Opening URL in new browser = ["+url+"]");
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				startActivity(intent);
			}
			
			return true;
		}
	}
	
	
	/**
	 * Allows access to the webview to show the offer wall (when Show More Apps is pressed on the featured app page).
	 */
	private void showOffers()
	{
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Showing offers, userID: " + userID);
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Showing offers, URL PARAMS: " + urlParams);

		Intent offersIntent = new Intent(this, com.tapjoy.TJCOffersWebView.class);
		offersIntent.putExtra(TapjoyConstants.EXTRA_USER_ID, userID);
		offersIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, urlParams);
		this.startActivity(offersIntent);
	}
	
	
	/**
	 * Allows access to the webview to close this activity (when SKIP is pressed on featured app page).
	 */
	private void finishActivity()
	{
		this.finish();
	}
	
	
}
