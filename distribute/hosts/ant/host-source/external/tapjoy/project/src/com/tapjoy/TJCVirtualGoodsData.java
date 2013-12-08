// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.content.Context;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Environment;
import android.os.StatFs;


public class TJCVirtualGoodsData
{
	// USER ACCOUNT OBJECT INFO
	private static String userID = null;									// UserID.
	private static String tapPoints;										// Amount of virtual currency.
	private static String currencyName;										// Virtual currency name.
	
	private String clientPackage;
	
	private String basePathSaveToSDCard;
	private String basePathSaveToPhone;
	
	public final static String PREFS_FILE_NAME 					= "TJCPrefsFile";
	public static final String PREFS_POINTS_KEY					= "tapPoints";
	public static final String PREFS_CURRENCY_KEY				= "currencyName";
	
	private static final String TAPJOY_FILE_SYSTEM				= "TapjoyFileSystem";
	private static final String TAPJOY_VIRTUAL_GOODS_DATA		= "TapjoyVirtualGoodsData";
	private static final String TAPJOY_DATABASE					= "TapjoyDatabase";
	
	private static Context context = null;

	
	public TJCVirtualGoodsData(Context ctx, String clientpackage)
	{
		context = ctx;
		
		clientPackage = clientpackage;
		
		basePathSaveToPhone = "data/data/" + clientPackage + "/vgDownloads/";
		
		String externalRoot = Environment.getExternalStorageDirectory().toString();
		basePathSaveToSDCard = externalRoot + "/" + clientPackage + "/vgDownloads/";
		
		SharedPreferences settings = context.getSharedPreferences(PREFS_FILE_NAME, 0);
		currencyName = settings.getString(PREFS_CURRENCY_KEY, "Points");
		tapPoints = "" + settings.getInt(PREFS_POINTS_KEY, 0);
	}
	
	
	/**
	 * Gets an array of the purchased Virtual Goods for this device.
	 * @param ctx							The application context.
	 * @return								Array of VGStoreItem containing the purchased Virtual Goods data.
	 */
	public static ArrayList<VGStoreItem> getPurchasedItems(Context ctx)
	{
		ArrayList<VGStoreItem> purchasedItemsList = new ArrayList<VGStoreItem>();
		SQLiteDatabase myDB1 = TapjoyDatabaseUtil.getTapjoyDatabase(ctx);
		Cursor cursor = myDB1.rawQuery("SELECT i.VGStoreItemID,i.AppleProductID,i.Price,i.Name,i.Description,i.ItemTypeName,i.ItemsOwned,i.ThumbImageName,i.FullImageName,i.DataFileName,i.DataFileHash,a.AttributeName,a.AttributeValue FROM tapjoy_VGStoreItems i   left join tapjoy_VGStoreItemAttribute a on (i.VGStoreItemID=a.VGStoreItemID )  ", null);

		int vgStoreItemID = cursor.getColumnIndex("VGStoreItemID");
		int attributeNameIndex = cursor.getColumnIndex("AttributeName");
		int attributeValueIndex = cursor.getColumnIndex("AttributeValue");
		int appleProductIdIndex = cursor.getColumnIndex("AppleProductID");
		int priceIndex = cursor.getColumnIndex("Price");
		int nameIndex = cursor.getColumnIndex("Name");
		int descriptionIndex = cursor.getColumnIndex("Description");
		int itemTypeNameIndex = cursor.getColumnIndex("ItemTypeName");
		int itemOwnedIndex = cursor.getColumnIndex("ItemsOwned");
		int thumbImageNameIndex = cursor.getColumnIndex("ThumbImageName");
		int fullImageNameIndex = cursor.getColumnIndex("FullImageName");
		int dataFileNameIndex = cursor.getColumnIndex("DataFileName");
		int dataFileHashIndex = cursor.getColumnIndex("DataFileHash");
		
		String storeID = "";
		String appeProductId = "";
		Integer price;
		String name = "";
		String description = "";
		String itemTypeName = "";
		Integer itemsOwned;
		String thumbImageName = "";
		String fullImageName = "";
		String dataFileName = "";
		String attributeName = "";
		String attributeValue = "";
		String dataFileHash = "";
		
		VGStoreItem vgItems = null;

		if (cursor != null)
		{
			cursor.moveToFirst();
			
			if (cursor.isFirst())
			{
				String ids = ",";
				do
				{
					storeID = cursor.getString(vgStoreItemID);

					if (ids.indexOf(storeID) > -1)
					{
						attributeName = cursor.getString(attributeNameIndex);
						attributeValue = cursor.getString(attributeValueIndex);

						VGStoreItemAttributeValue vgAttribute = new VGStoreItemAttributeValue();
						vgAttribute.setAttributeType(TJCStringUtility.getStringFromSpecialCharacter(attributeName));
						vgAttribute.setAttributeValue(TJCStringUtility.getStringFromSpecialCharacter(attributeValue));

						if (vgItems != null)
						{
							if (vgItems.getVgStoreItemsAttributeValueList() != null)
								vgItems.getVgStoreItemsAttributeValueList().add(vgAttribute);
							else
							{
								ArrayList<VGStoreItemAttributeValue> lst = new ArrayList<VGStoreItemAttributeValue>();
								lst.add(vgAttribute);
								vgItems.setVgStoreItemsAttributeValueList(lst);
							}
						}
					}
					else
					{
						vgItems = new VGStoreItem();
						ids = ids + storeID + ",";

						appeProductId = cursor.getString(appleProductIdIndex);
						price = cursor.getInt(priceIndex);
						name = cursor.getString(nameIndex);
						description = cursor.getString(descriptionIndex);
						itemTypeName = cursor.getString(itemTypeNameIndex);
						itemsOwned = cursor.getInt(itemOwnedIndex);
						thumbImageName = cursor.getString(thumbImageNameIndex);
						fullImageName = cursor.getString(fullImageNameIndex);
						dataFileName = cursor.getString(dataFileNameIndex);
						dataFileHash = cursor.getString(dataFileHashIndex);

						attributeName = cursor.getString(attributeNameIndex);
						attributeValue = cursor.getString(attributeValueIndex);
						VGStoreItemAttributeValue vgAttribute = new VGStoreItemAttributeValue();
						vgAttribute.setAttributeType(TJCStringUtility.getStringFromSpecialCharacter(attributeName));
						vgAttribute.setAttributeValue(TJCStringUtility.getStringFromSpecialCharacter(attributeValue));

						ArrayList<VGStoreItemAttributeValue> lst = new ArrayList<VGStoreItemAttributeValue>();
						
						if (vgAttribute != null && vgAttribute.getAttributeType() != null)
							lst.add(vgAttribute);

						vgItems.setVgStoreItemID(storeID);
						vgItems.setProductID(appeProductId);
						vgItems.setPrice(price);
						vgItems.setName(TJCStringUtility.getStringFromSpecialCharacter(name));
						vgItems.setDescription(TJCStringUtility.getStringFromSpecialCharacter(description));
						vgItems.setVgStoreItemTypeName(TJCStringUtility.getStringFromSpecialCharacter(itemTypeName));
						vgItems.setNumberOwned(itemsOwned);
						vgItems.setThumbImageUrl(thumbImageName);
						vgItems.setFullImageUrl(fullImageName);
						vgItems.setDatafileUrl(dataFileName);
						vgItems.setDataFileHash(dataFileHash);

						vgItems.setVgStoreItemsAttributeValueList(lst);

						purchasedItemsList.add(vgItems);
					}

				}
				while (cursor.moveToNext());
			}
		}
		
		if (cursor != null)
		{
			cursor.close();
		}
		
		return purchasedItemsList;
	}
	
