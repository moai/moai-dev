// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

/**
 * Class for representing attributes of a Virtual Good Item.
 */
public final class VGStoreItemAttributeValue
{
	private String attributeType = "";
	private String attributeValue = "";

	public VGStoreItemAttributeValue()
	{
	}

	public String getAttributeType() 
	{
		return attributeType;
	}

	public void setAttributeType(String type) 
	{
		attributeType = type;
	}

	public String getAttributeValue() 
	{
		return attributeValue;
	}

	public void setAttributeValue(String value) 
	{
		attributeValue = value;
	}
}