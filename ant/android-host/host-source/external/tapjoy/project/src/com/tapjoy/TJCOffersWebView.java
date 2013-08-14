// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.util.Hashtable;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;

/**
 * This is customized Tapjoy web view. It initially shows list of applications that user can download to earn points.
 */
public class TJCOffersWebView extends Activity
{
	private WebView webView = null;
	private String offersURL = null;													// URL to load the offer wall.

	private ProgressBar progressBar;
	private Dialog dialog = null;

	private String clientPackage = "";													// Package of the application using TapjoyConnect
	private String urlParams = "";														// URL parameters.
	private String userID = "";															// Publisher defined userID.

	final String TAPJOY_OFFERS = "Offers";
	
	private boolean skipOfferWall = false;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// Get URL parameters from the extras.
		Bundle extras = getIntent().getExtras();
		
		// Read the extras data if it's available.
		if (extras != null)
		{
			// Is this a display ad URL?
			if (extras.getString(TapjoyConstants.EXTRA_DISPLAY_AD_URL) != null)
			{
				skipOfferWall = true;
				offersURL = extras.getString(TapjoyConstants.EXTRA_DISPLAY_AD_URL);
			}
			// Normal offers URL
			else
			{
				skipOfferWall = false;
				
				urlParams = extras.getString(TapjoyConstants.EXTRA_URL_PARAMS);
				userID = extras.getString(TapjoyConstants.EXTRA_USER_ID);
				
				// Generate the url parameters, then append the publisher ID.
				urlParams += "&" + TapjoyConstants.TJC_USER_ID + "=" + userID;
				
				if (TapjoyConnectCore.getVideoIDs().length() > 0)
				{
					urlParams += "&" + TapjoyConstants.TJC_VIDEO_OFFER_IDS + "=" + TapjoyConnectCore.getVideoIDs();
					
					TapjoyLog.i(TAPJOY_OFFERS, "getVideoIDs: [" + TapjoyConnectCore.getVideoIDs() + "]");
				}
				
				TapjoyLog.i(TAPJOY_OFFERS, "urlParams: [" + urlParams + "]");
				
				// Build the URL with parameters.
				offersURL = TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_SHOW_OFFERS_URL_PATH + urlParams;
			}	
		}
		else
		{
			TapjoyLog.e(TAPJOY_OFFERS, "Tapjoy offers meta data initialization fail.");
		}
		
		offersURL = offersURL.replaceAll(" ", "%20");
		
		// Get client package.
		clientPackage = TapjoyConnectCore.getClientPackage();
		
		TapjoyLog.i(TAPJOY_OFFERS, "clientPackage: [" + clientPackage + "]");
		
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		//setContentView( this.getResources().getIdentifier("tapjoy_offers_web_view", "layout", clientPackage));
		
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
		
		webView.loadUrl(offersURL);
		
