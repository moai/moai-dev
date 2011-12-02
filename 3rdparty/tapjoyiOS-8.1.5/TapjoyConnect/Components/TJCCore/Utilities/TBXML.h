// ================================================================================================
//  TBXML.h
//  Fast processing of XML files
//
// ================================================================================================
//  Created by Tom Bradley on 21/10/2009.
//  Version 1.4
//  
//  Copyright (c) 2009 Tom Bradley
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
// ================================================================================================

#import <Foundation/Foundation.h>

// ================================================================================================
//  Defines
// ================================================================================================
#define MAX_ELEMENTS 100
#define MAX_ATTRIBUTES 100

#define TBXML_ATTRIBUTE_NAME_START 0
#define TBXML_ATTRIBUTE_NAME_END 1
#define TBXML_ATTRIBUTE_VALUE_START 2
#define TBXML_ATTRIBUTE_VALUE_END 3
#define TBXML_ATTRIBUTE_CDATA_END 4

// ================================================================================================
//  Structures
// ================================================================================================
/*!	\struct TBXMLAttribute
 *	\brief The TBXML attribute list node.
 *
 */
typedef struct TBXMLAttribute 
{
	char * name;							/*!< The name of the XML attribute. */
	char * value;							/*!< The value of the XML attribute. */
	struct TBXMLAttribute * next;			/*!< Pointer to the next #TBXMLAttribute. */
} TBXMLAttribute;

/*!	\struct TBXMLElement
 *	\brief The TBXML element list node.
 *
 */
typedef struct TBXMLElement
{
	char * name;							/*!< The name of the XML Element. */
	char * text;							/*!< The text associated with the XML Element. */
	TBXMLAttribute * firstAttribute;		/*!< Pointer to the first #TBXMLAttribute node in this element. */
	struct TBXMLElement * parentElement;	/*!< Pointer to the parent #TBXMLElement of this element. */
	struct TBXMLElement * firstChild;		/*!< Pointer to the first child #TBXMLElement of this element. */
	struct TBXMLElement * currentChild;		/*!< Pointer to the current chile #TBXMLElement of this element. */
	struct TBXMLElement * nextSibling;		/*!< Pointer to the next sibling #TBXMLElement of this element. */
	struct TBXMLElement * previousSibling;	/*!< Pointer to the previous sibling #TBXMLElement of this element. */
} TBXMLElement;

/*!	\struct TBXMLElementBuffer
 *	\brief The TBXML element buffer.
 *
 */
typedef struct TBXMLElementBuffer
{
	TBXMLElement * elements;				/*!< The current TBXMLElement. */
	struct TBXMLElementBuffer * next;		/*!< Pointer to the next TBXMLElementBuffer. */
	struct TBXMLElementBuffer * previous;	/*!< Pointer to the previous TBXMLElementBuffer. */
} TBXMLElementBuffer;

/*!	\struct TBXMLAttributeBuffer
 *	\brief The TBXML attribute buffer.
 *
 */
typedef struct TBXMLAttributeBuffer 
{
	TBXMLAttribute * attributes;			/*!< The current TBXMLAttribute. */
	struct TBXMLAttributeBuffer * next;		/*!< Pointer to the next TBXMLAttributeBuffer. */
	struct TBXMLAttributeBuffer * previous;	/*!< Pointer to the previous TBXMLAttributeBuffer. */
} TBXMLAttributeBuffer;

/*!	\interface TBXML
 *	\brief The Tapjoy Connect TBXML public interface.
 *
 */
@interface TBXML : NSObject 
{
	
@private
	TBXMLElement * rootXMLElement;					/*!< The TBXML XML root element that is set when one of the initialize functions are invoked. */
	TBXMLElementBuffer * currentElementBuffer;		/*!< Points to a #TBXMLElementBuffer in the #TBXMLElement linked list. */
	TBXMLAttributeBuffer * currentAttributeBuffer;	/*!< Points to a #TBXMLAttributeBuffer in the #TBXMLElement linked list. */
	long currentElement;							/*!< The index into the #TBXMLElement linked list of #currentElementBuffer. */
	long currentAttribute;							/*!< The index into the #TBXMLElement linked list of #currentAttributeBuffer. */
	char * bytes;									/*!< Holds encoded XML data. */
	long bytesLength;								/*!< Size of the encoded XML data in bytes. */
}

