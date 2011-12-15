// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.Map.Entry;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.util.EntityUtils;

import android.net.Uri;

public class TapjoyURLConnection
{
	private static final String	TAPJOY_URL_CONNECTION	= "TapjoyURLConnection";

	public TapjoyURLConnection()
	{

	}

	
	public String connectToURL(String url)
	{
		return connectToURL(url, "");
	}
	
	
	/**
	 * Performs a network request call to the specified URL and parameters.
	 * 
	 * @param url							The base URL.
	 * @param params						The URL parameters.
	 * @return 								Response from the server.
	 */
	public String connectToURL(String url, String params)
	{
		String httpResponse = null;

		BufferedReader rd = null;
		StringBuilder sb = null;
		String line = null;

		try
		{
			String requestURL = url + params;

			// Replaces all spaces.
			requestURL = requestURL.replaceAll(" ", "%20");

			TapjoyLog.i(TAPJOY_URL_CONNECTION, "baseURL: " + url);
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "requestURL: " + requestURL);
			
			// USE java.net HTTP connection instead.
			
			URL httpURL = new URL(requestURL);
			HttpURLConnection connection = (HttpURLConnection) httpURL.openConnection();
			connection.setConnectTimeout(15000);
			connection.setReadTimeout(30000);
			httpResponse = connection.getResponseMessage();

			connection.connect();

			// Read the result from the server.
			rd = new BufferedReader(new InputStreamReader(connection.getInputStream()));
			sb = new StringBuilder();
			
			while ((line = rd.readLine()) != null)
			{
				sb.append(line + '\n');
			}
			
			httpResponse = sb.toString();
			
			// OLD CODE USING APACHE API
			
			// HttpGet http = new HttpGet(requestURL);
			//
			// // Create a HttpParams object so we can set our timeout times.
			// HttpParams httpParameters = new BasicHttpParams();
			//
			// // Time to wait to establish initial connection.
			// HttpConnectionParams.setConnectionTimeout(httpParameters, 15000);
			//
			// // Time to wait for incoming data.
			// HttpConnectionParams.setSoTimeout(httpParameters, 30000);
			//
			// // Create a http client with out timeout settings.
			// HttpClient client = new DefaultHttpClient(httpParameters);
			//
			// HttpResponse response = client.execute(http);
			// HttpEntity entity = response.getEntity();
			//
			// httpResponse = EntityUtils.toString(entity);
			
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "--------------------");
			// TapjoyLog.i(TAPJOY_URL_CONNECTION, "response status: " + response.getStatusLine().getStatusCode());
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "response size: " + httpResponse.length());
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "response: ");
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "" + httpResponse);
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "--------------------");
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_URL_CONNECTION, "Exception: " + e.toString());
		}

		return httpResponse;
	}
	
	
	/**
	 * Gets the file size of the specified resource from the HTTP header field "content-length"
	 * @param url							URL of the specified resource.
	 * @return								content-length, null otherwise.
	 */
	public String getContentLength(String url)
	{
		String contentLength = null;
		try
		{
			String requestURL = url;

			// Replaces all spaces.
			requestURL = requestURL.replaceAll(" ", "%20");

			TapjoyLog.i(TAPJOY_URL_CONNECTION, "requestURL: " + requestURL);
			
			URL httpURL = new URL(requestURL);
			HttpURLConnection connection = (HttpURLConnection) httpURL.openConnection();
			connection.setConnectTimeout(15000);
			connection.setReadTimeout(30000);
			contentLength = connection.getHeaderField("content-length");
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_URL_CONNECTION, "Exception: " + e.toString());
		}
		
		TapjoyLog.i(TAPJOY_URL_CONNECTION, "content-length: " + contentLength);
		
		return contentLength;
	}

	
	/**
	 * Performs a network request call using HTTP POST to the specified URL and parameters.
	 * 
	 * @param url							The URL to connect to.
	 * @param params						POST parameters in key/value format.
	 * @param paramsData					Any additional POST parameters in key/value format.
	 * @return							 	Response from the server.
	 */
	public String connectToURLwithPOST(String url, Hashtable<String, String> params, Hashtable<String, String> paramsData)
	{
		String httpResponse = null;

		try
		{
			String requestURL = url;

			// Replaces all spaces.
			requestURL = requestURL.replaceAll(" ", "%20");

			TapjoyLog.i(TAPJOY_URL_CONNECTION, "baseURL: " + url);
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "requestURL: " + requestURL);

			HttpPost httpPost = new HttpPost(requestURL);

			List<NameValuePair> pairs = new ArrayList<NameValuePair>();

			Set<Entry<String, String>> entries = params.entrySet();
			Iterator<Entry<String, String>> iterator = entries.iterator();

			while (iterator.hasNext())
			{
				Entry<String, String> item = iterator.next();
				pairs.add(new BasicNameValuePair(item.getKey(), item.getValue()));

				TapjoyLog.i(TAPJOY_URL_CONNECTION, "key: " + item.getKey() + ", value: " + Uri.encode(item.getValue()));
			}

			if (paramsData != null && paramsData.size() > 0)
			{
				entries = paramsData.entrySet();
				iterator = entries.iterator();

				while (iterator.hasNext())
				{
					Entry<String, String> item = iterator.next();
					pairs.add(new BasicNameValuePair("data[" + item.getKey() + "]", item.getValue()));

					TapjoyLog.i(TAPJOY_URL_CONNECTION, "key: " + item.getKey() + ", value: " + Uri.encode(item.getValue()));
				}
			}

			httpPost.setEntity(new UrlEncodedFormEntity(pairs));

			TapjoyLog.i(TAPJOY_URL_CONNECTION, "HTTP POST: " + httpPost.toString());

			// Create a HttpParams object so we can set our timeout times.
			HttpParams httpParameters = new BasicHttpParams();

			// Time to wait to establish initial connection.
			HttpConnectionParams.setConnectionTimeout(httpParameters, 15000);

			// Time to wait for incoming data.
			HttpConnectionParams.setSoTimeout(httpParameters, 30000);

			// Create a http client with out timeout settings.
			HttpClient client = new DefaultHttpClient(httpParameters);

			HttpResponse response = client.execute(httpPost);
			HttpEntity entity = response.getEntity();

			httpResponse = EntityUtils.toString(entity);

			TapjoyLog.i(TAPJOY_URL_CONNECTION, "--------------------");
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "response status: " + response.getStatusLine().getStatusCode());
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "response size: " + httpResponse.length());
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "response: ");
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "" + httpResponse);
			TapjoyLog.i(TAPJOY_URL_CONNECTION, "--------------------");
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_URL_CONNECTION, "Exception: " + e.toString());
		}

		return httpResponse;
	}
}
