/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml parser

 ****************************************************************************/

// Added this to remove legacy code warnings
#pragma warning(disable : 4996) // Deprecated functions
#define _CRT_SECURE_NO_DEPRECATE // Allow old unsecure standard library functions

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mxml.h"


/*
 * 
 * Defines
 *
 */

#define MXML_ELEMENT_STACK_GROW_BY      16
#define MXML_ATTRIBUTE_GROW_BY          8
#define MXML_BUFFER_INITIAL_SIZE        64

#define MXML_ISWHITESPACE(c) (((c) == ' ') || ((c) == '\r') || ((c) == '\n') || ((c) == '\t'))
#define MXML_ISNAMESTART(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || (c) == '_' || (c) == ':')
#define MXML_ISNAMECHAR(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9') || (c) == '_' || (c) == '-' || (c) == ':' || (c) == '.')
#define MXML_MATCH(c, MATCH, ERROR, NEXT) \
{ \
  if((c) != (MATCH)) \
  { \
    sprintf(sysParser->m_errorString, ERROR" line %d, col %d", sysParser->m_line, sysParser->m_column); \
    return MXML_RESULT_ERROR; \
  } \
  sysParser->m_parserState = NEXT; \
  break; \
}


typedef struct _MXMLEntity
{
  const char * m_entity;
  const char * m_entityFull;
  int m_character;
} MXMLEntity;


// These are dependant on s_entities
enum EntitySymbols
{
  SYMBOL_APOS = 0,
  SYMBOL_AMP =  1,
  SYMBOL_GT =   2,
  SYMBOL_LT =   3,
  SYMBOL_QUOT = 4,
};

static MXMLEntity s_entities[] =
{
  {"apos", "&apos;", '\''},     // SYMBOL_APOS = 0
  {"amp",  "&amp;",  '&'},      // SYMBOL_AMP =  1
  {"gt",   "&gt;",   '>'},      // SYMBOL_GT =   2
  {"lt",   "&lt;",   '<'},      // SYMBOL_LT =   3
  {"quot", "&quot;", '\"'}      // SYMBOL_QUOT = 4
};


/*
 * 
 * MXMLBuffer
 *
 */


typedef struct _MXMLBuffer
{
  char * m_buffer;
  size_t m_bufferSize;
  size_t m_maxBufferSize;
  MXMLAlloc m_Alloc;
  MXMLFree m_Free;
  void* m_memContext;
} MXMLBuffer;


static void MXMLBufferInit(MXMLBuffer * a_buffer, MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext)
{
  memset(a_buffer, 0, sizeof(MXMLBuffer));
  a_buffer->m_Alloc = a_Alloc;
  a_buffer->m_Free = a_Free;
  a_buffer->m_memContext = a_memContext;
}


static void MXMLBufferDone(MXMLBuffer * a_buffer)
{
  if(a_buffer->m_buffer)
  {
    a_buffer->m_Free(a_buffer->m_buffer, a_buffer->m_memContext);
  }
  memset(a_buffer, 0, sizeof(MXMLBuffer));
}


static void MXMLBufferReset(MXMLBuffer * a_buffer)
{
  a_buffer->m_bufferSize = 0;
}


static void MXMLBufferAppend(MXMLBuffer * a_buffer, int a_char)
{
  size_t maxSize;
  char * buffer;
  char unpack[4];
  int numBytes;

  /* calculate required size */
  numBytes = MXMLUtf8FromChar(a_char, unpack);

  /* grow buffer by power of 2 if required */
  if(a_buffer->m_bufferSize + numBytes >= a_buffer->m_maxBufferSize)
  {
    if(a_buffer->m_maxBufferSize < MXML_BUFFER_INITIAL_SIZE)
      maxSize = MXML_BUFFER_INITIAL_SIZE;
    else
      maxSize = a_buffer->m_maxBufferSize << 1;
    buffer = (char*) a_buffer->m_Alloc(sizeof(char) * maxSize, a_buffer->m_memContext);
    memcpy(buffer, a_buffer->m_buffer, sizeof(char) * a_buffer->m_bufferSize);
    if(a_buffer->m_buffer)
      a_buffer->m_Free(a_buffer->m_buffer, a_buffer->m_memContext);
    a_buffer->m_buffer = buffer;
    a_buffer->m_maxBufferSize = maxSize;
  }

  /* add character */
  memcpy(a_buffer->m_buffer + a_buffer->m_bufferSize, unpack, numBytes);
  a_buffer->m_bufferSize += numBytes;
}


// Round a number up to the next power of 2
static int CeilPow2(int a_val)
{
  --a_val;

  a_val |= a_val >> 16;
  a_val |= a_val >> 8;
  a_val |= a_val >> 4;
  a_val |= a_val >> 2;
  a_val |= a_val >> 1;

  return a_val + 1;
}

