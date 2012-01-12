// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import android.util.Log;

/**
 * 
 * This class is used to control debug messages to the console for Tapjoy SDK related classes.
 *
 */
public class TapjoyLog 
{
	private static boolean showLog = false;
	
	/**
	 * Enables or disables logging.
	 * @param enable		Set to true if logging should be enabled, false to disable logging.
	 */
	public static void enableLogging(boolean enable)
	{
		Log.i("TapjoyLog", "enableLogging: " + enable);
		showLog = enable;
	}
	
	/**
	 * Set an info log message.
	 * @param tag			Tag for the log message.
	 * @param msg			Log message to output to the console.
	 */
	public static void i(String tag, String msg)
	{
		if (showLog)
			Log.i(tag , msg);
	}

	/**
	 * Set an error log message.
	 * @param tag			Tag for the log message.
	 * @param msg			Log message to output to the console.
	 */
	public static void e(String tag, String msg)
	{
		if (showLog)
			Log.e(tag , msg);
	}

	/**
	 * Set a warning log message.
	 * @param tag			Tag for the log message.
	 * @param msg			Log message to output to the console.
	 */
	public static void w(String tag , String msg)
	{
		if (showLog)
			Log.w(tag, msg);
	}

	/**
	 * Set a debug log message.
	 * @param tag			Tag for the log message.
	 * @param msg			Log message to output to the console.
	 */
	public static void d(String tag , String msg)
	{
		if (showLog)
			Log.d(tag, msg);
	}

	/**
	 * Set a verbose log message.
	 * @param tag			Tag for the log message.
	 * @param msg			Log message to output to the console.
	 */
	public static void v(String tag , String msg)
	{
		if (showLog)
			Log.v(tag, msg);
	}
}
