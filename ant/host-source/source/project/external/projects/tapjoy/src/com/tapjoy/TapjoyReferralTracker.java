// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

public class TapjoyReferralTracker extends BroadcastReceiver
{
	final String	REFERRAL_TRACKING	= "ReferralTracking";

	@Override
	public void onReceive(Context context, Intent intent)
	{
		// Need to enable logging here since this callback gets called when the app is installed, not launched.
		//TapjoyLog.enableLogging(true);
		
		TapjoyLog.i(REFERRAL_TRACKING, "----------------------------------------");
		TapjoyLog.i(REFERRAL_TRACKING, "RECEIVED REFERRAL INFO");
		
		String uri = intent.toURI();
		
		TapjoyLog.i(REFERRAL_TRACKING, "referrer: [" + uri + "]");
		
		if (uri != null && uri.length() > 0)
		{
			int index = uri.indexOf("referrer=");
			
			if (index > -1)
			{
				// The last 4 characters are ";end" so remove them.
				String referrerURL = uri.substring(index, uri.length() - 4);
				
				TapjoyLog.i(REFERRAL_TRACKING, "Referral URI: [" + referrerURL + "]");
				
				// referralURL contains both referrer parameter and its value
				// ex: referralURL="referrer=com.package.app"
				SharedPreferences settings = context.getSharedPreferences(TapjoyConstants.TJC_PREFERENCE, 0);
				SharedPreferences.Editor editor = settings.edit();
				editor.putString(TapjoyConstants.PREF_REFERRAL_URL, referrerURL);
				
				// For debugging the entire referrer data.
				editor.putString(TapjoyConstants.PREF_REFERRER_DEBUG, uri);
				
				editor.commit();
			}
			else
			{
				TapjoyLog.i(REFERRAL_TRACKING, "No Referral URL.");
			}
		}
		
		TapjoyLog.i(REFERRAL_TRACKING, "----------------------------------------");
	}
}
