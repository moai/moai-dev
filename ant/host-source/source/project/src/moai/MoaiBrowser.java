//----------------------------------------------------------------//
// Copyright (c) 2010-2013 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.Context;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings.Secure;
import android.util.DisplayMetrics;

import java.lang.reflect.Method;
import java.lang.Runtime;
import java.util.Calendar;
import java.util.TimeZone;
import java.util.ArrayList;
import java.util.UUID;
import java.util.Locale;

//================================================================//
// MoaiBrowser
//================================================================//
public class MoaiBrowser {
	private static Activity sActivity = null;

    //----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void openURL ( String url ) {
		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, Uri.parse ( url )));
	}

    //----------------------------------------------------------------//
	public static void openURLWithParams ( String url ) {
		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, Uri.parse ( url )));
	}
}
