// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
//import android.os.AsyncTask;

public class TapjoyFeaturedApp
{
	private TapjoyFeaturedAppObject featuredAppObject = null;
	private static TapjoyFeaturedAppNotifier featuredAppNotifier;
	private static TapjoyURLConnection tapjoyURLConnection = null;
	private Context context;
	
	public static String featuredAppURLParams;
	private String currencyID;
	private int displayCount = 5;											// By default, each featured app is only displayed five times before it no longer being displayed.
	
	final String TAPJOY_FEATURED_APP = "Featured App";
	
	
	/**
	 * Constructor.
	 */
	public TapjoyFeaturedApp(Context ctx)
	{	
		context = ctx;
		tapjoyURLConnection = new TapjoyURLConnection();
	}
	
	
	/**
	 * Connects to the server and to get featured app data.
	 * @param notifier				Class which receives callback with Featured App data.
	 */
	public void getFeaturedApp(TapjoyFeaturedAppNotifier notifier)
	{
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Getting Featured App");
		
		getFeaturedApp(null, notifier);
	}
	
	
	/**
	 * Connects to the server and to get featured app data.
	 * @param currencyID			ID of the currency to display.
	 * @param notifier				Class which receives callback with Featured App data.
	 */
	public void getFeaturedApp(String theCurrencyID, TapjoyFeaturedAppNotifier notifier)
	{
		// Store the currency ID.  We need to use use this if the offer wall is launched from the webView.
		currencyID = theCurrencyID;
		
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Getting Featured App userID: " + TapjoyConnectCore.getUserID() + ", currencyID: " + currencyID);
		
		featuredAppNotifier = notifier;
		featuredAppObject = new TapjoyFeaturedAppObject();
		
		// Add the user ID to our request.
		featuredAppURLParams = TapjoyConnectCore.getURLParams();
		featuredAppURLParams += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
		
		// Add the currency ID if it was specified.
		if (currencyID != null)
			featuredAppURLParams += "&" + TapjoyConstants.TJC_CURRENCY_ID + "=" + currencyID;
		
		new Thread(new Runnable() 
		{
			public void run()
			{
				boolean returnValue = false;
				
				String response = tapjoyURLConnection.connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_FEATURED_APP_URL_PATH, TapjoyFeaturedApp.featuredAppURLParams);
				
				if (response != null)
				{
					returnValue = buildResponse(response);
				}
				
				if (returnValue == false)
					featuredAppNotifier.getFeaturedAppResponseFailed("Error retrieving featured app data from the server.");
			}
		}).start();
	}
	
	
	/**
	 * Displays the Featured App fullscreen ad.
	 * Should be called after getFeaturedApp() and after receiving the TapjoyFeaturedAppNotifier.getFeaturedAppResponse callback. 
	 */
	public void showFeaturedAppFullScreenAd()
	{
		String fullscreenURL = "";
		
		if (featuredAppObject != null)
			fullscreenURL = featuredAppObject.fullScreenAdURL;
		
		TapjoyLog.i(TAPJOY_FEATURED_APP, "Displaying Full Screen AD with URL: " + fullscreenURL);
		
		if (fullscreenURL.length() != 0)
		{
			String urlParams = TapjoyConnectCore.getURLParams();
			
			// Add the currencyID so the correct currency appears on the offer wall.
			if (currencyID != null && currencyID.length() > 0)
				urlParams += "&" + TapjoyConstants.TJC_CURRENCY_ID + "=" + currencyID;
			
			Intent featuredAppIntent = new Intent(context, com.tapjoy.TapjoyFeaturedAppWebView.class);
			featuredAppIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			featuredAppIntent.putExtra(TapjoyConstants.EXTRA_USER_ID, TapjoyConnectCore.getUserID());
			featuredAppIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, urlParams);
			featuredAppIntent.putExtra(TapjoyConstants.EXTRA_FEATURED_APP_FULLSCREEN_AD_URL, fullscreenURL);
			context.startActivity(featuredAppIntent);
		}
	}
	
	
	/**
	 * Sets the maximum number of times a featured app ad should appear.
	 * @param count					Number of times an ad should appear.
	 */
	public void setDisplayCount(int count)
	{	
		displayCount = count;
	}

	
	/**
	 * Parses the response from the server and determines whether the
	 * connection was successful.
	 * @param response			HTTP Response from the server.
	 * @return 					TRUE if the response has been handled and notified, FALSE otherwise.
	 */
	private boolean buildResponse(String response)
	{
		boolean retValue = false;
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder documentBuilder;
		
		try 
		{
			// Convert the string to an input stream.
			InputStream is = new ByteArrayInputStream(response.getBytes("UTF-8"));
			
			documentBuilder = factory.newDocumentBuilder();
			Document document = documentBuilder.parse(is);
			
			featuredAppObject.cost 				= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Cost"));
			
			String amount = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Amount"));
			
			if (amount != null)
				featuredAppObject.amount 		= Integer.parseInt(amount);
			
			featuredAppObject.description 		= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Description"));
			featuredAppObject.iconURL 			= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("IconURL"));
			featuredAppObject.name 				= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Name"));
			featuredAppObject.redirectURL 		= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("RedirectURL"));
			featuredAppObject.storeID 			= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("StoreID"));
			featuredAppObject.fullScreenAdURL 	= TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("FullScreenAdURL"));
			
			TapjoyLog.i(TAPJOY_FEATURED_APP, "cost: " + featuredAppObject.cost);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "amount: " + featuredAppObject.amount);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "description: " + featuredAppObject.description);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "iconURL: " + featuredAppObject.iconURL);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "name: " + featuredAppObject.name);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "redirectURL: " + featuredAppObject.redirectURL);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "storeID: " + featuredAppObject.storeID);
			TapjoyLog.i(TAPJOY_FEATURED_APP, "fullScreenAdURL: " + featuredAppObject.fullScreenAdURL);
			
			// Invalid data.
			if (featuredAppObject.fullScreenAdURL == null || featuredAppObject.fullScreenAdURL.length() == 0)
				retValue = false;
			else
				retValue = true;
		}
		catch (Exception e) 
		{
			TapjoyLog.e(TAPJOY_FEATURED_APP, "Error parsing XML: " + e.toString());
		}

		// Successfully parsed XML.
		if (retValue)
		{
			// Are we under the display count?
			if (getDisplayCountForStoreID(featuredAppObject.storeID) < displayCount)
			{
				// Send the Featured App data to the callback and increment the count for this ID.
				featuredAppNotifier.getFeaturedAppResponse(featuredAppObject);
				
				// Only increment if the store ID does not match this appID.
				// If they match, then this is a test offer so let that display unlimited.
				if (TapjoyConnectCore.getAppID().equals(featuredAppObject.storeID) == false)
					incrementDisplayCountForStoreID(featuredAppObject.storeID);
			}
			else
			{
				featuredAppNotifier.getFeaturedAppResponseFailed("Featured App to display has exceeded display count");
			}
		}
		else
		{
			featuredAppNotifier.getFeaturedAppResponseFailed("Failed to parse XML file from response");
			retValue = true;
		}
		
		return retValue;
	}
	
	
	/**
	 * Returns the number of times an app has been displayed as a featured app ad.
	 * @param storeID			ID of the app to check.
	 * @return					Number of times the specified app has been displayed.
	 */
	private int getDisplayCountForStoreID(String storeID)
	{
		// Restore preferences
		SharedPreferences settings = context.getSharedPreferences(TapjoyConstants.TJC_FEATURED_APP_PREFERENCE, 0);
		int count = settings.getInt(storeID, 0);
		
		TapjoyLog.i(TAPJOY_FEATURED_APP, "getDisplayCount: " + count + ", storeID: " + storeID);
		
		return count;
	}
	
	
	/**
	 * Increments the number of times an app has been displayed as a featured app ad.
	 * @param storeID			ID of the app to check.
	 */
	private void incrementDisplayCountForStoreID(String storeID)
	{
		SharedPreferences settings = context.getSharedPreferences(TapjoyConstants.TJC_FEATURED_APP_PREFERENCE, 0);
		SharedPreferences.Editor editor = settings.edit();
		
		int count = settings.getInt(storeID, 0);
		count++;
		
		TapjoyLog.i(TAPJOY_FEATURED_APP, "incrementDisplayCount: " + count + ", storeID: " + storeID);
		
		editor.putInt(storeID, count);		
		editor.commit();
	}
	
	
	/**
	 * Returns the Featured App Object containing Featured App data.
	 * @return					Featured App Object
	 */
	public TapjoyFeaturedAppObject getFeaturedAppObject()
	{
		return featuredAppObject;
	}
	
}
