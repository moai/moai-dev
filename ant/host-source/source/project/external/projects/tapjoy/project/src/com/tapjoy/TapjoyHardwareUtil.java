// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

/**
 * This class retrieves the serial number of the device.
 * This requires android-9 to compile and run.
 * 
 * THIS CLASS WILL ONLY BE LOADED IF THE DEVICE IS AT LEAST RUNNING ANDROID-9.
 * For devices running a lower version of Android, this class never gets loaded
 * and the device will run fine.
 */
public class TapjoyHardwareUtil
{
	public TapjoyHardwareUtil()
	{
		
	}
	
	
	/**
	 * Gets the serial number of the device.
	 * @return								Serial number of the device.
	 */
	public String getSerial()
	{
		// NOTE FOR DEVELOPERS:
		//
		// The Tapjoy SDK need to be compiled on at android-9 or higher, but is backwards compatible with devices running older versions of 
		// Android and will run fine on them.  The higher level Android APIs used by the Tapjoy SDK (ex: android.OS.Build.SERIAL) are called 
		// only in wrapper classes (like this one, TapjoyHardwareUtil); these classes only get loaded and initialized when the device passes 
		// a minimum Android OS version check.  If the device does not meet the minimum Android version requirement, the wrapper class is 
		// never loaded and the device will run fine.
		return android.os.Build.SERIAL;
	}
}
