// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.io.File;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;

import java.util.concurrent.RejectedExecutionException;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;

import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.StateListDrawable;
import android.graphics.drawable.GradientDrawable.Orientation;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.StatFs;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager.BadTokenException;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TabHost;
import android.widget.TableLayout;
import android.widget.TextView;

import com.tapjoy.TJCVirtualGoodsConnection;
import com.tapjoy.TJCVirtualGoods;
import com.tapjoy.TJCVirtualGoodUtil.DownloadVirtualGoodTask;

import com.tapjoy.VGStoreItem;
import com.tapjoy.VGStoreItemAttributeValue;

public class TJCVirtualGoods extends Activity implements View.OnClickListener
{
	private static final int STORE_ITEM = 0;  
	private static final int PURCHASED_ITEM = 2;

	public static final String TAPJOY_DOWNLOAD_FAILED 						= "Download Failed";
	public static final String TAPJOY_ERROR_NO_SPACE_ON_DEVICE				= "No more space is available on Device.";
	public static final String TAPJOY_ERROR_NO_SPACE_ON_DEVICE_AND_SD_CARD	= "No more space is available on Device and SD Card.";
	
	public final static String TAPJOY_DOWNLOAD_PENDING 						= "Download Pending";
	public final static String NETWORK_DOWN_MESSAGE 						= "Service is unreachable.\nDo you want to try again?";
	
	public final static String TAPJOY_VIRTUAL_GOODS 						= "Virtual Goods";
	public final static String TAPJOY_DOWNLOADVIRTUALGOOD 					= "DownloadVirtualGood";
	public final static String TAPJOY_DATABASE		 						= "Database";
	public final static String TAPJOY 										= "Tapjoy";
	public final static String TAPJOY_FILE_SYSTEM							= "File System";
	
	public static final String EXTRA_MY_ITEMS_TAB							= "my_items_tab";
	
	public final static String TAPJOY_DOWNLOAD_COMPLETED 					= "Download Completed";
	
	public final static String tapjoyWebServiceURL 							= TapjoyConstants.TJC_SERVICE_URL;
	
	public final static int NETWORK_DOWN 									= 11;
	public final static int NETWORK_TIME_OUT 								= 12;
	public final static int RESPONSE_FAIL 									= 13;
	public final static int PARSER_CONFIGURATION_EXCEPTION 					= 14;
	public final static int SAX_EXCEPTION 									= 15;
	
	// STATUS STATES WHEN DOWNLOADING A PURCHASED VIRTUAL GOOD
	public static final int VG_STATUS_DOWNLOAD_PENDING						= 0;
	public static final int VG_STATUS_DOWNLOAD_INIT							= 1;
	public static final int VG_STATUS_DOWNLOAD_SUCCESS_TO_PHONE				= 10;
	public static final int VG_STATUS_DOWNLOAD_SUCCESS_TO_SD_CARD			= 20;
	public static final int VG_STATUS_NETWORK_ERROR_SD_CARD					= 41;
	public static final int VG_STATUS_NETWORK_ERROR_PHONE					= 42;
	public static final int VG_STATUS_INSUFFICIENT_DISK_SPACE_SD_CARD		= 43;
	public static final int VG_STATUS_INSUFFICIENT_DISK_SPACE_PHONE			= 44;
	
	// Set this to a low value.  Android can only handle a few simultaneous AsyncTasks at a time.
	AsyncTaskPool asyncTaskPool = new AsyncTaskPool(2);
	
	public static TJCVirtualGoodsConnection tapjoyVGConnection = null;
	private TJCVirtualGoodUtil virtualGoodUtil;
	
	private static TapjoyDownloadListener tapjoyVGDownloadListener = null;
	private static FocusListener tapjoyVGFocusListener = null;
	
	static Context ctx = null;
	
	private static String urlParams;							// URL parameters to use in HTTP calls.
	private static String clientPackage;						// Client Package.
	
	private TabHost tabs;

	// Flags
	public static boolean doNotify								= true;
	public static boolean offersFromVG							= false;
	public static boolean vgFromOffers							= false;
	public static boolean vgFromDownloadedVG					= false;
	
	TextView quantityTextView;
	
	private int storeItemStart;
	private int purchasedItemStart;
	private boolean isItemDetailView = false;
	private Boolean checkPurchased = true;						// Flag indicating if we need to check purchased VG to download.
	private int purchasedThroughAvailableItem = 0;

	private ProgressBar storeProgress;							// Loading spinner on the store items list.
	private ProgressBar purchasedItemProgress;					// Loading spinner on the purchased items screen.

	private ArrayList<VGStoreItem> storeItemArray				= null;			// Array containing available virtual goods.
	private ArrayList<VGStoreItem> purchasedItemArray 			= null;			// Array containing purchased virtual goods data.

	private TableLayout storeItemTable;
	private TableLayout purchasedItemTable;

	private Button moreStoreItems;
	private Button morePurchasedItem;
	private FetchAndBuildStoreItemsTask fetchAllStoreItems = null;
	private FetchAndBuildPurchasedItemsTask fetPurchasedVGItems = null;

	private LinearLayout detailView;

	// DETAIL VIEW
	public static TextView detailDescQuantity;									// TextView for the "quantity: x" in the item detail view.
	
	private static ProgressBar detailProgressBar;
	private static ImageView detailErrorIcon;									// Error icon when a purchase/download is no successful.
	public static Button detailRetryButton;										// Retry button when an error occurs.
	private static TextView detailDownloadStatusTextView;						// Status of the purchase/download.
	private Button detailGetThisItemButton;											
	
	private TextView storeHeaderLeftTitle = null;
	private TextView storeHeaderRightTitle = null;

	private TextView vgFooterLeft = null;
	private Button vgGetMoreBtn = null;
	private TextView vgPurchasedFooterLeft = null;
	private Button vgPurchasedGetMoreBtn = null;
	private TextView vgDetailFooterLeft = null;
	private Button vgDetailGetMoreBtn = null;

	private TextView myItemsHeaderLeftTitle = null;
	private TextView myItemsHeaderRightTitle = null;

	private Dialog getItemDialog;
	private Dialog getMoreItemDialog;
	private Dialog itemDownloadedDialog;
	private ProgressDialog progressDialog;
	private Dialog exceptionDialog;

	private static TextView storeNoData;									// Textview to display when there are no items in the store.
	private static TextView yourItemNoData;									// Textview to display when there are no purchased items.

	private PurchaseVirtualGoodTask getPurchasedVGItems = null;
	private VGStoreItem currentVgStoreItem = null;

	// USER ACCOUNT OBJECT INFO
	private static String userID = null;									// UserID.
	private static String tapPoints;										// Amount of virtual currency.
	private static String currencyName;										// Virtual currency name.

	AlertDialog allItemAlert = null;
	AlertDialog purchasedItemAlert = null;

	private static boolean downloadStarted = false;

	static boolean dataSavedAtSDCard;										// Data was saved on the SD card.
	
	public static boolean isPurchasedItemDetailView = false;				// Whether the current view is a purchased item detail view.
	public static String detailViewStoreID = "";							// Store ID of the virtual good being displayed in the detail view.
	
	// Corner Radii configurations.
	float [] cornerNone = { 
				0.0f, 0.0f, 
        		0.0f, 0.0f, 
        		0.0f, 0.0f, 
        		0.0f, 0.0f };

	float [] cornerAll = { 
				10.0f, 10.0f, 
				10.0f, 10.0f, 
				10.0f, 10.0f, 
				10.0f, 10.0f };
	
	float [] cornerLeft = { 
				10.0f, 10.0f,
				0.0f, 0.0f,
				0.0f, 0.0f,
				10.0f, 10.0f };
	
	float [] cornerRight = { 
				0.0f, 0.0f,
				10.0f, 10.0f,
				10.0f, 10.0f,
				0.0f, 0.0f };
	

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		boolean showMyItems = false;
		ctx = this;

		super.onCreate(savedInstanceState);
		
		// Get URL parameters from the extras.
		Bundle extras = getIntent().getExtras();
		
		if (extras != null)
		{
			// These extras are passed in from TapjoyConnect calling showOffers
			if (extras.containsKey(TapjoyConstants.EXTRA_URL_PARAMS))
				urlParams = extras.getString(TapjoyConstants.EXTRA_URL_PARAMS);
			
			clientPackage = TapjoyConnectCore.getClientPackage();
			
			// This extra is passed in from checking downloaded virtual goods.
			if (extras.containsKey(EXTRA_MY_ITEMS_TAB))
				showMyItems = extras.getBoolean(EXTRA_MY_ITEMS_TAB);
		}
			
		virtualGoodUtil = new TJCVirtualGoodUtil(this, clientPackage);
		
