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
 * Status codes used in the callbacks for loadGameState and saveGameState.
 */
public class TapjoyVideoStatus
{
	/**
	 * SD card or external storage is not mounted to the device.
	 */
	public static final int STATUS_MEDIA_STORAGE_UNAVAILABLE			= 1;
	
	/**
	 * Network error occurred while trying to initialize videos.
	 */
	public static final int STATUS_NETWORK_ERROR_ON_INIT_VIDEOS			= 2;
	
	/**
	 * Error while trying to play video.
	 */
	public static final int STATUS_UNABLE_TO_PLAY_VIDEO					= 3;
}
