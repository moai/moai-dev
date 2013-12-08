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
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Environment;

public class TapjoyVideo
{
	private static TapjoyVideo tapjoyVideo = null;
	private static TapjoyVideoNotifier tapjoyVideoNotifier;

	Context context;
	private String videoCacheDir = null;
	private String imageCacheDir = null;
	
	private int videoCacheLimit = 5;
	
	public static final String TAPJOY_VIDEO 										= "TapjoyVideo";
	private static final String BACKGROUND_FILE_NAME								= "background";							// Background stitch image.
	private static final String BACKGROUND_PLACEHOLDER_NAME							= "default";							// Video thumbnail placeholder.
	private static final String BACKGROUND_WATERMARK_NAME							= "watermark";							// Tapjoy watermark.
	
	private Vector <String> videoQueue;												// Queue of videos to download.
	private Hashtable<String, TapjoyVideoObject> uncachedVideos;					// Uncached videos.
	private Hashtable<String, TapjoyVideoObject> cachedVideos;						// Cached videos.
	
	public TapjoyVideoObject videoToPlay;
	
	private static final String imageBackgroundUrl									= "https://s3.amazonaws.com/tapjoy/videos/assets/" + BACKGROUND_FILE_NAME + ".png";
	private static final String imagePlaceholderUrl									= "https://s3.amazonaws.com/tapjoy/videos/assets/" + BACKGROUND_PLACEHOLDER_NAME + ".png";
	private static final String imageTapjoyUrl										= "https://s3.amazonaws.com/tapjoy/videos/assets/" + BACKGROUND_WATERMARK_NAME + ".png";
	
	private Vector <String> imageQueue;												// Queue of images to download.
	private Hashtable<String, String> uncachedImages;								// Uncached images.
	
	// Path where these video images are stored.
	public static String imageBackgroundLocation;
	public static String imagePlaceholderLocation;
	public static String imageTapjoyLocation;
	
	/**
	 * Constructor.
	 */
	public TapjoyVideo(Context applicationContext)
	{
		context = applicationContext;
		tapjoyVideo = this;
	}

	
	/**
	 * Instance for this class so the TapjoyVideoView activity has access to this.
	 * @return								Instance of this class.
	 */
	public static TapjoyVideo getInstance()
	{
		return tapjoyVideo;
	}
	
	
	/**
	 * Sets the limit of number of videos to keep in cache.  The default value is 5.
	 * @param count							Number of videos to cache.
	 */
	public void setVideoCacheCount(int count)
	{
		videoCacheLimit = count;
	}
	
	
	/**
	 * Initialize and cache a video.
	 * @param notifier						Class implementing TapjoyVideoNotifier. 
	 */
	public void initVideoAd(TapjoyVideoNotifier notifier)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "initVideoAd");
		
		if (notifier == null)
		{
			android.util.Log.e(TAPJOY_VIDEO, "Error during initVideoAd -- TapjoyVideoNotifier is null");
			return;
		}
		
		tapjoyVideoNotifier = notifier;

		// Videos can only play from external storage.  Does not work from phone internal storage.