		tapjoyVGConnection = new TJCVirtualGoodsConnection(tapjoyWebServiceURL, urlParams);
		
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "onCreate");
		
		isItemDetailView = false;
		
		// Load these values.
		userID = TJCVirtualGoodsData.getUserID();
		currencyName = TJCVirtualGoodsData.getCurrencyName();
		tapPoints = TJCVirtualGoodsData.getPoints();
		
		storeItemArray = new ArrayList<VGStoreItem>();
		purchasedItemArray = new ArrayList<VGStoreItem>();
		
		initView();
		
		// Create the Store and Purchased Item tables/views.
		syncAndRebuildAllTables();
		
		// Set the tab to "My Items" if we're coming from a prompt.
		if (showMyItems)
			tabs.setCurrentTab(1);
		
		virtualGoodUtil.setPurchasedItemArray(purchasedItemArray);
	}

	
	@Override
	protected void onPause()
	{
		super.onPause();
		

		if (doNotify)
		{
			FocusListener focusListener;
			focusListener = getVirtualGoodsFocusListener();
			if (focusListener != null)
			{
				TapjoyLog.i("ExtendedFocusListener", "On Pause");
				focusListener.onFocusLost();
			}
		}
		else
		{
			TapjoyLog.i("ExtendedFocusListener", "On Pause Not called");
		}
		
		doNotify = true;
	}

	
	@Override
	protected void onResume()
	{
		super.onResume();
		
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, virtualGoodUtil.downloadVirtualGoods.size()+" resume");
		
		if (doNotify && (!vgFromOffers && !vgFromDownloadedVG))
		{
			FocusListener focusListener;
			focusListener = getVirtualGoodsFocusListener();
			
			if (focusListener != null)
			{
				TapjoyLog.i("ExtendedFocusListener", "On Resume");
				focusListener.onFocusGain();
			}
		}
		else
		{
			TapjoyLog.i("ExtendedFocusListener", "On Resume Not Called");
		}
		
		doNotify = true;
		vgFromOffers = false;
	}
	

	@Override
	protected void onDestroy()
	{
		TapjoyLog.i("ExtendedFocusListener","onDestroy");
		super.onDestroy();
	}

	
	public void onClick(View v)
	{
		//----------------------------------------
		// SHOW DETAIL VIEW
		//----------------------------------------
		switch (v.getId())
		{
			case STORE_ITEM:
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "storeItemArray.size(): " + storeItemArray.size());
					
					isItemDetailView = true;
					tabs.setVisibility(View.GONE);
					detailView.setVisibility(View.VISIBLE);
					
					TextView index = (TextView) v.findViewById(this.getResources().getIdentifier("vg_row_index", "id", getClientPackage()));
					int i = Integer.parseInt(index.getText().toString());
					
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "onClick index: " + i);
					
					// Bounds check, incase this array got cleared for some reason?
					if (i < storeItemArray.size())
						showItemDetail(storeItemArray.get(i), STORE_ITEM, v, i);
				}
				break;
				
			case PURCHASED_ITEM:
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "purchasedItemArray.size(): " + purchasedItemArray.size());
					
					TextView index = (TextView) v.findViewById(this.getResources().getIdentifier("vg_row_index", "id", getClientPackage()));
					int i = Integer.parseInt(index.getText().toString());
					
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "onClick index: " + i);
					
					// Bounds check, incase this array got cleared for some reason?
					if (i < purchasedItemArray.size())
					{
						//if (virtualGoodUtil.virtualGoodsToDownload.get(purchasedItemArray.get(i).getVgStoreItemID()) == null)
						{
							isItemDetailView = true;
							tabs.setVisibility(View.GONE);
							detailView.setVisibility(View.VISIBLE);
							
							showItemDetail(purchasedItemArray.get(i), PURCHASED_ITEM, v, i);
						}
					}
				}
				break;
		}
		
		if (v instanceof Button) 
		{
			//----------------------------------------
			// SHOW OFFERS
			//----------------------------------------
			if (v == vgGetMoreBtn || v == vgPurchasedGetMoreBtn || v == vgDetailGetMoreBtn)
			{
				offersFromVG = true;
				vgFromOffers = true;
				doNotify = false;
				
				showOffers();
			}
			else
			//----------------------------------------
			// MORE STORE ITEMS
			//----------------------------------------
			if (v == moreStoreItems)
			{
				moreStoreItems.setEnabled(false);
				
				storeItemStart += VGStoreItem.MAX_ITEMS;
				
				fetchAllStoreItems = new FetchAndBuildStoreItemsTask();
				fetchAllStoreItems.execute();
			}
			else
			//----------------------------------------
			// MORE PURCHASED ITEMS
			//----------------------------------------
			if (v == morePurchasedItem)
			{
				morePurchasedItem.setEnabled(false);
				
				purchasedItemStart += VGStoreItem.MAX_ITEMS;
				
				fetPurchasedVGItems = new FetchAndBuildPurchasedItemsTask();
				fetPurchasedVGItems.execute();
			}
		}

	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if ((keyCode == KeyEvent.KEYCODE_BACK))
		{
			// DETAIL VIEW
			if (isItemDetailView == true)
			{
				tabs.setVisibility(View.VISIBLE);
				detailView.setVisibility(View.GONE);

				if (downloadStarted == true)
				{
					downloadStarted = false;
					tabs.setCurrentTab(1);
				}

				isItemDetailView = false;
				isPurchasedItemDetailView = false;
			}
			else
			{
				doNotify = false;

				for (int i = 0; i < TJCVirtualGoodUtil.currentTasks.size(); i++)
				{
					DownloadVirtualGoodTask dGood = (DownloadVirtualGoodTask) TJCVirtualGoodUtil.currentTasks.get(i);
					if (dGood != null && dGood.getStatus() == AsyncTask.Status.RUNNING)
						dGood.cancel(true);
				}

				TJCVirtualGoodUtil.currentTasks.clear();
				TJCVirtualGoodUtil.pendingTasks.clear();

				// TapjoyLog.e("TJC_EXCEPTION", "In else of back button");

				VGStoreItem.availableItemsMoreDataAvailable = 0;
				VGStoreItem.purchasedItemsMoreDataAvailable = 0;
				checkPurchased = true;
				downloadStarted = false;
				purchasedThroughAvailableItem = 0;

				// Cancel all async requests.
				if (fetchAllStoreItems != null && fetchAllStoreItems.getStatus() == AsyncTask.Status.RUNNING)
					fetchAllStoreItems.cancel(true);

				if (fetPurchasedVGItems != null && fetPurchasedVGItems.getStatus() == AsyncTask.Status.RUNNING)
					fetPurchasedVGItems.cancel(true);
				virtualGoodUtil.cancelExecution();

				finish();
			}

			return true;
		}
		return super.onKeyDown(keyCode, event);
	}


	private void initView()
	{
		setContentView(this.getResources().getIdentifier("tapjoy_virtualgoods", "layout", getClientPackage()));

		//--------------------------------------------------------------------------------
		// Create gradient backgrounds and buttons
		//--------------------------------------------------------------------------------
		
		SharedPreferences settings = ctx.getSharedPreferences(TapjoyConstants.TJC_PREFERENCE, 0);
		
		// Get the base color used for the UI.
		int baseColor = settings.getInt(TapjoyConstants.PREF_PRIMARY_COLOR, 0x000000);
		
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "baseColor: " + Integer.toHexString(baseColor));
		
		// Create the alternate colors based off the base color. 
		int buttonNormalColor = createColor(baseColor, -0x10, -0x10, -0x10);
		int buttonFocusedColor = createColor(baseColor, -0x40, -0x40, -0x40);
		int detailBGColor = createColor(baseColor, 0x40, 0x40, 0x40);
		int strokeColor = createColor(baseColor, -0x50, -0x50, -0x50);
		
		// Stroke sizes.
		float strokeWidthButton = 1.0f;
		float strokeWidthArea = 2.0f;
		
		// Gradient ranges.
		int button_start_mod = 0x30;
		int button_end_mod = 0x50;	
		int bg_start_mod = 0x20;
		int bg_end_mod = 0x20;	
		int detail_start_mod = 0x20;
		int detail_end_mod = 0x4;
		
		int statePressed = android.R.attr.state_pressed;
		int stateFocused = android.R.attr.state_focused;
		int stateSelected = android.R.attr.state_selected;
		
		// Create drawables for each state: focused, normal and selected.
		Drawable s1 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerLeft);
		Drawable n1 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerLeft);
		Drawable f1 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerLeft);
		
		Drawable s2 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerRight);
		Drawable n2 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerRight);
		Drawable f2 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerRight);
		
		Drawable s3 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerAll);
		Drawable n3 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		Drawable f3 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		
		Drawable s4 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerAll);
		Drawable n4 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		Drawable f4 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		
		Drawable s5 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerAll);
		Drawable n5 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		Drawable f5 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		
		Drawable s6 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerAll);
		Drawable n6 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		Drawable f6 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		
		Drawable s7 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, true, cornerAll);
		Drawable n7 = createGradient(buttonNormalColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		Drawable f7 = createGradient(buttonFocusedColor, button_start_mod, button_end_mod, strokeWidthButton, strokeColor, false, cornerAll);
		
		// Combine the drawables of various states into 1 drawable.
		StateListDrawable drawableLeft = new StateListDrawable();
		StateListDrawable drawableRight = new StateListDrawable();
		StateListDrawable drawableStoreGetMore = new StateListDrawable();
		StateListDrawable drawableMyItemsGetMore = new StateListDrawable();
		StateListDrawable drawableDetailGetMore = new StateListDrawable();
		StateListDrawable drawableGetThisItem = new StateListDrawable();
		StateListDrawable drawableRetryDetail = new StateListDrawable();
		
		drawableLeft.addState(new int[]{ statePressed}, s1);
		drawableLeft.addState(new int[]{ -stateFocused, -stateSelected}, n1);
		drawableLeft.addState(new int[]{ -stateFocused, stateSelected}, f1);
		
		drawableRight.addState(new int[]{ statePressed}, s2);
		drawableRight.addState(new int[]{ -stateFocused, -stateSelected}, n2);
		drawableRight.addState(new int[]{ -stateFocused, stateSelected}, f2);
		
		drawableStoreGetMore.addState(new int[]{ statePressed}, s3);
		drawableStoreGetMore.addState(new int[]{ -stateFocused, -stateSelected}, n3);
		drawableStoreGetMore.addState(new int[]{ -stateFocused, stateSelected}, f3);
		
		drawableMyItemsGetMore.addState(new int[]{ statePressed}, s4);
		drawableMyItemsGetMore.addState(new int[]{ -stateFocused, -stateSelected}, n4);
		drawableMyItemsGetMore.addState(new int[]{ -stateFocused, stateSelected}, f4);
		
		drawableDetailGetMore.addState(new int[]{ statePressed}, s5);
		drawableDetailGetMore.addState(new int[]{ -stateFocused, -stateSelected}, n5);
		drawableDetailGetMore.addState(new int[]{ -stateFocused, stateSelected}, f5);
		
		drawableGetThisItem.addState(new int[]{ statePressed}, s6);
		drawableGetThisItem.addState(new int[]{ -stateFocused, -stateSelected}, n6);
		drawableGetThisItem.addState(new int[]{ -stateFocused, stateSelected}, f6);
		
		drawableRetryDetail.addState(new int[]{ statePressed}, s7);
		drawableRetryDetail.addState(new int[]{ -stateFocused, -stateSelected}, n7);
		drawableRetryDetail.addState(new int[]{ -stateFocused, stateSelected}, f7);
		
		//--------------------------------------------------------------------------------
		// Take the background elements and assign our generated gradients to them.
		//--------------------------------------------------------------------------------
		LinearLayout vgBackground = (LinearLayout)findViewById(getResources().getIdentifier("vg_background", "id", getClientPackage()));
		LinearLayout vgStoreFooter = (LinearLayout)findViewById(getResources().getIdentifier("vg_store_footer", "id", getClientPackage()));
		LinearLayout vgItemFooter = (LinearLayout)findViewById(getResources().getIdentifier("vg_item_footer", "id", getClientPackage()));
		TextView vgDetailHeader = (TextView)findViewById(getResources().getIdentifier("vg_detail_header", "id", getClientPackage()));
		RelativeLayout vgDetailInfo = (RelativeLayout)findViewById(getResources().getIdentifier("vg_detail_info", "id", getClientPackage()));
		ScrollView vgDetailDesc = (ScrollView)findViewById(getResources().getIdentifier("vg_detail_desc", "id", getClientPackage()));
		LinearLayout vgDetailFooter = (LinearLayout)findViewById(getResources().getIdentifier("vg_detail_footer", "id", getClientPackage()));
		
		vgBackground.setBackgroundDrawable(createGradient(baseColor, bg_start_mod, bg_end_mod, 0, 0, false, cornerNone));
		vgStoreFooter.setBackgroundDrawable(createGradient(baseColor, bg_start_mod, bg_end_mod,0, 0, false, cornerNone));
		vgItemFooter.setBackgroundDrawable(createGradient(baseColor, bg_start_mod, bg_end_mod, 0, 0, false, cornerNone));
		vgDetailHeader.setBackgroundDrawable(createGradient(detailBGColor, detail_start_mod, detail_end_mod, strokeWidthArea, strokeColor, false, cornerAll));
		vgDetailInfo.setBackgroundDrawable(createGradient(detailBGColor, detail_start_mod, detail_end_mod, strokeWidthArea, strokeColor, true, cornerAll));
		vgDetailDesc.setBackgroundDrawable(createGradient(detailBGColor, detail_start_mod, detail_end_mod, strokeWidthArea, strokeColor, true, cornerAll));
		vgDetailFooter.setBackgroundDrawable(createGradient(detailBGColor, detail_start_mod, detail_end_mod, strokeWidthArea, strokeColor, false, cornerAll));
		
		//--------------------------------------------------------------------------------
		// Tab setup
		//--------------------------------------------------------------------------------
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Setting up tabs");
		tabs = (TabHost) findViewById(this.getResources().getIdentifier("VGTabHost", "id", getClientPackage()));
		tabs.setup();
		
		// Left tab button.
		TabHost.TabSpec storeTab = tabs.newTabSpec("tab1");
		storeTab.setContent(this.getResources().getIdentifier("Store", "id", getClientPackage()));
		
		Button button_left = new Button(this);
		button_left.setText("Available Items");
		button_left.setTextColor(Color.WHITE);
		button_left.setPadding(8, 8, 8, 8);
		button_left.setBackgroundDrawable(drawableLeft);
		button_left.setShadowLayer(2.0f, 2.0f, 2.0f, TJCVirtualGoods.this.getResources().getIdentifier("vg_shadow_color", "color", getClientPackage()));
		storeTab.setIndicator(button_left);		
		tabs.addTab(storeTab);
		
		// Right tab button.
		TabHost.TabSpec purchasedItemTab = tabs.newTabSpec("tab3");
		purchasedItemTab.setContent(this.getResources().getIdentifier("YourItem", "id", getClientPackage()));

		Button button_right = new Button(this);
		button_right.setText("My Items");
		button_right.setTextColor(Color.WHITE);
		button_right.setPadding(8, 8, 8, 8);
		button_right.setBackgroundDrawable(drawableRight);
		button_right.setShadowLayer(2.0f, 2.0f, 2.0f, TJCVirtualGoods.this.getResources().getIdentifier("vg_shadow_color", "color", getClientPackage()));
		purchasedItemTab.setIndicator(button_right);
		tabs.addTab(purchasedItemTab);
		//--------------------------------------------------------------------------------

		// Progress bars.
		storeProgress = (ProgressBar) findViewById(this.getResources().getIdentifier("StoreProgress", "id", getClientPackage()));
		purchasedItemProgress = (ProgressBar) findViewById(this.getResources().getIdentifier("YourItemProgress", "id", getClientPackage()));
		storeItemTable = (TableLayout) findViewById(this.getResources().getIdentifier("StoreTable", "id", getClientPackage()));
		purchasedItemTable = (TableLayout) findViewById(this.getResources().getIdentifier("YourItemTable", "id", getClientPackage()));

		// DIALOGS
		getItemDialog = new Dialog(this);
		getMoreItemDialog = new Dialog(this);
		itemDownloadedDialog = new Dialog(this);
		
		// DETAIL VIEW
		detailView = (LinearLayout) findViewById(this.getResources().getIdentifier("ItemDetail", "id", getClientPackage()));
		detailView.setVisibility(View.GONE);
		
		storeHeaderLeftTitle = (TextView) findViewById(this.getResources().getIdentifier("storeHeaderLeftTitle", "id", getClientPackage()));
		storeHeaderLeftTitle.setText("Item");

		detailErrorIcon = (ImageView) findViewById(this.getResources().getIdentifier("vg_detail_error_icon", "id", getClientPackage()));
		myItemsHeaderLeftTitle = (TextView) findViewById(this.getResources().getIdentifier("myItemsHeaderLeftTitle", "id", getClientPackage()));
		myItemsHeaderLeftTitle.setText("Item");

		storeHeaderRightTitle = (TextView) findViewById(this.getResources().getIdentifier("storeHeaderRightTitle", "id", getClientPackage()));
		storeHeaderRightTitle.setText(currencyName);

		myItemsHeaderRightTitle = (TextView) findViewById(this.getResources().getIdentifier("myItemsHeaderRightTitle", "id", getClientPackage()));
		myItemsHeaderRightTitle.setText(currencyName);
		
		vgFooterLeft = (TextView) findViewById(this.getResources().getIdentifier("VGFooterLeft", "id", getClientPackage()));
		vgFooterLeft.setText("Your " + currencyName + ": " + tapPoints);

		vgPurchasedFooterLeft = (TextView) findViewById(this.getResources().getIdentifier("VGYourItemFooterLeft", "id", getClientPackage()));
		vgPurchasedFooterLeft.setText("Your " + currencyName + ": " + tapPoints);

		vgDetailFooterLeft = (TextView) findViewById(this.getResources().getIdentifier("VGDetailFooterLeft", "id", getClientPackage()));
		vgDetailFooterLeft.setText("Your " + currencyName + ": " + tapPoints);

		//--------------------------------------------------------------------------------
		// BUTTONS
		//--------------------------------------------------------------------------------
		vgGetMoreBtn = (Button) findViewById(this.getResources().getIdentifier("VGGetMoreBtn", "id", getClientPackage()));
		vgGetMoreBtn.setText("Get more " + currencyName);
		vgGetMoreBtn.setBackgroundDrawable(drawableStoreGetMore);
		vgGetMoreBtn.setOnClickListener(this);

		vgPurchasedGetMoreBtn = (Button) findViewById(this.getResources().getIdentifier("VGYourItemGetMoreBtn", "id", getClientPackage()));
		vgPurchasedGetMoreBtn.setText("Get more " + currencyName);
		vgPurchasedGetMoreBtn.setBackgroundDrawable(drawableMyItemsGetMore);
		vgPurchasedGetMoreBtn.setOnClickListener(this);

		vgDetailGetMoreBtn = (Button) findViewById(this.getResources().getIdentifier("VGDetailGetMoreBtn", "id", getClientPackage()));
		vgDetailGetMoreBtn.setText("Get more " + currencyName);
		vgDetailGetMoreBtn.setBackgroundDrawable(drawableDetailGetMore);
		vgDetailGetMoreBtn.setOnClickListener(this);

		detailGetThisItemButton = (Button) findViewById(this.getResources().getIdentifier("vg_detail_action_button", "id", getClientPackage()));
		detailGetThisItemButton.setText("Get this item");
		detailGetThisItemButton.setBackgroundDrawable(drawableGetThisItem);
		
		detailRetryButton = (Button) findViewById(this.getResources().getIdentifier("RetryDetailBtn", "id", getClientPackage()));
		detailRetryButton.setText("Retry");
		detailRetryButton.setBackgroundDrawable(drawableRetryDetail);
		
		detailRetryButton.setOnClickListener(virtualGoodUtil.retryDetailClickListener);
		
		moreStoreItems = new Button(this);
		moreStoreItems.setTag("MoreStoreItemsButton");
		moreStoreItems.setOnClickListener(this);

		morePurchasedItem = new Button(this);
		morePurchasedItem.setTag("MorePurchasedItemsButton");
		morePurchasedItem.setOnClickListener(this);		
		//--------------------------------------------------------------------------------

		storeNoData = (TextView) findViewById(this.getResources().getIdentifier("StoreNoDataText", "id", getClientPackage()));
		yourItemNoData = (TextView) findViewById(this.getResources().getIdentifier("YourItemNoDataText", "id", getClientPackage()));
		
		tabs.setCurrentTab(0);
		ScrollView scrollView = (ScrollView) findViewById(this.getResources().getIdentifier("StoreLayout", "id", getClientPackage()));
		scrollView.setFillViewport(false);
		scrollView.setScrollContainer(false);		
		
		//--------------------------------------------------------------------------------
		// DETAIL VIEW
		detailDownloadStatusTextView = (TextView) findViewById(this.getResources().getIdentifier("vg_detail_download_status_text", "id", getClientPackage()));
		detailProgressBar = (ProgressBar) findViewById(this.getResources().getIdentifier("vg_detail_progress_bar", "id", getClientPackage()));
	}
	
	
	/**
	 * Creates a color based on the red, green and blue offsets.
	 * @category				Color
	 * @param baseColor			Base color to use.
	 * @param redOffset 		Offset from red.
	 * @param greenOffset 		Offset from green.
	 * @param blueOffset		Offset from blue.
	 * @return					The new color created.
	 */
	private static int createColor(int baseColor, int redOffset, int greenOffset, int blueOffset)
    {
		int color;
		
		int red = ((baseColor >> 16) & 0x0000FF) + redOffset;
		int green = ((baseColor >> 8) & 0x0000FF) + greenOffset;
		int blue = (baseColor & 0x0000FF) + blueOffset;
		
		// Check minimum bounds.
		if (red < 0)
		    red = 0;
		if (green < 0)
		    green = 0;
		if (blue < 0)
		    blue = 0;
		
		// Check maximum bounds.
		if (red > 0xFF)
		    red = 0xFF;
		if (green > 0xFF)
		    green = 0xFF;
		if (blue > 0xFF)
		    blue = 0xFF;
		
		color = Color.argb(0xFF, red, green, blue);
		
		return color;
    }
    
	
    /**
     * Creates a Drawable that has a gradient background.
     * @param baseColor			The base color to use for the gradient.
     * @param start_mod			The offset from the base color as the starting gradient color.
     * @param end_mod			The offset from the base color as the ending gradient color.
     * @param strokeWidth		The width of the stroke/border.
     * @param strokeColor		The color of the stroke/border.
     * @param invertOrientation	If true, colors from top to bottom, otherwise bottom to top.
     * @param cornerRadii		Contains the radius info for each corner.
     * @return					The Drawable gradient shape.
     */
    private static GradientDrawable createGradient(int baseColor, 
	    			int start_mod,
	    			int end_mod,
	    			float strokeWidth, 
	    			int strokeColor, 
	    			boolean invertOrientation,
	    			float [] cornerRadii)
    {
		GradientDrawable gradientDrawable = null;
		
		int red = ((baseColor >> 16) & 0x0000FF);
		int green = ((baseColor >> 8) & 0x0000FF);
		int blue = baseColor & 0x0000FF;
		
		int [] colors = new int[100];
		int length = colors.length;
		
		int red_start;
		int green_start;
		int blue_start;
		
		int delta;
		
		int r;
		int g;
		int b;
		int a;
		
		red_start = red - start_mod;
		green_start = green - start_mod;
		blue_start = blue - start_mod;
		
		// Check minimum bounds.
		if (red_start < 0)
		    red_start = 0;
		if (green_start < 0)
		    green_start = 0;
		if (blue_start < 0)
		    blue_start = 0;
		
		delta = (start_mod + end_mod)/length;
		
		// If the delta is zero, use one as a minimum or else it'll be a solid color.
		if (delta <= 0)
		    delta = 1;
		
//		TapjoyLog.i("COLOR", "start_mod: " + start_mod + ", end_mod: " + end_mod + ", total: " + (start_mod + end_mod));
//		TapjoyLog.i("COLOR", "delta: " + delta + ", length: " + length);
		
		a = 0xFF;
		r = red_start;
		g = green_start;
		b = blue_start;
		
		for (int i = 0; i < length; i++)
		{
		    r += delta;
		    g += delta;
		    b += delta;
		    
		    // Check maximum bounds.
		    if (r > 0xFF)
		    	r = 0xFF;
		    if (g > 0xFF)
		    	g = 0xFF;
		    if (b > 0xFF)
		    	b = 0xFF;
		    
		    int color = Color.argb(a, r, g, b);
			colors[i] = color;
		}
		
//		TapjoyLog.i("COLOR", "baseColor: " + Integer.toHexString(baseColor));
//		TapjoyLog.i("COLOR", "red: " + Integer.toHexString(red) + ", int: " + red);
//		TapjoyLog.i("COLOR", "green: " + Integer.toHexString(green) + ", int: " + green);
//		TapjoyLog.i("COLOR", "blue: " + Integer.toHexString(blue)+ ", int: " + blue);
		
		Orientation orientation = Orientation.BOTTOM_TOP;
		
		if (invertOrientation)
		    orientation = Orientation.TOP_BOTTOM;
		
		// Set the orientation to draw the gradient in.
		gradientDrawable = new GradientDrawable(orientation, colors);
		
		// Set the corner shape.
		gradientDrawable.setCornerRadii(cornerRadii);
		
		// Set the stroke if the width is greater than zero.
		if (strokeWidth > 0)
		    gradientDrawable.setStroke((int)strokeWidth, strokeColor);
		
		gradientDrawable.setGradientRadius(50);
		
		return gradientDrawable;
    }

    
    /**
     * Rebuilds the Store Item and Purchased Item tables.
     * Should be called after a purchased is made to refresh the data.
     * @return					N/A
     */
    private void syncAndRebuildAllTables()
    {
		syncAndRebuildStoreItemTable();
		syncAndRebuildPurchasedItemTable();		
    }

    /**
     * Downloads the store item data from the server and rebuilds the table in the view.
     */
    private void syncAndRebuildStoreItemTable()
    {
		// Clear the arrays.
		storeItemArray.clear();
		storeItemStart = 0;
		fetchAllStoreItems = new FetchAndBuildStoreItemsTask();
		fetchAllStoreItems.execute();
    }
    
    
    /**
     * Downloads the purchased item data from the server and rebuilds the table in the view.
     */
    public void syncAndRebuildPurchasedItemTable()
    {
		purchasedItemArray.clear();
		purchasedItemStart = 0;
		fetPurchasedVGItems = new FetchAndBuildPurchasedItemsTask();
		fetPurchasedVGItems.execute();		
    }
    
	
	/**
	 * Builds the row for the Store Item table.
	 * @param item					Virtual Good item data.
	 * @param i						
	 * @param rowId
	 * @param ctx
	 * @return						LinearLayout of the row.
	 */
	private LinearLayout buildStoreItemRow(VGStoreItem item, int i, int rowId, Context ctx)
	{
		LinearLayout vgItem = new LinearLayout(ctx);

		vgItem = (LinearLayout) View.inflate(ctx, ctx.getResources().getIdentifier("tapjoy_virtualgoods_row", "layout", getClientPackage()), null);
		vgItem.setId(rowId);
		vgItem.setOnClickListener((View.OnClickListener) ctx);
		vgItem.setBackgroundResource(ctx.getResources().getIdentifier("tapjoy_tablerowstates", "drawable", getClientPackage()));
		
		// Used to keep track of the row.  Not displayed.
		TextView index = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_index", "id", getClientPackage()));
		index.setText("" + i);
		
		ImageView itemIcon = (ImageView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_item_icon", "id", getClientPackage()));
		Drawable icon = item.getThumbImage();
		
		if (icon != null)
		{
			itemIcon.setImageDrawable(icon);
		}
		else if (item.getThumbImageUrl() != null && !item.getThumbImageUrl().equals(""))
		{
			Object[] obj = new Object[2];
			obj[0] = itemIcon;
			obj[1] = item;
			asyncTaskPool.addTask(new FetchItemIconTask(), obj);
		}

		// Virtual Good Name.
		TextView itemName = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_name", "id", getClientPackage()));
		itemName.setText(item.getName());

		// Type.
		TextView itemType = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_type", "id", getClientPackage()));
		itemType.setText(item.getVgStoreItemTypeName());

		// Attributes.
		TextView itemAttribute = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_attribute", "id", getClientPackage()));
		
		// If an attribute exists, get the first one.
		if (item.getVgStoreItemsAttributeValueList().size() > 0)
		{
			itemAttribute.setText(item.getVgStoreItemsAttributeValueList().get(0).getAttributeType() + ": "
					+ item.getVgStoreItemsAttributeValueList().get(0).getAttributeValue());
		}
		else
		{
			itemAttribute.setVisibility(View.GONE);
		}
		
		// Price
		TextView price = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_points_text", "id", getClientPackage()));
		String text = "" + item.getPrice();
		price.setText(text);
		price.setSelected(true);

		return vgItem;
	}

	
	/**
	 * Builds the row for the Purchase Table view. 
	 * @param item					Virtual good item data.
	 * @param i
	 * @param rowId
	 * @param ctx
	 * @return						LinearLayout of the row.
	 */
	private LinearLayout buildPurchasedItemRow(VGStoreItem item, int i, int rowId, Context ctx)
	{
		LinearLayout vgItem = new LinearLayout(ctx);

		vgItem = (LinearLayout) View.inflate(ctx, ctx.getResources().getIdentifier("tapjoy_virtualgoods_purchaseitems_row", "layout", getClientPackage()), null);
		vgItem.setId(rowId);
		vgItem.setOnClickListener((View.OnClickListener) ctx);
		vgItem.setBackgroundResource(ctx.getResources().getIdentifier("tapjoy_tablerowstates", "drawable", getClientPackage()));

		// Used to keep track of the row.  Not displayed.
		TextView index = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_index", "id", getClientPackage()));
		index.setText("" + i);
		
		ImageView itemIcon = (ImageView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_item_icon", "id", getClientPackage()));
		Drawable icon = item.getThumbImage();
		
		if (icon != null)
		{
			itemIcon.setImageDrawable(icon);
		}
		else if (item.getThumbImageUrl() != null && !item.getThumbImageUrl().equals(""))
		{
			Object[] obj = new Object[2];
			obj[0] = itemIcon;
			obj[1] = item;
			asyncTaskPool.addTask(new FetchItemIconTask(), obj);
		}

		// Virtual Good name.
		TextView itemName = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_name", "id", getClientPackage()));
		itemName.setText(item.getName());

		// Type.
		TextView itemType = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_type", "id", getClientPackage()));
		itemType.setText(item.getVgStoreItemTypeName());

		// Attributes.
		TextView itemAttribute = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_attribute", "id", getClientPackage()));
		
		// If an attribute exists, get the first one.
		if (item.getVgStoreItemsAttributeValueList().size() > 0)
		{
			itemAttribute.setText(item.getVgStoreItemsAttributeValueList().get(0).getAttributeType() + ": "
					+ item.getVgStoreItemsAttributeValueList().get(0).getAttributeValue());
		}
		else
		{
			itemAttribute.setVisibility(View.GONE);
		}

		// Points.
		TextView price = (TextView) vgItem.findViewById(ctx.getResources().getIdentifier("vg_row_points_text", "id", getClientPackage()));
		String text = "" + item.getPrice();
		price.setText(text);
		price.setSelected(true);

		return vgItem;
	}
	
	
	/**
	 * Builds the Store Item Table View.
	 * @param start
	 * @param ctx
	 * @return						N/A
	 */
	private void showStoreItemTableView(int start, Context ctx)
	{
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "buildStoreItemTable start: " + start);
		
		// Reset the view if we're building the table from the first element.
		// Otherwise it's a "SHOW X MORE" call and we need to append it to our list.
		if (start == 0)
			storeItemTable.removeAllViews();
		
		// If no VG available
		if (storeItemArray.size() == 0)
		{
			storeNoData.setVisibility(View.VISIBLE);
			return;
		}
		else
		{
			storeNoData.setVisibility(View.GONE);
		}

		// Populate the table with Items.
		for (int i = start; i < storeItemArray.size(); i++)
		{
			LinearLayout storeItemRow = buildStoreItemRow(storeItemArray.get(i), i, STORE_ITEM, ctx);
			
			// Create separator.
			View line = new View(this);
			line.setBackgroundResource(this.getResources().getIdentifier("tapjoy_gradientline", "drawable", getClientPackage()));
			if (storeItemArray.get(i).isShown() == false)
			{
				storeItemRow.setVisibility(View.GONE);
				line.setVisibility(View.GONE);
			}

			storeItemTable.addView(storeItemRow);
			
			// Draw separator after each Item.
			storeItemTable.addView(line, new LayoutParams(LayoutParams.FILL_PARENT, 1));
		}
		
		updateHeaderFooters(this);
	}
	
	
	/**
	 * Builds the Purchased Items Table view.
	 * @param start
	 * @param ctx
	 * @return						N/A
	 */
	private void showPurchasedStoreItemTableView(int start, Context ctx)
	{
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "buildPurchasedStoreItemTable");
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "purchasedItemArray.size(): " + purchasedItemArray.size());
		
		// Reset the view if we're building the table from the first element.
		// Otherwise it's a "SHOW X MORE" call and we need to append it to our list.
		if (start == 0)
			purchasedItemTable.removeAllViews();
		
		// If no VG available
		if (purchasedItemArray.size() == 0)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "No items to display");
			yourItemNoData.setVisibility(View.VISIBLE);
			
			return;
		}
		else
		{
			yourItemNoData.setVisibility(View.GONE);
		}
		
		// Populate the table with Items
		for (int i = start; i < purchasedItemArray.size(); i++)
		{
			purchasedItemTable.addView(buildPurchasedItemRow(purchasedItemArray.get(i), i, PURCHASED_ITEM, ctx));
			
			// Draw Line after each Item
			View line = new View(this);
			line.setBackgroundResource(this.getResources().getIdentifier("tapjoy_gradientline", "drawable", getClientPackage()));
			purchasedItemTable.addView(line, new LayoutParams(LayoutParams.FILL_PARENT, 1));
		}
		
		// Is this table non-empty?  Then handle the "SHOW X MORE..." button at the bottom.
		if (purchasedItemArray.size() > 0)
			showMoreStoreItemsButton(VGStoreItem.availableItemsMoreDataAvailable);
		
		updateHeaderFooters(this);
	}
	
	
	/**
     * Updates the "SHOW X MORE..." button on the store items list.
     * @param moreData
     */
    private void showMoreStoreItemsButton(int moreData)
	{
		// remove more data button view from table and add it at the end if
		// needed
		View moreDataButtonView = storeItemTable.findViewWithTag("MoreStoreItemsButton");
		
		if (moreDataButtonView != null)
		{
			storeItemTable.removeView(moreDataButtonView);
			moreStoreItems = (Button) moreDataButtonView;
		}

		// if more data available show button to fetch more records
		if (moreData > 0)
		{
			if (moreData > VGStoreItem.MAX_ITEMS)
				moreStoreItems.setText("Show " + VGStoreItem.MAX_ITEMS + " more...");
			else
				moreStoreItems.setText("Show " + moreData + " more...");

			storeItemTable.addView(moreStoreItems);
		}
	}
    
    
	/**
	 * Creates the Item Detail screen.
	 * @param item					Store Item data.
	 * @param itemType
	 * @param v
	 * @param index
	 */
	private void showItemDetail(VGStoreItem item, int itemType, View v, int index)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "********************");
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "showItemDetail");
		
		ImageView itemIcon = (ImageView) TJCVirtualGoods.this.findViewById(this.getResources().getIdentifier("vg_detail_item_icon", "id", getClientPackage()));
		
		// Remove previous icon if set already.
		itemIcon.setImageDrawable(null); 
		
		if (item.getThumbImage() != null)
		{
			itemIcon.setImageDrawable(item.getThumbImage());
		}
		else
		if (item.getThumbImageUrl() != null && !item.getThumbImageUrl().equals(""))
		{
			Object[] obj = new Object[2];
			obj[0] = itemIcon;
			obj[1] = item;
		}
		
		// Item name.
		TextView itemName = (TextView) this.findViewById(this.getResources().getIdentifier("vg_detail_item_name_text", "id", getClientPackage()));
		itemName.setText(item.getName());

		// Type.
		TextView vgType = (TextView) findViewById(this.getResources().getIdentifier("vg_detail_item_type_text", "id", getClientPackage()));
		vgType.setText(item.getVgStoreItemTypeName());

		// Price.
		TextView price = (TextView) findViewById(this.getResources().getIdentifier("vg_detail_points_text", "id", getClientPackage()));
		price.setText(("" + item.getPrice()));
		price.setSelected(true);
		
		// Currency Name.
		TextView currency = (TextView) findViewById(this.getResources().getIdentifier("vg_detail_currency_text", "id", getClientPackage()));
		currency.setText(currencyName);

		LinearLayout scrollLayout = (LinearLayout) findViewById(this.getResources().getIdentifier("ScrollLayout", "id", getClientPackage()));
		scrollLayout.removeAllViews();

		ArrayList<VGStoreItemAttributeValue> attributes = item.getVgStoreItemsAttributeValueList();

		int color = getResources().getColor(getResources().getIdentifier("virtual_goods_details_text_color", "color", getClientPackage()));
		
		// Loop through the attributes and add them to the view.
		for (int i = 0; i < attributes.size(); i++)
		{
			TextView attr = new TextView(this);
			String attrType = attributes.get(i).getAttributeType();

			// If this is quantity, then use the getNumberOwned.
			// This is because when we download an item, we need to update the "quantity: X" by 1
			// on the detail page.  The detailDescQuantity TextView allows us to easily do that.
			if (attrType.equals("quantity"))
			{
				detailDescQuantity = new TextView(this);
				
				detailDescQuantity.setText(attrType + ": " + item.getNumberOwned());
				detailDescQuantity.setPadding(0, 0, 0, 10);
				
				detailDescQuantity.setTextColor(color);
				scrollLayout.addView(detailDescQuantity);
			}
			// All other attributes.
			else
			{
				attr.setText(attrType + ": " + attributes.get(i).getAttributeValue());
				attr.setPadding(0, 0, 0, 10);
				
				attr.setTextColor(color);
				scrollLayout.addView(attr);
			}
		}

		// Add the description to the view if it exists.
		if (item.getDescription() != null && !item.getDescription().equals(""))
		{
			TextView des = new TextView(this);
			des.setText("Description:");
			des.setTypeface(Typeface.DEFAULT_BOLD);
			des.setPadding(0, 0, 0, 3);
			des.setTextColor(color);
			scrollLayout.addView(des);

			TextView description = new TextView(this);
			description.setText(item.getDescription());
			description.setTextColor(color);
			scrollLayout.addView(description);
		}
		
		item.getVgStoreItemID();
		final String storeItemName = item.getName();
		final String storeItemPrice = "" + item.getPrice();
		currentVgStoreItem = item;
		
		switch (itemType)
		{
			case STORE_ITEM:
				// Add the "GET THIS ITEM" Button.
				detailGetThisItemButton.setOnClickListener(new View.OnClickListener()
				{
					public void onClick(View view)
					{
						getItemDialog = new AlertDialog.Builder(TJCVirtualGoods.this).setTitle("").setMessage(
								"Are you sure you want " + " \n" + storeItemName + " for " + storeItemPrice + " " + currencyName + "?").setPositiveButton("OK",
										new DialogInterface.OnClickListener()
								{
									public void onClick(DialogInterface dialog, int whichButton)
									{
										purchaseVirtualGood();
									}
								}).setNegativeButton("Cancel", new DialogInterface.OnClickListener()
								{
									public void onClick(DialogInterface dialog, int whichButton)
									{
										
									}
								}).create();
						try
						{
							getItemDialog.show();
						}
						catch (BadTokenException e)
						{
							// Activity is not visible so dialog should also be not visible
						}
					}
				});
				
				if (detailGetThisItemButton.getBackground() == null)
					detailGetThisItemButton.setText("Get this item");
				
				detailGetThisItemButton.setVisibility(View.VISIBLE);
				
				detailDownloadStatusTextView.setVisibility(View.GONE);
				detailProgressBar.setVisibility(View.GONE);
				detailErrorIcon.setVisibility(View.GONE);
				detailRetryButton.setVisibility(View.GONE);
				break;
				
			case PURCHASED_ITEM:
				// Hide the "GET THIS ITEM" Button.
				detailGetThisItemButton.setVisibility(View.GONE);
				
				String itemID = item.getVgStoreItemID();
				
				TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "virtualGoodsToDownload size: " + virtualGoodUtil.virtualGoodsToDownload.size());
				
				// Is this item downloading or in progress ?
				if (virtualGoodUtil.virtualGoodsToDownload.containsKey(itemID))
				{
					TJCVirtualGoodUtil.DownloadVirtualGoodTask activeVGDownload = virtualGoodUtil.virtualGoodsToDownload.get(itemID); 
					
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "showing detail view of an download in progress... " + item.getName() + ", status: " + activeVGDownload.virtualGoodDownloadStatus);
					
					updateDetailViewFromDownloader(activeVGDownload);
				}
				// Download completed.
				else
				{
					detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_COMPLETED); 
					detailDownloadStatusTextView.setVisibility(View.VISIBLE);
					
					detailProgressBar.setVisibility(View.GONE);
					detailErrorIcon.setVisibility(View.GONE);
					detailRetryButton.setVisibility(View.GONE);
				}
				
				detailViewStoreID = itemID;
				isPurchasedItemDetailView = true;
				
				virtualGoodUtil.setDetailIndex(index);
				
				break;
		}
		
		isItemDetailView = true;
	}
	
	
	/**
	 * Handles the "You successfully purchased ..." dialog popup.
	 * From this dialog, the virtual good will start downloading it's data from the server.
	 * @param errorMessage
	 * @param storeItem
	 */
	private void showItemDownloadedDialog(String errorMessage, final VGStoreItem storeItem)
	{
		// Download should start whether OK or BACK was pressed.
		
		itemDownloadedDialog = new AlertDialog.Builder(TJCVirtualGoods.this).setTitle("").setMessage(errorMessage).
			// Handle OK button.
			setPositiveButton("OK", new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int whichButton)
				{
					startDownloadingPurchasedVG(storeItem);
				}
			// Handle BACK button.
			}).setOnCancelListener(new DialogInterface.OnCancelListener()
			{
				public void onCancel(DialogInterface dialog)
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "onCANCEL");
					startDownloadingPurchasedVG(storeItem);
				}
			})
		.create();
		
		try
		{
			itemDownloadedDialog.show();
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, "showItemDownloadedDialog: " + e.toString());
		}
	}

	
	private void showGetStoreItemsNetworkErrorDialog()
	{
		this.runOnUiThread(new Runnable()
		{
			public void run()
			{
				//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "!!! showFetchAllItemsDialog !!!");
				
				AlertDialog.Builder builder = new AlertDialog.Builder(TJCVirtualGoods.this);

				builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener()
				{
					public void onClick(DialogInterface dialog, int id)
					{
						dialog.cancel();
					}
				});

				builder.setPositiveButton("Retry", new DialogInterface.OnClickListener()
				{
					public void onClick(DialogInterface dialog, int id)
					{
						fetchAllStoreItems = new FetchAndBuildStoreItemsTask();
						fetchAllStoreItems.execute();

						dialog.cancel();
					}
				});
				builder.setMessage(NETWORK_DOWN_MESSAGE);

				allItemAlert = builder.create();
				
				try
				{
					allItemAlert.show();
				}
				catch (Exception e)
				{
					// activity is not visible so dialog should also be not visible
				}
			}
		});
	}

	
	private void showGetPurchasedItemsNetworkErrorDialog(VGStoreItem vgStoreItem)
	{
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "!!! showExceptionDialog !!!");
		
		currentVgStoreItem = vgStoreItem;
		exceptionDialog = new AlertDialog.Builder(TJCVirtualGoods.this).setMessage(NETWORK_DOWN_MESSAGE).setNegativeButton("Cancel",
				new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int id)
			{
				dialog.cancel();
			}
		}).setPositiveButton("Retry", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int id)
			{
				purchaseVirtualGood();
			}
		}).create();
		try
		{
			if (exceptionDialog != null)// && dialogStatus != null)
			{
				exceptionDialog.show();
			}
		}
		catch (BadTokenException e)
		{
			// activity is not visible so dialog should also be not visible
		}
	}
	
	
	private void showCachedPurchasedItemsDialog()
	{
		this.runOnUiThread(new Runnable()
		{
			public void run()
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(TJCVirtualGoods.this);
				builder.setTitle("Can't load my items");
				
				builder.setPositiveButton("OK", new DialogInterface.OnClickListener()
				{
					public void onClick(DialogInterface dialog, int id)
					{
						dialog.cancel();
					}
				});

				builder.setMessage("Service is unreachable.\nOnly showing downloaded items.");
				AlertDialog alert = builder.create();
				
				try
				{
					alert.show();
				}
				catch (BadTokenException e)
				{
					// activity is not visible so dialog should also be not visible
				}
			}
		});
	}

	
	private void updateHeaderFooters(Context ctx)
	{
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "updateHeaderFooters");
		
		userID = TJCVirtualGoodsData.getUserID();
		currencyName = TJCVirtualGoodsData.getCurrencyName();
		tapPoints = TJCVirtualGoodsData.getPoints();
		
		storeHeaderRightTitle.setText(currencyName);
		myItemsHeaderRightTitle.setText(currencyName);
		vgFooterLeft.setText("Your " + currencyName + ": " + tapPoints);
		vgPurchasedFooterLeft.setText("Your " + currencyName + ": " + tapPoints);
		vgDetailFooterLeft.setText("Your " + currencyName + ": " + tapPoints);
		
		if (vgGetMoreBtn.getBackground() == null)
			vgGetMoreBtn.setText("Get more " + currencyName);
		
		if (vgPurchasedGetMoreBtn.getBackground() == null)
			vgPurchasedGetMoreBtn.setText("Get more " + currencyName);
		
		if (vgDetailGetMoreBtn.getBackground() == null)
			vgDetailGetMoreBtn.setText("Get more " + currencyName);
		
		//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "updateHeaderFooters DONE");
	}
	
	
	/**
	 * Checks for purchased virtual goods that need to be downloaded.
	 * @param moreData
	 * @param ctx
	 */
	private void checkPurchasedVGForDownload(int moreData, Context ctx)
	{
		// remove more data button view from table and add it at the end if
		// needed
		View moreDataButtonView = purchasedItemTable.findViewWithTag("MorePurchasedItemsButton");
		if (moreDataButtonView != null)
		{
			purchasedItemTable.removeView(moreDataButtonView);
			morePurchasedItem = (Button) moreDataButtonView;
		}
		
		// if more data available show button to fetch more records
		if (moreData > 0)
		{
			if (moreData > VGStoreItem.MAX_ITEMS)
				morePurchasedItem.setText("Show " + VGStoreItem.MAX_ITEMS + " more...");
			else
				morePurchasedItem.setText("Show " + moreData + " more...");
			purchasedItemTable.addView(morePurchasedItem);
		}

		// Check for virtual goods to download?
		if (checkPurchased)
		{
			checkPurchased = false;
			
			// Checks for purchased virtual goods that we need to download.
			virtualGoodUtil.downLoadPurcahasedVirtualGood(purchasedItemArray, purchasedItemTable, purchasedItemStart + purchasedThroughAvailableItem);
		}
	}
	
	
	private void purchaseVirtualGood()
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "callGetPurchasedVGItems");
		
		getPurchasedVGItems = new PurchaseVirtualGoodTask();
		
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Processing your request...");
		
		try
		{
			progressDialog = ProgressDialog.show(TJCVirtualGoods.this, "", "Processing your request ...", true);
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, "exception: " + e.toString());
		}
		
		try
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "getPurchasedVGItems...");
			getPurchasedVGItems.execute(currentVgStoreItem);
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, "exception: " + e.toString());
			progressDialog.cancel();
			showGetPurchasedItemsNetworkErrorDialog(currentVgStoreItem);
		}
	}

	
	/**
	 * Starts downloading a successfully purchased virtual good.
	 * @param storeItem			The store item purchased.
	 */
	private void startDownloadingPurchasedVG(VGStoreItem storeItem)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "--------------------");
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "SUCCESSFUL PURCHASE");
		
		// DEBUG
		// Free memory debug.
		File path = Environment.getDataDirectory();
		StatFs stat = new StatFs(path.getPath());
		long free_memory = stat.getAvailableBlocks() * stat.getBlockSize();
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "FREE INTERNAL MEMORY: " + (free_memory/1024) + " KB");
		
		path = Environment.getExternalStorageDirectory();
		stat = new StatFs(path.getPath());
		free_memory = stat.getAvailableBlocks() * stat.getBlockSize();
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "FREE EXTERNAL MEMORY: " + (free_memory/1024) + " KB");
		
		// If we have already purchased this virtual good, just update the quantity and don't download it again.
		for (int i = 0; i < purchasedItemArray.size(); i++)
		{
			VGStoreItem purchasedItem = purchasedItemArray.get(i);
			
			if (purchasedItem.getVgStoreItemID().equals(storeItem.getVgStoreItemID()))
			{
				// Update the quantity temporarily so it appears correctly on the detail view.
				int owned = purchasedItem.getNumberOwned();
				purchasedItem.setNumberOwned(owned+1);
				TJCVirtualGoods.detailDescQuantity.setText("quantity: " + (owned+1));
				
				TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "ALREADY DOWNLOADED... updating quantity only");
				
				// Hide the "GET THIS ITEM" button since this is supposed to be the purchased view now.
				detailGetThisItemButton.setVisibility(View.GONE);
				detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_COMPLETED); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				return;
			}
		}
		
		// Set these flags so the detailView can be updated.
		isPurchasedItemDetailView = true;
		detailViewStoreID = storeItem.getVgStoreItemID();

		downloadStarted = true;
		storeItemArray.indexOf(storeItem);

		storeItem.setShown(false);
		
		// Start downloading and add it to My Items
		TJCVirtualGoodUtil.DownloadVirtualGoodTask downloadVirtualGood = virtualGoodUtil.new DownloadVirtualGoodTask();
		
		if (virtualGoodUtil.virtualGoodsToDownload.get(storeItem.getVgStoreItemID()) == null)
		{
			virtualGoodUtil.virtualGoodsToDownload.put(storeItem.getVgStoreItemID(), downloadVirtualGood);
			purchasedThroughAvailableItem++;

			View moreDataButtonView = null;
			if (purchasedItemArray.size() > 0)
			{
				// remove the moredata button at the end
				moreDataButtonView = purchasedItemTable.findViewWithTag("MorePurchasedItemsButton");
				if (moreDataButtonView != null)
				{
					purchasedItemTable.removeView(moreDataButtonView);
				}
			}
			
			//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "adding StoreItem to array: " + storeItem.name + ", #: " + storeItem.numberOwned);
			purchasedItemArray.add(storeItem);
			
			if (virtualGoodUtil.purchaseItems == null)
			{
				virtualGoodUtil.purchaseItems = purchasedItemArray;
				yourItemNoData.setVisibility(View.GONE);
			}
			
			int index = purchasedItemArray.indexOf(storeItem);
			
			// Update the quantity temporarily so it appears correctly on the detail view.
			int owned = purchasedItemArray.get(index).getNumberOwned();
			purchasedItemArray.get(index).setNumberOwned(owned+1);
			TJCVirtualGoods.detailDescQuantity.setText("quantity: " + (owned+1));
			
			//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "owned: " + owned);
			
			LinearLayout newPurchasedItem = buildPurchasedItemRow(storeItem, index, PURCHASED_ITEM, TJCVirtualGoods.this);
			purchasedItemTable.addView(newPurchasedItem);

			View line = new View(TJCVirtualGoods.this);
			line.setBackgroundResource(TJCVirtualGoods.this.getResources().getIdentifier("tapjoy_gradientline", "drawable", getClientPackage()));
			purchasedItemTable.addView(line, new LayoutParams(LayoutParams.FILL_PARENT, 1));

			if (moreDataButtonView != null)
				purchasedItemTable.addView((Button) moreDataButtonView);
			
			TextView indexTextView = (TextView) newPurchasedItem.findViewById(ctx.getResources().getIdentifier("vg_row_index", "id", getClientPackage()));
			indexTextView.setText(index + "");
		
			downloadVirtualGood.localProgressBar = (ProgressBar) newPurchasedItem.findViewById(ctx.getResources().getIdentifier("vg_row_progress_bar", "id", getClientPackage()));
			
			detailGetThisItemButton.setVisibility(View.GONE);
			detailProgressBar.setVisibility(View.VISIBLE);
			
			downloadVirtualGood.localDownloadStatusText =  (TextView) newPurchasedItem.findViewById(ctx.getResources().getIdentifier("vg_row_download_status_text", "id", getClientPackage()));
			downloadVirtualGood.localDownloadStatusText.setText(TAPJOY_DOWNLOAD_PENDING);

			downloadVirtualGood.localRetryButton = (Button) newPurchasedItem.findViewById(ctx.getResources().getIdentifier("vg_row_retry_button", "id", getClientPackage()));
			downloadVirtualGood.localErrorIcon = (ImageView) newPurchasedItem.findViewById(ctx.getResources().getIdentifier("vg_row_error_icon", "id", getClientPackage()));;

			downloadVirtualGood.localRetryButton.setOnClickListener(virtualGoodUtil.retryClickListener);
			downloadVirtualGood.localErrorIcon.setOnClickListener(virtualGoodUtil.errorMsgClickListener);
			
			virtualGoodUtil.setDetailIndex(index);
			updateDetailViewFromDownloader(downloadVirtualGood);
			
			TJCVirtualGoodUtil.addTask(downloadVirtualGood, storeItem);
		}
		
		itemDownloadedDialog.dismiss();
	}
	
	
	public static String getClientPackage() 
	{
		return clientPackage;
	}
	
	
	/**
	 * Updates the progress bar on the detail view.
	 * @param progress						Progress percentage.
	 */
	public static void updateDetailProgressBar(int progress)
	{		
		if (detailProgressBar != null)
		{
			detailDownloadStatusTextView.setText("Downloading... " + progress + "%"); 
			detailDownloadStatusTextView.setVisibility(View.VISIBLE); 
			
			detailProgressBar.setProgress(progress);
			detailProgressBar.setVisibility(View.VISIBLE);
		}
	}
	
	
	/**
	 * Updates the detail view of an in-progress downloading depending on the state of the virtual good task. 
	 * @param task							Virtual Good download task to show the detail view.
	 */
	public static void updateDetailViewFromDownloader(TJCVirtualGoodUtil.DownloadVirtualGoodTask task)
	{
		int state = task.virtualGoodDownloadStatus;
		
		switch (state)
		{
			//--------------------------------------------------------------------------------
			case VG_STATUS_DOWNLOAD_INIT:				
				detailDownloadStatusTextView.setText("Downloading... " + task.progressPercent + "%"); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				detailProgressBar.setProgress(task.progressPercent);
				
				detailErrorIcon.setVisibility(View.GONE);
				detailRetryButton.setVisibility(View.GONE);
				detailProgressBar.setVisibility(View.VISIBLE);
				break;
			
			//--------------------------------------------------------------------------------
			case VG_STATUS_DOWNLOAD_PENDING:
				detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_PENDING); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				detailProgressBar.setVisibility(View.GONE);
				detailErrorIcon.setVisibility(View.GONE);
				detailRetryButton.setVisibility(View.GONE);
				break;
			
			//--------------------------------------------------------------------------------
			case VG_STATUS_DOWNLOAD_SUCCESS_TO_SD_CARD:
			case VG_STATUS_DOWNLOAD_SUCCESS_TO_PHONE:
				detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_COMPLETED); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				detailProgressBar.setVisibility(View.GONE);
				detailErrorIcon.setVisibility(View.GONE);
				detailRetryButton.setVisibility(View.GONE);
				break;
			
			//--------------------------------------------------------------------------------
			case VG_STATUS_NETWORK_ERROR_SD_CARD:
			case VG_STATUS_NETWORK_ERROR_PHONE:
				detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_FAILED); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				detailProgressBar.setVisibility(View.GONE);
				detailErrorIcon.setVisibility(View.VISIBLE);
				detailRetryButton.setVisibility(View.VISIBLE);
				break;
				
			//--------------------------------------------------------------------------------
			case VG_STATUS_INSUFFICIENT_DISK_SPACE_SD_CARD:
			case VG_STATUS_INSUFFICIENT_DISK_SPACE_PHONE:
				detailDownloadStatusTextView.setText(TAPJOY_DOWNLOAD_FAILED); 
				detailDownloadStatusTextView.setVisibility(View.VISIBLE);
				
				detailProgressBar.setVisibility(View.GONE);
				detailErrorIcon.setVisibility(View.VISIBLE);
				detailRetryButton.setVisibility(View.VISIBLE);
				break;
		}
	}
	
	
	public static void setVirtualGoodDownloadListener(TapjoyDownloadListener listener)
	{
		tapjoyVGDownloadListener = listener;
	}
	
	
	public static TapjoyDownloadListener getVirtualGoodDownloadListener()
	{
		return tapjoyVGDownloadListener;
	}
	
	
	public static void setVirtualGoodsFocusListener(FocusListener listener)
	{
		tapjoyVGFocusListener = listener;
	}
	
	
	public static FocusListener getVirtualGoodsFocusListener()
	{
		return tapjoyVGFocusListener;
	}
	
	
	private void showOffers()
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Showing offers (userID = " + userID + ")");
		
		Intent offersIntent = new Intent(this, com.tapjoy.TJCOffersWebView.class);
		offersIntent.putExtra(TapjoyConstants.EXTRA_USER_ID, userID);
		offersIntent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, urlParams);
		this.startActivity(offersIntent);
	}
	
	//================================================================================
	// TASKS
	//================================================================================
	
	
	/**
	 * Fetch available virtual good store items from the server and builds the store item table view.
	 */
	private class FetchAndBuildStoreItemsTask extends AsyncTask<Void, Void, ArrayList<VGStoreItem>>
	{
		@Override
		protected void onCancelled()
		{
			int size = storeItemArray.size();
			for (int i = 0; i < size; i++)
			{
				if (storeItemArray.get(i).getThumbImage() != null)
					((BitmapDrawable) storeItemArray.get(i).getThumbImage()).getBitmap().recycle();
			}
		}

		protected void onPreExecute()
		{			
			// Move the loading spinner to the front and make it visible.
			storeProgress.bringToFront();
			storeProgress.setVisibility(View.VISIBLE);
			storeNoData.setVisibility(View.GONE);
		}

		protected ArrayList<VGStoreItem> doInBackground(Void... nothing)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Fetching All Store Items");

			try
			{
				String response = tapjoyVGConnection.getAllStoreItemsFromServer(storeItemStart, VGStoreItem.MAX_ITEMS);
				
				if (response != null && response.length() > 0)
					return TJCVirtualGoodsData.parseVGItemListResponse(response, VGStoreItem.STORE_ITEM, TJCVirtualGoods.this);
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, "exception: " + e.toString());
			}

			return null;
		}

		protected void onPostExecute(ArrayList<VGStoreItem> result)
		{
			// Remove the loading spinner.
			storeProgress.setVisibility(View.GONE);
			
			moreStoreItems.setEnabled(true);

			// NO DATA
			if (result == null)
			{
				if (storeItemArray.size() < 1)
					storeNoData.setVisibility(View.VISIBLE);
				
				//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "don't display popup");
				showGetStoreItemsNetworkErrorDialog();
			}
			else
			{
				int start = storeItemArray.size();
				storeItemArray.addAll(result);

				showStoreItemTableView(start, ctx);
			} 
		}
	}

	/**
	 * Fetch virtual good icons from the server.
	 * 
	 *
	 */
	private class FetchItemIconTask extends AsyncTask<Object, Void, Drawable>
	{
		private ImageView itemIcon;
		private VGStoreItem item;

		protected Drawable doInBackground(Object... obj)
		{
			itemIcon = (ImageView) obj[0];
			item = (VGStoreItem) obj[1];
			
			try
			{
				URL fileURL = new URL(item.getThumbImageUrl());
				URLConnection connection = fileURL.openConnection();
				connection.setConnectTimeout(15000);
		        connection.setReadTimeout(30000);
				connection.connect();
				
				InputStream is = new java.io.BufferedInputStream(connection.getInputStream());
				Drawable icon = Drawable.createFromStream(is, "src");
				is.close();
				return icon;
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, "FetchItemIconTask error: " + e.toString());
				return null;
			}
		}

		protected void onPostExecute(Drawable result)
		{
			asyncTaskPool.removeAndExecuteNext(this);
			
			item.setThumbImage(result);
			itemIcon.setImageDrawable(result);
		}
	}


	/**
	 * Fetch purchased virtual good items from the server and builds the "My Items" table view.
	 * 
	 *
	 */
	private class FetchAndBuildPurchasedItemsTask extends AsyncTask<Void, Void, ArrayList<VGStoreItem>>
	{
		@Override
		protected void onCancelled()
		{
			int size = purchasedItemArray.size();
			for (int i = 0; i < size; i++)
			{
				if (purchasedItemArray.get(i).getThumbImage() != null)
					((BitmapDrawable) purchasedItemArray.get(i).getThumbImage()).getBitmap().recycle();
			}
		}

		protected void onPreExecute()
		{
			// Bring loading spinner to the front and visible.
			purchasedItemProgress.bringToFront();
			purchasedItemProgress.setVisibility(View.VISIBLE);
			
			yourItemNoData.setVisibility(View.GONE);
		}

		protected ArrayList<VGStoreItem> doInBackground(Void... nothing)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Fetching All Purchased Items");
			
			try
			{
				String response = tapjoyVGConnection.getAllPurchasedItemsFromServer(purchasedItemStart, VGStoreItem.MAX_ITEMS);
				
				if (response != null && response.length() > 0)
					return TJCVirtualGoodsData.parseVGItemListResponse(response, VGStoreItem.PURCHASED_ITEM, TJCVirtualGoods.this);
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY_VIRTUAL_GOODS, e.toString());
			}
			return null;
		}

		protected void onPostExecute(ArrayList<VGStoreItem> result)
		{
			// Remove the loading spinner.
			purchasedItemProgress.setVisibility(View.GONE);
			morePurchasedItem.setEnabled(true);

			// No DATA
			if (result == null)
			{
				// In case of exception fetch cached results. and remove existing table views
				purchasedItemTable.removeAllViews();
				purchasedItemArray.clear();
				purchasedItemArray.addAll(TJCVirtualGoodsData.getPurchasedItems(ctx));

				virtualGoodUtil.virtualGoodsToDownload.clear();
				
				VGStoreItem.purchasedItemsMoreDataAvailable = 0;
				showCachedPurchasedItemsDialog();
				showPurchasedStoreItemTableView(0, ctx);
			}
			else
			{
				int start = purchasedItemArray.size();
				purchasedItemArray.addAll(result);

				showPurchasedStoreItemTableView(start, ctx);

				if (result.size() > 0)
				{
					checkPurchased = true;
					checkPurchasedVGForDownload(VGStoreItem.purchasedItemsMoreDataAvailable, ctx);
				}

				if (purchasedItemArray.size() < 1)
					yourItemNoData.setVisibility(View.VISIBLE);
			}
		}
	}

	
	/**
	 * Purchases the specified virtual good from the server.  If successful, it will display a popup dialog informing the user
	 * that the virtual good has been successfully purchased.
	 * 
	 *
	 */
	private class PurchaseVirtualGoodTask extends AsyncTask<VGStoreItem, Void, VGStoreItem>
	{		
		String responseMessage = null;
		boolean networkTimeout = false;
		boolean successfulPurchase = false;

		protected VGStoreItem doInBackground(VGStoreItem... storeItem)
		{
			try
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "virtual_good_id: " + storeItem[0].getVgStoreItemID());
				
				String response = tapjoyVGConnection.purchaseVGFromServer(storeItem[0].getVgStoreItemID());
				
				if (response != null && response.length() > 0)
				{	
					ArrayList <String> resultList = TJCVirtualGoodsData.parsePurchaseVGWithCurrencyResponse(response, TJCVirtualGoods.this);
					
					if (resultList != null && resultList.size() > 0)
					{
						if (!((String) resultList.get(2)).equals(""))
						{
							networkTimeout = true;
						}
						
						responseMessage = (String) resultList.get(0);
						
						if (resultList.get(1).equals("false"))
						{
							successfulPurchase = false;
						}
						else
						{
							successfulPurchase = true;
						}
					}
					else
					{
						networkTimeout = true;
					}
					
					TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "responseMessage: " + responseMessage + ", successfulPurchase: " + successfulPurchase);
					
					return storeItem[0];
				}
				else
				{
					networkTimeout = true;
				}
				
				return storeItem[0];
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY, "Failed to purchase item.  e: " + e.toString());
				networkTimeout = true;
			}
			return storeItem[0];
		}

		protected void onPostExecute(final VGStoreItem storeItem)
		{
			TJCVirtualGoods.this.exceptionDialog = exceptionDialog;
			progressDialog.cancel();
			
			// Network timeout
			if (networkTimeout)
			{
				//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "exception NOT null");
				
				currentVgStoreItem = storeItem;
				exceptionDialog = new AlertDialog.Builder(TJCVirtualGoods.this).setMessage(NETWORK_DOWN_MESSAGE).setNegativeButton("Cancel",
						new DialogInterface.OnClickListener()
				{
					public void onClick(DialogInterface dialog, int id)
					{
						dialog.cancel();
					}
				}).setPositiveButton("Retry", new DialogInterface.OnClickListener()
				{
					public void onClick(DialogInterface dialog, int id)
					{
						purchaseVirtualGood();
					}
				}).create();
				try
				{
					if (exceptionDialog != null)// && dialogStatus != null)
					{
						exceptionDialog.show();
					}
				}
				catch (Exception e)
				{
					
				}
			}
			// We got a response from the server.
			else
			{
				storeItem.getVgStoreItemID();
				
				// ERROR trying to download.
				if (successfulPurchase == false)
				{
					getItemDialog.cancel();
					
					if (responseMessage.equalsIgnoreCase("Balance too low"))
					{
						responseMessage = "You do not have enough balance to purchase this item.";
					}
					
					// Dialog Box when balance is too low.
					getMoreItemDialog = new AlertDialog.Builder(TJCVirtualGoods.this).setTitle("").setMessage(responseMessage).setPositiveButton("OK",
							new DialogInterface.OnClickListener()
					{
						// OKAY
						public void onClick(DialogInterface dialog, int whichButton)
						{
							getMoreItemDialog.cancel();
						}
					}).setNegativeButton("Get More", new DialogInterface.OnClickListener()
					{
						// GET MORE
						public void onClick(DialogInterface dialog, int whichButton)
						{
							offersFromVG = true;
							vgFromOffers = true;
							doNotify = false;
							
							showOffers();
						}
					}).create();
					
					try
					{
						getMoreItemDialog.show();
					}
					catch (Exception e)
					{
						
					}
				}
				// Successful purchase.
				else 
				{
					//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "result is false [" + responseMessage + "]");
					updateHeaderFooters(TJCVirtualGoods.this);
					
					showItemDownloadedDialog(responseMessage, storeItem);
				}
			}
			
			// If we got an exception, then don't try to fetch the store items.
			if (!networkTimeout)
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Now fetch store items again...");
				
				storeItemStart = 0;
				storeItemTable.removeAllViews();
				storeItemArray.clear();
				
				fetchAllStoreItems = new FetchAndBuildStoreItemsTask();
				fetchAllStoreItems.execute();
			}

			//TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "Done!");
		}
	}

	
	public static class AsyncTaskPool
	{
		private int poolSize;
		private ArrayList<AsyncTask> currentTasks = new ArrayList<AsyncTask>();
		private ArrayList<Object> pendingTasks = new ArrayList<Object>();

		/**
		 * @param poolSize			Should be less than 5 since Android can only handle a few simultaneous AsyncTasks.
		 */
		public AsyncTaskPool(int maxPoolSize)
		{
			poolSize = maxPoolSize;
			
			// Should be less than 5 since Android can only handle a few simultaneous AsyncTasks.
			if (poolSize >= 5)
				poolSize = 4;
		}

		public int getPoolSize()
		{
			return poolSize;
		}

		public boolean addTask(AsyncTask asyncTask, Object... params)
		{
			if (currentTasks.size() < poolSize)
			{
				currentTasks.add(asyncTask);
				try
				{
					if (params != null)
						asyncTask.execute(params);
					else
						asyncTask.execute();
				}
				catch (RejectedExecutionException e)
				{
					//TapjoyLog.i(TJCConstants.TJC_EXCEPTION, "RejectedExecutionException");
				}
			}
			else
			{
				Object[] task = new Object[2];
				task[0] = asyncTask;
				task[1] = params;

				pendingTasks.add(task);
			}

			return true;
		}

		public boolean removeTask(AsyncTask task)
		{
			if (currentTasks.contains(task))
			{
				currentTasks.remove(task);
				return true;
			}
			return false;
		}

		// Add this method in the onPostExecute method of AsyncTask
		public boolean removeAndExecuteNext(AsyncTask atask)
		{
			removeTask(atask);
			if (pendingTasks.size() > 0 && currentTasks.size() < poolSize)
			{
				Object[] task = (Object[]) pendingTasks.get(0);
				pendingTasks.remove(task);

				addTask((AsyncTask) task[0], (Object[]) task[1]);
			}

			return false;
		}
	}
	
	
	//================================================================================
	// Interfaces
	//================================================================================
	
	
	public static interface TapjoyDownloadListener
	{
		public void onDownLoad(VGStoreItem downloadItem);
	}

	public static interface FocusListener
	{
		void onFocusGain();
		void onFocusLost();
	}
}
