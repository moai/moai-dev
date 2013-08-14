// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.util.ArrayList;
import android.graphics.drawable.Drawable;

/**
 * Class for representing a Virtual Good Item.
 */
public final class VGStoreItem
{
	public static final int MAX_ITEMS 					= 25; 
	public static final int STORE_ITEM					= 0;
	public static final int PURCHASED_ITEM				= 1;

	public static int availableItemsMoreDataAvailable 	= 0;
	public static int purchasedItemsMoreDataAvailable 	= 0;

	private String vgStoreItemID = "";
	private String productID = "";
	private int price = 0;
	private String name = "";
	private String description = "";
	private String vgStoreItemTypeName = "";
	private int numberOwned = 0;
	private String thumbImageUrl="";
	private String fullImageUrl="";
	private String datafileUrl = "";
	private String dataFileHash = "";
	
	private Drawable thumbImage = null;
	private boolean shown = true;

	ArrayList<VGStoreItemAttributeValue> vgStoreItemsAttributeValueList = null;

	
	public VGStoreItem()
	{
		
	}

	public String getVgStoreItemID() 
	{
		return vgStoreItemID;
	}
	
	public void setVgStoreItemID(String id) 
	{
		vgStoreItemID = id;
	}
	
	public String getProductID() 
	{
		return productID;
	}
	
	public void setProductID(String pid) 
	{
		productID = pid;
	}
	
	public int getPrice() 
	{
		return price;
	}
	
	public void setPrice(int value) 
	{
		price = value;
	}
	
	public String getName() 
	{
		return name;
	}
	
	public void setName(String itemname)
	{
		name = itemname;
	}
	
	public String getDescription()
	{
		return description;
	}
	
	public void setDescription(String desc) 
	{
		description = desc;
	}
	
	public String getVgStoreItemTypeName() 
	{
		return vgStoreItemTypeName;
	}
	
	public void setVgStoreItemTypeName(String value) 
	{
		vgStoreItemTypeName = value;
	}

	public ArrayList<VGStoreItemAttributeValue> getVgStoreItemsAttributeValueList() 
	{
		return vgStoreItemsAttributeValueList;
	}

	public void setVgStoreItemsAttributeValueList(ArrayList<VGStoreItemAttributeValue> list) 
	{
		vgStoreItemsAttributeValueList = list;
	}

	public int getNumberOwned()
	{
		return numberOwned;
	}

	public void setNumberOwned(int owned)
	{
		numberOwned = owned;
	}

	public String getThumbImageUrl()
	{
		return thumbImageUrl;
	}

	public void setThumbImageUrl(String url)
	{
		thumbImageUrl = url;
	}

	public String getFullImageUrl()
	{
		return fullImageUrl;
	}

	public void setFullImageUrl(String url)
	{
		fullImageUrl = url;
	}

	public String getDatafileUrl()
	{
		return datafileUrl;
	}

	public void setDatafileUrl(String url)
	{
		datafileUrl = url;
	}

	public Drawable getThumbImage()
	{
		return thumbImage;
	}

	public void setThumbImage(Drawable image)
	{
		thumbImage = image;
	}
	
	public void setDataFileHash(String hash)
	{
		dataFileHash = hash;
	}
	
	public String getDataFileHash()
	{
		return dataFileHash;
	}

	public void setShown(boolean show)
	{
		shown = show;
	}

	public boolean isShown()
	{
		return shown;
	}
}
