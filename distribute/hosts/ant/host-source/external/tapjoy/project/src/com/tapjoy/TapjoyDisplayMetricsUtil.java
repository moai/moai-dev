// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import android.content.Context;
import android.content.res.Configuration;
import android.util.DisplayMetrics;
import android.view.WindowManager;

/**
 * Helper class for Android compatibility.  
 * This requires android-4 to compile and run.
 * 
 * THIS CLASS WILL ONLY BE LOADED IF THE DEVICE IS AT LEAST RUNNING ANDROID-4.
 * For devices running a lower version of Android, this class never gets loaded
 * and the device will run fine.
 *
 */
public class TapjoyDisplayMetricsUtil
{
	private Context context;
	private Configuration configuration;
	private DisplayMetrics metrics;
	
	/**
	 * Constructor
	 * @param theContext					Application or Activity context.
	 */
	public TapjoyDisplayMetricsUtil(Context theContext)
	{
		context = theContext;
		
		metrics = new DisplayMetrics();
		WindowManager windowManager = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
		windowManager.getDefaultDisplay().getMetrics(metrics);
		
		configuration = context.getResources().getConfiguration();
	}
	
	
	/**
	 * Gets the screen density.
	 * @return								Screen density in DPI.
	 */
	public int getScreenDensity()
	{
		return metrics.densityDpi;
		
	}
	
	
	/**
	 * Gets the screen layout size.
	 * @return								Screen layout size.
	 */
	public int getScreenLayoutSize()
	{
		return (configuration.screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK); 
	}
}