	/**
	 * Returns the DataFileHash parameter for this virtual good item ID.  If the virtual good ID does not exist,
	 * then the DataFileHash will be an empty string.
	 * @param ctx
	 * @param vgstoreItemID
	 * @return
	 */
	public static String getDataFileHashForVGStoreItemID(Context ctx, String vgstoreItemID)
	{
		SQLiteDatabase myDB1 = TapjoyDatabaseUtil.getTapjoyDatabase(ctx);
		Cursor cursor = myDB1.rawQuery("SELECT DataFileHash FROM tapjoy_VGStoreItems WHERE VGStoreItemID='" + vgstoreItemID + "'", null);
		
		String dataFileHash = "";
		
		if (cursor != null)
		{
			cursor.moveToFirst();
			
			if (cursor.isFirst())
			{
				dataFileHash = cursor.getString(cursor.getColumnIndex("DataFileHash"));
			}
			
			cursor.close();;
		}
		
		return dataFileHash;
	}
	
	
	/**
	 * Clears the database entry with the specified virtual good item ID.
	 * @param ctx
	 * @param vgstoreItemID
	 * @return
	 */
	public static void clearRow(Context ctx, String vgstoreItemID)
	{
		SQLiteDatabase myDB1 = TapjoyDatabaseUtil.getTapjoyDatabase(ctx);
		myDB1.rawQuery("DELETE FROM tapjoy_VGStoreItems WHERE VGStoreItemID='" + vgstoreItemID + "'", null);
	}
	
	
	/**
	 * Determines whether an entry in the database exists for this virtual good store item ID.
	 * @param ctx
	 * @param vgstoreItemID
	 * @return
	 */
	public static boolean rowExists(Context ctx, String vgstoreItemID)
	{
		SQLiteDatabase myDB1 = TapjoyDatabaseUtil.getTapjoyDatabase(ctx);
		Cursor cursor = myDB1.rawQuery("SELECT * FROM tapjoy_VGStoreItems WHERE VGStoreItemID='" + vgstoreItemID + "'", null);
		
		boolean rowExists = false;
		
		if (cursor != null)
		{
			if (cursor.moveToFirst())
				rowExists = true;
			
			cursor.close();
		}
		
		return rowExists;
	}
	
	
	/**
	 * Parses the response from the server after purchasing a virtual good.
	 * @param data
	 * @param ctx
	 * @return
	 */
	public static ArrayList <String> parsePurchaseVGWithCurrencyResponse(String data, Context ctx)
	{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder; 

		try
		{
			// Convert the string to an input stream.
			InputStream is = new ByteArrayInputStream(data.getBytes("UTF-8"));
			
			builder = factory.newDocumentBuilder();
			Document doc = builder.parse(is);

			ArrayList <String> connectReturnList = new ArrayList<String>();
			
			NodeList node;
			
			node = doc.getElementsByTagName("UserAccountObject");

			for (int i = 0; i < node.getLength(); i++)
			{
				Node firstNode = node.item(i);
				if (firstNode.getNodeType() == Node.ELEMENT_NODE)
				{
					Element element = (Element) firstNode;

					// Virtual currency amount.
					String str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("TapPoints"));
					if (str != null && !str.equals(""))
					{
						savePoints(Integer.parseInt(str));
					}

					// Publisher User ID.
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("PointsID"));
					if (str != null && !str.equals(""))
					{
						saveUserID(str);
					}
					
					// Currency Name.
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("CurrencyName"));
					if (str != null && !str.equals(""))
					{
						saveCurrencyName(str);
					}
				}
			}

			node = doc.getElementsByTagName("TapjoyConnectReturnObject");

			for (int i = 0; i < node.getLength(); i++)
			{
				Node firstNode = node.item(i);
				if (firstNode.getNodeType() == Node.ELEMENT_NODE)
				{
					Element element = (Element) firstNode;

					String str;
					
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Message"));
					if (str != null && !str.equals(""))
					{
						connectReturnList.add(str);
					}
					else 
					{
						// Add dummy.
						connectReturnList.add("");
					}

					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Success"));
					if (str != null)
					{
						connectReturnList.add(str);
					}
					else 
					{
						// Add dummy.
						connectReturnList.add("");
					}

					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("ErrorMessage"));
					if (str != null && !str.equals(""))
					{
						connectReturnList.add(str);
					}
					else 
					{
						// Add dummy.
						connectReturnList.add("");
					}
				}
			}

			return connectReturnList;
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS_DATA, "parseVGPurchaseResponse: " + e.toString());
		}
		
		return null;
	}
	
	
	/**
	 * Parses the virtual good response from the server and returns the data into an array list of VGStoreItems.
	 * @param data
	 * @param itemType
	 * @param ctx
	 * @return
	 * @throws Exception
	 */
	public static ArrayList<VGStoreItem> parseVGItemListResponse(String data, int itemType, Context ctx) throws Exception
	{
		// TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "====================");
		// TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "Parse Virtual Goods");

		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder;
		NodeList nodelist = null;
		Node node = null;
		Element element = null;
		String str = "";

		try
		{
			InputStream is = new ByteArrayInputStream(data.getBytes("UTF-8"));

			builder = factory.newDocumentBuilder();
			Document doc = builder.parse(is);

			ArrayList<VGStoreItem> storeItems = new ArrayList<VGStoreItem>();

			nodelist = doc.getElementsByTagName("UserAccountObject");

			for (int i = 0; i < nodelist.getLength(); i++)
			{
				Node firstNode = nodelist.item(i);
				if (firstNode.getNodeType() == Node.ELEMENT_NODE)
				{
					element = (Element) firstNode;

					// Save the virtual currency amount.
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("TapPoints"));
					if (str != null && !str.equals(""))
					{
						TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "Virtual Currency: " + str);

						savePoints(Integer.parseInt(str));
					}

					// Publisher User ID.
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("PointsID"));
					if (str != null && !str.equals(""))
					{
						saveUserID(str);
					}

					// Save the virtual currency name.
					str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("CurrencyName"));
					if (str != null && !str.equals(""))
					{
						saveCurrencyName(str);
					}
				}
			}

			nodelist = doc.getElementsByTagName("VGStoreItemReturnClass");
			for (int i = 0; i < nodelist.getLength(); i++)
			{
				node = nodelist.item(i);

				storeItems.add(parseNodeForStoreItems(node));
			}

			// see if more data is available
			nodelist = doc.getElementsByTagName("MoreDataAvailable");
			int moreDataAvailable = 0; // reset the value

			if (nodelist.getLength() > 0)
			{
				element = (Element) node;

				str = TapjoyUtil.getNodeTrimValue(nodelist);
				if (str != null && !str.equals(""))
				{
					moreDataAvailable = Integer.parseInt(str);
				}
			}
			
			if (itemType == VGStoreItem.STORE_ITEM)
				VGStoreItem.availableItemsMoreDataAvailable = moreDataAvailable;
			else if (itemType == VGStoreItem.PURCHASED_ITEM)
				VGStoreItem.purchasedItemsMoreDataAvailable = moreDataAvailable;

			// TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "====================");
			return storeItems;
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS_DATA, "parseVGItemListResponse: " + e.toString());
			throw new Exception(e.getMessage());
			//throw new TJCException(e.getMessage(), RESPONSE_FAIL);
		}
	}

	
	private static VGStoreItem parseNodeForStoreItems(Node node)
	{
		Element element;
		String str = "";
		VGStoreItem storeItem = null;

		if (node != null && node.getNodeType() == Node.ELEMENT_NODE)
		{
			storeItem = new VGStoreItem();

			element = (Element) node;

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("VGStoreItemID"));
			if (str != null && !str.equals(""))
				storeItem.setVgStoreItemID(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("AppleProductID"));
			if (str != null && !str.equals(""))
				storeItem.setProductID(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Price"));
			if (str != null && !str.equals(""))
				storeItem.setPrice(Integer.parseInt(str));

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Name"));
			if (str != null && !str.equals(""))
				storeItem.setName(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Description"));
			if (str != null && !str.equals(""))
				storeItem.setDescription(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("VGStoreItemTypeName"));
			if (str != null && !str.equals(""))
				storeItem.setVgStoreItemTypeName(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("NumberOwned"));
			if (str != null && !str.equals(""))
				storeItem.setNumberOwned(Integer.parseInt(str));

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("ThumbImageURL"));
			if (str != null && !str.equals(""))
				storeItem.setThumbImageUrl(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("FullImageURL"));
			if (str != null && !str.equals(""))
				storeItem.setFullImageUrl(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("DatafileURL"));
			if (str != null && !str.equals(""))
				storeItem.setDatafileUrl(str);

			str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("DataHash"));
			if (str != null && !str.equals(""))
				storeItem.setDataFileHash(str);
			
			// TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "" + storeItem.getName() + ", owned: " + storeItem.getNumberOwned());

			storeItem.setVgStoreItemsAttributeValueList(parseNodeForStoreItemsAttributes(node));
		}

		try
		{
			// Update the Database so it has the latest quantity.
			// TapjoyLog.i(TAPJOY_DATABASE, "UPDATING DB");
			int new_value = storeItem.getNumberOwned();
			SQLiteDatabase myDB = TapjoyDatabaseUtil.getTapjoyDatabase(context);
			StringBuilder q = new StringBuilder();

			q.append("UPDATE tapjoy_VGStoreItems SET ItemsOwned='" + new_value + "' ");
			q.append("WHERE VGStoreItemID='" + storeItem.getVgStoreItemID() + "'");
			myDB.execSQL(q.toString());

			// Update the "quantity" attribute.
			StringBuilder p = new StringBuilder();
			p.append("UPDATE tapjoy_VGStoreItemAttribute SET AttributeValue='" + new_value + "' ");
			p.append("WHERE VGStoreItemID='" + storeItem.getVgStoreItemID() + "' AND AttributeName='quantity'");
			myDB.execSQL(p.toString());
		}
		catch (Exception e)
		{

		}

		return storeItem;
	}

	private static ArrayList<VGStoreItemAttributeValue> parseNodeForStoreItemsAttributes(Node node)
	{
		Element element = (Element) node;
		NodeList nodelist = null;
		String str = "";

		ArrayList<VGStoreItemAttributeValue> storeItemAtrributes = new ArrayList<VGStoreItemAttributeValue>();

		nodelist = element.getElementsByTagName("VGStoreItemAttributeValueReturnClass");
		for (int i = 0; i < nodelist.getLength(); i++)
		{
			node = nodelist.item(i);
			VGStoreItemAttributeValue storeItemAtrribute = null;
			if (node != null && node.getNodeType() == Node.ELEMENT_NODE)
			{
				storeItemAtrribute = new VGStoreItemAttributeValue();
				element = (Element) node;

				str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("AttributeType"));
				if (str != null && !str.equals(""))
					storeItemAtrribute.setAttributeType(str);

				str = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("AttributeValue"));
				if (str != null && !str.equals(""))
					storeItemAtrribute.setAttributeValue(str);
			}

			storeItemAtrributes.add(storeItemAtrribute);
		}

		return storeItemAtrributes;
	}

	
	/**
	 * Saves the virtual currency amount for this user.
	 * @param points						Virtual currency amount.
	 */
	private static void savePoints(int points)
	{
		SharedPreferences settings = context.getSharedPreferences(PREFS_FILE_NAME, 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putInt(PREFS_POINTS_KEY, points);
		editor.commit();
		
		tapPoints = "" + points;
	}
	
	
	/**
	 * Saves the virtual currency name for this user.
	 * @param name							Virtual currency name.
	 */
	private static void saveCurrencyName(String name)
	{
		SharedPreferences settings = context.getSharedPreferences(PREFS_FILE_NAME, 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putString(PREFS_CURRENCY_KEY, name);
		editor.commit();
		
		currencyName = name;
	}
	
	
	/**
	 * Saves the user ID for this device.
	 * @param id							User ID.
	 */
	private static void saveUserID(String id)
	{
		userID = id;
	}
	
	
	/**
	 * Gets the virtual currency for this user.
	 * @return								Virtual currency for this user.
	 */
	public static String getPoints()
	{
		return tapPoints;
	}
	
	
	/**
	 * Gets the virtual currency name for this user.
	 * @return								Virtual currency name.
	 */
	public static String getCurrencyName()
	{
		return currencyName;
	}
	
	
	/**
	 * Gets the user ID for this device.
	 * @return								User ID.
	 */
	public static String getUserID()
	{
		return userID;
	}
	
	
	/**
	 * Extracts a zip file to the specified location.
	 * @param file							Zip file to extract.
	 * @param destinationFolder				Location to extract the zip file contents.
	 * @return								TRUE if successful, false otherwise.
	 */
	public boolean extractZipFileToFolder(File file, String destinationFolder)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "extractZipFileToFolder file: " + file.getName() + ", size: " + file.length() + ", destinationFolder: " + destinationFolder);
		
		boolean success = true;
		
		// Create the destination folder
		File f = new File(destinationFolder);

		// DEBUGGING
		// TapjoyLog.i(TAPJOY_FILE_SYSTEM, "========================================");
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "File saving to: " + f.getPath());
		TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "File saving to: " + f.getName());

		File path = Environment.getDataDirectory();
		StatFs stat = new StatFs(path.getPath());
		long free_memory = stat.getAvailableBlocks() * stat.getBlockSize();
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "FREE INTERNAL MEMORY: " + (free_memory / 1024) + " KB");

		path = Environment.getExternalStorageDirectory();
		stat = new StatFs(path.getPath());
		free_memory = stat.getAvailableBlocks() * stat.getBlockSize();
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "FREE EXTERNAL MEMORY: " + (free_memory / 1024) + " KB");
		// TapjoyLog.i(TAPJOY_FILE_SYSTEM, "========================================");

		// Create the directory path to the file.
		if (f.mkdirs())
			TapjoyLog.i(TAPJOY_VIRTUAL_GOODS_DATA, "Created directory: " + f.getPath());

		try
		{
			ZipFile zipFile = new ZipFile(file);

			if (zipFile != null)
			{
				Enumeration<? extends ZipEntry> en = zipFile.entries();
				
				// Iterate through the elements within this zip file.
				while (en.hasMoreElements())
				{
					ZipEntry zipentry = (ZipEntry) en.nextElement();
					
					if (zipentry != null)
					{
						try
						{
							createFilesAndFolders(destinationFolder, zipentry, zipFile);
						}
						catch (Exception e)
						{
							TapjoyLog.e(TAPJOY_VIRTUAL_GOODS_DATA, "Error trying to create files and folders e: " + e.toString());
							success = false;
						}
					}
				}

				zipFile.close();
			}
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_VIRTUAL_GOODS_DATA, "Extraction failed e: " + e.toString());
			success = false;
		}

		if (!success)
		{
			// Delete the extraction directory.
			deleteDir(f);
		}
		
		return success;
	}

	public static void createFilesAndFolders(String destFolder, ZipEntry zipentry, ZipFile zipFile) throws Exception
	{
		String entryName = zipentry.getName();

		//TapjoyLog.i(TAPJOY_FILE_SYSTEM, "createFile: " + entryName);

		// Is this a directory?
		if (zipentry.isDirectory())
		{
			File dir = new File((destFolder + entryName).replaceAll(" ", "%20"));
			if (!dir.exists())
			{
				// Create the directory.
				if (dir.mkdirs())
				{
					TapjoyLog.i(TAPJOY_FILE_SYSTEM, "Created directory");
				}
			}
		}
		else
		{
			try
			{
				String folder = "";
				if (entryName.indexOf('/') > -1)
				{
					folder = entryName.substring(0, entryName.lastIndexOf("/"));

					String newPath = destFolder + folder;
					File dir = new File(newPath.replaceAll(" ", "%20"));
					if (!dir.exists())
					{
						if (dir.mkdirs())
							TapjoyLog.i(TAPJOY_FILE_SYSTEM, "Created directory");
					}
				}

				String fileName = entryName.substring(entryName.lastIndexOf("/") + 1, entryName.length());
				if (fileName.startsWith("."))
				{
					fileName = "DOT" + fileName.substring(1, fileName.length());
					entryName = folder + "/" + fileName;
				}

				entryName = entryName.replaceAll(" ", "%20");
				File file = new File(destFolder, entryName);
				if (file.createNewFile())
				{
					byte[] buf = new byte[1024];
					int n;
					FileOutputStream fileoutputstream = null;
					fileoutputstream = new FileOutputStream(file);
					InputStream inputStream = zipFile.getInputStream(zipentry);
					while ((n = inputStream.read(buf, 0, 1024)) > -1)
						fileoutputstream.write(buf, 0, n);
					fileoutputstream.close();
				}
				else
				{
					// TapjoyLog.i(TAPJOY_VIRTUAL_GOODS, "File already exists, filename = [" + entryName + "]");
				}
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY_FILE_SYSTEM, "createFilesAndFolders: " + e.toString());
				throw e;
			}
		}
	}
	
	public void deleteVGZip(VGStoreItem storeItem, boolean saveToSDCard)
	{
		if (storeItem == null)
			return;
		
		String zipURL = storeItem.getDatafileUrl();					// URL of the zip file.
		String zipFileLocation = null;								// Location to save the zip file.
		String storeItemFolder;										// Folder to store this virtual good's data.
	
		// If no data file, just exit.
		if (zipURL.length() == 0)
			return;
		
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "delete getDatafileUrl: " + zipURL);
		
		int last_slash = zipURL.lastIndexOf("/");
		int second_to_last_slash = (zipURL.substring(0, last_slash - 1)).lastIndexOf("/"); 
		
		if (second_to_last_slash > -1 && zipURL.length() > second_to_last_slash)
		{
			// Take out the start and end slash.
			storeItemFolder = zipURL.substring(second_to_last_slash + 1, zipURL.length() - 1);
			
			TapjoyLog.i(TAPJOY_FILE_SYSTEM, "storeItemFolder: " + storeItemFolder);

			if (storeItemFolder != null && !storeItemFolder.equals(""))
			{
				if (saveToSDCard)
				{
					String externalRoot = Environment.getExternalStorageDirectory().toString();
					zipFileLocation = externalRoot + "/" + clientPackage + "/tempZipDownloads/" + storeItemFolder + ".zip";
				}
				else
				{
					zipFileLocation = "data/data/" + clientPackage + "/tempZipDownloads/" + storeItemFolder + ".zip";
				}
			}
		}
		
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "deleteVGZip: " + zipFileLocation);
		
		try
		{
			File zipFile = new File(zipFileLocation);
			
			if (zipFile.exists())
			{
				TapjoyLog.i(TAPJOY_FILE_SYSTEM, "deleting file at: " + zipFileLocation);
				zipFile.delete();
			}
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_FILE_SYSTEM, "Error deleting virtual good zip file: " + e.toString());
		}
	}
	
	
	/**
	 * Deletes a directory from the file system.
	 * @param dir							Directory to delete.
	 * @return								TRUE if successful, FALSE otherwise.	
	 */
	public static boolean deleteDir(File dir)
	{
		TapjoyLog.i(TAPJOY_FILE_SYSTEM, "deleting directory: " + dir.getPath());
		
		// Does the directory exist?
		if (dir.exists())
		{
			// Is this a valid directory?
			if (dir.isDirectory())
			{
				String[] children = dir.list();
				
				// Delete all children.
				for (int i = 0; i < children.length; i++)
				{
					boolean success = deleteDir(new File(dir, children[i]));
					
					if (!success)
					{
						return false;
					}
				}
			}
			
			// The directory is now empty so now it be deleted.
			return dir.delete();
		}
		else
		{
			return false;
		}
	}
	
	
	/**
	 * Writes the Virtual Good Item data into the SQL database.
	 * Only called when an item is purchased and successfully downloaded and extracted.
	 * @param storeItem						The virtual good item data to save.
	 * @throws Exception
	 */
	public void saveInfo(VGStoreItem storeItem, boolean savedToSDCard)
	{
		TapjoyLog.i(TAPJOY_DATABASE, "saveInfo to SQL DB: " + storeItem.getName());
		//TapjoyLog.i(TAPJOY_DATABASE, "vgDownloadState: " + vgDownloadState);
		
		boolean SQLinsert = true;
		String dataFileUrl = storeItem.getDatafileUrl();
		
		if (dataFileUrl != null && !dataFileUrl.equals(""))
		{
			int index = dataFileUrl.lastIndexOf("/");
			dataFileUrl = dataFileUrl.substring(index + 1, dataFileUrl.length() - 4);

			if (savedToSDCard)
				dataFileUrl = basePathSaveToSDCard + dataFileUrl;
			else
				dataFileUrl = basePathSaveToPhone + dataFileUrl;
		}
		else
		{
			dataFileUrl = "";
		}

		TapjoyLog.i(TAPJOY_DATABASE, "dataFileUrl: " + dataFileUrl);
		
		if (storeItem != null)
		{
			SQLiteDatabase myDB = TapjoyDatabaseUtil.getTapjoyDatabase(context);
			StringBuilder q = new StringBuilder();
			
			//TapjoyLog.i(TAPJOY_DATABASE, "owned: " + storeItem.getNumberOwned());
			//TapjoyLog.i(TAPJOY_DATABASE, "INSERT StoreItems");
			
			q.append("INSERT INTO tapjoy_VGStoreItems (VGStoreItemID, AppleProductID, Price, Name,Description , ItemTypeName , ItemsOwned , ThumbImageName , FullImageName , DataFileName , DataFileHash)");
			q.append(" VALUES (");
			q.append("'" + storeItem.getVgStoreItemID() + "',");
			q.append("'" + storeItem.getProductID() + "',");

			String price = "" + storeItem.getPrice();
			
			if (price != null)
			{
				if (price.indexOf(",") > -1)
					price = price.replaceAll(",", "");
				q.append(price + ",");
			}
			q.append("'" + TJCStringUtility.replaceSpecialChars(storeItem.getName()) + "',");
			q.append("'" + TJCStringUtility.replaceSpecialChars(storeItem.getDescription()) + "',");
			q.append("'" + TJCStringUtility.replaceSpecialChars(storeItem.getVgStoreItemTypeName()) + "',");
			q.append(storeItem.getNumberOwned() + ",");
			q.append("'" + storeItem.getThumbImageUrl() + "',");
			q.append("'" + storeItem.getFullImageUrl() + "',");
			q.append("'" + dataFileUrl + "',");
			q.append("'" + storeItem.getDataFileHash() + "')");
			
			//TapjoyLog.i(TAPJOY_DATABASE, "query [" + q + "]");
			
			try
			{
				myDB.execSQL(q.toString());
			}
			catch (SQLException e1)
			{
				// Row already exists in the database, so don't insert the attributes either.
				TapjoyLog.i(TAPJOY_DATABASE, "Row already exists");
				SQLinsert = false;
			}
			
			// Do we need to do an SQL Insert?
			if (SQLinsert)
			{
				List<?> lst = storeItem.getVgStoreItemsAttributeValueList();

				//TapjoyLog.i(TAPJOY_DATABASE, "Insert Attributes");
				
				for (int i = 0; i < lst.size(); i++)
				{
					VGStoreItemAttributeValue obj = (VGStoreItemAttributeValue) lst.get(i);
					StringBuilder p = new StringBuilder();
					p.append("INSERT INTO tapjoy_VGStoreItemAttribute ( VGStoreItemID, AttributeName, AttributeValue)");
					p.append(" Values (");
					p.append("'" + storeItem.getVgStoreItemID() + "',");
					p.append("'" + TJCStringUtility.replaceSpecialChars(obj.getAttributeType()) + "',");
					p.append("'" + TJCStringUtility.replaceSpecialChars(obj.getAttributeValue()) + "')");
					
					//TapjoyLog.i(TAPJOY_DATABASE, "p [" + p + "]");
					
					try
					{
						myDB.execSQL(p.toString());
					}
					catch (SQLException e)
					{
						TapjoyLog.e(TAPJOY_DATABASE, "SQLException while trying to insert into table: " + e.toString());
					}
				}
			}
		}
	}
	
	
	public static class TapjoyDatabaseUtil
	{
		public static TJCSQLLiteDatabase db	= null;

		private TapjoyDatabaseUtil()
		{
			
		}

		public static SQLiteDatabase getTapjoyDatabase(Context ctx)
		{
			if (db == null)
				db = new TJCSQLLiteDatabase(ctx);
			
			return db.getTapjoyDatabase();
		}
	}
	
	
	public static class TJCSQLLiteDatabase extends SQLiteOpenHelper 
	{
		// Database version must be updated whenever the table changes.
		private static final String DATABASE_NAME = TapjoyConstants.DATABASE_NAME;
		private static final int DATABASE_VERSION = TapjoyConstants.DATABASE_VERSION;
		
		SQLiteDatabase tapjoyDatabase = null;
		
		public TJCSQLLiteDatabase(Context context)
		{
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		@Override
		public void onCreate(SQLiteDatabase db)
		{
			db.execSQL("CREATE TABLE tapjoy_VGStoreItems(VGStoreItemID TEXT PRIMARY KEY, AppleProductID TEXT(50), Price INTEGER, Name TEXT(100),Description TEXT(1000), ItemTypeName TEXT(100), ItemsOwned INTEGER, ThumbImageName TEXT(100), FullImageName TEXT(100), DataFileName TEXT(100), DataFileHash TEXT(100))");
			db.execSQL("CREATE TABLE tapjoy_VGStoreItemAttribute(id INTEGER PRIMARY KEY, VGStoreItemID TEXT, AttributeName TEXT(100), AttributeValue TEXT(100))");
		}

		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
		{
			TapjoyLog.w(TAPJOY_DATABASE, "****************************************");
			TapjoyLog.w(TAPJOY_DATABASE, "Upgrading database from version " + oldVersion + " to "+ newVersion + ", which will add new column to DB");
			TapjoyLog.w(TAPJOY_DATABASE, "****************************************");
			
			// Add new column to database.
			try
			{
				db.execSQL("ALTER TABLE tapjoy_VGStoreItems ADD DataFileHash TEXT(100)");
			}
			catch (Exception e)
			{
				TapjoyLog.e("SQLUpgrade", "No need to add the column 'DataFileHash'");
			}
			
//			db.execSQL("DROP TABLE IF EXISTS tapjoy_VGStoreItems");
//			db.execSQL("DROP TABLE IF EXISTS tapjoy_VGStoreItemAttribute");
//			onCreate(db);
		}

		public SQLiteDatabase getTapjoyDatabase()
		{
			return this.getWritableDatabase();
		}
	}
	
	
	public static class TJCStringUtility
	{
		public static String replaceSpecialChars(String stringWithCommas)
		{
			if (stringWithCommas != null)
				return stringWithCommas.replaceAll("'", "COMMA");
			return null;
		}

		public static String getStringFromSpecialCharacter(String str)
		{
			if (str != null)
				return str.replaceAll("COMMA", "'");
			return null;
		}
	}
	
	
	public static class TJCUtility 
	{
		static public long internalFreeMemorySize()
		{
			File path = Environment.getDataDirectory();

			StatFs stat = new StatFs(path.getPath());
			long availableBlocks = stat.getAvailableBlocks();
			long blockSize = stat.getBlockSize();
			return availableBlocks * blockSize;
		}

		static public long externalFreeMemorySize()
		{
			if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED))
			{
				File path = Environment.getExternalStorageDirectory();

				StatFs stat = new StatFs(path.getPath());
				long availableBlocks = stat.getAvailableBlocks();
				long blockSize = stat.getBlockSize();
				return availableBlocks * blockSize;
			}
			else
			{
				return -1;
			}
		}
	}
}
