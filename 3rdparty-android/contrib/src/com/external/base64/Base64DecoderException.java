// This code was converted from code at http://iharder.sourceforge.net/base64/
// Lots of extraneous features were removed.
// The original code said:
/* <p>
 * I am placing this code in the Public Domain. Do with it as you will.
 * This software comes with no guarantees or warranties but with
 * plenty of well-wishing instead!
 * Please visit
 * <a href="http://iharder.net/xmlizable">http://iharder.net/xmlizable</a>
 * periodically to check for updates or to contribute improvements.
 * </p>
 *
 * @author Robert Harder
 * @author rharder@usa.net
 * @version 1.3
 */

package com.external.base64;

public class Base64DecoderException extends Exception {
	
	public Base64DecoderException () {
		super();
	}

	public Base64DecoderException (String s) {
		super(s);
	}

	private static final long serialVersionUID = 1L;
}
