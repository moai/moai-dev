// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.util.UUID;
import org.w3c.dom.Document;
import android.content.Context;
import android.content.Intent;

/**
 * 
 * Class for displaying offers and getting/spending Tap Points (virtual currency).
 *
 */
public class TJCOffers
{
	private static TapjoyNotifier tapjoyNotifier;	
	private static TapjoySpendPointsNotifier tapjoySpendPointsNotifier;
	private static TapjoyAwardPointsNotifier tapjoyAwardPointsNotifier;
	private static TapjoyEarnedPointsNotifier tapjoyEarnedPointsNotifier;

	String spendTapPoints = null;													// Amount of virtual currency to spend.
	int awardTapPoints = 0;															// Amount of virtual currency to award.
	
	Context context;
	
	// For multiple currencies.
	private String multipleCurrencyID = "";
	private String multipleCurrencySelector = "";
	
	public static final String TAPJOY_OFFERS 										= "TapjoyOffers";
	public static final String TAPJOY_POINTS 										= "TapjoyPoints";

	
	/**
	 * Constructor.
	 * @param urlparams						URL parameters.
	 * @param userid						User ID.
	 */
	public TJCOffers(Context applicationContext)
	{
		context = applicationContext;
	}
	
	
	//================================================================================
	// OFFERS Methods
	//================================================================================
	/**
	 * Show available offers using a default userID of the device's unique identifier (IMEI/MEID).
	 */
	public void showOffers()
	{
		TapjoyLog.i(TAPJOY_OFFERS, "Showing offers with userID: " + TapjoyConnectCore.getUserID());
		
		Intent offersIntent = new Intent(context, com.tapjoy.TJCOffersWebView.class);
		offersIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		offersIntent.putExtra(TapjoyConstants.EXTRA_USER_ID, TapjoyConnectCore.getUserID());
		offersIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, TapjoyConnectCore.getURLParams());
		context.startActivity(offersIntent);
	}
	
	
	/**
	 * Show available offers using publisher provided userID, currencyID and currency selector flag.  
	 * This method should only be used if the application supports multiple currencies.
	 * Apps that support multiple currencies cannot be managed by Tapjoy.
	 * @param currencyID				ID of the currency to display.
	 * @param enableCurrencySelector	Whether to display the currency selector to toggle currency.
	 */
	public void showOffersWithCurrencyID(String currencyID, boolean enableCurrencySelector)
	{
		TapjoyLog.i(TAPJOY_OFFERS, "Showing offers with currencyID: " + currencyID + ", selector: " + enableCurrencySelector + " (userID = " + TapjoyConnectCore.getUserID() + ")");
		
		multipleCurrencyID = currencyID;
		multipleCurrencySelector = (enableCurrencySelector ? "1" : "0");
		
		// Add the currency related parameters to our URL parameters.
		String offersURLParams = TapjoyConnectCore.getURLParams();
		offersURLParams += "&" + TapjoyConstants.TJC_CURRENCY_ID + "=" + multipleCurrencyID;
		offersURLParams += "&" + TapjoyConstants.TJC_MULTIPLE_CURRENCY_SELECTOR_FLAG + "=" + multipleCurrencySelector;
		
		Intent offersIntent = new Intent(context, com.tapjoy.TJCOffersWebView.class);
		offersIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		offersIntent.putExtra(TapjoyConstants.EXTRA_USER_ID, TapjoyConnectCore.getUserID());
		offersIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, offersURLParams);
		context.startActivity(offersIntent);
	}
	
	
	/**
	 * Gets the virtual currency data from the server for this device.
	 * The data will be returned in a callback to updatePoints() to the class implementing the notifier.
	 * @param notifier The class implementing the TapjoyNotifier callback.
	 */
	public void getTapPoints(TapjoyNotifier notifier)
	{
		tapjoyNotifier = notifier;
		
		new Thread(new Runnable() 
		{
			public void run() 
			{
				boolean returnValue = false;
		
				String url_params = TapjoyConnectCore.getURLParams();
				url_params += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
				
				// Get the result of the http request.
				String result = new TapjoyURLConnection().connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_USERDATA_URL_PATH, url_params);
				
				// Handle the response for a connect call.
				if (result != null)
				{
					returnValue = handleGetPointsResponse(result);
				}
				
				// Notify of a failure.
				if (returnValue == false)
					tapjoyNotifier.getUpdatePointsFailed("Failed to retrieve points from server");
			}
		}).start();
	}
	

	/**
	 * Spends virtual currency.  This can only be used for currency managed by Tapjoy.
	 * The data will be returned in a callback to updatePoints() to the class implementing the notifier.
	 * @param notifier The class implementing the TapjoySpendPointsNotifier callback.
	 */
	public void spendTapPoints(int amount, TapjoySpendPointsNotifier notifier)
	{
		if (amount < 0)
		{
			TapjoyLog.e(TAPJOY_POINTS, "spendTapPoints error: amount must be a positive number");
			return;
		}
		
		spendTapPoints = "" + amount;
		
		tapjoySpendPointsNotifier = notifier;

		new Thread(new Runnable() 
		{
			public void run() 
			{
				boolean returnValue = false;

				// Get the result of the http request.
				String url_params = TapjoyConnectCore.getURLParams();
				url_params += "&" + TapjoyConstants.TJC_TAP_POINTS + "=" + spendTapPoints;
				url_params += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
				String result = new TapjoyURLConnection().connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_SPEND_POINTS_URL_PATH, url_params);
				
				// Handle the response for a connect call.
				if (result != null)
				{
					returnValue = handleSpendPointsResponse(result);
				}
				
				// Notify of a failure.
				if (returnValue == false)
					tapjoySpendPointsNotifier.getSpendPointsResponseFailed("Failed to spend points.");
				
				//return returnValue;
			}
		}).start();
	}
	
	
	/**
	 * Spends virtual currency.  This can only be used for currency managed by Tapjoy.
	 * The data will be returned in a callback to updatePoints() to the class implementing the notifier.
	 * @param notifier The class implementing the TapjoyAwardPointsNotifier callback.
	 */
	public void awardTapPoints(int amount, TapjoyAwardPointsNotifier notifier)
	{
		if (amount < 0)
		{
			TapjoyLog.e(TAPJOY_POINTS, "spendTapPoints error: amount must be a positive number");
			return;
		}
		
		awardTapPoints = amount;
		
		tapjoyAwardPointsNotifier = notifier;

		new Thread(new Runnable() 
		{
			public void run() 
			{
				boolean returnValue = false;

				String guid = UUID.randomUUID().toString();
				long time = System.currentTimeMillis()/1000;
				
				
				// Get the result of the http request.
				String url_params = TapjoyConnectCore.getURLParams();
				url_params += "&" + TapjoyConstants.TJC_TAP_POINTS + "=" + awardTapPoints;
				url_params += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
				
				url_params += "&" + TapjoyConstants.TJC_GUID + "=" + guid;
				url_params += "&" + TapjoyConstants.TJC_TIMESTAMP + "=" + time;
				url_params += "&" + TapjoyConstants.TJC_VERIFIER + "=" + TapjoyConnectCore.getAwardPointsVerifier(time, awardTapPoints, guid);
				
				String result = new TapjoyURLConnection().connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_AWARD_POINTS_URL_PATH, url_params);
				
				// Handle the response for a connect call.
				if (result != null)
				{
					returnValue = handleAwardPointsResponse(result);
				}
				
				// Notify of a failure.
				if (returnValue == false)
					tapjoyAwardPointsNotifier.getAwardPointsResponseFailed("Failed to award points.");
			}
		}).start();
	}
	
	
	/**
	 * Sets the notifier which gets informed whenever virtual currency is earned.
	 * @param notifier						Class implementing TapjoyEarnedPointsNotifier.
	 */
	public void setEarnedPointsNotifier(TapjoyEarnedPointsNotifier notifier)
	{
		tapjoyEarnedPointsNotifier = notifier;
	}
	
	
	/**
	 * Handles the response from calling getTapPoints.
	 * @param response					HTTP response from the server.
	 * @return							TRUE if the response is handled and notifier has been called, FALSE otherwise.
	 */
	private synchronized boolean handleGetPointsResponse(String response)
	{
		// Create a document so we can parse the XML easily.
		Document document = TapjoyUtil.buildDocument(response);
		
		if (document != null)
		{
			String nodeValue = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Success"));
			
			// The request was successful.
			if (nodeValue != null && nodeValue.equals("true"))
			{
				
				// Check if we're getting tap points.
				String points = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("TapPoints"));
				String name = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("CurrencyName"));
				
				// Was there tap points/currency data in the response?
				if (points != null && name != null)
				{
					int pointTotal = Integer.parseInt(points);
					int lastLocalPointTotal = TapjoyConnectCore.getLocalTapPointsTotal();
					//currencyName = name;
					
					//TapjoyLog.i(TAPJOY_POINTS, "currencyName: " + currencyName);
					//TapjoyLog.i(TAPJOY_POINTS, "pointsTotal: " + pointsTotal);
					
					// If the notifier is valid, try to use it.
					if (tapjoyEarnedPointsNotifier != null)
					{
						// Is there valid data in the last saved local tap points?
						if (lastLocalPointTotal != -9999)
						{
							// Did we get more points than what we last recorded?  Then call the notifier that points have been earned.
							if (pointTotal > lastLocalPointTotal)
							{
								TapjoyLog.i(TAPJOY_POINTS, "earned: " + (pointTotal - lastLocalPointTotal));
								tapjoyEarnedPointsNotifier.earnedTapPoints(pointTotal - lastLocalPointTotal);
							}
						}
					}
					
					// Save the last known tap points total.
					TapjoyConnectCore.saveTapPointsTotal(Integer.parseInt(points));
					
					// Trigger the notifier to fire back.
					tapjoyNotifier.getUpdatePoints(name, Integer.parseInt(points));
					return true;
				}
				else
				{
					TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing tags.");
				}
			}
			else
			{
				TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing <Success> tag.");
			}
		}
		
		return false;
	}
	
	
	/**
	 * Handles the response from calling spendTapPoints.
	 * @param response					HTTP response from the server.
	 * @return							TRUE if the response is handled and notifier has been called, FALSE otherwise.
	 */
	private boolean handleSpendPointsResponse(String response)
	{
		String message = "";
		
		// Create a document so we can parse the XML easily.
		Document document = TapjoyUtil.buildDocument(response);
		
		if (document != null)
		{
			String nodeValue = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Success"));
			
			// The request was successful.
			if (nodeValue != null && nodeValue.equals("true"))
			{
				
				// Check if we're getting tap points.
				String pointsTotal = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("TapPoints"));
				String currencyName = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("CurrencyName"));
				
				// Was there tap points/currency data in the response?
				if (pointsTotal != null && currencyName != null)
				{
					//TapjoyLog.i(TAPJOY_POINTS, "currencyName: " + currencyName);
					//TapjoyLog.i(TAPJOY_POINTS, "pointsTotal: " + pointsTotal);
					
					// Save the last known tap points total.
					TapjoyConnectCore.saveTapPointsTotal(Integer.parseInt(pointsTotal));
					
					// Trigger the notifier to fire back.
					tapjoySpendPointsNotifier.getSpendPointsResponse(currencyName, Integer.parseInt(pointsTotal));
					return true;
				}
				else
				{
					TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing tags.");
				}
			}
			else
			// Error spending virtual currency.
			if (nodeValue != null && nodeValue.endsWith("false"))
			{
				message = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Message"));
				TapjoyLog.i(TAPJOY_POINTS, message);
				
				tapjoySpendPointsNotifier.getSpendPointsResponseFailed(message);
				return true;
			}
			else
			{
				TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing <Success> tag.");
			}
		}
		
		return false;
	}
	
	
	/**
	 * Handles the response from calling awardTapPoints.
	 * @param response					HTTP response from the server.
	 * @return							TRUE if the response is handled and notifier has been called, FALSE otherwise.
	 */
	private boolean handleAwardPointsResponse(String response)
	{
		String message = "";
		
		// Create a document so we can parse the XML easily.
		Document document = TapjoyUtil.buildDocument(response);
		
		if (document != null)
		{
			String nodeValue = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Success"));
			
			// The request was successful.
			if (nodeValue != null && nodeValue.equals("true"))
			{
				
				// Check if we're getting tap points.
				String pointsTotal = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("TapPoints"));
				String currencyName = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("CurrencyName"));
				
				// Was there tap points/currency data in the response?
				if (pointsTotal != null && currencyName != null)
				{
					//TapjoyLog.i(TAPJOY_POINTS, "currencyName: " + currencyName);
					//TapjoyLog.i(TAPJOY_POINTS, "pointsTotal: " + pointsTotal);
					
					// Save the last known tap points total.
					TapjoyConnectCore.saveTapPointsTotal(Integer.parseInt(pointsTotal));
					
					// Trigger the notifier to fire back.
					tapjoyAwardPointsNotifier.getAwardPointsResponse(currencyName, Integer.parseInt(pointsTotal));
					return true;
				}
				else
				{
					TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing tags.");
				}
			}
			else
			// Error spending virtual currency.
			if (nodeValue != null && nodeValue.endsWith("false"))
			{
				message = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Message"));
				TapjoyLog.i(TAPJOY_POINTS, message);
				
				tapjoyAwardPointsNotifier.getAwardPointsResponseFailed(message);
				return true;
			}
			else
			{
				TapjoyLog.e(TAPJOY_POINTS, "Invalid XML: Missing <Success> tag.");
			}
		}
		
		return false;
	}
}
