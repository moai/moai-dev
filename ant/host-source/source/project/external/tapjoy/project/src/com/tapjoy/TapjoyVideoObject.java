// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

public class TapjoyVideoObject
{
	public String offerID;
	public String clickURL;
	public String videoURL;
	
	public String videoAdName;
	public String currencyName;
	public String currencyAmount;
	public String iconURL;
	
	public String dataLocation;
	
	public String [][] buttonData;
	public int buttonCount;
	
	public static final int BUTTON_MAX = 10;
	
	
	public TapjoyVideoObject()
	{
		buttonData = new String[BUTTON_MAX][2];
	}
	
	public void addButton(String name, String url)
	{
		buttonData[buttonCount][0] = name;
		buttonData[buttonCount][1] = url;
		buttonCount++;
	}
}