@property (nonatomic, readonly) TBXMLElement * rootXMLElement;


/*!	\fn tbxmlWithURL:(NSURL*)aURL
 *	\brief Returns a #TBXML object with the given NSURL.
 *  
 * This method allocates a TBXML object that is return after the XML data is decoded.
 *	\param aURL The NSURL to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
+ (id)tbxmlWithURL:(NSURL*)aURL;

/*!	\fn tbxmlWithXMLString:(NSString*)aXMLString
 *	\brief Returns a #TBXML object with the given NSString.
 *  
 * This method allocates a TBXML object that is return after the XML data is decoded.
 *	\param aXMLString The NSString that refers to an NSURL that is used to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
+ (id)tbxmlWithXMLString:(NSString*)aXMLString;

/*!	\fn tbxmlWithXMLData:(NSData*)aData
 *	\brief Returns a #TBXML object with the given NSData.
 *  
 * The NSData contains the XML data that is then decoded and stored within the #TBXML object.
 * This method allocates a TBXML object that is return after the XML data is decoded.
 *	\param aData The NSData that contains XML data that is used to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
+ (id)tbxmlWithXMLData:(NSData*)aData;

/*!	\fn initWithURL:(NSURL*)aURL
 *	\brief Returns a #TBXML object with the given NSURL.
 *  
 * The NSURL points to an XML file that is then decoded and stored within the #TBXML object.
 *	\param aURL The NSURL to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
- (id)initWithURL:(NSURL*)aURL;

/*!	\fn initWithXMLString:(NSString*)aXMLString
 *	\brief Returns a #TBXML object with the given NSString.
 *  
 * The NSString refers to an NSURL which points to an XML file that is then decoded and stored within the #TBXML object.
 *	\param aXMLString The NSString that refers to an NSURL that is used to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
- (id)initWithXMLString:(NSString*)aXMLString;

/*!	\fn initWithXMLData:(NSData*)aData
 *	\brief Returns a #TBXML object with the given NSData.
 *  
 * The NSData contains the XML data that is then decoded and stored within the #TBXML object.
 *	\param aData The NSData that contains XML data that is used to initialize the #TBXML object with.
 *  \return A #TBXML object.
 */
- (id)initWithXMLData:(NSData*)aData;

@end

// ================================================================================================
//  TBXML Static Functions Interface
// ================================================================================================

/*!	\category TBXML(StaticFunctions)
 *	\brief The Tapjoy Connect TBXML static functions category.
 *
 */
@interface TBXML(StaticFunctions)

/*!	\fn elementName:(TBXMLElement*)aXMLElement
 *	\brief Returns the element name of the given #TBXMLElement.
 *  
 *	\param aXMLElement The #TBXMLElement from which to retrieve the name from.
 *  \return An NSString containing the name of the given #TBXMLElement.
 */
+ (NSString*) elementName:(TBXMLElement*)aXMLElement;

/*!	\fn textForElement:(TBXMLElement*)aXMLElement
 *	\brief Returns the element text of the given #TBXMLElement.
 *  
 *	\param aXMLElement The #TBXMLElement from which to retrieve the text from.
 *  \return An NSString containing the text of the given #TBXMLElement.
 */
+ (NSString*) textForElement:(TBXMLElement*)aXMLElement;

/*!	\fn numberForElement:(TBXMLElement*)aXMLElement
 *	\brief Returns the element integer value of the given #TBXMLElement.
 *  
 *	\param aXMLElement The #TBXMLElement from which to retrieve the integer value from.
 *  \return An integer value of the given #TBXMLElement. Returns 0 if no integer value exists.
 */
