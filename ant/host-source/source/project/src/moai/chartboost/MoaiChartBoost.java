//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.view.View;

import com.chartboost.sdk.ChartBoost;

//================================================================//
// MoaiChartBoost
//================================================================//
public class MoaiChartBoost extends Activity {

	private static Activity sActivity = null;
    private static ChartBoost _cb = null;
    private static MoaiChartBoost sChartboost = null;

    protected static native void	AKUNotifyChartBoostInterstitialDismissed ();
	protected static native void	AKUNotifyChartBoostInterstitialLoadFailed ();
    
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiChartBoost onCreate: Initializing ChartBoost" );
		
		sActivity = activity;
	}

    
    @Override 
    protected void onResume()
    {
        super.onResume();
        ChartBoost.getSharedChartBoost(sActivity);
    }
    
	//================================================================//
	// MoaiChartBoost JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void init ( String identifier, String signature ) {

 
        sChartboost = new MoaiChartBoost();
        
        MoaiLog.i ( "MoaiCharBoost init" );      
        
		_cb = ChartBoost.getSharedChartBoost(sActivity);
        _cb.setAppId(identifier);
        _cb.setAppSignature(signature);
        
        // Notify an install
        _cb.install();
  
	}

	//----------------------------------------------------------------//
	public static void loadInterstitial ( String location ) {

        
     
        if (location != null && !location.isEmpty() && !location.trim().isEmpty())
        {
          MoaiLog.i ( "MoaiCharBoost loadInterstitial: location=" + location );
            // Show interstitial with location identifier
            _cb.cacheInterstitial(location);
            
        }
        else {
               MoaiLog.i ( "MoaiCharBoost loadInterstitial: no location" );
            // Show an interstitial
            _cb.cacheInterstitial();
            
        }
    
	}

	public static boolean showInterstitial ( String location ) {
        
    
        if (location != null && !location.isEmpty() && !location.trim().isEmpty())
        {
                      MoaiLog.i ( "MoaiCharBoost showInterstitial: location=" + location );
            // Show interstitial with location identifier
            _cb.showInterstitial(location);
            
        }
        else {
            MoaiLog.i ( "MoaiCharBoost showInterstitial: no location" );
            // Show an interstitial
            _cb.showInterstitial();
            
        }
        
        
        return true;
    }

    
    /**
     * Called when an interstitial has been received, before it is presented on screen
     * Return false if showing an interstitial is currently inappropriate, for example if the user has entered the main game mode.
     * This is also the method you want to use if you're going to display the interestitial yourself.
     * 
     * @param interstitialView the intertitial to display
     * @return whether to display the interstitial using the default display controller
     */
    public boolean shouldDisplayInterstitial(View interstitialView) {
        
         MoaiLog.i ( "MoaiCharBoost shouldDisplayInterstitial" );
        return true;
    }
    
    /**
     * Called when the user dismisses the interstitial
     * If you are displaying the add yourself, dismiss it now.
     * 
     * @param interstitialView the interstitial view to dismiss
     */
    public void didDismissInterstitial(View interstitialView) {
        
        
            MoaiLog.i ( "MoaiCharBoost didDismissInterstitial" );
        AKUNotifyChartBoostInterstitialDismissed();
        
    }
    

    /**
     * Called when the user dismisses the interstitial by clicking on the ad
     * 
     * @param interstitialView the interstitial view to dismiss
     */
    public void didClickInterstitial(View interstitialView) {
        
                 MoaiLog.i ( "MoaiCharBoost didClickInterstitial" );
        AKUNotifyChartBoostInterstitialDismissed();
        
    }
    
    
    

}