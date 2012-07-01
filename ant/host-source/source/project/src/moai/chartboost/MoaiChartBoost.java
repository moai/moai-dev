//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import com.crittercism.app.Crittercism;

//================================================================//
// MoaiCrittercism
//================================================================//
public class MoaiChartBoost {

	private static Activity sActivity = null;
    private static ChartBoost _cb = null

    protected static native void	AKUNotifyChartBoostInterstitialDismissed ();
	protected static native void	AKUNotifyChartBoostInterstitialLoadFailed ();
    
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiChartBoost onCreate: Initializing ChartBoost" );
		
		sActivity = activity;
	}

	//================================================================//
	// Crittercism JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void init ( String identifier, String signature ) {

		_cb = ChartBoost.getSharedChartBoost(this);
        _cb.setAppId(identifier);
        _cb.setAppSignature(signature);
        
        // Notify an install
        _cb.install();
        

	}

	//----------------------------------------------------------------//
	public static void loadInterstitial ( String location ) {

        
        if location != null && !location.isEmpty() && !location.trim().isEmpty()
        {
        
            // Show interstitial with location identifier
            _cb.showInterstitial(location);
            
        }
        else {
            
            // Show an interstitial
            _cb.showInterstitial();
            
        }
    
	}

	public static boolean showInterstitial ( String location ) {



        if location != null && !location.isEmpty() && !location.trim().isEmpty()
        {

            return [cb showInterstitial:location];

        }
        else {

            return [cb showInterstitial];

        }    

    }

    
    /**
     * Called when the user dismisses the interstitial
     * If you are displaying the add yourself, dismiss it now.
     * 
     * @param interstitialView the interstitial view to dismiss
     */
    public void didDismissInterstitial(View interstitialView) {
        
        AKUNotifyChartBoostInterstitialDismissed()
        
    }
    

    /**
     * Called when the user dismisses the interstitial by clicking on the ad
     * 
     * @param interstitialView the interstitial view to dismiss
     */
    public void didClickInterstitial(View interstitialView) {
        
         AKUNotifyChartBoostInterstitialDismissed()
        
    }
    
    
    

}