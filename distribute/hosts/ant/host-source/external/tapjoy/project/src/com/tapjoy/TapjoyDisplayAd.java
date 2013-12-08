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
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
//import android.util.Base64;		// Only supported in Android v2.2+
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;

public class TapjoyDisplayAd
{
	private static TapjoyDisplayAdNotifier displayAdNotifier;
	private static TapjoyURLConnection tapjoyURLConnection = null;
	
	private Context context;
	private boolean autoRefresh;
	public static String displayAdURLParams;
	private static String displayAdSize;									// Dimensions of the display ad.
	private static String adClickURL;										// Click URL to launch when display ad is clicked.
	Bitmap bitmapImage;														// Image to use for the display ad.
	View adView;															// View with ad image and click handling.
	
	Timer timer;
	
	final String TAPJOY_DISPLAY_AD 											= "Display Ad";
	
	
	/**
	 * Constructor.
	 */
	public TapjoyDisplayAd(Context ctx)
	{	
		displayAdSize = TapjoyDisplayAdSize.TJC_AD_BANNERSIZE_320X50;
		context = ctx;
		tapjoyURLConnection = new TapjoyURLConnection();
	}
	
	
	/**
	 * Sets the size (dimensions) of the banner ad.  By default this is 320x50.<br>
	 * Supported sizes are:<br>
	 * {@link TapjoyDisplayAdSize#TJC_AD_BANNERSIZE_320X50}<br>
	 * {@link TapjoyDisplayAdSize#TJC_AD_BANNERSIZE_640X100}<br>
	 * {@link TapjoyDisplayAdSize#TJC_AD_BANNERSIZE_768X90}<br>
	 * @param dimensions			Dimensions of the banner.
	 */
	public void setBannerAdSize(String dimensions)
	{
		displayAdSize = dimensions;
	}
	
	
	/**
	 * Gets the banner ad size.
	 * @return						Dimensions of the banner.
	 */
	public String getBannerAdSize()
	{
		return displayAdSize;
	}
	
	
	/**
	 * Enables automatic refreshing of the banner ads.  Default is FALSE.
	 * @param shouldAutoRefresh		Whether banner ad should auto-refresh or not.
	 */
	public void enableAutoRefresh(boolean shouldAutoRefresh)
	{
		autoRefresh = shouldAutoRefresh;
	}
	
	
	/**
	 * Connects to the server and to get display ad data.
	 * @param notifier				Class which receives callback with Display Ad data.
	 */
	public void getDisplayAd(TapjoyDisplayAdNotifier notifier)
	{
		TapjoyLog.i(TAPJOY_DISPLAY_AD, "Get display ad");
		getDisplayAd(null, notifier);
	}
	
	
	/**
	 * Connects to the server and to get display ad data.
	 * @param currencyID			ID of the currency to display.
	 * @param notifier				Class which receives callback with Display Ad data.
	 */
	public void getDisplayAd(String currencyID, TapjoyDisplayAdNotifier notifier)
	{
		TapjoyLog.i(TAPJOY_DISPLAY_AD, "Get display ad, currencyID: " + currencyID);
		
		displayAdNotifier = notifier;
		
		// Add the user ID to our request.
		displayAdURLParams = TapjoyConnectCore.getURLParams();
		displayAdURLParams += "&" + TapjoyConstants.TJC_USER_ID + "=" + TapjoyConnectCore.getUserID();
		displayAdURLParams += "&" + TapjoyConstants.TJC_DISPLAY_AD_SIZE + "=" + displayAdSize;
		
		// Add the currency ID if it was specified.
		if (currencyID != null)
			displayAdURLParams += "&" + TapjoyConstants.TJC_CURRENCY_ID + "=" + currencyID;
		
		new Thread(new Runnable() 
		{
			public void run()
			{
				boolean returnValue = false;
				
				String response = tapjoyURLConnection.connectToURL(TapjoyConstants.TJC_SERVICE_URL + TapjoyConstants.TJC_DISPLAY_AD_URL_PATH, displayAdURLParams);
				
				// Response has no data.
				if (response == null || response.length() == 0)
				{
					displayAdNotifier.getDisplayAdResponseFailed("Network error.");
				}
				else
				{
					returnValue = buildResponse(response);
					
					// Couldn't parse the response.
					if (returnValue == false)
						displayAdNotifier.getDisplayAdResponseFailed("No ad to display.");
				}
			}
		}).start();
	}
	
	
	/**
	 * Parses the response from the server and determines whether the
	 * connection was successful.
	 * @param response			HTTP Response from the server.
	 * @return 					TRUE if the response has been handled and notified, FALSE otherwise.
	 */
	private boolean buildResponse(String response)
	{
		boolean retValue = false;
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder documentBuilder;
		
		try 
		{
			// Convert the string to an input stream.
			InputStream is = new ByteArrayInputStream(response.getBytes("UTF-8"));
			
			documentBuilder = factory.newDocumentBuilder();
			Document document = documentBuilder.parse(is);
			
			adClickURL = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("ClickURL"));
			String image_data = TapjoyUtil.getNodeTrimValue(document.getElementsByTagName("Image"));
			
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "decoding...");
			
