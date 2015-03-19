//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.view.View;

import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;

//================================================================//
// MoaiChartBoost
//================================================================//
public class MoaiChartBoost extends ChartboostDelegate {

	public enum ListenerEvent {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
    }

	private static Activity sActivity = null;
	
	protected static native void AKUInvokeListener ( int eventID );
	
	//----------------------------------------------------------------//
	public static void onBackPressed ( Activity activity ) {
		
        MoaiLog.i ( "MoaiChartBoost: onBackPressed" );

		if ( Chartboost.onBackPressed ()) {
        	return;
		}
    	else {
        	activity.onBackPressed ();
    	}
    }
	
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
	
		MoaiLog.i ( "MoaiChartBoost: onCreate" );
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onDestroy ( Activity activity ) {
 
		MoaiLog.i ( "MoaiChartBoost: onDestroy" );
    	Chartboost.onDestroy ( sActivity );
	}

	//----------------------------------------------------------------//
	public static void onPause () {
 
		MoaiLog.i ( "MoaiChartBoost: onPause" );
    	Chartboost.onPause ( sActivity );
	}

	//----------------------------------------------------------------//
	public static void onResume () {
 
		MoaiLog.i ( "MoaiChartBoost: onResume" );
    	Chartboost.onResume ( sActivity );
	}
		
	//----------------------------------------------------------------//
	public static void onStart () {
		
		MoaiLog.i ( "MoaiChartBoost: onStart" );
    	Chartboost.onStart ( sActivity );
	}
	
	//----------------------------------------------------------------//
	public static void onStop () {

		MoaiLog.i ( "MoaiChartBoost: onStop" );
    	Chartboost.onStop ( sActivity );
	}

	//================================================================//
	// ChartBoost JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiChartBoost: cacheInterstitial" );
		
		if ( location != null ) {
		 	Chartboost.cacheInterstitial ( location );
		}
		else {
			Chartboost.cacheInterstitial ( CBLocation.LOCATION_DEFAULT );
		}
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiChartBoost: hasCachedInterstitial" );
		
		if ( location != null ) {
		 	return Chartboost.hasInterstitial ( location );
		}
		return Chartboost.hasInterstitial ( CBLocation.LOCATION_DEFAULT );
	}

	//----------------------------------------------------------------//
	public static void init ( String appId, String appSignature ) {
		
		MoaiLog.i ( "MoaiChartBoost: init" );

		Chartboost.startWithAppId ( sActivity, appId, appSignature );
    	Chartboost.setDelegate ( new MoaiChartBoost ());
    	Chartboost.onCreate ( sActivity );	
	}

	//----------------------------------------------------------------//
	public static void showInterstitial ( String location ) {
				
		MoaiLog.i ( "MoaiChartBoost: showInterstitial" );
		if ( location != null ) {
		 	Chartboost.showInterstitial ( location );
		}
		else {
			Chartboost.showInterstitial ( CBLocation.LOCATION_DEFAULT );
		}
	}

	//================================================================//
	// ChartBoostDelegate methods
	//================================================================//

	//----------------------------------------------------------------//
	public void didDismissInterstitial ( String location ) {

		MoaiLog.i ( "MoaiChartBoost: didDismissInterstitial" );
		AKUInvokeListener ( ListenerEvent.INTERSTITIAL_DISMISSED.ordinal ());
	}

	//----------------------------------------------------------------//
	public void didFailToLoadInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiChartBoost: didFailToLoadInterstitial" );
		AKUInvokeListener ( ListenerEvent.INTERSTITIAL_LOAD_FAILED.ordinal ());
	}

	//----------------------------------------------------------------//1
	public boolean shouldDisplayMoreApps () {
		
		return false;
	}
	
	//----------------------------------------------------------------//1
	public boolean shouldRequestMoreApps () {
		
		return false;
	}
}