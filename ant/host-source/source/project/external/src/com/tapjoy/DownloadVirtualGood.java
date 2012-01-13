// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.tapjoy.TJCVirtualGoods;

/**
 * This activity is launched after checking for a purchased virtual good has been 
 * detected but not yet downloaded.  It displays a prompt asking if the user would like
 * to download the virtual good. 
 *
 */
public class DownloadVirtualGood extends Activity
{
	private Button downloadBtn;
	private Button cancelBtn;
	private TextView vgAcquiredMsg;
	
	private String urlParams = "";
	private String clientPackage = "";
	
	public static final String EXTRA_KEY_VIRTUAL_GOOD_NAME	= "NAME";
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		Bundle extras = getIntent().getExtras();
		
		if (extras != null)
		{
			clientPackage = TapjoyConnectCore.getClientPackage();
			
			if (extras.containsKey(TapjoyConstants.EXTRA_URL_PARAMS))
				urlParams = extras.getString(TapjoyConstants.EXTRA_URL_PARAMS);
		}
		
		TapjoyLog.i("DOWNLOAD VIRTUAL GOODS", "clientPackage: " + TJCVirtualGoods.getClientPackage());
		
		setContentView(this.getResources().getIdentifier("tapjoy_virtualgoods_reconnectvirtualgoods", "layout", clientPackage));
		downloadBtn = (Button) findViewById(this.getResources().getIdentifier("DownloadBtn", "id", clientPackage));
		cancelBtn = (Button) findViewById(this.getResources().getIdentifier("CancelBtn", "id", clientPackage));
		vgAcquiredMsg = (TextView) findViewById(this.getResources().getIdentifier("VGAcquiredMsgText", "id", clientPackage));

		downloadBtn.setOnClickListener(downloadListener);
		cancelBtn.setOnClickListener(cancelListener);

		vgAcquiredMsg.setText("You have successfully acquired item " + "'" + extras.getString(EXTRA_KEY_VIRTUAL_GOOD_NAME) + "'." + " Would you like to download it now?");
	}

	OnClickListener downloadListener = new OnClickListener()
	{
		public void onClick(View v)
		{
			Intent virtualGoodsIntent = new Intent(DownloadVirtualGood.this, TJCVirtualGoods.class);
			virtualGoodsIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, urlParams);
			virtualGoodsIntent.putExtra(TJCVirtualGoods.EXTRA_MY_ITEMS_TAB, true);
			startActivity(virtualGoodsIntent);
			
			finish();
		}
	};
	
	OnClickListener cancelListener = new OnClickListener()
	{
		public void onClick(View v)
		{
			finish();
		}
	};
}
