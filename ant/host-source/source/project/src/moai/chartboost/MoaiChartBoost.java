package com.ziplinegames.moai;

import android.app.Activity;

import com.chartboost.sdk.*;


class MoaiChartBoost {

	private static Activity sActivity = null;
	private static MoaiChartBoost instance = null;
	private static Chartboost cb;

	private static ChartboostDelegate chartBoostDelegate = new ChartboostDelegate() {

	    public boolean shouldDisplayInterstitial(String location) {
	        return true;
	    }

	    @Override
	    public boolean shouldRequestInterstitial(String location) {
	        return true;
	    }

	    @Override
	    public void didCacheInterstitial(String location) {
	    	//MoaiLog.i ( "MoaiChartboost didCacheInterstitial: didCacheInterstitial" );
	    }

	    @Override
	    public void didFailToLoadInterstitial(String location) {
	        //MoaiLog.i ( "MoaiChartboost didFailToLoadInterstitial: didFailToLoadInterstitial" );
	    }

	    @Override
	    public void didDismissInterstitial(String location) {
	        cb.cacheInterstitial(location);
	    }

	    @Override
	    public void didCloseInterstitial(String location) {
	        // Know that the user has closed the interstitial
	    }

	    @Override
	    public void didClickInterstitial(String location) {
	        // Know that the user has clicked the interstitial
	    }

	    @Override
	    public void didShowInterstitial(String location) {
	        //MoaiLog.i ( "MoaiChartboost didShowInterstitial: didShowInterstitial" );
	    }

	    @Override
	    public boolean shouldDisplayLoadingViewForMoreApps() {
	        return true;
	    }

	    @Override
	    public boolean shouldRequestMoreApps() {
	        return true;
	    }

	    @Override
	    public boolean shouldDisplayMoreApps() {
	        return true;
	    }

	    @Override
	    public void didFailToLoadMoreApps() {
	        // Do something else when the More-Apps page fails to load
	    }

	    @Override
	    public void didCacheMoreApps() {
	        // Know that the More-Apps page is cached and ready to display
	    }

	    @Override
	    public void didDismissMoreApps() {
	        // Know that the More-Apps page has been dismissed
	    }

	    @Override
	    public void didCloseMoreApps() {
	        // Know that the More-Apps page has been closed
	    }

	    @Override
	    public void didClickMoreApps() {
	        // Know that the More-Apps page has been clicked

	    }

	    @Override
	    public void didShowMoreApps() {
	        // Know that the More-Apps page has been presented on the screen
	    }

	    @Override
	    public boolean shouldRequestInterstitialsInFirstSession() {
	        return true;
	    }
	};

	public static void onCreate ( Activity activity ) {
		MoaiLog.i ( "MoaiChartBoost onCreate: Initializing Chartboost" );
		sActivity = activity;
	}

	public static void onDestroy () {
		MoaiLog.i ( "MoaiChartBoost onDestroy: Destroying Chartboost service" );
		cb.onDestroy(sActivity);
	}

	public static void init(String appId, String appSignature) {
		cb = Chartboost.sharedChartboost();
		cb.onCreate(sActivity, appId, appSignature, chartBoostDelegate);
		cb.startSession();
	}

	public static void showInterstitial(String location) {
		cb.showInterstitial();
	}

	public static boolean hasCachedInterstitial() {
		return cb.hasCachedInterstitial();
	}

	public static void loadInterstitial(String location) {
		cb.cacheInterstitial();
	}
}
