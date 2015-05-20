//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;
import com.ziplinegames.moai.*;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;

//================================================================//
// MoaiWildTangentAds
//================================================================//
public class MoaiWildTangentAdsListener extends AdListener {

	@Override
	public void onAdClosed () {
		MoaiLog.i ( "MoaiWildTangentAdsListener: onAdClosed" );
	}

	@Override
	public void onAdFailedToLoad ( int errorCode ) {
		MoaiLog.i ( "MoaiWildTangentAdsListener: onAdFailedToLoad" );
		
		switch ( errorCode ) {
			case AdRequest.ERROR_CODE_INTERNAL_ERROR:
			
			MoaiLog.i ( "MoaiWildTangentAdsListener: ERROR_CODE_INTERNAL_ERROR: Something happened internally; for instance, an invalid response was received from the ad server." );
			break;
			case AdRequest.ERROR_CODE_INVALID_REQUEST:
			
			MoaiLog.i ( "MoaiWildTangentAdsListener: ERROR_CODE_INVALID_REQUEST: The ad request was invalid; for instance, the ad unit ID was incorrect." );
			break;
			case AdRequest.ERROR_CODE_NETWORK_ERROR:
			
			MoaiLog.i ( "MoaiWildTangentAdsListener: ERROR_CODE_NETWORK_ERROR: The ad request was unsuccessful due to network connectivity." );
			break;
			case AdRequest.ERROR_CODE_NO_FILL:
			
			MoaiLog.i ( "MoaiWildTangentAdsListener: ERROR_CODE_NO_FILL: The ad request was successful, but no ad was returned due to lack of ad inventory." );
			break;
		}

		MoaiWildTangentAds.mloadAdRequested = false;
	}

	@Override
	public void onAdLeftApplication () {
		MoaiLog.i ( "MoaiWildTangentAdsListener: onAdLeftApplication" );

	}

	@Override
	public void onAdLoaded () {
		MoaiLog.i ( "MoaiWildTangentAdsListener: onAdLoaded" );

	}

	@Override
	public void onAdOpened () {
		MoaiLog.i ( "MoaiWildTangentAdsListener: onAdOpened" );
	}
}