// ================================================================================================
//  TBXML.m
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
#import "TBXML.h"

// ================================================================================================
// Private methods
// ================================================================================================
@interface TBXML (Private)
- (void) decodeData:(NSData*)data;
- (void) decodeBytes;
- (TBXMLElement*) nextAvailableElement;
- (TBXMLAttribute*) nextAvailableAttribute;
@end



// ================================================================================================
// Public Implementation
// ================================================================================================
@implementation TBXML

@synthesize rootXMLElement;

+ (id)tbxmlWithURL:(NSURL*)aURL 
{
	return [[[TBXML alloc] initWithURL:aURL] autorelease];
}

+ (id)tbxmlWithXMLString:(NSString*)aXMLString 
{
	return [[[TBXML alloc] initWithXMLString:aXMLString] autorelease];
}

+ (id)tbxmlWithXMLData:(NSData*)aData
{
	return [[[TBXML alloc] initWithXMLData:aData] autorelease];
}

- (id)init 
{
	self = [super init];
	if (self != nil) {
		rootXMLElement = nil;
		
		currentElementBuffer = 0;
		currentAttributeBuffer = 0;
		
		currentElement = 0;
		currentAttribute = 0;		
		
		bytes = 0;
		bytesLength = 0;
	}
	return self;
}

- (id)initWithURL:(NSURL*)aURL 
{
	self = [self initWithXMLString:[NSString stringWithContentsOfURL:aURL encoding:NSUTF8StringEncoding error:nil]];
	if (self != nil) {
	}
	return self;
}