static void MXMLBufferAppend(MXMLBuffer * a_buffer, const char* a_string, int a_stringLength = 0)
{
  unsigned int maxLength;
  char * buffer;

  if( !a_stringLength )
  {
    a_stringLength = (unsigned int)strlen(a_string);
  }

  // Grow buffer by power of 2 if required
  unsigned int requiredSize = (unsigned int)(a_buffer->m_bufferSize + a_stringLength);
  if(requiredSize >= a_buffer->m_maxBufferSize)
  {
    if(requiredSize < MXML_BUFFER_INITIAL_SIZE)
    {
      maxLength = MXML_BUFFER_INITIAL_SIZE;
    }
    else
    {
      maxLength = CeilPow2(requiredSize);
    }
    
    buffer = (char*)a_buffer->m_Alloc(maxLength, a_buffer->m_memContext);
    memcpy(buffer, a_buffer->m_buffer, a_buffer->m_bufferSize);
    if( a_buffer->m_buffer )
    {
      a_buffer->m_Free(a_buffer->m_buffer, a_buffer->m_memContext);
    }
    a_buffer->m_buffer = buffer;
    a_buffer->m_maxBufferSize = maxLength;
  }

  // Add characters
  memcpy(a_buffer->m_buffer + a_buffer->m_bufferSize, a_string, a_stringLength);
  a_buffer->m_bufferSize += a_stringLength;
}


/*
 * 
 * MXMLAttribute
 *
 */


typedef struct _MXMLAttribute
{
  MXMLBuffer m_name;
  MXMLBuffer m_value;

} MXMLAttribute;


static void MXMLAttributeInit(MXMLAttribute * a_attribute, MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext)
{
  MXMLBufferInit(&a_attribute->m_name, a_Alloc, a_Free, a_memContext);
  MXMLBufferInit(&a_attribute->m_value, a_Alloc, a_Free, a_memContext);
}


static void MXMLAttributeDone(MXMLAttribute * a_attribute)
{
  MXMLBufferDone(&a_attribute->m_name);
  MXMLBufferDone(&a_attribute->m_value);
}


static void MXMLAttriubteReset(MXMLAttribute * a_attribute)
{
  MXMLBufferReset(&a_attribute->m_name);
  MXMLBufferReset(&a_attribute->m_value);
}


/*
 * 
 * MXMLElement
 *
 */


typedef struct _MXMLElement
{
  unsigned long m_nameHash;
} MXMLElement;


/*
 * 
 * MXMLParserState
 *
 */


typedef enum
{
  MXML_PS_BOM = 0,
  MXML_PS_DOCSTART,
    
  /* start element */    
    
  MXML_PS_STARTELEMENTPRENAME,
  MXML_PS_STARTELEMENTNAME,
  MXML_PS_STARTELEMENTENDNAME,
  MXML_PS_STARTELEMENTENDELEMENT,
  MXML_PS_STARTELEMENT,

  /* attributes */

  MXML_PS_ATTRIBUTENAME,
  MXML_PS_ATTRIBUTEASSIGN,
  MXML_PS_ATTRIBUTEPOSTASSIGN,
  MXML_PS_ATTRIBUTEVALUE,

  /* text */

  MXML_PS_TEXT,
  MXML_PS_TEXTEND,

  /* esacpe, we don't handle true entities, just escape sequences */

  MXML_PS_ESCAPE,

  /* end element */

  MXML_PS_ENDELEMENTNAMESTART,
  MXML_PS_ENDELEMENTNAME,
  MXML_PS_ENDELEMENTEND,

  /* cdata */

  MXML_PS_CDATAA,
  MXML_PS_CDATAB,
  MXML_PS_CDATAC,
  MXML_PS_CDATAD,
  MXML_PS_CDATAE,
  MXML_PS_CDATAF,
  MXML_PS_CDATAG,
  MXML_PS_CDATAH,
  MXML_PS_CDATAI,

  /* comment */

  MXML_PS_COMMENTA,
  MXML_PS_COMMENTB,
  MXML_PS_COMMENTC,
  MXML_PS_COMMENTD,

  /* declaration */

  MXML_PS_DECLARATIONSTART,
  MXML_PS_DECLARATIONA,

  MXML_PS_DONE,

} MXMLParserState;


/*
 * 
 * MXMLSysParser
 *
 */


typedef struct _MXMLSysParser
{
  MXMLParser m_parser;

  MXMLAlloc m_Alloc;
  MXMLFree m_Free;
  void* m_memContext;

  /* element stack */

  MXMLElement * m_elements;
  unsigned int m_numElements;
  unsigned int m_maxElements;

  /* input buffer */

  const unsigned char * m_buffer;
  size_t m_cursor;
  size_t m_bufferSize;
  unsigned int m_line;
  unsigned int m_column;

  /* parser state */

  unsigned char m_encoding[4];
  int m_encodingBytesRequired;
  int m_encodingBytes;
  MXMLParserState m_parserState;
  MXMLParserState m_escapeRestoreState;
  MXMLBuffer m_characterData;
  MXMLBuffer m_escapeData;
  MXMLBuffer m_name;
  int m_parsedRoot;
  
  /* attributes */

  MXMLAttribute * m_attributes;
  const char ** m_attributeMarshal;
  unsigned int m_numAttributes;
  unsigned int m_maxAttributes;

  /* error */
  char m_errorString[128];

} MXMLSysParser;


