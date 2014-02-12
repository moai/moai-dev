package com.ziplinegames.moai;

import android.app.Activity;

import com.heyzap.sdk.*;

class MoaiHeyzap {

	private static Activity sActivity = null;
	private static MoaiHeyzap instance = null;

	private static OnStatusListener heyzapListener = new OnStatusListener() {
		public void onShow(String tag) {}
		public void onClick(String tag) {}
		public void onHide(String tag) {}
		public void onFailedToShow(String tag) {}
		public void onAvailable(String tag) {}
		public void onFailedToFetch(String tag) {}
		public void onAudioStarted() {}
		public void onAudioFinished() {}
	};

	public static void init() {
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
		if (tag == NULL) { tag = "default"; }
		InterstitialAd.display(sActivity, tag);
	}

	public static boolean hasInterstitialAvailable(String tag) {
		if (tag == NULL) { tag = "default"; }
		return InterstitialAd.isAvailable(tag);
	}

	public static void loadInterstitial(String tag) {
		if (tag == NULL) { tag = "default"; }
		InterstitialAd.fetch(tag);
	}
}