- (id)initWithXMLString:(NSString*)aXMLString 
{
	self = [self init];
	if (self != nil) 
	{
		// copy string to byte array
		bytesLength = [aXMLString lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
		bytes = malloc(bytesLength+1);
		[aXMLString getBytes:bytes maxLength:bytesLength usedLength:0 encoding:NSUTF8StringEncoding options:NSStringEncodingConversionAllowLossy range:NSMakeRange(0, bytesLength) remainingRange:nil];
		
		// set null terminator at end of byte array
		bytes[bytesLength] = 0;
		
		// decode xml data
		[self decodeBytes];
	}
	return self;
}

- (id)initWithXMLData:(NSData*)aData 
{
    self = [self init];
    if (self != nil) 
	{
		// decode aData
		[self decodeData:aData];
    }
    
    return self;
}

@end


// ================================================================================================
// Static Functions Implementation
// ================================================================================================

#pragma mark -
#pragma mark Static Functions implementation

@implementation TBXML (StaticFunctions)

+ (NSString*) elementName:(TBXMLElement*)aXMLElement 
{
	if (nil == aXMLElement) return @"";
	if (nil == aXMLElement->name) return @"";
	return [NSString stringWithCString:&aXMLElement->name[0] encoding:NSUTF8StringEncoding];
}

+ (NSString*) attributeName:(TBXMLAttribute*)aXMLAttribute 
{
	if (nil == aXMLAttribute) return @"";
	if (nil == aXMLAttribute->name) return @"";
	return [NSString stringWithCString:&aXMLAttribute->name[0] encoding:NSUTF8StringEncoding];
}

+ (NSString*) attributeValue:(TBXMLAttribute*)aXMLAttribute 
{
	if (nil == aXMLAttribute) return @"";
	if (nil == aXMLAttribute->value) return @"";
	return [NSString stringWithCString:&aXMLAttribute->value[0] encoding:NSUTF8StringEncoding];
}

+ (NSString*) textForElement:(TBXMLElement*)aXMLElement 
{
	if (nil == aXMLElement) return @"";
	if (nil == aXMLElement->text) return @"";
	return [NSString stringWithCString:&aXMLElement->text[0] encoding:NSUTF8StringEncoding];
}

+ (int) numberForElement:(TBXMLElement*)aXMLElement 
{
	if (nil == aXMLElement) return 0;
	if (nil == aXMLElement->text) return 0;
	NSString *val = [NSString stringWithCString:&aXMLElement->text[0] encoding:NSUTF8StringEncoding];
	@try 
	{
		return [val intValue];
	}
	@catch (NSException * e) 
	{
		return 0;
	}
	return 0;
}

+ (BOOL) boolForElement:(TBXMLElement*)aXMLElement 
{
	if (nil == aXMLElement) return false;
	if (nil == aXMLElement->text) return false;
	NSString *val = [NSString stringWithCString:&aXMLElement->text[0] encoding:NSUTF8StringEncoding];
	if ([[val lowercaseString] isEqualToString:@"yes"] || [[val lowercaseString] isEqualToString:@"true"])
	{
		return true;
	}
	return false;
}


+ (int) negativeNumberForUnknownElement:(TBXMLElement*)aXMLElement 
{
	if (nil == aXMLElement) return -1;
	if (nil == aXMLElement->text) return -1;
	NSString *val = [NSString stringWithCString:&aXMLElement->text[0] encoding:NSUTF8StringEncoding];
	@try
	{
		return [val intValue];
	}
	@catch (NSException * e)
	{
		return -1;
	}
	return -1;
}

+ (NSString*) valueOfAttributeNamed:(NSString *)aName forElement:(TBXMLElement*)aXMLElement 
{
	const char * name = [aName cStringUsingEncoding:NSUTF8StringEncoding];
	NSString * value = nil;
	TBXMLAttribute * attribute = aXMLElement->firstAttribute;
	while (attribute) 
	{
		if (strlen(attribute->name) == strlen(name) && memcmp(attribute->name,name,strlen(name)) == 0) 
		{
			value = [NSString stringWithCString:&attribute->value[0] encoding:NSUTF8StringEncoding];
			break;
		}
		attribute = attribute->next;
	}
	return value;
}

+ (TBXMLElement*) childElementNamed:(NSString*)aName parentElement:(TBXMLElement*)aParentXMLElement
{
	if (nil == aParentXMLElement) return nil;
	TBXMLElement * xmlElement = aParentXMLElement->firstChild;
	const char * name = [aName cStringUsingEncoding:NSUTF8StringEncoding];
	while (xmlElement) 
	{
		if (strlen(xmlElement->name) == strlen(name) && memcmp(xmlElement->name,name,strlen(name)) == 0)
		{
			return xmlElement;
		}
		xmlElement = xmlElement->nextSibling;
	}
	return nil;
}

+ (TBXMLElement*) nextSiblingNamed:(NSString*)aName searchFromElement:(TBXMLElement*)aXMLElement
{
	if (nil == aXMLElement) return nil;
	TBXMLElement * xmlElement = aXMLElement->nextSibling;
	const char * name = [aName cStringUsingEncoding:NSUTF8StringEncoding];
	while (xmlElement) 
	{
		if (strlen(xmlElement->name) == strlen(name) && memcmp(xmlElement->name,name,strlen(name)) == 0) 
		{
			return xmlElement;
		}
		xmlElement = xmlElement->nextSibling;
	}
	return nil;
}

@end


// ================================================================================================
// Private Implementation
// ================================================================================================

#pragma mark -
#pragma mark Private implementation

@implementation TBXML (Private)

- (void)decodeData:(NSData*)data
{
	// copy data to byte array
	bytesLength = [data length];
	bytes = malloc(bytesLength+1);
	[data getBytes:bytes length:bytesLength];
	
	// set null terminator at end of byte array
	bytes[bytesLength] = 0;
	
	// decode xml data
	[self decodeBytes];
}

- (void) decodeBytes
{
	
	// -----------------------------------------------------------------------------
	// Process xml
	// -----------------------------------------------------------------------------
	
	// set elementStart pointer to the start of our xml
	char * elementStart=bytes;
	
	// set parent element to nil
	TBXMLElement * parentXMLElement = nil;
	
	// find next element start
	while ((elementStart = strstr(elementStart,"<"))) 
	{
		
		// detect comment section
		if (strncmp(elementStart,"<!--",4) == 0) 
		{
			elementStart = strstr(elementStart,"-->") + 3;
			continue;
		}

		// detect cdata section within element text
		int isCDATA = strncmp(elementStart,"<![CDATA[",9);
		
		// if cdata section found, skip data within cdata section and remove cdata tags
		if (isCDATA==0) 
		{
			
			// find end of cdata section
			char * CDATAEnd = strstr(elementStart,"]]>");
			
			// find start of next element skipping any cdata sections within text
			char * elementEnd = CDATAEnd;
			
			// find next open tag
			elementEnd = strstr(elementEnd,"<");
			// if open tag is a cdata section
			while (strncmp(elementEnd,"<![CDATA[",9) == 0) 
			{
				// find end of cdata section
				elementEnd = strstr(elementEnd,"]]>");
				// find next open tag
				elementEnd = strstr(elementEnd,"<");
			}
			
			// calculate length of cdata content
			long CDATALength = CDATAEnd-elementStart;
			
			// calculate total length of text
			long textLength = elementEnd-elementStart;
			
			// remove begining cdata section tag
			memcpy(elementStart, elementStart+9, CDATAEnd-elementStart-9);

			// remove ending cdata section tag
			memcpy(CDATAEnd-9, CDATAEnd+3, textLength-CDATALength-3);
			
			// blank out end of text
			memset(elementStart+textLength-12,' ',12);
			
			// set new search start position 
			elementStart = CDATAEnd-9;
			continue;
		}
		
		
		// find element end, skipping any cdata sections within attributes
		char * elementEnd = elementStart+1;		
		while ((elementEnd = strpbrk(elementEnd, "<>"))) 
		{
			if (strncmp(elementEnd,"<![CDATA[",9) == 0) 
			{
				elementEnd = strstr(elementEnd,"]]>")+3;
			}
			else 
			{
				break;
			}
		}
		
		
		// null terminate element end
		if (elementEnd) *elementEnd = 0;
		
		// null terminate element start so previous element text doesnt overrun
		*elementStart = 0;
		
		// get element name start
		char * elementNameStart = elementStart+1;
		
		// ignore tags that start with ? or ! unless cdata "<![CDATA"
		if (*elementNameStart == '?' || (*elementNameStart == '!' && isCDATA != 0)) 
		{
			elementStart = elementEnd+1;
			continue;
		}
		
		// ignore attributes/text if this is a closing element
		if (*elementNameStart == '/') 
		{
			elementStart = elementEnd+1;
			if (parentXMLElement) 
			{

				if (parentXMLElement->text) 
				{
					// trim whitespace from start of text
					while (isspace(*parentXMLElement->text)) 
						parentXMLElement->text++;
					
					// trim whitespace from end of text
					char * end = parentXMLElement->text + strlen(parentXMLElement->text)-1;
					while (end > parentXMLElement->text && isspace(*end)) 
						*end--=0;
				}
				
				parentXMLElement = parentXMLElement->parentElement;
				
				// if parent element has children clear text
				if (parentXMLElement && parentXMLElement->firstChild)
					parentXMLElement->text = 0;
				
			}
			continue;
		}
		
		
		// is this element opening and closing
		BOOL selfClosingElement = NO;
		if ((int)(elementEnd-1) == '/') 
		{
			selfClosingElement = YES;
		}
		
		
		// create new xmlElement struct
		TBXMLElement * xmlElement = [self nextAvailableElement];
		
		// set element name
		xmlElement->name = elementNameStart;
		
		// if there is a parent element
		if (parentXMLElement) 
		{
			
			// if this is first child of parent element
			if (parentXMLElement->currentChild)
			{
				// set next child element in list
				parentXMLElement->currentChild->nextSibling = xmlElement;
				xmlElement->previousSibling = parentXMLElement->currentChild;
				
				parentXMLElement->currentChild = xmlElement;
			} 
			else 
			{
				// set first child element
				parentXMLElement->currentChild = xmlElement;
				parentXMLElement->firstChild = xmlElement;
			}
			
			xmlElement->parentElement = parentXMLElement;
		}
		
		
		// in the following xml the ">" is replaced with \0 by elementEnd. 
		// element may contain no atributes and would return nil while looking for element name end
		// <tile> 
		// find end of element name
		char * elementNameEnd = strpbrk(xmlElement->name," /");
		
		
		// if end was found check for attributes
		if (elementNameEnd) 
		{
			
			// null terminate end of elemenet name
			*elementNameEnd = 0;
			
			char * chr = elementNameEnd;
			char * name = nil;
			char * value = nil;
			char * CDATAStart = nil;
			char * CDATAEnd = nil;
			TBXMLAttribute * lastXMLAttribute = nil;
			TBXMLAttribute * xmlAttribute = nil;
			BOOL singleQuote = NO;
			
			int mode = TBXML_ATTRIBUTE_NAME_START;
			
			// loop through all characters within element
			while (chr++ < elementEnd) 
			{
				
				switch (mode)
				{
					// look for start of attribute name
					case TBXML_ATTRIBUTE_NAME_START:
						if (isspace(*chr)) continue;
						name = chr;
						mode = TBXML_ATTRIBUTE_NAME_END;
						break;
					// look for end of attribute name
					case TBXML_ATTRIBUTE_NAME_END:
						if (isspace(*chr) || *chr == '=')
						{
							*chr = 0;
							mode = TBXML_ATTRIBUTE_VALUE_START;
						}
						break;
					// look for start of attribute value
					case TBXML_ATTRIBUTE_VALUE_START:
						if (isspace(*chr)) continue;
						if (*chr == '"' || *chr == '\'')
						{
							value = chr+1;
							mode = TBXML_ATTRIBUTE_VALUE_END;
							if (*chr == '\'') 
								singleQuote = YES;
							else
								singleQuote = NO;
						}
						break;
					// look for end of attribute value
					case TBXML_ATTRIBUTE_VALUE_END:
						if (*chr == '<' && strncmp(chr, "<![CDATA[", 9) == 0)
						{
							mode = TBXML_ATTRIBUTE_CDATA_END;
						}
						else if ((*chr == '"' && singleQuote == NO) || (*chr == '\'' && singleQuote == YES)) 
						{
							*chr = 0;
							
							// remove cdata section tags
							while ((CDATAStart = strstr(value, "<![CDATA["))) 
							{
								
								// remove begin cdata tag
								memcpy(CDATAStart, CDATAStart+9, strlen(CDATAStart)-8);
								
								// search for end cdata
								CDATAEnd = strstr(CDATAStart,"]]>");
								
								// remove end cdata tag
								memcpy(CDATAEnd, CDATAEnd+3, strlen(CDATAEnd)-2);
							}
							
							
							// create new attribute
							xmlAttribute = [self nextAvailableAttribute];
							
							// if this is the first attribute found, set pointer to this attribute on element
							if (!xmlElement->firstAttribute) xmlElement->firstAttribute = xmlAttribute;
							// if previous attribute found, link this attribute to previous one
							if (lastXMLAttribute) lastXMLAttribute->next = xmlAttribute;
							// set last attribute to this attribute
							lastXMLAttribute = xmlAttribute;

							// set attribute name & value
							xmlAttribute->name = name;
							xmlAttribute->value = value;
							
							// clear name and value pointers
							name = nil;
							value = nil;
							
							// start looking for next attribute
							mode = TBXML_ATTRIBUTE_NAME_START;
						}
						break;
						// look for end of cdata
					case TBXML_ATTRIBUTE_CDATA_END:
						if (*chr == ']')
						{
							if (strncmp(chr, "]]>", 3) == 0)
							{
								mode = TBXML_ATTRIBUTE_VALUE_END;
							}
						}
						break;						
					default:
						break;
				}
			}
		}
		
		// if tag is not self closing, set parent to current element
		if (!selfClosingElement) 
		{
			// set text on element to element end+1
			if (*(elementEnd+1) != '>')
				xmlElement->text = elementEnd+1;
			
			parentXMLElement = xmlElement;
		}
		
		// start looking for next element after end of current element
		elementStart = elementEnd+1;
	}
}

// Deallocate used memory
- (void) dealloc 
{
	
	if (bytes) 
	{
		free(bytes);
		bytes = nil;
	}
	
	while (currentElementBuffer) 
	{
		if (currentElementBuffer->elements)
			free(currentElementBuffer->elements);
		
		if (currentElementBuffer->previous) 
		{
			currentElementBuffer = currentElementBuffer->previous;
			free(currentElementBuffer->next);
		} 
		else 
		{
			free(currentElementBuffer);
			currentElementBuffer = 0;
		}
	}
	
	while (currentAttributeBuffer) 
	{
		if (currentAttributeBuffer->attributes)
			free(currentAttributeBuffer->attributes);
		
		if (currentAttributeBuffer->previous)
		{
			currentAttributeBuffer = currentAttributeBuffer->previous;
			free(currentAttributeBuffer->next);
		} 
		else
		{
			free(currentAttributeBuffer);
			currentAttributeBuffer = 0;
		}
	}
	
	[super dealloc];
}

- (TBXMLElement*) nextAvailableElement 
{
	currentElement++;
	
	if (!currentElementBuffer) 
	{
		currentElementBuffer = calloc(1, sizeof(TBXMLElementBuffer));
		currentElementBuffer->elements = (TBXMLElement*)calloc(1,sizeof(TBXMLElement)*MAX_ELEMENTS);
		currentElement = 0;
		rootXMLElement = &currentElementBuffer->elements[currentElement];
	} 
	else if (currentElement >= MAX_ELEMENTS) 
	{
		currentElementBuffer->next = calloc(1, sizeof(TBXMLElementBuffer));
		currentElementBuffer->next->previous = currentElementBuffer;
		currentElementBuffer = currentElementBuffer->next;
		currentElementBuffer->elements = (TBXMLElement*)calloc(1,sizeof(TBXMLElement)*MAX_ELEMENTS);
		currentElement = 0;
	}
	
	return &currentElementBuffer->elements[currentElement];
}

- (TBXMLAttribute*) nextAvailableAttribute 
{
	currentAttribute++;
	
	if (!currentAttributeBuffer) 
	{
		currentAttributeBuffer = calloc(1, sizeof(TBXMLAttributeBuffer));
		currentAttributeBuffer->attributes = (TBXMLAttribute*)calloc(MAX_ATTRIBUTES,sizeof(TBXMLAttribute));
		currentAttribute = 0;
	} 
	else if (currentAttribute >= MAX_ATTRIBUTES) 
	{
		currentAttributeBuffer->next = calloc(1, sizeof(TBXMLAttributeBuffer));
		currentAttributeBuffer->next->previous = currentAttributeBuffer;
		currentAttributeBuffer = currentAttributeBuffer->next;
		currentAttributeBuffer->attributes = (TBXMLAttribute*)calloc(MAX_ATTRIBUTES,sizeof(TBXMLAttribute));
		currentAttribute = 0;
	}
	
	return &currentAttributeBuffer->attributes[currentAttribute];
}

@end