//		cacheDir = Environment.getExternalStorageDirectory().toString() + "/tapjoy/cache/" + TapjoyConnectCore.getClientPackage() + "/";
		
		videoCacheDir = Environment.getExternalStorageDirectory().toString() + "/tjcache/data/";
		imageCacheDir = Environment.getExternalStorageDirectory().toString() + "/tjcache/tmp/";
		
		// Delete old cache directory.  8.1.1 and older video cache directory.
		TapjoyUtil.deleteFileOrDirectory(new File(Environment.getExternalStorageDirectory().toString() + "/tapjoy/"));
		
		videoQueue = new Vector<String>();
		uncachedVideos = new Hashtable<String, TapjoyVideoObject>();
		cachedVideos = new Hashtable<String, TapjoyVideoObject>();
		
		// Image queue
		imageQueue = new Vector<String>();
		imageQueue.addElement(imageBackgroundUrl);
		imageQueue.addElement(imagePlaceholderUrl);
		imageQueue.addElement(imageTapjoyUrl);
		
		uncachedImages = new Hashtable<String, String>();
		uncachedImages.put(BACKGROUND_FILE_NAME, imageBackgroundUrl);
		uncachedImages.put(BACKGROUND_PLACEHOLDER_NAME, imagePlaceholderUrl);
		uncachedImages.put(BACKGROUND_WATERMARK_NAME, imageTapjoyUrl);
		
		imageBackgroundLocation = imageCacheDir + BACKGROUND_FILE_NAME;
		imagePlaceholderLocation = imageCacheDir + BACKGROUND_PLACEHOLDER_NAME;
		imageTapjoyLocation = imageCacheDir + BACKGROUND_WATERMARK_NAME;
		
		// Clear video IDs.
		setVideoIDs();
		
		final String state = Environment.getExternalStorageState();
		
		if (!Environment.MEDIA_MOUNTED.equals(state))
		{
			TapjoyLog.i(TAPJOY_VIDEO, "Media storage unavailable.");
			
			// Notify there is an error because no media storage is available.
			tapjoyVideoNotifier.videoError(TapjoyVideoStatus.STATUS_MEDIA_STORAGE_UNAVAILABLE);
			return;
		}
		
		// Start thread to get list of videos and start caching videos.
		new Thread(new Runnable() 
		{
			public void run() 
			{
				boolean returnValue = false;
				
				String urlParams = TapjoyConnectCore.getURLParams();
				urlParams += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
				
				//urlParams += "&" + TapjoyConstants.TJC_CURRENCY_ID + "=" + TapjoyConnectCore.getAppID();
				
				// Get the result of the http request.
				String result = new TapjoyURLConnection().connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_GET_VIDEOS_URL_PATH, urlParams);
				
				// Handle the response for a connect call.
				if (result != null && result.length() > 0)
				{
					returnValue = handleGetVideosResponse(result);
				}
				
				// Succesfully parsed XML.
				if (returnValue)
				{
					// Get a list of our cached files on our media storage.
					File[] cachedFilesOnDisk = new File(videoCacheDir).listFiles();
					
					// We have local cached video files.
					if (cachedFilesOnDisk != null)
					{
						// Iterate through them.
						for (int i = 0; i < cachedFilesOnDisk.length; i++)
						{
							String key = cachedFilesOnDisk[i].getName();
							//key = key.substring(0, key.indexOf("."));
							
							TapjoyLog.i(TAPJOY_VIDEO, "file[" + i + "]: " + cachedFilesOnDisk[i].getAbsolutePath() + " --- " + cachedFilesOnDisk[i].getName());
							TapjoyLog.i(TAPJOY_VIDEO, "key: " + key + ", " + key);
							TapjoyLog.i(TAPJOY_VIDEO, "uncachedVideos: " + uncachedVideos.keySet().toString());
							
							// Keys match.
							if (uncachedVideos.containsKey(key))
							{
								TapjoyLog.i(TAPJOY_VIDEO, "*** Match found, comparing file sizes... ***");
								TapjoyLog.i(TAPJOY_VIDEO, "local file size: " + cachedFilesOnDisk[i].length());
								
								TapjoyVideoObject videoObject = uncachedVideos.get(key);
								String contentLength = (new TapjoyURLConnection()).getContentLength(videoObject.videoURL);
								
								// Check if file size and content-size match.
								if (contentLength != null && (Integer.parseInt(contentLength) ==  cachedFilesOnDisk[i].length()))
								{
									TapjoyLog.i(TAPJOY_VIDEO, "*** VIDEO ALREADY DOWNLOAD!!! " + key + " ***");
									
									videoObject.dataLocation = cachedFilesOnDisk[i].getAbsolutePath();
									cachedVideos.put(key, uncachedVideos.get(key));
									uncachedVideos.remove(key);
									videoQueue.remove(key);
									
									TapjoyLog.i(TAPJOY_VIDEO, "setting video location to: " + videoObject.dataLocation);
								}
								// File size mistmatch, delete the file.
								else
								{
									TapjoyLog.i(TAPJOY_VIDEO, "file size mismatch --- deleting video: " + cachedFilesOnDisk[i].getAbsolutePath());
									TapjoyUtil.deleteFileOrDirectory(cachedFilesOnDisk[i]);
								}
							}
							// No matching key in uncached videos, delete the file.
							else
							{
								TapjoyLog.i(TAPJOY_VIDEO, "VIDEO EXPIRED? removing video from cache: " + key + " --- " + cachedFilesOnDisk[i].getAbsolutePath());
								TapjoyUtil.deleteFileOrDirectory(cachedFilesOnDisk[i]);
							}
						}
					}
					
					setVideoIDs();
					
					if (cachedVideos.size() > 0)
						tapjoyVideoNotifier.videoReady();
					
					// Cache/load the next video.
					loadNextVideo();
					
					// Get a list of our cached files on our media storage.
					File[] cachedImagesOnDisk = new File(imageCacheDir).listFiles();
					
					TapjoyLog.i(TAPJOY_VIDEO, "------------------------------");
					TapjoyLog.i(TAPJOY_VIDEO, "checking image cache...");
					
					// We have local cached image files.
					if (cachedImagesOnDisk != null)
					{
						// Iterate through them.
						for (int i = 0; i < cachedImagesOnDisk.length; i++)
						{
							String key = cachedImagesOnDisk[i].getName();
							
							// Trim
							//key = key.substring(0, key.indexOf("."));
							
							TapjoyLog.i(TAPJOY_VIDEO, "file[" + i + "]: " + cachedImagesOnDisk[i].getAbsolutePath() + " --- " + cachedImagesOnDisk[i].getName());
							TapjoyLog.i(TAPJOY_VIDEO, "key: " + key);
							TapjoyLog.i(TAPJOY_VIDEO, "uncachedVideos: " + uncachedVideos.keySet().toString());
							
							// Keys match.
							if (uncachedImages.containsKey(key))
							{
								TapjoyLog.i(TAPJOY_VIDEO, "*** Match found, comparing file sizes... ***");
								TapjoyLog.i(TAPJOY_VIDEO, "local file size: " + cachedImagesOnDisk[i].length());
								
								String contentLength = (new TapjoyURLConnection()).getContentLength(uncachedImages.get(key));
								
								// Check if file size and content-size match.
								if (contentLength != null && (Integer.parseInt(contentLength) ==  cachedImagesOnDisk[i].length()))
								{
									TapjoyLog.i(TAPJOY_VIDEO, "*** image already downloaded, removing from queue: " + uncachedImages.get(key));
									
									imageQueue.remove(uncachedImages.get(key));
								}
								// File size mistmatch, delete the file.
								else
								{
									TapjoyLog.i(TAPJOY_VIDEO, "file size mismatch --- deleting image");
									TapjoyUtil.deleteFileOrDirectory(cachedImagesOnDisk[i]);
								}
							}
							// No matching key in uncached videos, delete the file.
							else
							{
								TapjoyLog.i(TAPJOY_VIDEO, "invalid image");
								TapjoyUtil.deleteFileOrDirectory(cachedImagesOnDisk[i]);
							}
						}
					}
					
					// Opted out or no videos in list.
					if ((uncachedVideos.size() == 0) && (cachedVideos.size() == 0))
					{
						TapjoyLog.i(TAPJOY_VIDEO, "*** no videos returned ***");
						TapjoyLog.i(TAPJOY_VIDEO, "*** delete cache directory ***");
						
						// Delete the cache directory.
						TapjoyUtil.deleteFileOrDirectory(new File(Environment.getExternalStorageDirectory().toString() + "/tjcache/"));
					}
					else
					{
						if (imageQueue.size() > 0)
						{
							cacheImage(imageQueue.elementAt(0));
						}
					}
					
					TapjoyLog.i(TAPJOY_VIDEO, "------------------------------");
					TapjoyLog.i(TAPJOY_VIDEO, "------------------------------");
					TapjoyLog.i(TAPJOY_VIDEO, "INIT DONE!");
					TapjoyLog.i(TAPJOY_VIDEO, "------------------------------");
				}
			}
		}).start();
	}

	
	/**
	 * Handles the response to "get videos" by parsing XML and creating video data objects.
	 * @param response						HTTP response of get videos.
	 * @return
	 */
	private boolean handleGetVideosResponse(String response)
	{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder documentBuilder;
		
		TapjoyLog.i(TAPJOY_VIDEO, "========================================");
		
		try 
		{
			// Convert the string to an input stream.
			InputStream is = new ByteArrayInputStream(response.getBytes("UTF-8"));
			
			documentBuilder = factory.newDocumentBuilder();
			Document document = documentBuilder.parse(is);
			
			// XML escape
			document.getDocumentElement().normalize();
			
			NodeList nodelistParent = document.getElementsByTagName("TapjoyVideos");
			NodeList nodelist = nodelistParent.item(0).getChildNodes();
			Node node;
			
			TapjoyLog.i(TAPJOY_VIDEO, "nodelistParent length: " + nodelistParent.getLength());
			TapjoyLog.i(TAPJOY_VIDEO, "nodelist length: " + nodelist.getLength());
			
			for (int i = 0; i < nodelist.getLength(); i++)
			{
				node = nodelist.item(i);

				Element element;
				TapjoyVideoObject videoObject = new TapjoyVideoObject();

				if (node != null && node.getNodeType() == Node.ELEMENT_NODE)
				{
					element = (Element) node;

					String value; 
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("ClickURL"));
					if (value != null && !value.equals(""))
						videoObject.clickURL = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("OfferID"));
					if (value != null && !value.equals(""))
						videoObject.offerID = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Name"));
					if (value != null && !value.equals(""))
						videoObject.videoAdName = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("Amount"));
					if (value != null && !value.equals(""))
						videoObject.currencyAmount = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("CurrencyName"));
					if (value != null && !value.equals(""))
						videoObject.currencyName = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("VideoURL"));
					if (value != null && !value.equals(""))
						videoObject.videoURL = value;
					
					value = TapjoyUtil.getNodeTrimValue(element.getElementsByTagName("IconURL"));
					if (value != null && !value.equals(""))
						videoObject.iconURL = value;

					TapjoyLog.i(TAPJOY_VIDEO, "-----");
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.clickURL: " + videoObject.clickURL);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.offerID: " + videoObject.offerID);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.videoAdName: " + videoObject.videoAdName);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.currencyAmount: " + videoObject.currencyAmount);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.currencyName: " + videoObject.currencyName);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.videoURL: " + videoObject.videoURL);
					TapjoyLog.i(TAPJOY_VIDEO, "videoObject.iconURL: " + videoObject.iconURL);
					
					NodeList buttonData = element.getElementsByTagName("Buttons");
					NodeList itemNodeList = buttonData.item(0).getChildNodes();
					
					//TapjoyLog.i(TAPJOY_VIDEO, "buttons node length: " + itemNodeList.getLength());
					
					// "Button" tag
					for (int j = 0; j < itemNodeList.getLength(); j++)
					{
						// BUTTON
						NodeList child = itemNodeList.item(j).getChildNodes();
						//TapjoyLog.i(TAPJOY_VIDEO, "button length: " + child.getLength());
						
						if (child.getLength() == 0)
						{
							
						}
						else
						{
							// NAME
							//String tag = ((Element)child.item(0)).getTagName();
							String name = child.item(0).getFirstChild().getNodeValue();
							
							TapjoyLog.i(TAPJOY_VIDEO, "name: " + name);
							
							// URL
							//String tag2 = ((Element)child.item(1)).getTagName();
							String url = child.item(1).getFirstChild().getNodeValue();
							
							TapjoyLog.i(TAPJOY_VIDEO, "url: " + url);
							
							videoObject.addButton(name, url);
						}
					}
					
					videoQueue.addElement(videoObject.offerID);
					uncachedVideos.put(videoObject.offerID, videoObject);
					
					setVideoIDs();
				}
			}
		}
		catch (Exception e) 
		{
			TapjoyLog.e(TAPJOY_VIDEO, "Error parsing XML: " + e.toString());
			return false;
		}
		
		TapjoyLog.i(TAPJOY_VIDEO, "========================================");
		
		return true;
	}
	
	
	/**
	 * Get video data of currently playing video.  Used by the video view.
	 * @return								Video Object data.
	 */
	public TapjoyVideoObject getCurrentVideoData()
	{
		return videoToPlay;
	}
	
	
	/**
	 * Plays a video.
	 */
	public boolean startVideo(String videoID, String currencyName, String currencyAmount, String clickURL)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "Starting video activity with video: " + videoID);
		
		videoToPlay = cachedVideos.get(videoID);
		
		final String state = Environment.getExternalStorageState();
		
		// SD/external media issue.
		if (!Environment.MEDIA_MOUNTED.equals(state))
		{
			TapjoyLog.e(TAPJOY_VIDEO, "Cannot access external storage");
			
			// Notify there is an error because no media storage is available.
			tapjoyVideoNotifier.videoError(TapjoyVideoStatus.STATUS_MEDIA_STORAGE_UNAVAILABLE);
			return false;
		}
		
		// Video object is null?
		if (videoToPlay == null)
		{
			TapjoyLog.e(TAPJOY_VIDEO, "null video object?");
			TapjoyLog.e(TAPJOY_VIDEO, "cached videos: " + cachedVideos.keys().toString());
			return false;
		}
		
		videoToPlay.currencyName = currencyName;
		videoToPlay.currencyAmount = currencyAmount;
		videoToPlay.clickURL = clickURL;
		
		TapjoyLog.i(TAPJOY_VIDEO, "videoToPlay: " + videoToPlay.offerID);
		TapjoyLog.i(TAPJOY_VIDEO, "amount: " + videoToPlay.currencyAmount);
		TapjoyLog.i(TAPJOY_VIDEO, "currency: " + videoToPlay.currencyName);
		TapjoyLog.i(TAPJOY_VIDEO, "clickURL: " + videoToPlay.clickURL);
		TapjoyLog.i(TAPJOY_VIDEO, "location: " + videoToPlay.dataLocation);
		
		// File doesn't exist.
		if (new File(videoToPlay.dataLocation).exists() == false)
		{
			TapjoyLog.e(TAPJOY_VIDEO, "video file does not exist?");
			return false;
		}
		
		Intent videoIntent = new Intent(context, com.tapjoy.TapjoyVideoView.class);
		videoIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		
		// The video to play is always the first element in the Vector.
		videoIntent.putExtra(TapjoyConstants.EXTRA_VIDEO_PATH, videoID);
		context.startActivity(videoIntent);
		
		return true;
	}
	
	
	/**
	 * Load the next video and cache it.
	 */
	public void loadNextVideo()
	{
		TapjoyLog.i(TAPJOY_VIDEO, "++++++++++++++++++++++++++++++++++++++++");
		TapjoyLog.i(TAPJOY_VIDEO, "LOAD NEXT VIDEO");
		TapjoyLog.i(TAPJOY_VIDEO, "videoQueue size: " + videoQueue.size());
		TapjoyLog.i(TAPJOY_VIDEO, "uncachedVideos size: " + uncachedVideos.size());
		TapjoyLog.i(TAPJOY_VIDEO, "cachedVideos size: " + cachedVideos.size());
		
		// Load the next video if we're under the limit and the queue isn't empty.
		if (cachedVideos.size() < videoCacheLimit && videoQueue.size() > 0)
		{
			String url = uncachedVideos.get(videoQueue.elementAt(0)).videoURL;
			cacheVideo(url);
		}
		
		TapjoyLog.i(TAPJOY_VIDEO, "++++++++++++++++++++++++++++++++++++++++");
	}
	
	
	/**
	 * Helper method to allow access to the TapjoyVideoNotifier.
	 * @return								The TapjoyVideoNotifier being used.
	 */
	public static TapjoyVideoNotifier getVideoNotifier()
	{
		return tapjoyVideoNotifier;
	}
	
	
	/**
	 * Caches a video from the source URL.
	 * @param url							URL of the video to cache.
	 */
	private void cacheVideo(final String url)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "download and cache video from: " + url);
		
		new Thread(new Runnable() 
		{
			public void run() 
			{
				long time = System.currentTimeMillis();
				
				boolean networkTimeout = false;
				boolean downloadError = false;
				
				BufferedInputStream inputStream = null;
				OutputStream out = null;
				
				String fileName = null;
				String path = null;
				File savedFile = null;
				
				try
				{
					URL fileURL = new URL(url);
					URLConnection connection = fileURL.openConnection();
					connection.setConnectTimeout(15000);
					connection.setReadTimeout(30000);
					connection.connect();
		
					inputStream = new java.io.BufferedInputStream(connection.getInputStream());
		
					//int fileLength = connection.getContentLength();	
					
					File fileDir = new File(videoCacheDir);
					
					path = url.substring(0, url.lastIndexOf("/")+1);
					fileName = url.substring(url.lastIndexOf("/")+1);
					
					// Trim extension from fileName.
					fileName = fileName.substring(0, fileName.indexOf('.'));
					
					TapjoyLog.i(TAPJOY_VIDEO, "fileDir: " + fileDir);
					TapjoyLog.i(TAPJOY_VIDEO, "path: " + path);
					TapjoyLog.i(TAPJOY_VIDEO, "file name: " + fileName);
					
					// Directories must be created on SD card before file is created.
					// Otherwise the file cannot be created on the SD card.
					if (fileDir.mkdirs())
						TapjoyLog.i(TAPJOY_VIDEO, "created directory at: " + fileDir.getPath());
					
					savedFile = new File(videoCacheDir, fileName);
					out = new FileOutputStream(savedFile);
					
					TapjoyLog.i(TAPJOY_VIDEO, "downloading video file to: " + savedFile.toString());
					
					byte buf[] = new byte[1024];
					int len;
					
					long total = 0;
					
					while ((len = inputStream.read(buf)) != -1)
					{
						total += len;
						out.write(buf, 0, len);
					}
					
					out.close();
					inputStream.close();
					
					TapjoyLog.i(TAPJOY_VIDEO, "FILE SIZE: " + savedFile.length());
					
					// If for some reason the file length is zero, assume there was a network error.
					if (savedFile.length() == 0)
						networkTimeout = true;
				}
				catch (SocketTimeoutException e)
				{
					TapjoyLog.e(TAPJOY_VIDEO, "Network timeout: " + e.toString());
					networkTimeout = true;
					downloadError = true;
				}
				catch (Exception e)
				{
					TapjoyLog.e(TAPJOY_VIDEO, "Error caching video file: " + e.toString());
					downloadError = true;
				}
				
				if (networkTimeout == true)
				{
					TapjoyLog.i(TAPJOY_VIDEO, "Network timeout");
					
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
				
				if (networkTimeout == false && downloadError == false)
				{
					String key = videoQueue.elementAt(0);
					TapjoyVideoObject newVideo = uncachedVideos.get(key);
					
					newVideo.dataLocation = savedFile.getAbsolutePath();
					
					SharedPreferences settings = context.getSharedPreferences(TapjoyConstants.TJC_VIDEOS_PREFERENCE, 0);
					SharedPreferences.Editor editor = settings.edit();
					editor.putString(key, key);		
					editor.commit();
					
					cachedVideos.put(key, newVideo);
					uncachedVideos.remove(key);
					videoQueue.removeElementAt(0);
					
					setVideoIDs();
					
					loadNextVideo();
					
					TapjoyLog.i(TAPJOY_VIDEO, "video cached in: " + (System.currentTimeMillis() - time) + "ms");
					
					tapjoyVideoNotifier.videoReady();
				}
				else
				{
					// Error.
					tapjoyVideoNotifier.videoError(TapjoyVideoStatus.STATUS_NETWORK_ERROR_ON_INIT_VIDEOS);
				}
			}
		}).start();
	}
	
	
	/**
	 * Cache images used in the video view.
	 * @param url
	 */
	private void cacheImage(final String url)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "cacheImage: " + url);
		
		new Thread(new Runnable() 
		{
			public void run() 
			{
				long time = System.currentTimeMillis();
				
				boolean networkTimeout = false;
				boolean downloadError = false;
				
				BufferedInputStream inputStream = null;
				OutputStream out = null;
				
				String fileName = null;
				String path = null;
				File savedFile = null;
				
				try
				{
					URL fileURL = new URL(url);
					URLConnection connection = fileURL.openConnection();
					connection.setConnectTimeout(15000);
					connection.setReadTimeout(30000);
					connection.connect();
		
					inputStream = new java.io.BufferedInputStream(connection.getInputStream());
		
					//int fileLength = connection.getContentLength();	
					
					File fileDir = new File(imageCacheDir);
					
					path = url.substring(0, url.lastIndexOf("/")+1);
					fileName = url.substring(url.lastIndexOf("/")+1);
					
					// Trim extension from fileName.
					fileName = fileName.substring(0, fileName.indexOf('.'));
					
					TapjoyLog.i(TAPJOY_VIDEO, "fileDir: " + fileDir);
					TapjoyLog.i(TAPJOY_VIDEO, "path: " + path);
					TapjoyLog.i(TAPJOY_VIDEO, "file name: " + fileName);
					
					// Directories must be created on SD card before file is created.
					// Otherwise the file cannot be created on the SD card.
					if (fileDir.mkdirs())
						TapjoyLog.i(TAPJOY_VIDEO, "created directory at: " + fileDir.getPath());
					
					savedFile = new File(imageCacheDir, fileName);
					out = new FileOutputStream(savedFile);
					
					TapjoyLog.i(TAPJOY_VIDEO, "downloading image file to: " + savedFile.toString());
					
					byte buf[] = new byte[1024];
					int len;
					
					long total = 0;
					
					while ((len = inputStream.read(buf)) != -1)
					{
						total += len;
						out.write(buf, 0, len);
					}
					
					out.close();
					inputStream.close();
					
					TapjoyLog.i(TAPJOY_VIDEO, "FILE SIZE: " + savedFile.length());
					
					// If for some reason the file length is zero, assume there was a network error.
					if (savedFile.length() == 0)
						networkTimeout = true;
				}
				catch (SocketTimeoutException e)
				{
					TapjoyLog.e(TAPJOY_VIDEO, "Network timeout: " + e.toString());
					networkTimeout = true;
					downloadError = true;
				}
				catch (Exception e)
				{
					TapjoyLog.e(TAPJOY_VIDEO, "Error caching image file: " + e.toString());
					downloadError = true;
				}
				
				if (networkTimeout == true)
				{
					TapjoyLog.i(TAPJOY_VIDEO, "Network timeout");
					
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
				
				if (networkTimeout == false && downloadError == false)
				{
					imageQueue.removeElementAt(0);
					
					if (imageQueue.size() > 0)
						cacheImage(imageQueue.elementAt(0));
					
					TapjoyLog.i(TAPJOY_VIDEO, "image cached in: " + (System.currentTimeMillis() - time) + "ms");
				}
				else
				{
					if (imageQueue.size() > 0)
						cacheImage(imageQueue.elementAt(0));
				}
			}
		}).start();
	}
	
	
	/**
	 * Set the video ids of cached videos so that they may be passed onto the offer wall.
	 */
	private void setVideoIDs()
	{
		String videoIDs = "";
		
		if (cachedVideos != null && cachedVideos.size() > 0)
		{
			Enumeration<String> keys = cachedVideos.keys();
	
			while (keys.hasMoreElements())
			{
				String key = keys.nextElement();
				videoIDs += key;
				
				if (keys.hasMoreElements())
					videoIDs += ",";
			}
			
			TapjoyLog.i(TAPJOY_VIDEO, "cachedVideos size: " + cachedVideos.size());
		}
		
		TapjoyLog.i(TAPJOY_VIDEO, "videoIDs: [" + videoIDs + "]");
		TapjoyConnectCore.setVideoIDs(videoIDs);
	}
}