/*
  Copyright (C) 2011 Markus Kauppila <markus.kauppila@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _XML_H
#define _XML_H

/*! Defines attribute for XML elements */
typedef struct Attribute {
	const char *attribute;
	const char *value;
} Attribute;


/*!
 * Opens XML document.
 * Creates header and start tag for root element.
 *
 * Note: XML creation is not thread-safe!
 *
 * \param rootTag Root tag for the XML document
 * \param xslStyle Name of the style sheet file. (empty string if no style is used)
 * \return The generated XML output
 */
char *XMLOpenDocument(const char *rootTag, const char *xslStyle);

/*!
 * Closes the XML-document.
 * Creates end tag for root element and closes other open elements
 * with correct nesting.
 */
char *XMLCloseDocument();

/*!
 *  Opens XML-element.
 *
 *  \param tag Element to be opened
 */
char *XMLOpenElement(const char *tag);

/*!
 * Add content to currently open element.
 *
 * \param content Content for the currently open element
 */
char *XMLAddContent(const char *content);

/*!
 * Closes previously opened element until tag given as parameter is met.
 * Enforces proper nesting by not allowing to close elements out-of-order.
 *
 * Closes all the opened elements until the given element/tag is found
 * which will be the last tag to be closed
 *
 * \param tag Element to close
 */
char *XMLCloseElement(const char *tag);

#endif

