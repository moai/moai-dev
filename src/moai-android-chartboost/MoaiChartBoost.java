//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.view.View;

import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDefaultDelegate;

//================================================================//
// MoaiChartBoost
//================================================================//
public class MoaiChartBoost extends ChartboostDefaultDelegate {

	public enum ListenerEvent {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
    }

	private static Activity sActivity = null;
	
	protected static native void AKUInvokeListener ( int eventID );
	
	//----------------------------------------------------------------//
	public static boolean onBackPressed ( Activity activity ) {
		MoaiLog.i ( "MoaiChartBoost: onBackPressed" );
		return Chartboost.sharedChartboost ().onBackPressed ();
	}
	
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		MoaiLog.i ( "MoaiChartBoost: onCreate" );
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onDestroy ( Activity activity ) {
		MoaiLog.i ( "MoaiChartBoost: onDestroy" );
		Chartboost.sharedChartboost ().onDestroy ( sActivity );
	}
	
	//----------------------------------------------------------------//
	public static void onStart () {
		MoaiLog.i ( "MoaiChartBoost: onStart" );
		Chartboost.sharedChartboost ().onStart ( sActivity );
		Chartboost.sharedChartboost ().startSession ();
	}
	
	//----------------------------------------------------------------//
	public static void onStop () {
		MoaiLog.i ( "MoaiChartBoost: onStop" );
		Chartboost.sharedChartboost ().onStop ( sActivity );
	}

	//================================================================//
	// ChartBoost JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiChartBoost: cacheInterstitial" );
		
		if ( location != null ) {
		 	Chartboost.sharedChartboost ().cacheInterstitial ( location );
		}
		else {
			Chartboost.sharedChartboost ().cacheInterstitial ();
		}
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiChartBoost: hasCachedInterstitial" );
		
		if ( location != null ) {
		 	return Chartboost.sharedChartboost ().hasCachedInterstitial ( location );
		}
		return Chartboost.sharedChartboost ().hasCachedInterstitial ();
	}

	//----------------------------------------------------------------//
	public static void init ( String appId, String appSignature ) {
		
		MoaiLog.i ( "MoaiChartBoost: init" );
	
		Chartboost.sharedChartboost ().onCreate ( sActivity, appId, appSignature, new MoaiChartBoost ());
	}

	//----------------------------------------------------------------//
	public static void showInterstitial ( String location ) {
				
		MoaiLog.i ( "MoaiChartBoost: showInterstitial" );
		if ( location != null ) {
		 	Chartboost.sharedChartboost ().showInterstitial ( location );
		}
		else {
			Chartboost.sharedChartboost ().showInterstitial ();
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