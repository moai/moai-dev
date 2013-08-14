// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.net.URL;
import java.net.URLConnection;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.RectF;
import android.graphics.Shader.TileMode;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.StateListDrawable;
import android.graphics.drawable.GradientDrawable.Orientation;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.VideoView;


public class TapjoyVideoView extends Activity implements OnCompletionListener, OnErrorListener, OnPreparedListener
{
	private VideoView videoView = null;
	private String videoPath = null;
	private TextView overlayText = null;
	
	private TextView detailsText = null;
	private TextView whatNowText = null;
	
	private Button replayButton = null;
	private Button closeButton = null;
	private Button linkButton = null;
	private Button linkAltButton = null;

	private RelativeLayout relativeLayout;
	private LinearLayout linearLayout;
	
	Timer timer = null;

	private static TapjoyVideoObject videoData;
	
	private boolean clickRequestSuccess = false;													// Whether the click request was successful or not (sent when video starts).
	private boolean allowBackKey = false;															// Whether to allow the BACK key to traverse to the previous activity.
	private int timeRemaining = 0;																	// Time remaining on video for textview.
	private int seekTime = 0;																		// Time to seek to if there is a pause/resume on the video.
	
	
	private static final int DIALOG_WARNING_ID = 0;
	private static final String videoWillResumeText = "";
	private static final String videoSecondsText = " seconds";
	
	private ImageView thumbnailImage;
	private ImageView tapjoyImage;																	// Tapjoy watermark image shown on bottom right while video is playing.
	private Bitmap videoScreenshot;																	// Screenshot/thumbnail of video on the video UI page.
	
	int deviceScreenDensity;
	int deviceScreenLayoutSize;
	
	private String currencyAmount;																	// Amount of currency being awarded for this video.
	private String currencyName;																	// Name of the currency.
	private String adText;																			// Video name/title.
	private String iconURL;
	
	// The UI uses a webview, so these are the parameters for the font.
	private static final String TJC_VIDEO_FONT_TYPE				= "\"helvetica\"";
	private static final String TJC_VIDEO_FONT_COLOR 			= "\"black\"";
	private static String TJC_VIDEO_FONT_SIZE				= "\"4\"";
	private static final String TJC_VIDEO_CURRENCY_TEXT_COLOR	= "#004194";
	private static String TJC_VIDEO_AD_TEXT_FONT_SIZE		= "\"3\"";
	private static final String TJC_VIDEO_AD_TEXT_FONT_COLOR	= "#909090";
	
	final String TAPJOY_VIDEO = "VIDEO";

	// Need handler for callbacks to the UI thread
	final Handler mHandler = new Handler();
	
	static int textSize = 16;
	static int padding = 10;
	static int buttonHeight = -1;
	
	final static int colorsBackground[] = {
			0xFF8A8A8A,
			0xFF444444,
	};

	final static int colorsInfo[] = {
			0xFFFFFFFF,
			0xFFE8E8E8,
	};
	
	final static int colorsButtons[] = {
			0xFFFFFFFF,
			0xFFE8E8E8,
	};
	
	final static float [] cornerAll = { 
			10.0f, 10.0f, 
			10.0f, 10.0f, 
			10.0f, 10.0f, 
			10.0f, 10.0f };
	
	final static float[] outerRadii = new float[] { 12, 12, 12, 12, 0, 0, 0, 0 };
	final static RectF   inset = new RectF(6, 6, 6, 6);
	final static float[] innerRadii = new float[] { 12, 12, 0, 0, 12, 12, 0, 0 };
	
	final static int statePressed = android.R.attr.state_pressed;
	final static int stateFocused = android.R.attr.state_focused;
	final static int stateSelected = android.R.attr.state_selected;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onCreate");
		
		// Something went wrong?  break out.
		if (TapjoyVideo.getInstance() == null)
		{
			finish();
			return;
		}
		
