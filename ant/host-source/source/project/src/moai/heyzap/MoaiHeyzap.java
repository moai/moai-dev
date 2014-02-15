package com.ziplinegames.moai;

import android.app.Activity;

import com.heyzap.sdk.ads.*;

class MoaiHeyzap {

    private static Activity sActivity = null;
    private static MoaiHeyzap instance = null;

    private static HeyzapAds.OnStatusListener heyzapListener = new HeyzapAds.OnStatusListener() {
        public void onShow(String tag) {
            MoaiLog.i ( "MoaiHeyzap onShow" );
        }
        public void onClick(String tag) {
            MoaiLog.i ( "MoaiHeyzap onClick" );
        }
        public void onHide(String tag) {
            MoaiLog.i ( "MoaiHeyzap onHide" );
        }
        public void onFailedToShow(String tag) {
            MoaiLog.i ( "MoaiHeyzap onFailedToShow" );
        }
        public void onAvailable(String tag) {
            MoaiLog.i ( "MoaiHeyzap onAvailable" );
        }
        public void onFailedToFetch(String tag) {
            MoaiLog.i ( "MoaiHeyzap onFailedToFetch" );
        }
        public void onAudioStarted() {
            MoaiLog.i ( "MoaiHeyzap onAudioStarted" );
        }
        public void onAudioFinished() {
            MoaiLog.i ( "MoaiHeyzap onAudioFinished" );
        }
    };

    public static void start() {
        HeyzapAds.start(sActivity, 0, heyzapListener);
    }

    public static void onCreate ( Activity activity ) {
        MoaiLog.i ( "MoaiHeyzap onCreate: Initializing Heyzap" );
        sActivity = activity;
    }

    public static void onDestroy () {
        MoaiLog.i ( "MoaiHeyzap onDestroy: Destroying Heyzap service" );
        InterstitialAd.dismiss();
    }

    public static void showInterstitial(String tag) {
        if (tag == null) { tag = "default"; }
        MoaiLog.i ( "MoaiHeyzap showInterstitial" );
        InterstitialAd.display(sActivity, tag);
    }

    public static boolean isInterstitialAvailable(String tag) {
        if (tag == null) { tag = "default"; }
        MoaiLog.i ( "MoaiHeyzap isInterstitialAvailable: " + (InterstitialAd.isAvailable(tag) ? "YES" : "NO") );
        return InterstitialAd.isAvailable(tag);
    }

    public static void loadInterstitial(String tag) {
        if (tag == null) { tag = "default"; }
        MoaiLog.i ( "MoaiHeyzap loadInterstitial" );
        InterstitialAd.fetch(tag);
    }
}
