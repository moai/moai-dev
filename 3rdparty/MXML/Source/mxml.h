/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml parser

 ****************************************************************************/

#ifndef _MXML_H_
#define _MXML_H_

#include "mxmlutf8.h"

/**
 * \addtogroup MXML Micro XML
 *
 * Micro XML was developed to provide an XML platform suitable for games development.  It allows memory
 * overrides, custom stream handling, propper streaming for loading, CDATA sections, UTF8 and basic escape
 * sequence entity support.
 *
 * TODO: conversion back to entities on DOM Save
 */

/**
 * \addtogroup MXMLParser Micro XML Parser
 * \ingroup MXML
 *
 * The Micro XML parser is implemented in pure c, and can be used without the Micro XML DOM.  The parser
 * supports input stream buffering, and provides parsed document information via 3 registered callbacks
 * on the created parser object.
 */

/*!
  \ingroup MXMLParser
  \enum MXMLResult
  \brief parser result type, returned from MXMLParserParse
*/
typedef enum
{
  MXML_RESULT_DONE = 0, /*!< xml parsing is done */
  MXML_RESULT_PARSING,  /*!< xml parsing is continuing */
  MXML_RESULT_ERROR,    /*!< error parsing xml */

} MXMLResult;


/*!
  \ingroup MXMLParser
  \enum MXMLCharacterDataType
  \brief type of data being given to the character data callback
*/
typedef enum
{
  MXML_CDT_DECLARATION = 0, /*!< declaration node */
  MXML_CDT_COMMENT,         /*!< comment node */
  MXML_CDT_TEXT,            /*!< text node */
  MXML_CDT_CDATA,           /*!< CDATA node */

} MXMLCharacterDataType;


/*!
  \ingroup MXMLParser
  \brief parser callback to handle xml start elements
  \param a_userData is the user data given to the MXMLParser object
  \param a_name is the name of the node (UTF8)
  \param a_attributes is an array of attribute name value pairs (UTF8)
*/
typedef void (*MXMLStartElementCallback)(void * a_userData,
                                         const char * a_name,
                                         const char ** a_attributes);


/*!
  \ingroup MXMLParser
  \brief parser callback to handle xml end elements
  \param a_userData is the user data given to the MXMLParser object
  \param a_name is the name of the node (UTF8)
*/
typedef void (*MXMLEndElementCallback)(void * a_userData,
                                       const char * a_name);


/*!
  \ingroup MXMLParser
  \brief parser callback to handle xml element data, a_size does not include terminating char
  \param a_userData is the user data given to the MXMLParser object
  \param a_data is the character data for the said node type (UTF8)
  \param a_size is the size of the data excluding the null terminating char
  \param a_type is the type of character data
*/
typedef void (*MXMLCharacterDataCallback)(void * a_userData,
                                          const char * a_data,
                                          size_t a_size,
                                          MXMLCharacterDataType a_type);

/*!
  \ingroup MXMLParser
  \brief parser alloc callback
*/
typedef void * (*MXMLAlloc)(size_t a_size, void* a_memContext);


/*!
  \ingroup MXMLParser
  \brief parser free callback
*/
typedef void (*MXMLFree)(void * a_mem, void* a_memContext);


/*!
  \ingroup MXMLParser
  \struct MXMLParser
  \brief XMLParser type returned by MXMLParserCreate
*/
typedef struct _MXMLParser
{
  MXMLStartElementCallback m_StartElementCallback;
  MXMLEndElementCallback m_EndElementCallback;
  MXMLCharacterDataCallback m_CharacterDataCallback;
  MXMLCharacterDataCallback m_CommentCallback;

  void * m_userData;
  void * m_sysData;

} MXMLParser;


/*!
  \ingroup MXMLParser
  \brief create a micro xml parser
  \param a_Alloc is a memory hook used for all internal memory allocations
  \param a_Free is a memory hook for freeing all memory allocations
  \return MXMLParser or NULL on error
*/
MXMLParser * MXMLParserCreate(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext);


/*!
  \ingroup MXMLParser
  \brief destroy a xml parser
  \param a_parser is a parser created using MXMLParserCreate
*/
void MXMLParserDestroy(MXMLParser * a_parser);


/*!
  \ingroup MXMLParser
  \brief parse function
  \param a_parser is a MXMLParser created with MXMLParserCreate
  \param a_buffer is the current input buffer (UTF8) of size a_bufferSize, and may be a portion of a larger stream
  \param a_bufferSize is the current buffer size in bytes
  \param a_lastBuffer is set to 1 if this is the last input buffer
  \return xml parsing result

  Below is an example usage of the MXMLParser:

  \verbatim

  static void StartElement(void *userData, const char *name, const char **atts)
  {
    printf("<name=%s ", name);
    for(int i = 0; atts[i]; i += 2)
    {
      const char * name = atts[i];
      const char * value = atts[i+1];
      printf(", %s = %s", name, value);
    }
    printf(">\n");
  }

  static void EndElement(void *userData, const char *name)
  {
    printf("</%s>\n", name);
  }

  static void CharacterData(void *userData, const char *s, size_t len, MXMLCharacterDataType a_type)
  {
    printf("%s\n", s);
  }

  static int Load(FILE * f)
  {
    unsigned char buf[8192];
    MXMLParser * parser = MXMLParserCreate(malloc, free);
    int done;

    parser->m_userData = NULL;
    parser->m_StartElementCallback = StartElement;
    parser->m_EndElementCallback = EndElement;
    parser->m_CharacterDataCallback = CharacterData;

    do 
    {
      size_t len = fread(buf, 1, sizeof(buf), f);
      done = len < sizeof(buf);
      if (MXMLParserParse(parser, buf, len, done) == MXML_RESULT_ERROR) 
      {
        MXMLParserGetErrorString(parser);
        MXMLParserDestroy(parser);
        return -1;
      }
    } while (!done);

    MXMLParserDestroy(parser);
    return 0;
  }

  \endverbatim  
*/
MXMLResult MXMLParserParse(MXMLParser * a_parser, 
                           const unsigned char * a_buffer, 
                           size_t a_bufferSize,
                           int a_lastBuffer);


/*!
  \ingroup MXMLParser
  \brief return an error string if MXMLParserParse returned MXML_RESULT_ERROR
*/
const char * MXMLParserGetErrorString(MXMLParser * a_parser);

/// \brief Call before encoding strings (eg. for a single file) to prepare resources
/// \param a_Alloc Allocator Alloc()
/// \param a_Free Allocator Free()
/// \param a_memContext Allocator context
/// \return returns the allocated encoder
void* MXMLEscapeEncodeBegin(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext);

/// \brief encode a string for xml compatability
/// \param a_encoder value returned by MXMLEncodeBegin()
/// \param a_inBuffer Input character buffer
/// \param a_inLength Input character buffer length
/// \param a_outBuffer Output character buffer. WARNING: Use this value immediately as it may return a shared buffer.
/// \param a_outLength Output character buffer length;
void MXMLEscapeEncodeBuffer(void* a_encoder, const char* a_inBuffer, size_t a_inLength,
                                       const char*& a_outBuffer, size_t& a_outLength);

/// \brief Call when finished encoding strings (eg. for a single file) to free resources
/// \param the encoder to destruct
void MXMLEscapeEncodeEnd(void* a_encoder);

#endif