			// Base64 is only available in Android v2.2 and higher...
			// Should enable this once Android v2.2+ are in majority of the devices.
			//byte [] data = Base64.decode(image_data, Base64.DEFAULT);
			//TapjoyLog.i(TAPJOY_DISPLAY_AD, "data: " + data.length);
			//bitmapImage = BitmapFactory.decodeByteArray(data, 0, data.length);
			
			// Decode our image data.  The result gets stored in byte [] buffer.
			decodeBase64(image_data.getBytes(), 0, image_data.getBytes().length);
			
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "pos: " + pos);
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "buffer_size: " + buffer.length);
			
			// Create our bitmap.
			bitmapImage = BitmapFactory.decodeByteArray(buffer, 0, pos);
			
//			if (bitmapImage == null)
//				TapjoyLog.e(TAPJOY_DISPLAY_AD, "bitmap is NULL");
			
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "image: " + bitmapImage.getWidth() + "x" + bitmapImage.getHeight());
			
			// Create a new view.
			adView = new View(context);
			
			// Set the view bounds.
			LayoutParams layout = new LayoutParams(bitmapImage.getWidth(), bitmapImage.getHeight());
			adView.setLayoutParams(layout);
			
			// Set the background to the bitmap iamge.
			adView.setBackgroundDrawable(new BitmapDrawable(bitmapImage));
			
			// Handle the click by launching the url.
			adView.setOnClickListener(new OnClickListener()
			{
				public void onClick(View v)
				{
					TapjoyLog.i(TAPJOY_DISPLAY_AD, "Opening URL in new browser = ["+adClickURL+"]");
					
					Intent intent = new Intent(context, com.tapjoy.TJCOffersWebView.class);
					intent.putExtra(TapjoyConstants.EXTRA_DISPLAY_AD_URL, adClickURL);
					intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
					context.startActivity(intent);
				}
			});	
			
			displayAdNotifier.getDisplayAdResponse(adView);
			
			retValue = true;
		}
		catch (Exception e) 
		{
			TapjoyLog.e(TAPJOY_DISPLAY_AD, "Error parsing XML: " + e.toString());
		}
		
		// Cancel any existing timers.
		if (timer != null)
		{
			timer.cancel();
			timer = null;
		}
		
		// If auto-refreshing is enabled, then refresh the banner in 15 seconds.
		if (autoRefresh && timer == null)
		{
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "will refresh banner ad in 15s...");
			timer = new Timer();
			timer.schedule(new RefreshTimer(), 15000);
		}
		
		return retValue;
	}
	
	
	private class RefreshTimer extends TimerTask
	{
		public void run()
		{
			TapjoyLog.i(TAPJOY_DISPLAY_AD, "refreshing banner ad...");
			getDisplayAd(displayAdNotifier);
			timer.cancel();
			timer = null;
		}
	}
	
	
	//--------------------------------------------------------------------------------
	// Base64 code based on org.apache.commons.codec.binary.Base64.java;
	// Including it in here since Base64 decoding is only supported in Android 2.2+.
	// This allows support for earlier Android versions.
	//--------------------------------------------------------------------------------
	
	
//	private static final int DEFAULT_BUFFER_RESIZE_FACTOR = 2;
//	private static final int DEFAULT_BUFFER_SIZE = 8192;
    
    /**
     * Byte used to pad output.
     */
    private static final byte PAD = '=';

    /**
     * This array is a lookup table that translates Unicode characters drawn from the "Base64 Alphabet" (as specified in
     * Table 1 of RFC 2045) into their 6-bit positive integer equivalents. Characters that are not in the Base64
     * alphabet but fall within the bounds of the array are translated to -1.
     * 
     * Note: '+' and '-' both decode to 62. '/' and '_' both decode to 63. This means decoder seamlessly handles both
     * URL_SAFE and STANDARD base64. (The encoder, on the other hand, needs to know ahead of time what to emit).
     * 
     * Thanks to "commons" project in ws.apache.org for this code.
     * http://svn.apache.org/repos/asf/webservices/commons/trunk/modules/util/
     */
    private static final byte[] DECODE_TABLE = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 62, -1, 63, 52, 53, 54,
            55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4,
            5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
            24, 25, -1, -1, -1, -1, 63, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34,
            35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };

//    /** Mask used to extract 6 bits, used when encoding */
//    private static final int MASK_6BITS = 0x3f;

    /** Mask used to extract 8 bits, used in decoding base64 bytes */
    private static final int MASK_8BITS = 0xff;

    // The static final fields above are used for the original static byte[] methods on Base64.
    // The private member fields below are used with the new streaming approach, which requires
    // some state be preserved between calls of encode() and decode().
    