		videoData = TapjoyVideo.getInstance().getCurrentVideoData();
		
		videoPath = videoData.dataLocation;
		
		currencyAmount = videoData.currencyAmount;
		currencyName = videoData.currencyName;
		adText = videoData.videoAdName;
		iconURL = videoData.iconURL;
		
		// Download the icon while we're waiting.
		if (iconURL != null && iconURL.length() > 0)
		{
			new Thread(new Runnable()
				{
					@Override
					public void run()
					{
						URL fileURL;
						try
						{
							fileURL = new URL(iconURL);
						
							URLConnection connection = fileURL.openConnection();
							connection.setConnectTimeout(15000);
					        connection.setReadTimeout(25000);
							connection.connect();
							
							videoScreenshot = BitmapFactory.decodeStream(fileURL.openConnection().getInputStream());
//							iconHandler.post(updateIcon);
						}
						catch (Exception e)
						{
							TapjoyLog.i(TAPJOY_VIDEO, "e: " + e.toString()); 
						}
					}
		
				}
			).start();
		}
		
		TapjoyLog.i(TAPJOY_VIDEO, "videoPath: " + videoPath);
		
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		relativeLayout = new RelativeLayout(this);
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
		relativeLayout.setLayoutParams(params);
		
		setContentView(relativeLayout);
		
		// Only allow for android-4 or greater.
		if (Integer.parseInt(android.os.Build.VERSION.SDK) > 3) 
		{
			TapjoyDisplayMetricsUtil displayMetricsUtil = new TapjoyDisplayMetricsUtil(this);
			
			deviceScreenLayoutSize = displayMetricsUtil.getScreenLayoutSize();
			
			TapjoyLog.i(TAPJOY_VIDEO, "deviceScreenLayoutSize: " + deviceScreenLayoutSize);
			
			// Resize for tablets.
			if (deviceScreenLayoutSize == Configuration.SCREENLAYOUT_SIZE_XLARGE)
			{
				Display display = getWindowManager().getDefaultDisplay(); 
				int width = display.getWidth();
				int height = display.getHeight();
				
				TapjoyLog.i(TAPJOY_VIDEO, "width: " + width + ", height: " + height);
				
				padding = 20;
				textSize = 32;
				buttonHeight = height/7;
				
				TJC_VIDEO_FONT_SIZE				= "\"10\"";
				TJC_VIDEO_AD_TEXT_FONT_SIZE		= "\"6\"";
				
				TapjoyLog.i(TAPJOY_VIDEO, "buttonHeight: " + buttonHeight);
			}
		}
		
		TapjoyLog.i(TAPJOY_VIDEO, "padding: " + padding);
		TapjoyLog.i(TAPJOY_VIDEO, "textSize: " + textSize);
		
		initVideoView();
		