+ (int)	numberForElement:(TBXMLElement*)aXMLElement;

/*!	\fn boolForElement:(TBXMLElement*)aXMLElement
 *	\brief Returns the element boolean value of the given #TBXMLElement.
 *  
 *	\param aXMLElement The #TBXMLElement from which to retrieve the boolean value from.
 *  \return An boolean value of the given #TBXMLElement.
 */
+ (BOOL) boolForElement:(TBXMLElement*)aXMLElement;

/*!	\fn negativeNumberForUnknownElement:(TBXMLElement*)aXMLElement
 *	\brief Returns the element integer value of the given #TBXMLElement.
 *  
 * This is similar to #numberForElement except -1 is returned instead of 0 when no integer value exists.
 *	\param aXMLElement The #TBXMLElement from which to retrieve the integer value from.
 *  \return An integer value of the given #TBXMLElement. Returns -1 if no integer value exists.
 */
+ (int) negativeNumberForUnknownElement:(TBXMLElement*)aXMLElement;

/*!	\fn valueOfAttributeNamed:forElement:(NSString* aName, TBXMLElement* aXMLElement)
 *	\brief Returns the value of the attribute that matches the given #TBXMLElement's attribute name.
 *  
 * The first matching attribute with the same given name in the given #TBXMLElement attribute list is returned.
 *	\param aName The name to search for in the given #TBXMLElement's attribute list.
 *	\param aXMLElement The attribute list is contained within this #TBXMLElement.
 *  \return An NSString containing the value of the attribute. If no matching attribute is found, the NSString is nil.
 */
+ (NSString*) valueOfAttributeNamed:(NSString *)aName forElement:(TBXMLElement*)aXMLElement;

/*!	\fn attributeName:(TBXMLAttribute*)aXMLAttribute
 *	\brief Returns the attribute name of the given #TBXMLAttribute.
 *  
 *	\param aXMLAttribute The #TBXMLAttribute from which to retrieve the name from.
 *  \return An NSString containing the name of the given #TBXMLAttribute.
 */
+ (NSString*) attributeName:(TBXMLAttribute*)aXMLAttribute;

/*!	\fn attributeValue:(TBXMLAttribute*)aXMLAttribute
 *	\brief Returns the attribute value of the given #TBXMLAttribute.
 *  
 *	\param aXMLAttribute The #TBXMLAttribute from which to retrieve the value from.
 *  \return An NSString containing the value of the given #TBXMLAttribute.
 */
+ (NSString*) attributeValue:(TBXMLAttribute*)aXMLAttribute;

/*!	\fn nextSiblingNamed:searchFromElement:(NSString* aName, TBXMLElement* aXMLElement)
 *	\brief Returns the next sibling of the given #TBXMLElement that contains a matching name with the given name.
 *  
 * The first matching next sibling of the given #TBXMLElement is returned.
 *	\param aName The name to search for in the given #TBXMLElement's sibling list.
 *	\param aXMLElement The sibling list is contained within this #TBXMLElement.
 *  \return A #TBXMLElement that contains the matching name to the given name. If no matching #TBXMLElement is found, nil is returned.
 */
+ (TBXMLElement*) nextSiblingNamed:(NSString*)aName searchFromElement:(TBXMLElement*)aXMLElement;

/*!	\fn childElementNamed:parentElement:(NSString* aName, TBXMLElement* aParentXMLElement)
 *	\brief Returns the child of the given #TBXMLElement that contains a matching name with the given name.
 *  
 * The first matching child of the given #TBXMLElement is returned.
 *	\param aName The name to search for in the given #TBXMLElement's child list.
 *	\param aParentXMLElement The child list is contained within this #TBXMLElement.
 *  \return A #TBXMLElement that contains the matching name to the given name. If no matching #TBXMLElement is found, nil is returned.
 */
+ (TBXMLElement*) childElementNamed:(NSString*)aName parentElement:(TBXMLElement*)aParentXMLElement;

@end