		TapjoyLog.i(TAPJOY_OFFERS, "Opening URL = [" + offersURL + "]");
	}

	
	@Override
	protected void onResume()
	{
		//TapjoyLog.i(TAPJOY_OFFERS, "onResume -- refreshing offers");
		
		super.onResume();
		
		// Refresh the offers page anytime we regain focus.
		if (offersURL != null && webView != null)
			webView.loadUrl(offersURL);
	}
	
	
	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		
		if (webView != null)
		{
			// Potential issue on Android v1.6 where the webView resources do not
			// automatically free themselves and can cause a signal 11 (SIGSEGV) crash error.
			webView.clearCache(true);
			webView.destroyDrawingCache();
			webView.destroy();
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
			//TapjoyLog.i(TAPJOY_OFFERS, "onPageFinished : " + System.currentTimeMillis());
			
			// Dismiss the progress wheel once the page is done loading.
			progressBar.setVisibility(View.GONE);
		}
		
		/**
		 * When any user hits any url from tapjoy custom view then this function is called before opening any user.
		 */
		public boolean shouldOverrideUrlLoading(WebView view, String url)
		{
			TapjoyLog.i(TAPJOY_OFFERS, "URL = ["+url+"]");
			
//			if (url.indexOf("market") > -1)
//			{
//				TapjoyLog.i(TAPJOY_OFFERS, "Market URL = ["+url+"]");
//				
//				try
//				{
//					// Format is: "market://search?q=com.package.app"));
//					String str[] = url.split("q=");
//					String composeedURL = "http://market.android.com/details?id="+str[1]+"&referrer="+clientPackage;
//					Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(composeedURL));
//					startActivity(intent);
//					TapjoyLog.i(TAPJOY_OFFERS, "Open URL of application = ["+composeedURL+"]");
//				}
//				catch(Exception e)
//				{
//
//					dialog = new AlertDialog.Builder(TJCOffersWebView.this).setTitle("").setMessage("Android market is unavailable at this device. To view this link install market.").setPositiveButton("OK", new DialogInterface.OnClickListener()
//					{
//						public void onClick(DialogInterface dialog, int whichButton)
//						{
//							dialog.dismiss();
//						}
//					}).create();
//					
//					try
//					{
//						dialog.show();
//					}
//					catch(Exception exp)
//					{
//						exp.printStackTrace();
//					}
//
//					TapjoyLog.i(TAPJOY_OFFERS, "Android market is unavailable at this device. To view this link install market.");
//				}
//				
//				TapjoyLog.i(TAPJOY_OFFERS, "skipOfferWall: " + skipOfferWall);
//				
//				// Close this webview if we're not displaying the offerwall so when 
//				// the market activity backs, it won't display a blank or redirecting webview
//				if (skipOfferWall)
//				{
//					finish();
//				}
//			}
//			else
			// Video Offer
			if (url.startsWith("tjvideo://"))
			{
				char c;
				int index = 0;
				
				String videoID;
				String currencyAmount;
				String currencyName;
				String clickURL;
				
				Hashtable <String, String> params = new Hashtable <String, String>();
				
				index = url.indexOf("://") + "://".length();
				
				final int KEY = 0;
				final int VALUE = 1;
				int mode = KEY;
				
				String word = "";
				String key = "";
				String value = "";
				
				while (index < url.length() && index != -1)
				{
					c = url.charAt(index);
					if (mode == KEY)
					{
						if (c == '=')
						{
							mode = VALUE;
							key = Uri.decode(word);
							word = "";
						}
						else
						{
							word += c;
						}
					}
					else
					if (mode == VALUE)
					{
						if (c == '&')
						{
							mode = KEY;
							value = Uri.decode(word);
							word = "";
							
							TapjoyLog.i(TAPJOY_OFFERS, "k:v: " + key + ", " + value);
							params.put(key, value);
						}
						else
						{
							word += c;
						}
					}
					
					index++;
				}
				
				if (mode == VALUE && word.length() > 0)
				{
					value = Uri.decode(word);
					TapjoyLog.i(TAPJOY_OFFERS, "k:v: " + key + ", " + value);
					params.put(key, value);
				}
				
				videoID = params.get("video_id");
				currencyAmount = params.get("amount");
				currencyName = params.get("currency_name");
				clickURL = params.get("click_url");
				
				TapjoyLog.i(TAPJOY_OFFERS, "videoID: " + videoID);
				TapjoyLog.i(TAPJOY_OFFERS, "currencyAmount: " + currencyAmount);
				TapjoyLog.i(TAPJOY_OFFERS, "currencyName: " + currencyName);
				TapjoyLog.i(TAPJOY_OFFERS, "clickURL: " + clickURL);
				
				if (TapjoyVideo.getInstance().startVideo(videoID, currencyName, currencyAmount, clickURL))
				{
					TapjoyLog.i(TAPJOY_OFFERS, "VIDEO");
				}
				// Unable to start video.
				else
				{
					TapjoyLog.e(TAPJOY_OFFERS, "Unable to play video: " + videoID);
					dialog = new AlertDialog.Builder(TJCOffersWebView.this).setTitle("").setMessage("Unable to play video.").setPositiveButton("OK", new DialogInterface.OnClickListener()
					{
						public void onClick(DialogInterface dialog, int whichButton)
						{
							dialog.dismiss();
						}
					}).create();
					
					try
					{
						dialog.show();
					}
					catch (Exception e)
					{
						TapjoyLog.e(TAPJOY_OFFERS, "e: " + e.toString());
					}
				}
			}
			else
			// Keep redirecting URLs in the webview.
			if (url.contains(TapjoyConstants.TJC_BASE_REDIRECT_DOMAIN))
			{
				TapjoyLog.i(TAPJOY_OFFERS, "Open redirecting URL = ["+url+"]");
				view.loadUrl(url);
			}
			else
			// Open all other items in a new window.
			{
				TapjoyLog.i(TAPJOY_OFFERS, "Opening URL in new browser = ["+url+"]");
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				startActivity(intent);
				
				TapjoyLog.i(TAPJOY_OFFERS, "skipOfferWall: " + skipOfferWall);
				
				// Close this webview if we're not displaying the offerwall so when 
				// the market activity backs, it won't display a blank or redirecting webview
				if (skipOfferWall)
				{
					finish();
				}
			}

			return true;
		}
	}

	/**
	 * This handle hard back key at Android device.
	 */
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if ((keyCode == KeyEvent.KEYCODE_BACK) && webView.canGoBack())
		{
			webView.goBack();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}
}