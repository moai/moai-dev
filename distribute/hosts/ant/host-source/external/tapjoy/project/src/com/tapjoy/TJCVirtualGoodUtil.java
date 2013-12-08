// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;
import java.util.concurrent.RejectedExecutionException;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import android.os.AsyncTask;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager.BadTokenException;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TableLayout;
import android.widget.TextView;

import com.tapjoy.TJCVirtualGoods;
import com.tapjoy.TJCVirtualGoodsData;
import com.tapjoy.TJCVirtualGoodsData.TapjoyDatabaseUtil;

/**
 * VirtualGoodUtil class helps manage and downloads purchased virtual goods.
 *
 */
public class TJCVirtualGoodUtil
{
	private static final String	TAPJOY_VIRTUAL_GOOD_UTIL = "VirtualGoodUtil";

	private static TJCVirtualGoodsConnection tapjoyVGConnection = null;
	private static TJCVirtualGoodsData tapjoyVGData				= null;
	
	private Context downloadPurchasedVGContext = null;
	private Context applicationContext = null;
	CheckForVirtualGoodsTask fetchPurchasedVGItems;
	
	public Hashtable<String, DownloadVirtualGoodTask> virtualGoodsToDownload;

	private ArrayList<VGStoreItem> purchasedItemArray;
	public ArrayList<VGStoreItem> purchaseItems;
	
	public int detailIndex = 0;
	public static boolean virtualGoodsUIOpened = false;									// Whether the virtual goods UI has been opened or not.
	
	private SQLiteDatabase myDB = null;

	private String tapjoyWebServiceURL = TapjoyConstants.TJC_SERVICE_URL;
	private String clientPackage = null;
	private String urlParams = null;
	private static String basePathSaveToSDCard;
	private static String basePathSaveToPhone;
	
	String dialogErrorMessage = "";

	public TJCVirtualGoodUtil(Context ctx, String client_package)
	{
		applicationContext = ctx;
		clientPackage = client_package;
		
		basePathSaveToPhone = "data/data/" + clientPackage + "/vgDownloads/";
		
		String externalRoot = Environment.getExternalStorageDirectory().toString();
		basePathSaveToSDCard = externalRoot + "/" + clientPackage + "/vgDownloads/";
		
		virtualGoodsToDownload = new Hashtable<String, DownloadVirtualGoodTask>();
		
		tapjoyVGData = new TJCVirtualGoodsData(ctx, clientPackage);
		
		myDB = TapjoyDatabaseUtil.getTapjoyDatabase(ctx);
		String destFolder = "data/data/" + clientPackage + "/vgDownloads";
		File dir = new File(destFolder);
		
		if (!dir.exists())
		{
			if (dir.mkdir())
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "vgDownloads directory created at device.....");
		}

