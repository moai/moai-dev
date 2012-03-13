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
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class TapjoyUtil
{
	private static final String	TAPJOY_UTIL	= "TapjoyUtil";

	/**
	 * Converts a String into a SHA-256 hash.
	 * 
	 * @param text
	 *            Text to convert.
	 * @return SHA1 hash.
	 * @throws NoSuchAlgorithmException
	 * @throws UnsupportedEncodingException
	 */
	public static String SHA256(String text) throws NoSuchAlgorithmException, UnsupportedEncodingException
	{
		TapjoyLog.i(TAPJOY_UTIL, "SHA256: " + text);

		MessageDigest md;
		byte[] sha1hash = new byte[40];

		// MD5, SHA-1, etc
		md = MessageDigest.getInstance("SHA-256");
		md.update(text.getBytes("iso-8859-1"), 0, text.length());
		sha1hash = md.digest();

		return convertToHex(sha1hash);
	}

	/**
	 * Converts a byte array into a hex string.
	 * 
	 * @param data
	 *            Data to convert.
	 * @return Data in hex as a string.
	 */
	private static String convertToHex(byte[] data)
	{
		StringBuffer buf = new StringBuffer();

		for (int i = 0; i < data.length; i++)
		{
			int halfbyte = (data[i] >>> 4) & 0x0F;
			int two_halfs = 0;

			do
			{
				if ((0 <= halfbyte) && (halfbyte <= 9))
					buf.append((char) ('0' + halfbyte));
				else
					buf.append((char) ('a' + (halfbyte - 10)));
				halfbyte = data[i] & 0x0F;
			}

			while (two_halfs++ < 1);
		}

		return buf.toString();
	}

	/**
	 * Generates a document from an XML formatted String.
	 * 
	 * @param xml
	 *            String in XML format.
	 * @return Document created from the XML data.
	 */
	public static Document buildDocument(String xml)
	{
		Document document = null;

		try
		{
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder documentBuilder;

			// Convert the string to an input stream.
			InputStream is = new ByteArrayInputStream(xml.getBytes("UTF-8"));

			documentBuilder = factory.newDocumentBuilder();
			document = documentBuilder.parse(is);
		}
		catch (Exception e)
		{
			TapjoyLog.e(TAPJOY_UTIL, "buildDocument exception: " + e.toString());
		}

		return document;
	}

	/**
	 * Helper class to help parse the server response.
	 * 
	 * @param nodeList
	 * @return
	 */
	public static String getNodeTrimValue(NodeList nodeList)
	{
		Element element = (Element) nodeList.item(0);
		String nodeValue = "";

		if (element != null)
		{
			NodeList itemNodeList = element.getChildNodes();

			int length = itemNodeList.getLength();

			for (int i = 0; i < length; i++)
			{
				Node node = ((Node) itemNodeList.item(i));
				if (node != null)
					nodeValue += node.getNodeValue();
			}

			if (nodeValue != null && !nodeValue.equals(""))
			{
				return nodeValue.trim();
			}
			else
			{
				return null;
			}
		}
		return null;
	}

	/**
	 * Recursively delete all files in a directory or a file.
	 * 
	 * @param fileOrDirectory
	 *            File or directory to delete
	 */
	public static void deleteFileOrDirectory(File fileOrDirectory)
	{
		if (fileOrDirectory.isDirectory())
		{
			for (File child : fileOrDirectory.listFiles())
				deleteFileOrDirectory(child);
		}

		TapjoyLog.i(TAPJOY_UTIL, "****************************************");
		TapjoyLog.i(TAPJOY_UTIL, "deleteFileOrDirectory: " + fileOrDirectory.getAbsolutePath());
		TapjoyLog.i(TAPJOY_UTIL, "****************************************");
		fileOrDirectory.delete();
	}
}
