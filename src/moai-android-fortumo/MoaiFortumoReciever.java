//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;
import android.content.BroadcastReceiver;

import com.fortumo.android.Fortumo;

import java.util.ArrayList;

//================================================================//
// MoaiFortumoReceiver
//================================================================//
public class MoaiFortumoReciever extends BroadcastReceiver {

	protected static native void AKUNotifyFortumoPurchaseResponseReceived ( int billing_status, String credit_amount,
																			String credit_name, String message_id,
																			String payment_code, String price_amount,
																			String price_currency, String product_name,
																			String service_id, String user_id );

	@Override
	public void onReceive(Context context, Intent intent) {
		Bundle extras = intent.getExtras();

		MoaiLog.i ( "- billing_status:  " + extras.getInt("billing_status"));
		MoaiLog.i ( "- credit_amount:   " + extras.getString("credit_amount"));
		MoaiLog.i ( "- credit_name:     " + extras.getString("credit_name"));
		MoaiLog.i ( "- message_id:      " + extras.getString("message_id") );
		MoaiLog.i ( "- payment_code:    " + extras.getString("payment_code"));
		MoaiLog.i ( "- price_amount:    " + extras.getString("price_amount"));
		MoaiLog.i ( "- price_currency:  " + extras.getString("price_currency"));
		MoaiLog.i ( "- product_name:    " + extras.getString("product_name"));
		MoaiLog.i ( "- service_id:      " + extras.getString("service_id"));
		MoaiLog.i ( "- user_id:         " + extras.getString("user_id"));

		int billingStatus = extras.getInt("billing_status");

		AKUNotifyFortumoPurchaseResponseReceived ( extras.getInt("billing_status"), extras.getString("credit_amount"),
												   extras.getString("credit_name"), extras.getString("message_id"),
												   extras.getString("payment_code"), extras.getString("price_amount"),
												   extras.getString("price_currency"), extras.getString("product_name"),
												   extras.getString("service_id"), extras.getString("user_id"));
	}
}