static MXMLElement * MXMLSysParserPushElement(MXMLSysParser * a_parser)
{
  unsigned int maxElements;
  MXMLElement * elements;

  /* grow stack if required */
  if(a_parser->m_numElements >= a_parser->m_maxElements)
  {
    maxElements = a_parser->m_maxElements + MXML_ELEMENT_STACK_GROW_BY;
    elements = (MXMLElement*) a_parser->m_Alloc(sizeof(MXMLElement) * maxElements, a_parser->m_memContext);
    memcpy(elements, a_parser->m_elements, (size_t) (sizeof(MXMLElement) * a_parser->m_numElements));
    if(a_parser->m_elements)
      a_parser->m_Free(a_parser->m_elements, a_parser->m_memContext);
    a_parser->m_elements = elements;
    a_parser->m_maxElements = maxElements;
  }

  /* increase stack */
  return &a_parser->m_elements[a_parser->m_numElements++];
}


static MXMLElement * MXMLSysParserGetElement(MXMLSysParser * a_parser)
{
  if(a_parser->m_numElements > 0)
  {
    return &a_parser->m_elements[a_parser->m_numElements - 1];
  }
  return 0;
}


static MXMLElement * MXMLSysParserPopElement(MXMLSysParser * a_parser)
{
  if(a_parser->m_numElements > 1)
  {
    return &a_parser->m_elements[--a_parser->m_numElements];
  }
  else if(a_parser->m_numElements > 0)
  {
    a_parser->m_parsedRoot = 1;
    return &a_parser->m_elements[--a_parser->m_numElements];
  }
  return 0;
}


static MXMLAttribute * MXMLSysParserAddAttribute(MXMLSysParser * a_parser)
{
  MXMLAttribute * attributes;
  unsigned int maxAttributes, i;

  if(a_parser->m_numAttributes >= a_parser->m_maxAttributes)
  {
    maxAttributes = a_parser->m_maxAttributes + MXML_ATTRIBUTE_GROW_BY;
    attributes = (MXMLAttribute *) a_parser->m_Alloc(sizeof(MXMLAttribute) * maxAttributes, a_parser->m_memContext);
    /* copy old attributes */
    memcpy(attributes, a_parser->m_attributes, (size_t) (sizeof(MXMLAttribute) * a_parser->m_numAttributes));
    /* initialise new attributes */
    for(i = a_parser->m_numAttributes; i < maxAttributes; ++i)
    {
      MXMLAttributeInit(&attributes[i], a_parser->m_Alloc, a_parser->m_Free, a_parser->m_memContext);
    }
    if(a_parser->m_attributes)
      a_parser->m_Free(a_parser->m_attributes, a_parser->m_memContext);
    a_parser->m_attributes = attributes;
    a_parser->m_maxAttributes = maxAttributes;

    if(a_parser->m_attributeMarshal)
      a_parser->m_Free((void *) a_parser->m_attributeMarshal, a_parser->m_memContext);
    a_parser->m_attributeMarshal = (const char **) a_parser->m_Alloc(sizeof(char *) * ((maxAttributes*2) + 1), a_parser->m_memContext); /* add 1 for null terminator */
  }

  MXMLAttriubteReset(&a_parser->m_attributes[a_parser->m_numAttributes]);
  return &a_parser->m_attributes[a_parser->m_numAttributes++];
}


static MXMLAttribute * MXMLSysParserGetAttribute(MXMLSysParser * a_parser)
{
  if(a_parser->m_numAttributes > 0)
  {
    return &a_parser->m_attributes[a_parser->m_numAttributes - 1];
  }
  return 0;
}


static void MXMLSysParserResetStartElementData(MXMLSysParser * a_parser)
{
  MXMLBufferReset(&a_parser->m_name);
  a_parser->m_numAttributes = 0;
}


static void MXMLSysParserHandleStartElement(MXMLSysParser * a_parser)
{
  unsigned int i, a;

  if(a_parser->m_parser.m_StartElementCallback)
  {
    /* fill attribute marshal buffer */
    for(i = 0, a = 0; i < a_parser->m_numAttributes; ++i)
    {
      a_parser->m_attributeMarshal[a++] = a_parser->m_attributes[i].m_name.m_buffer;
      a_parser->m_attributeMarshal[a++] = a_parser->m_attributes[i].m_value.m_buffer;
    }
    a_parser->m_attributeMarshal[a++] = (char *) 0;

    /* call the callback */
    a_parser->m_parser.m_StartElementCallback(a_parser->m_parser.m_userData, a_parser->m_name.m_buffer, a_parser->m_attributeMarshal);
  }
}


static void MXMLSysParserHandleEndElement(MXMLSysParser * a_parser)
{
  if(a_parser->m_parser.m_EndElementCallback)
  {
    /* call the callback */
    a_parser->m_parser.m_EndElementCallback(a_parser->m_parser.m_userData, a_parser->m_name.m_buffer);
  }
}


static void MXMLSysParserHandleCharacterData(MXMLSysParser * a_parser, MXMLCharacterDataType a_type)
{
  MXMLBufferAppend(&a_parser->m_characterData, 0);

  if(a_parser->m_parser.m_CharacterDataCallback)
  {
    /* if TEXT character data is empty, ignore it */
    if((a_type != MXML_CDT_TEXT) || (*a_parser->m_characterData.m_buffer))
    {
      /* call the callback */
      a_parser->m_parser.m_CharacterDataCallback(a_parser->m_parser.m_userData, a_parser->m_characterData.m_buffer, a_parser->m_characterData.m_bufferSize - 1, a_type);
    }
  }
}