		TapjoyLog.i(TAPJOY_VIDEO, "onCreate DONE");
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		
		// If the video is playing, store the seek time so we can resume to it.
		if (videoView.isPlaying())
		{
			TapjoyLog.i(TAPJOY_VIDEO, "onPause");
			seekTime = videoView.getCurrentPosition();
			TapjoyLog.i(TAPJOY_VIDEO, "seekTime: " + seekTime);
		}
	}
	
	
	@Override
	protected void onResume()
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onResume");
		
		// Set orientation to landscape.
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		super.onResume();
		
		// The video was interrupted, so resume where it left off at.
		if (seekTime > 0)
		{
			TapjoyLog.i(TAPJOY_VIDEO, "seekTime: " + seekTime);
			
			videoView.seekTo(seekTime);
			videoView.start();
		}
	}
	

	@Override
	public void onWindowFocusChanged(boolean hasFocus)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onWindowFocusChanged");
		super.onWindowFocusChanged(hasFocus);
	}
	
	
	private void initVideoView()
	{
		relativeLayout.removeAllViews();
		relativeLayout.setBackgroundColor(0xFF000000);
		
		if (videoView == null && overlayText == null)
		{
			//----------------------------------------
			// TAPJOY Watermark
			//----------------------------------------
			tapjoyImage = new ImageView(this);
			Bitmap bitmap = BitmapFactory.decodeFile(TapjoyVideo.imageTapjoyLocation);
			
			if (bitmap != null)
				tapjoyImage.setImageBitmap(bitmap);
			
			RelativeLayout.LayoutParams imageParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
			imageParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
			imageParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			tapjoyImage.setLayoutParams(imageParams);
			
			//----------------------------------------
			// VIDEO VIEW
			//----------------------------------------
			videoView = new VideoView(this);
			videoView.setOnCompletionListener(this);
			videoView.setOnErrorListener(this);
			videoView.setOnPreparedListener(this);
			videoView.setVideoPath(videoPath);
			
			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
			layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);
			videoView.setLayoutParams(layoutParams);
			
			//----------------------------------------
			// X SECONDS TEXTVIEW
			//----------------------------------------
			timeRemaining = videoView.getDuration()/1000;
	
			TapjoyLog.i(TAPJOY_VIDEO, "videoView.getDuration(): " + videoView.getDuration());
			TapjoyLog.i(TAPJOY_VIDEO, "timeRemaining: " + timeRemaining);
			
			overlayText = new TextView(this);
			overlayText.setTextSize(textSize);
			overlayText.setTypeface(Typeface.create("default", Typeface.BOLD), Typeface.BOLD);
	
			RelativeLayout.LayoutParams textParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
			textParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
			overlayText.setLayoutParams(textParams);
		}
		
		startVideo();
		
		relativeLayout.addView(videoView);
		relativeLayout.addView(tapjoyImage);
		relativeLayout.addView(overlayText);
	}
	
	
	private void initVideoUI()
	{
		Display display = getWindowManager().getDefaultDisplay(); 
		int width = display.getWidth();
		int height = display.getHeight();
		
		TapjoyLog.i(TAPJOY_VIDEO, "view dimensions: " + width + "x" + height);
		
		// Recheck these parameters?  Sometimes it doesn't get the correct values if done in onCreate..
		// Only allow for android-4 or greater.
		if (Integer.parseInt(android.os.Build.VERSION.SDK) > 3) 
		{
			TapjoyDisplayMetricsUtil displayMetricsUtil = new TapjoyDisplayMetricsUtil(this);
			
			deviceScreenLayoutSize = displayMetricsUtil.getScreenLayoutSize();
			
			TapjoyLog.i(TAPJOY_VIDEO, "deviceScreenLayoutSize: " + deviceScreenLayoutSize);
			
			// Resize for tablets.
			if (deviceScreenLayoutSize == Configuration.SCREENLAYOUT_SIZE_XLARGE)
			{
				padding = 20;
				textSize = 32;
				buttonHeight = height/7;
				
				TJC_VIDEO_FONT_SIZE				= "\"10\"";
				TJC_VIDEO_AD_TEXT_FONT_SIZE		= "\"6\"";
				
				TapjoyLog.i(TAPJOY_VIDEO, "buttonHeight: " + buttonHeight);
			}
		}
		
		int infoViewHeight = height/3;
		int buttonWidth = display.getWidth()/2 - padding*2;
		
		linearLayout = new LinearLayout(this);
		linearLayout.setOrientation(LinearLayout.VERTICAL);
		LinearLayout.LayoutParams linearLayoutParams = new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
		linearLayoutParams.setMargins(padding, padding, padding, padding);
		linearLayout.setLayoutParams(linearLayoutParams);
		
		//----------------------------------------
		// DETAILS TEXT
		//----------------------------------------
		detailsText = new TextView(this);
		detailsText.setText("Details");
		detailsText.setTextColor(0xFFFFFFFF);
		detailsText.setTextSize(textSize);
		detailsText.setTypeface(Typeface.create("default", Typeface.BOLD), Typeface.BOLD);
		detailsText.setShadowLayer(1.0f, 1.0f, 1.0f, 0xFF000000);
		LinearLayout.LayoutParams detailsParams = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		detailsParams.setMargins(padding, padding, padding, 0);
		detailsText.setLayoutParams(detailsParams);
		
		detailsText.setId(2111);
		
//		TapjoyLog.i(TAPJOY_VIDEO, "padding bottom: " + detailsText.getPaddingBottom());
//		TapjoyLog.i(TAPJOY_VIDEO, "padding top: " + detailsText.getPaddingTop());
//		TapjoyLog.i(TAPJOY_VIDEO, "padding left: " + detailsText.getPaddingLeft());
//		TapjoyLog.i(TAPJOY_VIDEO, "padding right: " + detailsText.getPaddingRight());
		
		//detailsText.setPadding(detailsText.getPaddingLeft(), detailsText.getPaddingBottom(), detailsText.getPaddingRight(), detailsText.getPaddingBottom());
		
		//----------------------------------------
		// Tiled background stitch
		//----------------------------------------
		try
		{
			Bitmap bitmap = BitmapFactory.decodeFile(TapjoyVideo.imageBackgroundLocation);
			
			if (bitmap != null)
			{
				BitmapDrawable tiledBackground = new BitmapDrawable(bitmap);
				tiledBackground.setTileModeX(TileMode. REPEAT);
				tiledBackground.setTileModeY(TileMode.REPEAT);
				
				linearLayout.setBackgroundDrawable(tiledBackground);
			}
		}
		catch (Exception e)
		{
			TapjoyLog.i(TAPJOY_VIDEO, "error decoding background image: " + e.toString());
		}
		
		//----------------------------------------
		// THUMBNAIL AND TEXT
		//----------------------------------------
		thumbnailImage = new ImageView(this);
		
		try
		{
			Bitmap bitmap;
			
			if (videoScreenshot != null)
				bitmap = videoScreenshot;
			else 
				bitmap = BitmapFactory.decodeFile(TapjoyVideo.imagePlaceholderLocation);
			
			if (bitmap != null)
			{
				int w = bitmap.getWidth();
				int h = bitmap.getHeight();
	
				// We have to make sure our rounded corners have an alpha channel in most cases
				Bitmap rounder = Bitmap.createBitmap(w,h,Bitmap.Config.ARGB_8888);
				Canvas canvas = new Canvas(rounder);    
	
				// We're going to apply this paint eventually using a porter-duff xfer mode.
				// This will allow us to only overwrite certain pixels. RED is arbitrary. This
				// could be any color that was fully opaque (alpha = 255)
				Paint xferPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
				xferPaint.setColor(Color.RED);
	
				// We're just reusing xferPaint to paint a normal looking rounded box, the 20.f
				// is the amount we're rounding by.
				canvas.drawRoundRect(new RectF(0, 0, w, h), 25.0f, 25.0f, xferPaint);     
	
				// Now we apply the 'magic sauce' to the paint  
				xferPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_IN));
				
				// Now apply this bitmap ontop of your image:
				Bitmap roundedBitmap = Bitmap.createBitmap(w,h,Bitmap.Config.ARGB_8888);
				Canvas result = new Canvas(roundedBitmap);
				
				result.drawBitmap(bitmap, 0, 0, null);
				
				// Uncomment for rounded edges.
//				result.drawBitmap(rounder, 0, 0, xferPaint);
				
				int desiredHeight = infoViewHeight;
				int desiredWidth = (w*desiredHeight)/h;
				
				// Scale the bitmap to fit inside the view.
				Bitmap scaled = Bitmap.createScaledBitmap(roundedBitmap, desiredWidth, desiredHeight, true);
				
				thumbnailImage.setImageBitmap(scaled);
				thumbnailImage.setPadding(0, 0, 0, 0);
			}
		}
		catch (Exception e)
		{
			TapjoyLog.i(TAPJOY_VIDEO, "error decoding thumbnail image: " + e.toString());
		}
		
		WebView webview = new WebView(this);
		webview.setBackgroundColor(0x00000000);
	
		String summary = 
			"<html><body>" +
			"<font face=" + TJC_VIDEO_FONT_TYPE + 
			" color=" + TJC_VIDEO_FONT_COLOR + 
			" size=" + TJC_VIDEO_FONT_SIZE + ">" + 
			"<p>You just earned <font color=" + TJC_VIDEO_CURRENCY_TEXT_COLOR + "><b>" + currencyAmount + "</b></font> " + currencyName + "! <br /> <font color=" + TJC_VIDEO_AD_TEXT_FONT_COLOR + 
			" size=" + TJC_VIDEO_AD_TEXT_FONT_SIZE +">" + 
			"<b>" + adText + "</b></p></font></font></body></html>";
		
		// Load the webview from our HTML string.
		webview.loadData(summary, "text/html", "utf-8");
		
		LinearLayout webLinearLayout = new LinearLayout(this);
		webLinearLayout.addView(webview);
		
		// Add padding on top or else the webview text is not centered vertically...
		// Add padding to the left to make it hug the thumbnail image.
		webLinearLayout.setPadding(-padding/2, padding/2, 0, 0);
		
		LinearLayout customLinearLayoutView = new LinearLayout(this);
		LinearLayout.LayoutParams customLinearLayoutViewParams = new LinearLayout.LayoutParams(width - padding*2, infoViewHeight);
		customLinearLayoutViewParams.setMargins(padding, padding, padding, padding);
		customLinearLayoutView.setLayoutParams(customLinearLayoutViewParams);
		
		customLinearLayoutView.setOrientation(LinearLayout.HORIZONTAL);
		customLinearLayoutView.setPadding(0, padding*2, 0, padding*2);
		customLinearLayoutView.setBackgroundDrawable(createGradient(colorsInfo));
		customLinearLayoutView.setGravity(Gravity.CENTER_VERTICAL|Gravity.LEFT);
		
		customLinearLayoutView.addView(thumbnailImage);
		customLinearLayoutView.addView(webLinearLayout);
		
		//----------------------------------------
		// WHAT NOW TEXT
		//----------------------------------------
		whatNowText = new TextView(this);
		whatNowText.setText("What now?");
		whatNowText.setTextColor(0xFFFFFFFF);
		whatNowText.setTextSize(textSize);
		whatNowText.setTypeface(Typeface.create("default", Typeface.BOLD), Typeface.BOLD);
		whatNowText.setShadowLayer(1.0f, 1.0f, 1.0f, 0xFF000000);
		LinearLayout.LayoutParams whatNowParams = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		whatNowParams.setMargins(padding, padding, padding, 0);
		whatNowText.setLayoutParams(whatNowParams);
		
		whatNowText.setId(3112);
		
		//----------------------------------------
		// REPLAY BUTTON
		//----------------------------------------
		replayButton = createButton("Replay this video");

		LinearLayout.LayoutParams replayParams = new LinearLayout.LayoutParams(buttonWidth, LayoutParams.WRAP_CONTENT);
		replayParams.setMargins(padding, padding, padding, padding);
		replayButton.setLayoutParams(replayParams);
		replayButton.setId(1113);
		replayButton.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				initVideoView();
			}
		});
		
		TapjoyLog.i(TAPJOY_VIDEO, "button dimensions: " + replayButton.getMeasuredWidth() + "x" + replayButton.getMeasuredHeight());
		
		//----------------------------------------
		// CLOSE BUTTON
		//----------------------------------------
		closeButton = createButton("View More Offers");
		
		LinearLayout.LayoutParams closeParams = new LinearLayout.LayoutParams(buttonWidth, LayoutParams.WRAP_CONTENT);
		closeParams.setMargins(padding, padding, padding, padding);
		closeButton.setLayoutParams(closeParams);
		closeButton.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				finish();
			}
		});

		LinearLayout linearLayoutRow = new LinearLayout(this);
		LinearLayout.LayoutParams linearLayoutRowParams = new LinearLayout.LayoutParams(width, LayoutParams.WRAP_CONTENT);
		linearLayoutRow.setLayoutParams(linearLayoutRowParams);
		linearLayoutRow.setOrientation(LinearLayout.HORIZONTAL);
		linearLayoutRow.addView(replayButton);
		linearLayoutRow.addView(closeButton);
		
		TapjoyLog.i(TAPJOY_VIDEO, "linear width: " + linearLayoutRow.getMeasuredWidth());
		
		linearLayout.addView(detailsText);
		linearLayout.addView(customLinearLayoutView);
		linearLayout.addView(whatNowText);
		linearLayout.addView(linearLayoutRow);
		
		//----------------------------------------
		// LINK BUTTON
		//----------------------------------------
		if (videoData.buttonCount >= 1)
		{
			linkButton = createButton(videoData.buttonData[0][0]);
			
			LinearLayout.LayoutParams linkParams = new LinearLayout.LayoutParams(buttonWidth, LayoutParams.WRAP_CONTENT);
			linkParams.setMargins(padding, padding, padding, padding);
			linkButton.setLayoutParams(linkParams);
			linkButton.setOnClickListener(new OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					// OPEN URL.
					Intent intent;
					Uri uri = Uri.parse(videoData.buttonData[0][1]);
					
					TapjoyLog.i(TAPJOY_VIDEO, "buton url: " + uri.toString());
					
					try
					{
						intent = new Intent(Intent.ACTION_VIEW, uri);
						startActivity(intent);
					}
					catch (Exception e)
					{
						android.util.Log.e("VIDEO BUTTON", "Bad URI " + uri + ": " + e.toString());
						return;
					}
				}
			});
			
			// 2 BUTTONS
			if (videoData.buttonCount >= 2)
			{
				linkAltButton = createButton(videoData.buttonData[1][0]);
				
				LinearLayout.LayoutParams linkAltParams = new LinearLayout.LayoutParams(buttonWidth, LayoutParams.WRAP_CONTENT);
				linkAltParams.setMargins(padding, padding, padding, padding);
				linkAltButton.setLayoutParams(linkParams);
				linkAltButton.setOnClickListener(new OnClickListener()
				{
					@Override
					public void onClick(View v)
					{
						// OPEN URL.
						Intent intent;
						Uri uri = Uri.parse(videoData.buttonData[1][1]);
						
						TapjoyLog.i(TAPJOY_VIDEO, "buton url: " + uri.toString());
						
						try
						{
							intent = new Intent(Intent.ACTION_VIEW, uri);
							startActivity(intent);
						}
						catch (Exception e)
						{
							android.util.Log.e("VIDEO BUTTON", "Bad URI " + uri + ": " + e.toString());
							return;
						}
					}
				});
				
				LinearLayout secondLayoutRow = new LinearLayout(this);
				LinearLayout.LayoutParams secondLayoutRowParams = new LinearLayout.LayoutParams(width, LayoutParams.WRAP_CONTENT);
				secondLayoutRow.setLayoutParams(secondLayoutRowParams);
				secondLayoutRow.setOrientation(LinearLayout.HORIZONTAL);
				secondLayoutRow.addView(linkButton);
				secondLayoutRow.addView(linkAltButton);
				
				linearLayout.addView(secondLayoutRow);
			}
			// 1 BUTTON
			else
			{
				linearLayout.addView(linkButton);
			}
		}
	}
	
	
	private void showVideoUI()
	{
		relativeLayout.removeAllViews();
		relativeLayout.setBackgroundDrawable(new GradientDrawable(Orientation.TOP_BOTTOM, colorsBackground));
		relativeLayout.addView(linearLayout);
	}
	
	
	private Button createButton(String text)
	{		
		// Create drawables for each state: focused, normal and selected.
		Drawable s1 = createGradient(colorsButtons);
		Drawable n1 = createGradient(colorsButtons);
		Drawable f1 = createGradient(colorsButtons);
		
		// Combine the drawables of various states into 1 drawable.
		StateListDrawable drawable = new StateListDrawable();
		
		drawable.addState(new int[]{ statePressed}, s1);
		drawable.addState(new int[]{ -stateFocused, -stateSelected}, n1);
		drawable.addState(new int[]{ -stateFocused, stateSelected}, f1);
		
		Button button = new Button(this);
		button.setBackgroundDrawable(drawable);
		
		button.setText(text);
		button.setTextColor(0xFF033c92);
		button.setTextSize(textSize);
		button.setTypeface(Typeface.create("default", Typeface.BOLD), Typeface.BOLD);
		
		if (buttonHeight > 0)
			button.setHeight(buttonHeight);
		
		TapjoyLog.i(TAPJOY_VIDEO, "padding bottom: " + button.getPaddingBottom());
		TapjoyLog.i(TAPJOY_VIDEO, "padding top: " + button.getPaddingTop());
		TapjoyLog.i(TAPJOY_VIDEO, "padding left: " + button.getPaddingLeft());
		TapjoyLog.i(TAPJOY_VIDEO, "padding right: " + button.getPaddingRight());
		
		button.setPadding(button.getPaddingLeft(), button.getPaddingBottom(), button.getPaddingRight(), button.getPaddingBottom());
		
		return button;
	}
    
	
    /**
     * Creates a Drawable that has a gradient background.
     * @param colors						An array of colors to compose the gradient with.
     * @return								The Drawable gradient shape.
     */
    private static GradientDrawable createGradient(int [] colors)
    {
		GradientDrawable gradientDrawable = null;
		Orientation orientation = Orientation.TOP_BOTTOM;
		
		// Set the orientation to draw the gradient in.
		gradientDrawable = new GradientDrawable(orientation, colors);
		
		// Set the corner shape.
		gradientDrawable.setCornerRadii(cornerAll);
		gradientDrawable.setGradientRadius(50);
		
		return gradientDrawable;
    }
    
    
	/**
	 * Plays the video.
	 */
	private void startVideo()
	{
		videoView.requestFocus();
		videoView.seekTo(0);
		
		TapjoyLog.i(TAPJOY_VIDEO, "start");
		videoView.start();
		
		// Cancel the timer if it's active for some reason.
		if (timer != null)
		{
			timer.cancel();
		}
		
		// This timer is used to update the remaining time text.
		timer = new Timer();
		timer.schedule(new RemainingTime(), 500, 100);
		
		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				String response = new TapjoyURLConnection().connectToURL(videoData.clickURL);
				
				if (response != null && response.contains("OK"))
				{
					TapjoyLog.i(TAPJOY_VIDEO, "CLICK REQUEST SUCCESS!");
					clickRequestSuccess = true;
				}
			}
		}).start();
	}


	/**
	 * Returns remaining time of video.
	 * @return								Video time remaining, in seconds.
	 */
	private int getRemainingVideoTime()
	{
		int timeRemaining = (videoView.getDuration() - videoView.getCurrentPosition())/1000;

		if (timeRemaining < 0)
			timeRemaining = 0;

		return timeRemaining;
	}


	/**
	 * TimerTask to update the remaining time text overlay.
	 */
	private class RemainingTime extends TimerTask
	{
		public void run()
		{
			// We must use a handler since we cannot update UI elements from a different thread.
			mHandler.post(mUpdateResults);
		}
	}


	// Create runnable for posting
	final Runnable mUpdateResults = new Runnable() 
	{
		public void run() 
		{
			// Update text onscreen.
			overlayText.setText(videoWillResumeText + getRemainingVideoTime() + videoSecondsText);
		}
	};


	//--------------------------------------------------------------------------------
	// VIDEO LISTENERS
	//--------------------------------------------------------------------------------
	@Override
	public void onPrepared(MediaPlayer mp)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onPrepared");
	}


	@Override
	public boolean onError(MediaPlayer mp, int what, int extra)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onError");
		
		TapjoyVideo.getVideoNotifier().videoError(TapjoyVideoStatus.STATUS_UNABLE_TO_PLAY_VIDEO);
		allowBackKey = true;
		
		if (timer != null)
			timer.cancel();
		
		return false;
	}


	@Override
	public void onCompletion(MediaPlayer mp)
	{
		TapjoyLog.i(TAPJOY_VIDEO, "onCompletion");
		
		if (timer != null)
			timer.cancel();
		
		initVideoUI();
		showVideoUI();
		
		//TapjoyVideo.getInstance().videoCompleted(videoPath);
		TapjoyVideo.getVideoNotifier().videoComplete();
		
		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				if (clickRequestSuccess)
					TapjoyConnectCore.getInstance().actionComplete(videoData.offerID);
			}
		}).start();
		
		allowBackKey = true;
	}


	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if ((keyCode == KeyEvent.KEYCODE_BACK))
		{
			// If the video is playing for the first time, don't allow interruption.
			if (allowBackKey == false)
			{
				// Prevent activity termination.
				// Show prompt whether to end video or not.
				seekTime = videoView.getCurrentPosition();
				videoView.pause();
				showDialog(DIALOG_WARNING_ID);
				
				TapjoyLog.i(TAPJOY_VIDEO, "PAUSE VIDEO time: " + seekTime);
				TapjoyLog.i(TAPJOY_VIDEO, "currentPosition: " + videoView.getCurrentPosition());
				TapjoyLog.i(TAPJOY_VIDEO, "duration: " + videoView.getDuration() + ", elapsed: " + (videoView.getDuration() - videoView.getCurrentPosition()));
				
				return true;
			}
			// Video has already played once.
			else
			{
				// If the video is RE-playing, stop it and show the UI.
				if (videoView.isPlaying())
				{
					videoView.stopPlayback();
					showVideoUI();
					
					if (timer != null)
						timer.cancel();
					
					return true;
				}
			}
		}

		return super.onKeyDown(keyCode, event);
	}
	
	
	protected Dialog onCreateDialog(int id)
	{
		Dialog dialog;
		switch (id)
		{
			case DIALOG_WARNING_ID:
				dialog = new AlertDialog.Builder(this).setTitle("Cancel Video?").setMessage("Currency will not be awarded, are you sure you want to cancel the video?").
						setNegativeButton("End", new DialogInterface.OnClickListener()
						{
							// Terminate this activity.
							public void onClick(DialogInterface dialog, int whichButton)
							{
								finish();
							}
						}).setPositiveButton("Resume", new DialogInterface.OnClickListener()
						{
							// Resume the video.
							public void onClick(DialogInterface dialog, int whichButton)
							{
								dialog.dismiss();
								
								videoView.seekTo(seekTime);
								videoView.start();
								
								TapjoyLog.i(TAPJOY_VIDEO, "RESUME VIDEO time: " + seekTime);
								TapjoyLog.i(TAPJOY_VIDEO, "currentPosition: " + videoView.getCurrentPosition());
								TapjoyLog.i(TAPJOY_VIDEO, "duration: " + videoView.getDuration() + ", elapsed: " + (videoView.getDuration() - videoView.getCurrentPosition()));
							}
						}).create();

				dialog.show();
				break;
			default:
				dialog = null;
		}
		return dialog;
	}
}