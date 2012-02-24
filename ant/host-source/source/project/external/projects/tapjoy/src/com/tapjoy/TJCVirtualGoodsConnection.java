// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;


public class TJCVirtualGoodsConnection
{
	
	final static String TAPJOY_VIRTUAL_GOODS_CONNECTION			= "TapjoyVirtualGoodsConnection";
		
	private static String urlDomain								= null;
	private static String urlParams								= null;
	
	private static TapjoyURLConnection tapjoyURLConnection 		= null;

	
	public TJCVirtualGoodsConnection(String baseDomain, String baseParams)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_CONNECTION, "base: " + baseDomain + ", params: " + baseParams);
		
		tapjoyURLConnection = new TapjoyURLConnection();
		
		urlDomain = baseDomain;
		urlParams = baseParams;
	}
	
	public String getAllStoreItemsFromServer(int start, int max)
	{
		String url = urlDomain + TapjoyConstants.TJC_VG_GET_ALL_URL_PATH;
		String params = urlParams + "&start=" + start + "&max=" + max;
		String response = tapjoyURLConnection.connectToURL(url, params);
		
		return response;
	}
	
	public String getAllPurchasedItemsFromServer(int start, int max)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_CONNECTION, "getAllPurchasedItemsFromServer");
		
		String url = urlDomain + TapjoyConstants.TJC_VG_GET_PURCHASED_URL_PATH;
		String params = urlParams + "&start=" + start + "&max=" + max;
		String response = tapjoyURLConnection.connectToURL(url, params);
		
		return response;
	}
	
	public String purchaseVGFromServer(String virtualGoodID)
	{
		String url = urlDomain + TapjoyConstants.TJC_VG_PURCHASE_WITH_CURRENCY_URL_PATH;
		String params = urlParams + "&virtual_good_id=" + virtualGoodID;
		String response = tapjoyURLConnection.connectToURL(url, params);
		
		return response;
	}
}