		destFolder = "data/data/" + clientPackage + "/tempZipDownloads";
		dir = new File(destFolder);
		if (!dir.exists())
		{
			if (dir.mkdir())
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "temporary zip file directory generated at device");
		}
	}

	public Hashtable<String, DownloadVirtualGoodTask> getdownloadVirtualGoods()
	{
		return virtualGoodsToDownload;
	}

	
	/**
	 * Downloads virtual goods that we've purchased, but have not yet downloaded.
	 * @param purchasedItems
	 * @param table
	 * @param start
	 * @param actionTxt
	 * @param detailBar
	 * @param retryDetailBtn
	 * @param errorMsgDetail
	 */
	public void downLoadPurcahasedVirtualGood(List<VGStoreItem> purchasedItems, TableLayout table, int start)
	{
		StringBuilder allPurchsedStoreItemsIds = new StringBuilder();

		Cursor cursor = myDB.rawQuery("SELECT VGStoreItemID FROM tapjoy_VGStoreItems", null);
		int firstNameColumn = cursor.getColumnIndex("VGStoreItemID");
		
		if (cursor != null)
		{
			cursor.moveToFirst();
			if (cursor.isFirst())
			{
				do
				{
					String firstName = cursor.getString(firstNameColumn);
					allPurchsedStoreItemsIds.append(firstName + ",");
				}
				while (cursor.moveToNext());
			}
		}
		
		cursor.deactivate();
		cursor.close();
		
		// List of item IDs of all purchased virtual goods.
		String allItems = "";
		
		if (allPurchsedStoreItemsIds.length() > 0)
			allItems = allPurchsedStoreItemsIds.substring(0, allPurchsedStoreItemsIds.length() - 1);

		purchaseItems = (ArrayList<VGStoreItem>) purchasedItems;

		//TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "allitems: " + allItems);
		
		for (int i = start; i < purchasedItems.size(); i++)
		{
			VGStoreItem vgItem = (VGStoreItem) purchasedItems.get(i);
			if (vgItem != null)
			{
				// Does the purchased VG item exist in our SQL entry?  If not, then we need to download it.
				if (allItems.indexOf(vgItem.getVgStoreItemID()) == -1)
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "download this purchased vg: " + vgItem.getVgStoreItemID() + ", name: " + vgItem.getName());
					
					DownloadVirtualGoodTask dGood = new DownloadVirtualGoodTask();
					View v = table.getChildAt(i * 2);
					if (v instanceof LinearLayout)
					{
						LinearLayout linearLayout = (LinearLayout) v;

						dGood.localProgressBar = (ProgressBar) linearLayout.findViewById(applicationContext.getResources().getIdentifier("vg_row_progress_bar", "id", clientPackage));
						dGood.localProgressBar.setVisibility(View.GONE);
						
						dGood.localDownloadStatusText = (TextView) linearLayout.findViewById(applicationContext.getResources().getIdentifier("vg_row_download_status_text", "id", clientPackage));
						dGood.localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_PENDING);
						
						dGood.localRetryButton = (Button) linearLayout.findViewById(applicationContext.getResources().getIdentifier("vg_row_retry_button", "id", clientPackage));
						dGood.localErrorIcon = (ImageView) linearLayout.findViewById(applicationContext.getResources().getIdentifier("vg_row_error_icon", "id", clientPackage));
						
						dGood.localRetryButton.setOnClickListener(retryClickListener);
						dGood.localErrorIcon.setOnClickListener(errorMsgClickListener);
						
						virtualGoodsToDownload.put(vgItem.getVgStoreItemID(), dGood);

						addTask(dGood, vgItem);
					}
				}
			}
		}
	}

	public void setDetailIndex(int index)
	{
		detailIndex = index;
	}

	/**
	 * Retries downloading a failed virtual good download task.
	 * @param failedTask
	 */
	public void retryDownloadVG(DownloadVirtualGoodTask failedTask)
	{
		DownloadVirtualGoodTask retryTask = new DownloadVirtualGoodTask();

		TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "RETRY DOWNLOAD VG: " + failedTask.vgItem.getName());
		
		retryTask.localProgressBar = failedTask.localProgressBar;
		retryTask.localProgressBar.setVisibility(View.GONE);
		retryTask.localProgressBar.setProgress(0);
		retryTask.localErrorIcon = failedTask.localErrorIcon;
		retryTask.localDownloadStatusText = failedTask.localDownloadStatusText;

		retryTask.localRetryButton = failedTask.localRetryButton;
		retryTask.localDownloadStatusText = failedTask.localDownloadStatusText;
		retryTask.localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_PENDING);
		retryTask.localRetryButton.setVisibility(View.GONE);
		retryTask.localErrorIcon.setVisibility(View.GONE);
		
		retryTask.virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_PENDING;
		
		VGStoreItem vgItem = failedTask.vgItem;
		
		if (failedTask.getStatus() == AsyncTask.Status.RUNNING)
			failedTask.cancel(true);

		virtualGoodsToDownload.remove(vgItem.getVgStoreItemID());
		virtualGoodsToDownload.put(vgItem.getVgStoreItemID(), retryTask);
		
		if (TJCVirtualGoods.isPurchasedItemDetailView && TJCVirtualGoods.detailViewStoreID.equals(vgItem.getVgStoreItemID()))
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "UPDATE DETAIL VIEW");
			TJCVirtualGoods.updateDetailViewFromDownloader(retryTask);
		}
		
		addTask(retryTask, vgItem);
	}
	
	public OnClickListener errorMsgClickListener = new OnClickListener()
	{
		public void onClick(View v)
		{
			showDLErrorDialog();
		}
	};

	public OnClickListener retryClickListener = new OnClickListener()
	{
		public void onClick(View v)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "retryClick");
			
			LinearLayout linearLayout = (LinearLayout) v.getParent().getParent();
			TextView indexTextView = (TextView) linearLayout.findViewById(applicationContext.getResources().getIdentifier("vg_row_index", "id", clientPackage));
			
			String index = indexTextView.getText().toString();
			int indexValue = Integer.parseInt(index);
			
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "retry index: " + indexValue);
			
			retryDownloadVG(virtualGoodsToDownload.get(purchasedItemArray.get(indexValue).getVgStoreItemID()));
		}
	};
	
	OnClickListener retryDetailClickListener = new OnClickListener()
	{
		public void onClick(View v)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "RETRY DETAIL CLICK");
				
			try
			{
				retryDownloadVG(virtualGoodsToDownload.get(purchasedItemArray.get(detailIndex).getVgStoreItemID()));
			}
			catch (Exception e)
			{
				TapjoyLog.e(TAPJOY_VIRTUAL_GOOD_UTIL, "RETRY DETAIl EXCEPTION: " + e.toString());
			}
		}
	};

	/**
	 * Handles downloading a virtual good's data after it has been purchased.
	 * 
	 *
	 */
	public class DownloadVirtualGoodTask extends AsyncTask<VGStoreItem, Integer, VGStoreItem>
	{
		private VGStoreItem vgItem = null;
		
		// These UI elements make up the row for the table view.
		public ProgressBar localProgressBar;
		public TextView localDownloadStatusText;
		public Button localRetryButton;
		public ImageView localErrorIcon;

		public boolean downloadCancel = false;
		boolean saveToSDCard = false;						// Whether or not to save the virtual good to the SD card.
		public int virtualGoodDownloadStatus;				// Status of the current download task.
		public int progressPercent = 0;						// Progress of the current download task.
		
		public DownloadVirtualGoodTask()
		{
			saveToSDCard = false;
			virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_PENDING;
		}
		
		public DownloadVirtualGoodTask(boolean saveToSD)
		{
			saveToSDCard = saveToSD;
			virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_PENDING;
		}

		@Override
		protected void onCancelled()
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "onCancelled");
			
			super.onCancelled();
			
			if (downloadCancel == false)
			{
				tapjoyVGData.deleteVGZip(vgItem, saveToSDCard);
			}
		}

		protected VGStoreItem doInBackground(VGStoreItem... obj)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "doInBackground");
			vgItem = obj[0];
			
			downLoadVirtualGood(vgItem, saveToSDCard);
			
			return vgItem;
		}

		protected void onProgressUpdate(Integer... progress)
		{
			int prog = progress[0];
			
			// Would it ever be negative?
			if (prog < 0)
				prog = 0;
			
			// Bounds checking.
			if (prog > 100)
				prog = 100;
			
			localDownloadStatusText.setText("Downloading... " + prog + "%");
			
			localProgressBar.setVisibility(View.VISIBLE);
			localProgressBar.setProgress((int) prog);		
			
			// UPDATE DETAIL VIEW
			progressPercent = prog;
			
			// Update the progress bar.
			if (TJCVirtualGoods.isPurchasedItemDetailView && TJCVirtualGoods.detailViewStoreID.equals(vgItem.getVgStoreItemID()))
			{
				TJCVirtualGoods.updateDetailProgressBar(prog);
			}
		}

		protected void onPostExecute(VGStoreItem vgItem)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "onPostExecute: " + virtualGoodDownloadStatus);
			
			switch (virtualGoodDownloadStatus)
			{
				//--------------------------------------------------------------------------------
				// SUCCESSFUL DOWNLOAD
				//--------------------------------------------------------------------------------
				case TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_SD_CARD:
				case TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_PHONE:
					{
						localRetryButton.setVisibility(View.GONE);
						localErrorIcon.setVisibility(View.GONE);
						localProgressBar.setVisibility(View.GONE);
						localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_COMPLETED);
						
//						// Are there are no more pending downloads?
//						// Don't do this if there is pending download or the status will be erased.
//						if (TJCVirtualGoodUtil.currentTasks.size() == 0)
//						{
//							// Rebuild the purchased item table.
//							((TJCVirtualGoods)TJCVirtualGoods.ctx).syncAndRebuildPurchasedItemTable();
//						}
						
						TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "*** Download completed! ***");

						if (TJCVirtualGoods.getVirtualGoodDownloadListener() != null)
						{
							TJCVirtualGoods.getVirtualGoodDownloadListener().onDownLoad(vgItem);
						}
					}
					break;
					
				//--------------------------------------------------------------------------------
				// NETWORK ERROR
				//--------------------------------------------------------------------------------
				case TJCVirtualGoods.VG_STATUS_NETWORK_ERROR_SD_CARD:
				case TJCVirtualGoods.VG_STATUS_NETWORK_ERROR_PHONE:
					{
						localRetryButton.setVisibility(View.VISIBLE);
						localErrorIcon.setVisibility(View.VISIBLE);
						localProgressBar.setVisibility(View.GONE);
						localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_FAILED);

						dialogErrorMessage = TJCVirtualGoods.TAPJOY_DOWNLOAD_FAILED;
						
						AlertDialog.Builder builder = new AlertDialog.Builder(applicationContext);
						builder.setTitle(DownloadVirtualGoodTask.this.vgItem.getName() + " fail to download. Would you like to download again?");
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int id)
							{
								dialog.cancel();
								DownloadVirtualGoodTask.this.cancel(true);
							}
						});

						builder.setPositiveButton("Retry", new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int id)
							{
								DownloadVirtualGoodTask.this.cancel(true);
								retryDownloadVG(DownloadVirtualGoodTask.this);
							}
						});
						AlertDialog alert = builder.create();
						try
						{
							alert.show();
						}
						catch (BadTokenException e)
						{
						}
					}
					break;
					
				//--------------------------------------------------------------------------------
				// INSUFFICIENT SD CARD SPACE
				//--------------------------------------------------------------------------------
				case TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_SD_CARD:
					{
						localRetryButton.setVisibility(View.VISIBLE);
						localErrorIcon.setVisibility(View.VISIBLE);
						localProgressBar.setVisibility(View.GONE);
						localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_FAILED);
						
						dialogErrorMessage = TJCVirtualGoods.TAPJOY_ERROR_NO_SPACE_ON_DEVICE_AND_SD_CARD;
						
						// Update the detail view.
						if (TJCVirtualGoods.isPurchasedItemDetailView && TJCVirtualGoods.detailViewStoreID.equals(vgItem.getVgStoreItemID()))
						{
							TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "UPDATE DETAIL VIEW");
							TJCVirtualGoods.updateDetailViewFromDownloader(this);
						}
						
						AlertDialog.Builder builder = new AlertDialog.Builder(applicationContext);
						builder.setMessage("Not enough space on the device or SD card.  Free more space and try again.");
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int id)
							{
								dialog.cancel();
							}
						});
						AlertDialog alert = builder.create();
						try
						{
							alert.show();
						}
						catch (BadTokenException e)
						{
						}
					}
					break;
					
				//--------------------------------------------------------------------------------
				// INSUFFICIENT PHONE SPACE
				//--------------------------------------------------------------------------------
				case TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_PHONE:
					{
						if (DownloadVirtualGoodTask.this.getStatus() == AsyncTask.Status.RUNNING)
							DownloadVirtualGoodTask.this.cancel(true);

						AlertDialog.Builder builder = new AlertDialog.Builder(applicationContext);
						builder.setMessage("Not enough space on the device. Would you like to download Item '"+DownloadVirtualGoodTask.this.vgItem.getName()+"' on SD card?");
						builder.setNegativeButton("No", new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int id)
							{
								dialog.cancel();
	
								localRetryButton.setVisibility(View.VISIBLE);
								localErrorIcon.setVisibility(View.VISIBLE);
								localProgressBar.setVisibility(View.GONE);
								localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_FAILED);
								
								dialogErrorMessage = "No more space is available on Device.";
							}
						});

						builder.setPositiveButton("Yes", new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int id)
							{
//								localProgressBar.setMax(100);
//								localProgressBar.setProgress(0);
//								localProgressBar.setVisibility(View.GONE);
//								
//								DownloadVirtualGoodTask downloadAgain = new DownloadVirtualGoodTask(true);
//								
//								
//								
//								downloadAgain.localProgressBar = localProgressBar;
//								downloadAgain.localDownloadStatusText = localDownloadStatusText;
//								
//								downloadAgain.localRetryButton = localRetryButton;
//								downloadAgain.localErrorIcon = localErrorIcon;
//								
//								virtualGoodsToDownload.put(DownloadVirtualGoodTask.this.vgItem.getVgStoreItemID(), downloadAgain);
//								addTask(downloadAgain, DownloadVirtualGoodTask.this.vgItem);
								
								DownloadVirtualGoodTask retryTask = new DownloadVirtualGoodTask(true);

								TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "RETRY DOWNLOAD VG: " + DownloadVirtualGoodTask.this.vgItem.getName());
								
								retryTask.localProgressBar = localProgressBar;
								retryTask.localProgressBar.setVisibility(View.GONE);
								retryTask.localProgressBar.setProgress(0);
								retryTask.localErrorIcon = localErrorIcon;
								retryTask.localDownloadStatusText = localDownloadStatusText;

								retryTask.localRetryButton = localRetryButton;
								retryTask.localDownloadStatusText = localDownloadStatusText;
								retryTask.localDownloadStatusText.setText(TJCVirtualGoods.TAPJOY_DOWNLOAD_PENDING);
								retryTask.localRetryButton.setVisibility(View.GONE);
								retryTask.localErrorIcon.setVisibility(View.GONE);
								
								retryTask.virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_PENDING;
								
								VGStoreItem vgItem = DownloadVirtualGoodTask.this.vgItem;
								
								if (DownloadVirtualGoodTask.this.getStatus() == AsyncTask.Status.RUNNING)
									DownloadVirtualGoodTask.this.cancel(true);

								virtualGoodsToDownload.remove(vgItem.getVgStoreItemID());
								virtualGoodsToDownload.put(vgItem.getVgStoreItemID(), retryTask);
								
								if (TJCVirtualGoods.isPurchasedItemDetailView && TJCVirtualGoods.detailViewStoreID.equals(vgItem.getVgStoreItemID()))
								{
									TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "UPDATE DETAIL VIEW");
									TJCVirtualGoods.updateDetailViewFromDownloader(retryTask);
								}
								
								addTask(retryTask, vgItem);
							}
						});
						AlertDialog alert = builder.create();
						try
						{
							alert.show();
						}
						catch (BadTokenException e)
						{
						}
					}
					break;
			}
			
			if (TJCVirtualGoods.isPurchasedItemDetailView && TJCVirtualGoods.detailViewStoreID.equals(vgItem.getVgStoreItemID()))
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "UPDATE DETAIL VIEW");
				TJCVirtualGoods.updateDetailViewFromDownloader(this);
			}
			
			removeAndExecuteNext(this);
			
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "currentTask size: " + TJCVirtualGoodUtil.currentTasks.size());
			
			// Remove it from the download list if it was successful.
			if (virtualGoodDownloadStatus == TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_SD_CARD || virtualGoodDownloadStatus == TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_PHONE)
			{
				if (virtualGoodsToDownload.containsKey(vgItem.getVgStoreItemID()))
				{
					virtualGoodsToDownload.remove(vgItem.getVgStoreItemID());
				}
				
//				// Are there are no more pending downloads?
//				// Don't do this if there is pending download or the status will be erased.
//				if (TJCVirtualGoodUtil.currentTasks.size() == 0)
//				{
//					// Rebuild the purchased item table.
//					((TJCVirtualGoods)TJCVirtualGoods.ctx).syncAndRebuildPurchasedItemTable();
//				}
			}
		}

		/**
		 * Downloads a virtual good's data (zip file) from the server and saves it to the device (phone or sd card).  If
		 * the operation was successful, a SQL entry is added to the database with the virtual good's info and attributes.
		 * 
		 * virtualGoodDownloadStatus is updated with the status of the download.
		 * 
		 * @param storeItem				Virtual good store item to download.
		 * @param downloadToSDCard		Whether or not this virtual good should be saved to the SD card.
		 * @return						TRUE if virtual good was downloaded and extracted successfully, FALSE otherwise.
		 */
		private boolean downLoadVirtualGood(VGStoreItem storeItem, boolean downloadToSDCard)
		{
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "downloadVirtualGood: " + storeItem.getName() + ", sd_download: " + downloadToSDCard);
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "Downloading virtual good data for: " + storeItem.getName());
			
			boolean success = false;
			boolean networkTimeout = false;
			
			String zipURL = storeItem.getDatafileUrl();					// URL of the zip file.
			String zipFileLocation = null;								// Location to save the zip file.
			String zipExtractLocation = null;							// Location to extract the zip contents.
			String storeItemFolder;										// Folder to store this virtual good's data.
			
			virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_INIT;
			
			publishProgress(0);
			
			// No zip file associated with this virtual good.
			if (zipURL.length() == 0)
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "No zip file associated with virtual good.");
				success = true;
			}
			else
			{
				int n = zipURL.lastIndexOf("/");
				
				if (n > -1 && zipURL.length() > n)
				{
					storeItemFolder = zipURL.substring(n + 1, zipURL.length());
					
					if (storeItemFolder != null && !storeItemFolder.equals(""))
					{
						// Remove .zip from end of file name
						storeItemFolder = storeItemFolder.substring(0, storeItemFolder.length() - 4);
						
						String externalRoot = Environment.getExternalStorageDirectory().toString();
						
						TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "externalRoot: " + externalRoot);
						
						if (saveToSDCard)
						{
							zipFileLocation = externalRoot + "/" + clientPackage + "/tempZipDownloads/";	// + storeItemFolder + ".zip";
							zipExtractLocation = basePathSaveToSDCard + storeItemFolder + "/";
						}
						else
						{
							zipFileLocation = "data/data/" + clientPackage + "/tempZipDownloads/";			// + storeItemFolder + ".zip";
							zipExtractLocation = basePathSaveToPhone + storeItemFolder + "/";
						}
					}
										
					boolean zipDownloaded = true;
					BufferedInputStream inputStream = null;
					OutputStream out = null;
					
					TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "downloading zip file: " + zipURL);
					
					try
					{
						URL fileURL = new URL(zipURL);
						URLConnection connection = fileURL.openConnection();
						connection.setConnectTimeout(15000);
				        connection.setReadTimeout(30000);
						connection.connect();
						
						inputStream = new java.io.BufferedInputStream(connection.getInputStream());
						
			            int fileLength = connection.getContentLength();			            
						File fileDir = new File(zipFileLocation);
						
						// Directories must be created on SD card before file is created.
						// Otherwise the file cannot be created on the SD card.
						if (fileDir.mkdirs())
							TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "created directory at: " + fileDir.getPath());
						
						File file = new File(zipFileLocation, storeItemFolder + ".zip");
						out = new FileOutputStream(file);
						
						byte buf[] = new byte[1024];
						int len;
						
			            long total = 0;
						
						while ((len = inputStream.read(buf)) != -1)
						{
							total += len;
							
							// Up to 90%
			                publishProgress((int)(total*90/fileLength));
							out.write(buf, 0, len);
						}
						
						out.close();
						inputStream.close();
                        
                        TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "ZIP FILE SIZE: " + file.length());
						
						// If for some reason the file length is zero, assume there was a network error.
						if (file.length() == 0)
							networkTimeout = true;
					}
					catch (SocketTimeoutException e)
					{
						TapjoyLog.e(TAPJOY_VIRTUAL_GOOD_UTIL, "Network timeout: " + e.toString());
						zipDownloaded = false;
						networkTimeout = true;
					}
					catch (Exception e)
					{
						TapjoyLog.e(TAPJOY_VIRTUAL_GOOD_UTIL, "Error downloading zip file: " + e.toString());
						zipDownloaded = false;
					}
					
					if (networkTimeout == true)
					{
						TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "zip file not downloaded");

						// Network error ?
						if (saveToSDCard)
							virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_NETWORK_ERROR_SD_CARD;
						else
							virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_NETWORK_ERROR_PHONE;
						
						// Close the streams.
						try
						{
							inputStream.close();
							out.close();
						}
						catch (Exception e)
						{
							
						}
					}
					else
					{
						TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "No network error.");

						// Save the zip file to the phone or sd card.
						if (zipDownloaded)
						{
							File tempZipFile = new File(zipFileLocation + storeItemFolder + ".zip");
							
							// Was the zip extraction successful?
							if (tapjoyVGData.extractZipFileToFolder(tempZipFile, zipExtractLocation))
							{
								// Update progress bar to 100.
								for (int i = 90; i <= 100; i++)
								{
									publishProgress(i);
								}
								
								success = true;
							}
							else
							{
								// Insufficient disk space ?
								if (saveToSDCard)
									virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_SD_CARD;
								else
									virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_PHONE;
							}
						}
						else
						{
							// Insufficient disk space ?
							if (saveToSDCard)
								virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_SD_CARD;
							else
								virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_INSUFFICIENT_DISK_SPACE_PHONE;
						}
					}
				}
			}
			
			// SUCCESSFUL DOWNLOAD AND EXTRACTION
			if (success)
			{
				// Save the virtual good item info to the database.
				tapjoyVGData.saveInfo(storeItem, saveToSDCard);
				
				if (saveToSDCard)
					virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_SD_CARD;
				else
					virtualGoodDownloadStatus = TJCVirtualGoods.VG_STATUS_DOWNLOAD_SUCCESS_TO_PHONE;
				
				storeItem.setDatafileUrl(zipExtractLocation);
				
				// Delete the zip file if it exists.
				if (zipURL.length() > 0)
					tapjoyVGData.deleteVGZip(storeItem, saveToSDCard);
				
				publishProgress(100);
				
				try
				{
					// Update the Database so it has the latest quantity.
					// TapjoyLog.i(TAPJOY_DATABASE, "UPDATING DB");
					int new_value = storeItem.getNumberOwned();
					SQLiteDatabase myDB = TapjoyDatabaseUtil.getTapjoyDatabase(applicationContext);
//					StringBuilder q = new StringBuilder();

//					q.append("UPDATE tapjoy_VGStoreItems SET ItemsOwned='" + new_value + "' ");
//					q.append("WHERE VGStoreItemID='" + storeItem.getVgStoreItemID() + "'");
//					myDB.execSQL(q.toString());

					// Update the "quantity" attribute.
					StringBuilder p = new StringBuilder();
					p.append("UPDATE tapjoy_VGStoreItemAttribute SET AttributeValue='" + new_value + "' ");
					p.append("WHERE VGStoreItemID='" + storeItem.getVgStoreItemID() + "' AND AttributeName='quantity'");
					myDB.execSQL(p.toString());
				}
				catch (Exception e)
				{

				}
			}	
			
			TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "downloadVirtualGood success: " + success);
			
			return success;
		}
	}


	public void showDLErrorDialog()
	{
		AlertDialog alert = null;
		AlertDialog.Builder builder = new AlertDialog.Builder(applicationContext);

		if (dialogErrorMessage.equals(""))
			builder.setMessage("An error occured while downloading the contents of acquired item.");
		else
			builder.setMessage(dialogErrorMessage);

		builder.setNegativeButton("OK", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int id)
			{
				dialog.cancel();
			}
		});

		alert = builder.create();

		try
		{
			alert.show();
		}
		catch (BadTokenException e)
		{
		}
	}

	/**
	 * Checks for purchased virtual goods which have not been downloaded.  If any exist,
	 * then a popup dialog will appear asking if these virtual goods should be downloaded.
	 * @param ctx
	 * @param urlParameters
	 * @param packageName
	 */
	public void checkForVirtualGoods(Context ctx, String urlParameters, String packageName)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "checkForVirtualGoods");
		
		// If checkForVirtualGoods is called before creating this Activity, then the virtual good
		// connection class will be null.  We must instantiate it here.
		if (tapjoyVGConnection == null)
		{
			clientPackage = packageName;
			urlParams = urlParameters;
			
			tapjoyVGConnection = new TJCVirtualGoodsConnection(tapjoyWebServiceURL, urlParams);
		}
		
		this.downloadPurchasedVGContext = ctx;
		purchasedItemArray = new ArrayList<VGStoreItem>();
		fetchPurchasedVGItems = new CheckForVirtualGoodsTask();
		fetchPurchasedVGItems.execute();
	}

	private class CheckForVirtualGoodsTask extends AsyncTask<Void, Void, ArrayList<VGStoreItem>>
	{
		protected ArrayList<VGStoreItem> doInBackground(Void... nothing)
		{
			// If the virtual goods UI is already open, then don't proceed.
			if (virtualGoodsUIOpened)
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "virtual goods UI is already open -- aborting check for virtual goods");
				return null;
			}
			
			try
			{
				TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "fetchPurchasedVGItems");
				
				String response = tapjoyVGConnection.getAllPurchasedItemsFromServer(0, VGStoreItem.MAX_ITEMS);
				
				if (response != null && response.length() > 0)
				{
					return TJCVirtualGoodsData.parseVGItemListResponse(response, VGStoreItem.PURCHASED_ITEM,TJCVirtualGoodUtil.this.applicationContext);
				}
			}
			catch (Exception e)
			{
				
			}
			
			return null;
		}

		protected void onPostExecute(ArrayList<VGStoreItem> result)
		{
			if (result != null)
			{
				purchasedItemArray.addAll(result);
				
				ArrayList <String> downloadedVirtualGoodsIDs = new ArrayList<String>();
				
				SQLiteDatabase myDB = TapjoyDatabaseUtil.getTapjoyDatabase(applicationContext);
				Cursor cursor = myDB.rawQuery("SELECT VGStoreItemID FROM tapjoy_VGStoreItems", null);
				int vgStoreItemIDIndex = cursor.getColumnIndex("VGStoreItemID");

				if (cursor != null)
				{
					cursor.moveToFirst();
					if (cursor.isFirst())
					{
						do
						{
							// Add the downloaded store item IDs to our list.
							downloadedVirtualGoodsIDs.add(new String(cursor.getString(vgStoreItemIDIndex)));
						}
						while (cursor.moveToNext());
					}
				}

				if (cursor != null)
				{
					cursor.deactivate();
					cursor.close();
				}

				// Iterate through the purchased list.
				for (int i = 0; i < purchasedItemArray.size(); i++)
				{
					VGStoreItem vgItem = (VGStoreItem) purchasedItemArray.get(i);
					
					// Does this virtual good have downloadable data?
					if (vgItem != null && !vgItem.getDatafileUrl().equals(""))
					{
						// Does the purchased virtual good not exist in our SQL database?
						// That means it hasn't been downloaded yet.
						if (downloadedVirtualGoodsIDs.contains(vgItem.getVgStoreItemID()) == false)
						{
							TJCVirtualGoodUtil.virtualGoodsUIOpened = true;
							
							// Launch activity to display prompt asking if user wants to download these already purchased virtual goods.
							Intent intent = new Intent(downloadPurchasedVGContext, com.tapjoy.DownloadVirtualGood.class);
							intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
							intent.putExtra(DownloadVirtualGood.EXTRA_KEY_VIRTUAL_GOOD_NAME, vgItem.getName());
							intent.putExtra(TapjoyConstants.EXTRA_URL_PARAMS, urlParams);
							
							TJCVirtualGoods.doNotify = false;

							downloadPurchasedVGContext.startActivity(intent);
							break;
						}
					}
				}
			}
		}
	}

	public void cancelExecution()
	{
		Enumeration<String> en = virtualGoodsToDownload.keys();
		while(en.hasMoreElements())
		{
			String key = (String)en.nextElement();
			DownloadVirtualGoodTask dGood = (DownloadVirtualGoodTask)virtualGoodsToDownload.get(key);
			
			if (dGood != null)
			{
				if (dGood.getStatus() == AsyncTask.Status.RUNNING)
				{
					dGood.cancel(true);
				}
			}
			
			virtualGoodsToDownload.remove(key);
		}
	}

	private static final int poolSize = 1;
	public static ArrayList<AsyncTask<VGStoreItem, Integer, VGStoreItem>> currentTasks = new ArrayList<AsyncTask<VGStoreItem, Integer, VGStoreItem>>();
	public static ArrayList<Object> pendingTasks = new ArrayList<Object>();

	public static boolean addTask(AsyncTask<VGStoreItem, Integer, VGStoreItem> asyncTask, VGStoreItem... params)
	{
		TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "addTask size: " + currentTasks.size() + ", pending size: " + pendingTasks.size());
		
		if (currentTasks.size() < poolSize)
		{
			currentTasks.add(asyncTask);
			
			try
			{
				//TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "task: " + asyncTask.getStatus());
				
				if (params != null)
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "execute with params");
					asyncTask.execute(params);
				}
				else
				{
					TapjoyLog.i(TAPJOY_VIRTUAL_GOOD_UTIL, "execute");
					asyncTask.execute();
				}
			}
			catch (RejectedExecutionException e)
			{
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

	public static boolean removeTask(AsyncTask<VGStoreItem, Integer, VGStoreItem> task)
	{
		if (currentTasks.contains(task))
		{
			currentTasks.remove(task);
			return true;
		}
		return false;
	}

	// Add this method in the onPostExecute method of AsyncTask
	public boolean removeAndExecuteNext(AsyncTask<VGStoreItem, Integer, VGStoreItem> atask)
	{
		removeTask(atask);
		if (pendingTasks.size() > 0 && currentTasks.size() < poolSize)
		{
			Object[] task = (Object[]) pendingTasks.get(0);
			pendingTasks.remove(task);
			addTask((DownloadVirtualGoodTask) task[0], (VGStoreItem[]) task[1]);
		}
		return false;
	}

	public void setPurchasedItemArray(ArrayList<VGStoreItem> purchasedItemArray)
	{
		this.purchasedItemArray = purchasedItemArray;
	}
}