//	/**
//     * Convenience variable to help us determine when our buffer is going to run out of room and needs resizing.
//     * <code>decodeSize = 3 + lineSeparator.length;</code>
//     */
//    private int decodeSize;
//
//    /**
//     * Convenience variable to help us determine when our buffer is going to run out of room and needs resizing.
//     * <code>encodeSize = 4 + lineSeparator.length;</code>
//     */
//    private final int encodeSize;

    /**
     * Buffer for streaming.
     */
    private byte[] buffer;

    /**
     * Position where next character should be written in the buffer.
     */
    private int pos;

//    /**
//     * Position where next character should be read from the buffer.
//     */
//    private int readPos;
//
//    /**
//     * Variable tracks how many characters have been written to the current line. Only used when encoding. We use it to
//     * make sure each encoded line never goes beyond lineLength (if lineLength > 0).
//     */
//    private int currentLinePos;

    /**
     * Writes to the buffer only occur after every 3 reads when encoding, an every 4 reads when decoding. This variable
     * helps track that.
     */
    private int modulus;

    /**
     * Boolean flag to indicate the EOF has been reached. Once EOF has been reached, this Base64 object becomes useless,
     * and must be thrown away.
     */
    private boolean eof;

    /**
     * Place holder for the 3 bytes we're dealing with for our base64 logic. Bitwise operations store and extract the
     * base64 encoding or decoding from this variable.
     */
    private int x;
	
	/**
     * <p>
     * Decodes all of the provided data, starting at inPos, for inAvail bytes. Should be called at least twice: once
     * with the data to decode, and once with inAvail set to "-1" to alert decoder that EOF has been reached. The "-1"
     * call is not necessary when decoding, but it doesn't hurt, either.
     * </p>
     * <p>
     * Ignores all non-base64 characters. This is how chunked (e.g. 76 character) data is handled, since CR and LF are
     * silently ignored, but has implications for other bytes, too. This method subscribes to the garbage-in,
     * garbage-out philosophy: it will not check the provided data for validity.
     * </p>
     * <p>
     * Thanks to "commons" project in ws.apache.org for the bitwise operations, and general approach.
     * http://svn.apache.org/repos/asf/webservices/commons/trunk/modules/util/
     * </p>
     * 
     * @param in
     *            byte[] array of ascii data to base64 decode.
     * @param inPos
     *            Position to start reading data from.
     * @param inAvail
     *            Amount of bytes available from input for encoding.
     */
	void decodeBase64(byte[] in, int inPos, int inAvail)
	{
		//TapjoyLog.i("BASE64", "decode: " + inPos + " to " + inAvail);

		// Reset variables.
		buffer = new byte[in.length];
		pos = 0;
		eof = false;
		modulus = 0;
		// resizeBuffer();

		// if (eof) {
		// return;
		// }
		if (inAvail < 0)
		{
			eof = true;
		}
		for (int i = 0; i < inAvail; i++)
		{
			// if (buffer == null || buffer.length - pos < decodeSize) {
			// resizeBuffer();
			// }
			byte b = in[inPos++];
			
			if (b == PAD)
			{
				// We're done.
				eof = true;
				break;
			}
			else
			{
				if (b >= 0 && b < DECODE_TABLE.length)
				{
					int result = DECODE_TABLE[b];
					if (result >= 0)
					{
						modulus = (++modulus) % 4;
						x = (x << 6) + result;
						if (modulus == 0)
						{
							buffer[pos++] = (byte) ((x >> 16) & MASK_8BITS);
							buffer[pos++] = (byte) ((x >> 8) & MASK_8BITS);
							buffer[pos++] = (byte) (x & MASK_8BITS);
						}
					}
				}
			}
		}

		//TapjoyLog.i("BASE64", "eof: " + eof + ", modulus: " + modulus);

		// Two forms of EOF as far as base64 decoder is concerned: actual
		// EOF (-1) and first time '=' character is encountered in stream.
		// This approach makes the '=' padding characters completely optional.
		if (eof && modulus != 0)
		{
			x = x << 6;
			switch (modulus)
			{
				case 2:
					x = x << 6;
					buffer[pos++] = (byte) ((x >> 16) & MASK_8BITS);
					break;
				case 3:
					buffer[pos++] = (byte) ((x >> 16) & MASK_8BITS);
					buffer[pos++] = (byte) ((x >> 8) & MASK_8BITS);
					break;
			}
		}
	}
    
//    /** Doubles our buffer. */
//    private void resizeBuffer() {
//        if (buffer == null) {
//            buffer = new byte[DEFAULT_BUFFER_SIZE];
//            pos = 0;
////            readPos = 0;
//        } else {
//            byte[] b = new byte[buffer.length * DEFAULT_BUFFER_RESIZE_FACTOR];
//            System.arraycopy(buffer, 0, b, 0, buffer.length);
//            buffer = b;
//        }
//    }
}