static int MXMLSysParserHandleEscape(MXMLSysParser * a_parser)
{
  /* handles escape sequences such as &amp; sequence is in parser escapedata. return 0 on error */
  /* escape sequence handled as 1 byte UTF8 sequence */
  const char * sequence;
  int i = 0, c = 0;

  MXMLBufferAppend(&a_parser->m_escapeData, 0);
  sequence = (const char *) a_parser->m_escapeData.m_buffer;

  if(sequence[0] == '#')
  {
    if(sequence[1] == 'x')
    {
      if(sscanf(sequence + 2, "%x", &c) == 1)
      {
        return c;
      }
    }
    else if(sscanf(sequence + 1, "%d", &c) == 1)
    {
      return c;
    }
    sprintf(a_parser->m_errorString, "invalid character escape  sequence line %d, col %d", a_parser->m_line, a_parser->m_column);
  }
  else
  {
    for(i = 0; i < (sizeof(s_entities) / sizeof(s_entities[0])); ++i)
    {
      if(strcmp(s_entities[i].m_entity, sequence) == 0)
      {
        return s_entities[i].m_character;
      }
    }
    sprintf(a_parser->m_errorString, "unrecognized escape sequence line %d, col %d", a_parser->m_line, a_parser->m_column);
  }

  return 0;
}


static MXMLSysParser * MXMLSysParserCreate(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext)
{
  MXMLSysParser * sysParser = (MXMLSysParser *) a_Alloc(sizeof(MXMLSysParser), a_memContext);
  memset(sysParser, 0, sizeof(MXMLSysParser));

  sysParser->m_Alloc = a_Alloc;
  sysParser->m_Free = a_Free;
  sysParser->m_memContext = a_memContext;

  sysParser->m_parser.m_sysData = sysParser;

  MXMLBufferInit(&sysParser->m_characterData, a_Alloc, a_Free, sysParser->m_memContext);
  MXMLBufferInit(&sysParser->m_escapeData, a_Alloc, a_Free, sysParser->m_memContext);
  MXMLBufferInit(&sysParser->m_name, a_Alloc, a_Free, sysParser->m_memContext);

  sysParser->m_line = 1;
  sysParser->m_column = 1;
  sysParser->m_parserState = MXML_PS_BOM;
  
  sysParser->m_attributeMarshal = (const char **) sysParser->m_Alloc(sizeof(char *) * 1, sysParser->m_memContext); /* 1 for null terminator */
  sysParser->m_numAttributes = 0;

  return sysParser;
}


static void MXMLSysParserDestroy(MXMLSysParser * a_parser)
{
  unsigned int i;

  if(a_parser->m_elements != 0)
    a_parser->m_Free(a_parser->m_elements, a_parser->m_memContext);

  MXMLBufferDone(&a_parser->m_characterData);
  MXMLBufferDone(&a_parser->m_escapeData);
  MXMLBufferDone(&a_parser->m_name);

  /* destroy the attriubtes */
  for(i = 0; i < a_parser->m_maxAttributes; ++i)
    MXMLAttributeDone(&a_parser->m_attributes[i]);

  if(a_parser->m_attributes)
    a_parser->m_Free(a_parser->m_attributes, a_parser->m_memContext);
  if(a_parser->m_attributeMarshal)
    a_parser->m_Free((void *) a_parser->m_attributeMarshal, a_parser->m_memContext);

  a_parser->m_Free(a_parser, a_parser->m_memContext);
}


/*
 * 
 * MXMLParser
 *
 */


// Default alloc(), with context
void* DefaultAlloc(size_t a_size, void* a_context)
{
  return malloc(a_size);
}

// Default free(), with context
void DefaultFree(void* a_mem, void* a_context)
{
  free(a_mem);
}


MXMLParser * MXMLParserCreate(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext)
{
  MXMLSysParser * sysParser = NULL;

  if(a_Alloc == NULL || a_Free == NULL)
  {
    a_Alloc = DefaultAlloc;
    a_Free = DefaultFree;
    a_memContext = NULL;
  }

  sysParser = MXMLSysParserCreate(a_Alloc, a_Free, a_memContext);

  return &sysParser->m_parser;
}


void MXMLParserDestroy(MXMLParser * a_parser)
{
  MXMLSysParser * sysParser = (MXMLSysParser *) a_parser->m_sysData;
  if(sysParser)
  {
    MXMLSysParserDestroy(sysParser);
  }
}


