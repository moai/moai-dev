package com.ziplinegames.moai;

import android.app.Activity;

import com.heyzap.sdk.ads.*;

class MoaiHeyzap {

    private static Activity sActivity = null;
    private static MoaiHeyzap instance = null;

    protected static native void AKUNotifyHeyzapInterstitialFetchFailed ();
    protected static native void AKUNotifyHeyzapInterstitialAvailable ();
    protected static native void AKUNotifyHeyzapInterstitialShow ();
    protected static native void AKUNotifyHeyzapInterstitialHide ();
    protected static native void AKUNotifyHeyzapInterstitialShowFailed ();
    protected static native void AKUNotifyHeyzapInterstitialClicked ();
    protected static native void AKUNotifyHeyzapInterstitialAudioStarted ();
    protected static native void AKUNotifyHeyzapInterstitialAudioFinished ();

    private static HeyzapAds.OnStatusListener heyzapListener = new HeyzapAds.OnStatusListener() {
        public void onShow(String tag) {
            MoaiLog.i ( "MoaiHeyzap onShow" );
            AKUNotifyHeyzapInterstitialShow ( );
        }
        public void onClick(String tag) {
            MoaiLog.i ( "MoaiHeyzap onClick" );
            AKUNotifyHeyzapInterstitialClicked ( );
        }
        public void onHide(String tag) {
            MoaiLog.i ( "MoaiHeyzap onHide" );
            AKUNotifyHeyzapInterstitialHide ( );
        }
        public void onFailedToShow(String tag) {
            MoaiLog.i ( "MoaiHeyzap onFailedToShow" );
            AKUNotifyHeyzapInterstitialShowFailed ( );
        }
        public void onAvailable(String tag) {
            MoaiLog.i ( "MoaiHeyzap onAvailable" );
            AKUNotifyHeyzapInterstitialAvailable ( );
        }
        public void onFailedToFetch(String tag) {
            MoaiLog.i ( "MoaiHeyzap onFailedToFetch" );
            AKUNotifyHeyzapInterstitialFetchFailed ( );
        }
        public void onAudioStarted() {
            MoaiLog.i ( "MoaiHeyzap onAudioStarted" );
            AKUNotifyHeyzapInterstitialAudioStarted ( );
        }
        public void onAudioFinished() {
            MoaiLog.i ( "MoaiHeyzap onAudioFinished" );
            AKUNotifyHeyzapInterstitialAudioFinished ( );
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
