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
 * Notifier to inform the app whenever virtual currency has been earned.
 */
public interface TapjoyEarnedPointsNotifier
{
	/**
	 * Callback whenever virtual currency has been earned.  This can get called on a getTapPoints call.
	 * @param amount						Amount of virtual currency earned (delta).
	 */
	public void earnedTapPoints(int amount);
}