MXMLResult MXMLParserParse(MXMLParser * a_parser, 
                           const unsigned char * a_buffer, 
                           size_t a_bufferSize,
                           int a_lastBuffer)
{
  MXMLSysParser * sysParser = (MXMLSysParser *) a_parser->m_sysData;
  MXMLAttribute * attribute = MXMLSysParserGetAttribute(sysParser);
  MXMLElement * element;
  int c = 0;
  int popChar = 1;
  unsigned char byte;

  /* reset input buffer */
  sysParser->m_buffer = a_buffer;
  sysParser->m_bufferSize = a_bufferSize;
  sysParser->m_cursor = 0;

  /* continue eating characters until we are done */
  while(sysParser->m_parserState != MXML_PS_DONE)
  {
    /* read the next character from the input buffer */
    if(popChar)
    {
      for(;;)
      {
        if(sysParser->m_cursor >= sysParser->m_bufferSize)
        {
          if(a_lastBuffer)
          {
            if(sysParser->m_parsedRoot)
            {
              return MXML_RESULT_DONE;
            }
            sprintf(sysParser->m_errorString, "expecting '<' line %d, col %d", sysParser->m_line, sysParser->m_column);
            return MXML_RESULT_ERROR;
          }
          return MXML_RESULT_PARSING;
        }

        byte = sysParser->m_buffer[sysParser->m_cursor++];
        if(sysParser->m_encodingBytes == sysParser->m_encodingBytesRequired)
        {
          /* first byte of a new encoding */
          sysParser->m_encodingBytesRequired = MXMLUtf8ByteCount[byte];
          sysParser->m_encoding[0] = byte;
          sysParser->m_encodingBytes = 1;
        }
        else
        {
          sysParser->m_encoding[sysParser->m_encodingBytes++] = byte;
        }
        if(sysParser->m_encodingBytes == sysParser->m_encodingBytesRequired)
        {
          MXMLUtf8ToChar((const char *) sysParser->m_encoding, &c);
          break;
        }
      }

      if(c == '\n')
      {
        ++sysParser->m_line;
        sysParser->m_column = 1;
      }
      else
      {
        ++sysParser->m_column;
      }
    }
    else
    {
      popChar = 1;
    }

    /* continue processing in current state */
    switch(sysParser->m_parserState)
    {
      case MXML_PS_BOM :
      {
        MXMLBufferReset(&sysParser->m_characterData);
        sysParser->m_parserState = MXML_PS_DOCSTART;
        if(c != 0x0000feff) /* stupid UTF8 BOM */
        {
          popChar = 0;
        }
        break;
      }
      case MXML_PS_DOCSTART :
      {
        if(c == '<')
        {
          MXMLSysParserHandleCharacterData(sysParser, MXML_CDT_TEXT);
          sysParser->m_parserState = MXML_PS_STARTELEMENTPRENAME;
        }
        else if(!MXML_ISWHITESPACE(c))
        {
          sprintf(sysParser->m_errorString, "expecting '<' line %d, col %d", sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        MXMLBufferAppend(&sysParser->m_characterData, c);
        break;
      }

      /* 
       * start element
       */

      case MXML_PS_STARTELEMENTPRENAME :
      {
        MXMLSysParserResetStartElementData(sysParser);

        if(MXML_ISNAMESTART(c))
        {
          if(sysParser->m_parsedRoot)
          {
            sprintf(sysParser->m_errorString, "already parsed document root element line %d, col %d", c, sysParser->m_line, sysParser->m_column);
            return MXML_RESULT_ERROR;
          }

          MXMLSysParserPushElement(sysParser);
          MXMLBufferAppend(&sysParser->m_name, c);
          sysParser->m_parserState = MXML_PS_STARTELEMENTNAME;
        }
        else if(c == '?')
        {
          MXMLBufferReset(&sysParser->m_characterData);
          sysParser->m_parserState = MXML_PS_DECLARATIONSTART;
        }
        else if(c == '!')
        {
          MXMLBufferReset(&sysParser->m_characterData);
          sysParser->m_parserState = MXML_PS_STARTELEMENT;
        }
        else
        {
          sprintf(sysParser->m_errorString, "invalid name start '%c' line %d, col %d", c, sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_STARTELEMENTNAME :
      {
        assert(sysParser->m_numAttributes == 0);
        if(MXML_ISNAMECHAR(c))
        {
          MXMLBufferAppend(&sysParser->m_name, c);
        }
        else if(MXML_ISWHITESPACE(c))
        {
          MXMLBufferAppend(&sysParser->m_name, 0);
          element = MXMLSysParserGetElement(sysParser);
          assert(element);
          element->m_nameHash = MXMLUtf8Hash(sysParser->m_name.m_buffer);
          sysParser->m_parserState = MXML_PS_STARTELEMENTENDNAME;
        }
        else if(c == '/' || c =='>')
        {
          popChar = 0;
          MXMLBufferAppend(&sysParser->m_name, 0);
          element = MXMLSysParserGetElement(sysParser);
          assert(element);
          element->m_nameHash = MXMLUtf8Hash(sysParser->m_name.m_buffer);
          sysParser->m_parserState = MXML_PS_STARTELEMENTENDNAME;
        }
        else
        {
          sprintf(sysParser->m_errorString, "unexpected character '%c' line %d, col %d", c, sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_STARTELEMENTENDNAME :
      {
        if(c == '>')
        {
          MXMLSysParserHandleStartElement(sysParser);
          MXMLBufferReset(&sysParser->m_characterData);
          sysParser->m_parserState = MXML_PS_TEXT;
        }
        else if(c == '/')
        {
          sysParser->m_parserState = MXML_PS_STARTELEMENTENDELEMENT;
        }
        else if(MXML_ISNAMESTART(c))
        {
          attribute = MXMLSysParserAddAttribute(sysParser);
          assert(attribute);
          MXMLBufferAppend(&attribute->m_name, c);
          sysParser->m_parserState = MXML_PS_ATTRIBUTENAME;
        }
        else if(!MXML_ISWHITESPACE(c))
        {
          sprintf(sysParser->m_errorString, "unexpected character '%c' line %d, col %d", c, sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_STARTELEMENTENDELEMENT :
      {
        if(c == '>')
        {
          MXMLSysParserHandleStartElement(sysParser);
          MXMLSysParserHandleEndElement(sysParser);
          MXMLBufferReset(&sysParser->m_characterData);
          if(MXMLSysParserPopElement(sysParser))
          {
            sysParser->m_parserState = MXML_PS_TEXT;
            break;
          }
          else
          {
            sysParser->m_parserState = MXML_PS_DOCSTART;
            break;
          }
        }
        sprintf(sysParser->m_errorString, "expecting '>' line %d, col %d", sysParser->m_line, sysParser->m_column);
        return MXML_RESULT_ERROR;
      }
      case MXML_PS_STARTELEMENT :
      {
        if(c == '-')
        {
          sysParser->m_parserState = MXML_PS_COMMENTA;
          break;
        }
        else if(c == '[')
        {
          if(!MXMLSysParserGetElement(sysParser))
          {
            sprintf(sysParser->m_errorString, "expecting element line %d, col %d", sysParser->m_line, sysParser->m_column);
            return MXML_RESULT_ERROR;
          }
          sysParser->m_parserState = MXML_PS_CDATAA;
          break;
        }
        sprintf(sysParser->m_errorString, "expecting '-' or '[' line %d, col %d", sysParser->m_line, sysParser->m_column);
        return MXML_RESULT_ERROR;
      }

      /* 
       * text
       */

      case MXML_PS_TEXT :
      {
        if(c == '&')
        {
          /* handle escape */
          MXMLBufferReset(&sysParser->m_escapeData);
          sysParser->m_parserState = MXML_PS_ESCAPE;
          sysParser->m_escapeRestoreState = MXML_PS_TEXT;
        }
        else if(c == '<')
        {
          /* handle the current character data */
          MXMLSysParserHandleCharacterData(sysParser, MXML_CDT_TEXT);
          sysParser->m_parserState = MXML_PS_TEXTEND;
        }
        else
        {
          MXMLBufferAppend(&sysParser->m_characterData, c);
        }
        break;
      }
      case MXML_PS_TEXTEND :
      {
        if(c == '/')
        {
          sysParser->m_parserState = MXML_PS_ENDELEMENTNAMESTART;
        }
        else
        {
          popChar = 0;
          sysParser->m_parserState = MXML_PS_STARTELEMENTPRENAME;
        }
        break;
      }

      /* 
       * escape
       */

      case MXML_PS_ESCAPE :
      {
        if(c == ';')
        {
          /* handle the current escape */
          int escape = MXMLSysParserHandleEscape(sysParser);
          if(escape == 0)
          {
            return MXML_RESULT_ERROR;
          }
          switch(sysParser->m_escapeRestoreState)
          {
            case MXML_PS_TEXT : MXMLBufferAppend(&sysParser->m_characterData, escape); break;
            case MXML_PS_ATTRIBUTEVALUE : MXMLBufferAppend(&attribute->m_value, escape); break;
            default : assert(false); break;
          }
          sysParser->m_parserState = sysParser->m_escapeRestoreState;
        }
        else
        {
          MXMLBufferAppend(&sysParser->m_escapeData, c);
        }
        break;
      }

      /* 
       * attribute
       */

      case MXML_PS_ATTRIBUTENAME :
      {
        assert(attribute);
        if(MXML_ISNAMECHAR(c))
        {
          MXMLBufferAppend(&attribute->m_name, c);
        }
        else if(MXML_ISWHITESPACE(c))
        {
          MXMLBufferAppend(&attribute->m_name, 0);
          sysParser->m_parserState = MXML_PS_ATTRIBUTEASSIGN;
        }
        else if(c == '=')
        {
          MXMLBufferAppend(&attribute->m_name, 0);
          sysParser->m_parserState = MXML_PS_ATTRIBUTEPOSTASSIGN;
        }
        else
        {
          sprintf(sysParser->m_errorString, "expecting '=' line %d, col %d", sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_ATTRIBUTEASSIGN :
      {
        if(c == '=')
        {
          sysParser->m_parserState = MXML_PS_ATTRIBUTEPOSTASSIGN;
        }
        else if(!MXML_ISWHITESPACE(c))
        {
          sprintf(sysParser->m_errorString, "expecting '=' line %d, col %d", sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_ATTRIBUTEPOSTASSIGN :
      {
        if(c == '"')
        {
          sysParser->m_parserState = MXML_PS_ATTRIBUTEVALUE;
        }
        else if(!MXML_ISWHITESPACE(c))
        {
          sprintf(sysParser->m_errorString, "expecting '\"' line %d, col %d", sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_ATTRIBUTEVALUE :
      {
        assert(attribute);
        if(c == '"')
        {
          MXMLBufferAppend(&attribute->m_value, 0);
          sysParser->m_parserState = MXML_PS_STARTELEMENTENDNAME;
        }
        else if(c == '&')
        {
          /* handle escape */
          MXMLBufferReset(&sysParser->m_escapeData);
          sysParser->m_parserState = MXML_PS_ESCAPE;
          sysParser->m_escapeRestoreState = MXML_PS_ATTRIBUTEVALUE;
        }
        else
        {
          MXMLBufferAppend(&attribute->m_value, c);
        }
        break;
      }

      /* 
       * end element
       */

      case MXML_PS_ENDELEMENTNAMESTART :
      {
        MXMLBufferReset(&sysParser->m_name);
        if(MXML_ISNAMESTART(c))
        {
          MXMLBufferAppend(&sysParser->m_name, c);
          sysParser->m_parserState = MXML_PS_ENDELEMENTNAME;
        }
        else
        {
          sprintf(sysParser->m_errorString, "unexpected char '%c' line %d, col %d", c, sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }
      case MXML_PS_ENDELEMENTNAME :
      {
        if(MXML_ISNAMECHAR(c))
        {
          MXMLBufferAppend(&sysParser->m_name, c);
        }
        else
        {
          MXMLBufferAppend(&sysParser->m_name, 0);
          popChar = 0;

          /* make sure the end element matches the start */
          element = MXMLSysParserGetElement(sysParser);
          assert(element);
          if(element->m_nameHash != MXMLUtf8Hash(sysParser->m_name.m_buffer))
          {
            sprintf(sysParser->m_errorString, "end tag does not match start tag, line %d, col %d", sysParser->m_line, sysParser->m_column);
            return MXML_RESULT_ERROR;
          }
          sysParser->m_parserState = MXML_PS_ENDELEMENTEND;
        }
        break;
      }
      case MXML_PS_ENDELEMENTEND :
      {
        if(c == '>')
        {
          MXMLSysParserHandleEndElement(sysParser);
          MXMLBufferReset(&sysParser->m_characterData);
          if(MXMLSysParserPopElement(sysParser))
          {
            sysParser->m_parserState = MXML_PS_TEXT;
            break;
          }
          else
          {
            sysParser->m_parserState = MXML_PS_DOCSTART;
          }
        }
        else if(!MXML_ISWHITESPACE(c))
        {
          sprintf(sysParser->m_errorString, "unexpected char '%c' line %d, col %d", c, sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        break;
      }

      /*
       * cdata
       */

      case MXML_PS_CDATAA :
      {
        MXML_MATCH(c, 'C', "invalid CDATA", MXML_PS_CDATAB);
      }
      case MXML_PS_CDATAB :
      {
        MXML_MATCH(c, 'D', "invalid CDATA", MXML_PS_CDATAC);
      }
      case MXML_PS_CDATAC :
      {
        MXML_MATCH(c, 'A', "invalid CDATA", MXML_PS_CDATAD);
      }
      case MXML_PS_CDATAD :
      {
        MXML_MATCH(c, 'T', "invalid CDATA", MXML_PS_CDATAE);
      }
      case MXML_PS_CDATAE :
      {
        MXML_MATCH(c, 'A', "invalid CDATA", MXML_PS_CDATAF);
      }
      case MXML_PS_CDATAF :
      {
        MXML_MATCH(c, '[', "invalid CDATA", MXML_PS_CDATAG);
      }
      case MXML_PS_CDATAG :
      {
        if(c == ']')
        {
          sysParser->m_parserState = MXML_PS_CDATAH;
          break;
        }
        MXMLBufferAppend(&sysParser->m_characterData, c);
        break;
      }
      case MXML_PS_CDATAH :
      {
        if(c == ']')
        {
          sysParser->m_parserState = MXML_PS_CDATAI;
          break;
        }
        MXMLBufferAppend(&sysParser->m_characterData, ']');
        MXMLBufferAppend(&sysParser->m_characterData, c);
        sysParser->m_parserState = MXML_PS_CDATAG;
        break;
      }
      case MXML_PS_CDATAI :
      {
        if(c == '>')
        {
          MXMLSysParserHandleCharacterData(sysParser, MXML_CDT_CDATA);
          if(MXMLSysParserGetElement(sysParser))
          {
            MXMLBufferReset(&sysParser->m_characterData);
            sysParser->m_parserState = MXML_PS_TEXT;
          }
          else
          {
            /* cant have CDATA node at top level */
            sprintf(sysParser->m_errorString, "cant have CDATA at top level line %d, col %d", sysParser->m_line, sysParser->m_column);
            return MXML_RESULT_ERROR;
          }
          break;
        }
        MXMLBufferAppend(&sysParser->m_characterData, ']');
        MXMLBufferAppend(&sysParser->m_characterData, ']');
        MXMLBufferAppend(&sysParser->m_characterData, c);
        sysParser->m_parserState = MXML_PS_CDATAG;
        break;
      }

      /*
       * comment
       */

      case MXML_PS_COMMENTA :
      {
        if(c != '-')
        {
          sprintf(sysParser->m_errorString, "expecting '-' line %d, col %d", sysParser->m_line, sysParser->m_column);
          return MXML_RESULT_ERROR;
        }
        sysParser->m_parserState = MXML_PS_COMMENTB;
        break;
      }
      case MXML_PS_COMMENTB :
      {
        if(c == '-') 
          sysParser->m_parserState = MXML_PS_COMMENTC;
        else
          MXMLBufferAppend(&sysParser->m_characterData, c);
        break;
      }
      case MXML_PS_COMMENTC :
      {
        if(c == '-') 
          sysParser->m_parserState = MXML_PS_COMMENTD;
        else
        {
          MXMLBufferAppend(&sysParser->m_characterData, '-');
          MXMLBufferAppend(&sysParser->m_characterData, c);
          sysParser->m_parserState = MXML_PS_COMMENTB;
        }
        break;
      }
      case MXML_PS_COMMENTD :
      {
        if(c == '>') 
        {
          MXMLSysParserHandleCharacterData(sysParser, MXML_CDT_COMMENT);
          MXMLBufferReset(&sysParser->m_characterData);
          if(MXMLSysParserGetElement(sysParser))
          {
            sysParser->m_parserState = MXML_PS_TEXT;
          }
          else
          {
            sysParser->m_parserState = MXML_PS_DOCSTART;
          }
        }
        else
        {
          MXMLBufferAppend(&sysParser->m_characterData, '-');
          MXMLBufferAppend(&sysParser->m_characterData, '-');
          MXMLBufferAppend(&sysParser->m_characterData, c);
          sysParser->m_parserState = MXML_PS_COMMENTB;
        }
        break;
      }

      /*
       * declaration
       */

      case MXML_PS_DECLARATIONSTART :
      {
        if(c == '?')
        {
          sysParser->m_parserState = MXML_PS_DECLARATIONA;
        }
        else
        {
          MXMLBufferAppend(&sysParser->m_characterData, c);
        }
        break;
      }
      case MXML_PS_DECLARATIONA :
      {
        if(c == '>')
        {
          MXMLSysParserHandleCharacterData(sysParser, MXML_CDT_DECLARATION);
          MXMLBufferReset(&sysParser->m_characterData);
          if(MXMLSysParserGetElement(sysParser))
          {
            sysParser->m_parserState = MXML_PS_TEXT;
          }
          else
          {
            sysParser->m_parserState = MXML_PS_DOCSTART;
          }
        }
        else 
        {
          MXMLBufferAppend(&sysParser->m_characterData, '?');
          MXMLBufferAppend(&sysParser->m_characterData, c);
          sysParser->m_parserState = MXML_PS_DECLARATIONSTART;
        }
        break;
      }

      /*
       * error
       */

      default :
      {
        sprintf(sysParser->m_errorString, "internal error");
        return MXML_RESULT_ERROR;
      }
    }
  }

  return MXML_RESULT_ERROR;
}


const char * MXMLParserGetErrorString(MXMLParser * a_parser)
{
  MXMLSysParser * sysParser = (MXMLSysParser *) a_parser->m_sysData;
  return sysParser->m_errorString;
}



struct MXMLEncodeStringDesc
{
  MXMLBuffer m_string;       // Output for encoding
};


void* MXMLEscapeEncodeBegin(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext)
{
  MXMLEncodeStringDesc* newEncoder = new MXMLEncodeStringDesc;
  MXMLBufferInit(&newEncoder->m_string, a_Alloc, a_Free, a_memContext);
  return newEncoder;
}


void MXMLEscapeEncodeBuffer(void* a_encoder, const char* a_inBuffer, size_t a_inLength,
                                       const char*& a_outBuffer, size_t& a_outLength)
{
  MXMLEncodeStringDesc* encoder = (MXMLEncodeStringDesc*)a_encoder;
  assert(encoder);
    
  MXMLBufferReset(&encoder->m_string);
  
  for(size_t index=0; index < a_inLength; ++index)
  {
    const char curChar = a_inBuffer[index];
    if(curChar == '&')
    {
      MXMLBufferAppend(&encoder->m_string, s_entities[SYMBOL_AMP].m_entityFull);
    }
    else if(curChar == '<')
    {
      MXMLBufferAppend(&encoder->m_string, s_entities[SYMBOL_LT].m_entityFull);
    }
    else if(curChar == '>')
    {
      MXMLBufferAppend(&encoder->m_string, s_entities[SYMBOL_GT].m_entityFull);
    }
    else if(curChar == '\'')
    {
      MXMLBufferAppend(&encoder->m_string, s_entities[SYMBOL_APOS].m_entityFull);
    }
    else if(curChar == '\"')
    {
      MXMLBufferAppend(&encoder->m_string, s_entities[SYMBOL_QUOT].m_entityFull);
    }
    else if( (curChar < 32) && (curChar != '\n') && (curChar != '\r') && (curChar != '\t') ) // Encode non-whitespace invisible chars
    {
      char mixBuf[32];
#if 1 // Safer version if available
      _snprintf(mixBuf, 32, "&#x%02X;", (unsigned char)(curChar));
#else
      sprintf(mixBuf, "&#x%02X;", (unsigned char)(curChar));
#endif      
      MXMLBufferAppend(&encoder->m_string, mixBuf, (int)strlen(mixBuf));
    }    
    else
    {
      MXMLBufferAppend(&encoder->m_string, curChar);
    }
  }
  a_outBuffer = encoder->m_string.m_buffer;
  a_outLength = encoder->m_string.m_bufferSize;
}


void MXMLEscapeEncodeEnd(void* a_encoder)
{
  MXMLEncodeStringDesc* encoder = (MXMLEncodeStringDesc*)a_encoder;
  MXMLBufferDone(&encoder->m_string);
  delete encoder;
}
