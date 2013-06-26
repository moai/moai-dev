/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml dom

 ****************************************************************************/

// Added this to remove legacy code warnings
#pragma warning(disable : 4996) // Deprecated functions
#define _CRT_SECURE_NO_DEPRECATE // Allow old unsecure standard library functions

#include "mxmldom.h"
#include <string.h>
#include <new>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>


void MXMLNode::Dispose()
{
  if(m_parent)
  {
    m_parent->RemoveChild(this);
  }
  MXMLDocument * document = m_document;
  this->~MXMLNode();
  document->Free(this);
}


MXMLNode::MXMLNode(MXMLDocument * a_document)
{
  m_type = T_NODE;
  m_document = a_document;
  m_parent = NULL;
  m_nextSibling = NULL;
  m_prevSibling = NULL;
}


MXMLNode * MXMLNode::GetNextSibling(Type a_nodeType)
{
  for(MXMLNode * node = GetNextSibling(); node; node = node->GetNextSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


const MXMLNode * MXMLNode::GetNextSibling(Type a_nodeType) const
{
  for(const MXMLNode * node = GetNextSibling(); node; node = node->GetNextSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


  
MXMLNode * MXMLNode::GetPrevSibling(Type a_nodeType)
{
  for(MXMLNode * node = GetPrevSibling(); node; node = node->GetPrevSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


const MXMLNode * MXMLNode::GetPrevSibling(Type a_nodeType) const
{
  for(const MXMLNode * node = GetPrevSibling(); node; node = node->GetPrevSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


void MXMLTextBase::SetText(const char * a_text, size_t a_size)
{
  const char * text = m_text;
  m_text = NULL;
  if(a_text)
  {
    size_t size = (a_size) ? a_size : MXMLUtf8Size(a_text);
    char * text = (char *) m_document->Alloc(size + 1);
    memcpy(text, a_text, size);
    text[size] = '\0';
    m_text = text;
  }
  if(text)
  {
    m_document->Free((void *) text);
  }
}


void MXMLTextBase::SetTextFormatted(const char * a_format, ...)
{
  if( !a_format )
  {
    return;
  }

  va_list argList;
  va_start(argList, a_format);

  const int bufferLen = 1024;
  char buffer[bufferLen];

  // Print to temp string
  vsnprintf(buffer, bufferLen, a_format, argList);

  SetText(buffer);
  
  va_end(argList);
}


MXMLTextBase::MXMLTextBase(MXMLDocument * a_document) : MXMLNode(a_document)
{
  m_text = NULL;
}


MXMLTextBase::~MXMLTextBase()
{
  if(m_text)
  {
    m_document->Free((void *) m_text);
  }
}


bool MXMLDeclaration::Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const
{
  int result = 0;
  size_t size = 0;
  const char * open = "<?";
  const char * close = "?>";
  const char * text = GetText();

  size = strlen(open);
  result |= (a_stream.Write(open, size) != size);

  size = MXMLUtf8Size(text);
  result |= (a_stream.Write(text, size) != size);

  size = strlen(close);
  result |= (a_stream.Write(close, size) != size);

  return (result == 0);
}


MXMLDeclaration::MXMLDeclaration(MXMLDocument * a_document) : MXMLTextBase(a_document)
{
  m_type |= T_DECLARATION;
}


bool MXMLComment::Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const
{
  int result = 0;
  size_t size = 0;
  const char * open = "<!--";
  const char * close = "-->";
  const char * text = GetText();

  size = strlen(open);
  result |= (a_stream.Write(open, size) != size);

  size = MXMLUtf8Size(text);
  result |= (a_stream.Write(text, size) != size);

  size = strlen(close);
  result |= (a_stream.Write(close, size) != size);

  return (result == 0);
}


MXMLComment::MXMLComment(MXMLDocument * a_document) : MXMLTextBase(a_document)
{
  m_type |= T_COMMENT;
}


bool MXMLText::Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const
{
  int result = 0;
  size_t size = 0;
  const char * text = GetText();

  size = MXMLUtf8Size(text);
  MXMLEscapeEncodeBuffer(a_encoder, text, size, text, size);
  result |= (a_stream.Write(text, size) != size);

  return (result == 0);
}


MXMLText::MXMLText(MXMLDocument * a_document) : MXMLTextBase(a_document)
{
  m_type |= T_TEXT;
}


bool MXMLCDATA::Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const
{
  int result = 0;
  size_t size = 0;
  const char * open = "<![CDATA[";
  const char * close = "]]>";
  const char * text = GetText();

  size = strlen(open);
  result |= (a_stream.Write(open, size) != size);

  size = MXMLUtf8Size(text);
  result |= (a_stream.Write(text, size) != size);

  size = strlen(close);
  result |= (a_stream.Write(close, size) != size);

  return (result == 0);
}


MXMLCDATA::MXMLCDATA(MXMLDocument * a_document) : MXMLText(a_document)
{
  m_type |= T_CDATA;
}


const char * MXMLElement::Attributes::GetNamedItem(const char * a_name, const char * a_default) const
{
  Attribute * attribute = m_attributes;
  while(attribute)
  {
    if(strcmp(attribute->GetName(), a_name) == 0)
    {
      if(attribute->m_value)
      {
        return attribute->m_value;
      }
      break;
    }
    attribute = attribute->m_next;
  }
  return a_default;
}


void MXMLElement::Attributes::SetNamedItem(const char * a_name, const char * a_value)
{
  Attribute ** attribute = &m_attributes;
  while(*attribute)
  {
    if(strcmp((*attribute)->GetName(), a_name) == 0)
    {
      if((*attribute)->m_value)
      {
        m_document->Free((void *) (*attribute)->m_value);
        (*attribute)->m_value = 0;
      }
      break;
    }
    attribute = &((*attribute)->m_next);
  }

  if(*attribute == NULL)
  {
    *attribute = ::new(m_document->Alloc(sizeof(Attribute))) Attribute();
  }

  (*attribute)->m_name = m_document->GetUniqueString(a_name);
  size_t size = MXMLUtf8Size(a_value);
  char * value = (char *) m_document->Alloc(size + 1);
  memcpy(value, a_value, size + 1);
  (*attribute)->m_value = value;
}


static int MXMLVsscanf(const char *str, const char *format, va_list ap)
{
  // can this be implemented in a more portable way?

  // NOTE: Using size_t for int_ptr size to improve 64bit compatibility
  size_t p1 = va_arg(ap, size_t);
  size_t p2 = va_arg(ap, size_t);
  size_t p3 = va_arg(ap, size_t);
  size_t p4 = va_arg(ap, size_t);
  size_t p5 = va_arg(ap, size_t);
  size_t p6 = va_arg(ap, size_t);
  size_t p7 = va_arg(ap, size_t);

  return sscanf(str, format, p1, p2, p3, p4, p5, p6, p7);
} 


int MXMLElement::Attributes::ScanNamedItem(const char * a_name, const char * a_format, ...)
{
  int rc = 0;
  va_list args;
  const char * value = GetNamedItem(a_name);
  va_start(args, a_format);
  rc = MXMLVsscanf(value, a_format, args);
  va_end(args);
  return rc;
}


void MXMLElement::Attributes::FormatNamedItem(const char * a_name, const char * a_format, ...)
{
  va_list argList;
  va_start(argList, a_format);

  const size_t bufferSize = 1024;
  char buffer[bufferSize];

  vsnprintf(buffer, bufferSize, a_format, argList);

  SetNamedItem(a_name, buffer);
  
  va_end(argList);
}


void MXMLElement::Attributes::RemoveNamedItem(const char * a_name)
{
  Attribute ** attribute = &m_attributes;
  while(*attribute)
  {
    if(strcmp((*attribute)->GetName(), a_name) == 0)
    {
      Attribute * remove = *attribute;
      *attribute = (*attribute)->m_next;
      if(remove->m_value)
      {
        m_document->Free((void *) remove->m_value);
      }
      m_document->Free(remove);
      break;
    }
    attribute = &((*attribute)->m_next);
  }
}


MXMLElement::Attributes::Attributes(MXMLDocument * a_document)
{
  m_document = a_document;
  m_attributes = NULL;
}


MXMLElement::Attributes::~Attributes()
{
  // dispose attributes
  while(m_attributes)
  {
    Attribute * attribute = m_attributes;
    m_attributes = attribute->m_next;
    if(attribute->m_value)
    {
      m_document->Free((void *) attribute->m_value);
    }
    m_document->Free(attribute);
  }
}


void MXMLElement::SetName(const char * a_name)
{
  m_name = m_document->GetUniqueString(a_name);
}


bool MXMLElement::IsName(const char * a_name) const
{
  if(m_name)
  {
    return (strcmp(a_name, m_name) == 0);
  }
  return false;
}


const char * MXMLElement::GetText() const
{
  const MXMLText * firstText = NULL;
  bool usedBuffer = false;

  for(const MXMLNode * node = GetFirstChild(); node; node = node->GetNextSibling())
  {
    if(node->IsType(MXMLNode::T_TEXT))
    {
      const MXMLText * text = static_cast<const MXMLText *>(node);
      if(firstText == NULL)
      {
        firstText = text;
      }
      else
      {
        if(!usedBuffer)
        {
          m_document->ResetBuffer();
          m_document->AppendBuffer(firstText->GetText());
          usedBuffer = true;
        }
        m_document->AppendBuffer(text->GetText());
      }
    }
  }

  if(!firstText)
  {
    return "";
  }
  else if(!usedBuffer)
  {
    return firstText->GetText();
  }
  return m_document->GetBuffer();
}


int MXMLElement::CountChildElements() const
{
  int count = 0;
  const MXMLElement * node = GetFirstChildElement();
  while( node )
  {
    ++count;
    node = node->GetNextSiblingElement();
  }
  return count;
}


int MXMLElement::CountChildElements(const char * a_name) const
{
  int count = 0;
  const MXMLElement* node = GetFirstChildElement();
  while( node )
  {
    if( node->IsName(a_name) )
    {
      ++count;
    }
    node = node->GetNextSiblingElement();
  }
  return count;
}


MXMLElement * MXMLElement::FindChild(const char * a_name, int a_index)
{
  int curIndex = 0;
  for(MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
  {
    if( element->IsName(a_name) )
    {
      if( curIndex == a_index )
      {
        return element;
      }
      ++curIndex;
    }
  }
  return NULL;
}


const MXMLElement * MXMLElement::FindChild(const char * a_name, int a_index) const
{
  int curIndex = 0;
  for(const MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
  {
    if( element->IsName(a_name) )
    {
      if( curIndex == a_index )
      {
        return element;
      }
      ++curIndex;
    }
  }
  return NULL;
}


MXMLElement * MXMLElement::FindChild(const char * a_name)
{
  for(MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
    if(element->IsName(a_name))
      return element;
  return NULL;
}


const MXMLElement * MXMLElement::FindChild(const char * a_name) const
{
  for(const MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
    if(element->IsName(a_name))
      return element;
  return NULL;
}


MXMLElement * MXMLElement::GetFirstChildElement()
{
  for(MXMLNode * node = GetFirstChild(); node; node = node->GetNextSibling())
    if(node->IsType(MXMLNode::T_ELEMENT))
      return static_cast<MXMLElement *>(node);
  return NULL;
}


const MXMLElement * MXMLElement::GetFirstChildElement() const
{
  for(const MXMLNode * node = GetFirstChild(); node; node = node->GetNextSibling())
    if(node->IsType(MXMLNode::T_ELEMENT))
      return static_cast<const MXMLElement *>(node);
  return NULL;
}


MXMLElement * MXMLElement::GetNextSiblingElement()
{
  for(MXMLNode * node = GetNextSibling(); node; node = node->GetNextSibling())
    if(node->IsType(MXMLNode::T_ELEMENT))
      return static_cast<MXMLElement *>(node);
  return NULL;
}


const MXMLElement * MXMLElement::GetNextSiblingElement() const
{
  for(const MXMLNode * node = GetNextSibling(); node; node = node->GetNextSibling())
    if(node->IsType(MXMLNode::T_ELEMENT))
      return static_cast<const MXMLElement *>(node);
  return NULL;
}


MXMLNode * MXMLElement::GetFirstChild(MXMLNode::Type a_nodeType)
{
  for(MXMLNode * node = GetFirstChild(); node; node = node->GetNextSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


const MXMLNode * MXMLElement::GetFirstChild(MXMLNode::Type a_nodeType) const
{
  for(const MXMLNode * node = GetFirstChild(); node; node = node->GetNextSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}

  
MXMLNode * MXMLElement::GetLastChild(MXMLNode::Type a_nodeType)
{
  for(MXMLNode * node = GetLastChild(); node; node = node->GetPrevSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


const MXMLNode * MXMLElement::GetLastChild(MXMLNode::Type a_nodeType) const
{
  for(const MXMLNode * node = GetLastChild(); node; node = node->GetPrevSibling())
  {
    if( node->IsType( a_nodeType ) )
    {
      return node;
    }
  }
  return NULL;
}


bool MXMLElement::RemoveChild(MXMLNode * a_node)
{
  if(a_node->GetParent() == this)
  {
    if(a_node->m_prevSibling)
    {
      a_node->m_prevSibling->m_nextSibling = a_node->m_nextSibling;
    }
    if(a_node->m_nextSibling)
    {
      a_node->m_nextSibling->m_prevSibling = a_node->m_prevSibling;
    }
    if(m_firstChild == a_node)
    {
      m_firstChild = a_node->m_nextSibling;
    }
    if(m_lastChild == a_node)
    {
      m_lastChild = a_node->m_prevSibling;
    }

    a_node->m_nextSibling = NULL;
    a_node->m_prevSibling = NULL;
    a_node->m_parent = NULL;

    return true;
  }
  return false;
}


int MXMLElement::RemoveChildren()
{
  int count = 0;
  while( MXMLNode* childNode = GetFirstChild() )
  {
    childNode->Dispose();
    ++count;
  }
  return count;
}


bool MXMLElement::InsertAfter(MXMLNode * a_cursor, MXMLNode * a_node)
{
  if(a_cursor->GetParent() != this || a_cursor == a_node)
  {
    return false;
  }

  assert(!a_node->GetParent());

  a_node->m_prevSibling = a_cursor;
  a_node->m_nextSibling = a_cursor->m_nextSibling;
  if(a_node->m_nextSibling)
    a_node->m_nextSibling->m_prevSibling = a_node;
  a_cursor->m_nextSibling = a_node;
  if(m_lastChild == a_cursor)
    m_lastChild = a_node;
  a_node->m_parent = this;

  return true;
}


bool MXMLElement::InsertBefore(MXMLNode * a_cursor, MXMLNode * a_node)
{
  if(a_cursor->GetParent() != this || a_cursor == a_node)
  {
    return false;
  }

  assert(!a_node->GetParent());

  a_node->m_nextSibling = a_cursor;
  a_node->m_prevSibling = a_cursor->m_prevSibling;
  if(a_node->m_prevSibling)
    a_node->m_prevSibling->m_nextSibling = a_node;
  a_cursor->m_prevSibling = a_node;
  if(m_firstChild == a_cursor)
    m_firstChild = a_node;
  a_node->m_parent = this;

  return true;
}


void MXMLElement::InsertLast(MXMLNode * a_node)
{
  if(a_node->GetParent())
  {
    a_node->GetParent()->RemoveChild(a_node);
  }

  a_node->m_prevSibling = m_lastChild;
  a_node->m_nextSibling = NULL;
  if(a_node->m_prevSibling)
  {
    a_node->m_prevSibling->m_nextSibling = a_node;
  }
  else
  {
    m_firstChild = a_node;
  }
  m_lastChild = a_node;

  a_node->m_parent = this;
}


MXMLDeclaration * MXMLElement::AppendDeclaration(const char * a_declaration, size_t a_size)
{
  MXMLDeclaration * declaration = m_document->CreateDeclaration();
  if(a_declaration)
  {
    declaration->SetText(a_declaration, a_size);
  }
  InsertLast(declaration);
  return declaration;
}


MXMLComment * MXMLElement::AppendComment(const char * a_comment, size_t a_size)
{
  MXMLComment * comment = m_document->CreateComment();
  if(a_comment)
  {
    comment->SetText(a_comment, a_size);
  }
  InsertLast(comment);
  return comment;
}


MXMLText * MXMLElement::AppendText(const char * a_text, size_t a_size)
{
  MXMLText * text = m_document->CreateText();
  if(a_text)
  {
    text->SetText(a_text, a_size);
  }
  InsertLast(text);
  return text;
}


MXMLText * MXMLElement::AppendTextFormatted(const char * a_format, ...)
{
  if( !a_format )
  {
    return MXMLElement::AppendText(NULL);
  }

  va_list argList;
  va_start(argList, a_format);

  const int bufferLen = 1024;
  char buffer[bufferLen];

  vsnprintf(buffer, bufferLen, a_format, argList);
 
  va_end(argList);

  return MXMLElement::AppendText(buffer);
}


MXMLCDATA * MXMLElement::AppendCDATA(const char * a_cdata, size_t a_size)
{
  MXMLCDATA * cdata = m_document->CreateCDATA();
  if(a_cdata)
  {
    cdata->SetText(a_cdata, a_size);
  }
  InsertLast(cdata);
  return cdata;
}


MXMLElement * MXMLElement::AppendElement(const char * a_name)
{
  MXMLElement * element = m_document->CreateElement();
  if(a_name)
  {
    element->SetName(a_name);
  }
  InsertLast(element);
  return element;
}


void MXMLElement::Unformat(bool a_trim)
{
  // first step is to trim all whitespace text
  for(MXMLNode * child = GetFirstChild(); child;)
  {
    MXMLNode * nextChild = child->GetNextSibling();
    if( child->IsType(MXMLNode::T_TEXT) )
    {
      MXMLText * text = static_cast<MXMLText *>(child);
      const char * data = text->GetText();
      while(*data)
      {
        if(!(*data == ' ' || *data == '\r' || *data == '\n' || *data == '\t'))
        {
          break;
        }
        ++data;
      }

      if( *data )
      {
        if( a_trim ) // Trim whitespace around text
        {
          // find the last valid character
          const char * end = data;
          const char * start = data;
          while(*data)
          {
            if(!(*data == ' ' || *data == '\r' || *data == '\n' || *data == '\t'))
            {
              end = data;
            }
            ++data;
          }
          ++end;
          if(*end || start != text->GetText())
          {
            text->SetText(start, end - start);
          }
        }
      }
      else
      {
        child->Dispose();
      }
    }
    child = nextChild;
  }

  // format children
  for(MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
  {
    element->Unformat(a_trim);
  }
}


void MXMLElement::Format(bool a_trim, int a_tabs)
{
  // first step is to trim all whitespace text
  for(MXMLNode * child = GetFirstChild(); child;)
  {
    MXMLNode * nextChild = child->GetNextSibling();
    if(child->IsType(MXMLNode::T_TEXT))
    {
      MXMLText * text = static_cast<MXMLText *>(child);
      const char * data = text->GetText();
      while(*data)
      {
        if(!(*data == ' ' || *data == '\r' || *data == '\n' || *data == '\t'))
        {
          break;
        }
        ++data;
      }

      if(*data)
      {
        if( a_trim )
        {
          // find the last valid character
          const char * end = data;
          const char * start = data;
          while(*data)
          {
            if(!(*data == ' ' || *data == '\r' || *data == '\n' || *data == '\t'))
            {
              end = data;
            }
            ++data;
          }
          ++end;
          if(*end || start != text->GetText())
          {
            text->SetText(start, end - start);
          }
        }
      }
      else
      {
        child->Dispose();
      }
    }
    child = nextChild;
  }

  // add formatting
  if(GetFirstChild())
  {
    m_document->ResetBuffer();
    const char * cr = "\r\n";
    m_document->AppendBuffer(cr, strlen(cr));
    for(int i = 1; i < a_tabs; ++i)
    {
      m_document->AppendBuffer("  ", 2);
    }

    // we dont format if there is a single text node
    if(GetFirstChild() != GetLastChild() || !(GetFirstChild()->IsType(MXMLNode::T_TEXT) && !GetFirstChild()->IsType(MXMLNode::T_CDATA)))
    {
      if(a_tabs > 0)
      {
        MXMLText * closingText = m_document->CreateText();
        closingText->SetText(m_document->GetBuffer());
        m_document->AppendBuffer("  ", 2);
  
        // format our children
        for(MXMLNode * child = GetFirstChild(); child;)
        {
          MXMLNode * nextChild = child->GetNextSibling();
  
          // append CR + tabs before the child
          MXMLText * text = m_document->CreateText();
          text->SetText(m_document->GetBuffer());
          InsertBefore(child, text);
          child = nextChild;
        }
  
        InsertLast(closingText);
      }
      else
      {
        // format our children
        for(MXMLNode * child = GetFirstChild(); child;)
        {
          MXMLNode * nextChild = child->GetNextSibling();
  
          // append CR after the child
          MXMLText * text = m_document->CreateText();
          text->SetText(m_document->GetBuffer());
          InsertAfter(child, text);
          child = nextChild;
        }
      }

      // format children
      for(MXMLElement * element = GetFirstChildElement(); element; element = element->GetNextSiblingElement())
      {
        element->Format(a_trim, a_tabs + 1);
      }
    }
  }
}


bool MXMLElement::Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const
{
  const char INDENT_SPACE_RUN_POW2 = 8; // Indent spaces buffer size, must be power of 2
  static char indentSpaces[INDENT_SPACE_RUN_POW2] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20}; // space X 8
    
  int result = 0;
  size_t size = 0;
  const char * openA = "<";
  const char * openB = "</";
  const char * closeA = "/>";
  const char * closeB = ">";
  const char * space = " ";
  const char * assign = "=\"";
  const char * closeAttr = "\"";
  const char * name = GetName();

  size = strlen(openA);
  result |= (a_stream.Write(openA, size) != size);

  size = MXMLUtf8Size(name);
  result |= (a_stream.Write(name, size) != size);

  // If last node was just text, and contained just white space
  // count the spaces from end of string back to last CR/NL, and that is how far tabbed in we are.
  int indentSpaceCount = 0;
  if( a_columnAttributes )
  {
    // Need to find the last whitespace text node.  It should be the previous sibling.
    MXMLNode* wsNode = NULL;
    if( this->m_prevSibling && (this->m_prevSibling->m_type == (T_TEXT | T_NODE)) )
    {
      wsNode = this->m_prevSibling;
    }
    if( wsNode ) // If last node was just a text node
    {
      const char* text = wsNode->GetText();
      int testLen = (int)strlen(text);
      const char* curByte = text + testLen - 1;
      while( curByte > text )
      {
        if( *curByte == ' ' )
        {
          ++indentSpaceCount;
        }
        else if( *curByte == '\t' )
        {
          indentSpaceCount += 2;
        }
        else
        {
          break;
        }
        --curByte;
      }
      #if 1
      // Paranoid check that this string was just white space
      while( curByte > text )
      {
        if( *curByte != ' ' && *curByte != '\r' && *curByte != '\n' && *curByte != '\t' )
        {
          indentSpaceCount = 0;
          break;
        }
        --curByte;
      } 
      #endif 
      if( indentSpaceCount > 0 )
      {
        indentSpaceCount += 2; // +2 for indent relative to owning node
      }
    }
  }

  // write attributes
  const Attributes::Attribute * attribute = GetAttributes().GetFirstAttribute();
  while(attribute)
  {
    if( a_columnAttributes && (indentSpaceCount > 0) ) // Column style 
    {
      // New line
      const char * NL = "\n";
      size = strlen(NL);
      result |= (a_stream.Write(NL, size) != size);
      // Indent
      
      // Write runs of spaces and remaining spaces using smaller space buffer
      int numRuns = indentSpaceCount / INDENT_SPACE_RUN_POW2; // NOTE: Compiler will convert to shift as int pow2
      int remainingSpaces = indentSpaceCount & (INDENT_SPACE_RUN_POW2-1); // NOTE: Integer remainder from pow2
      while( numRuns-- )
      {
        result |= (a_stream.Write(indentSpaces, INDENT_SPACE_RUN_POW2) != INDENT_SPACE_RUN_POW2);
      }
      if( remainingSpaces )
      {
        result |= (a_stream.Write(indentSpaces, remainingSpaces) != remainingSpaces);
      }
    }
    else // Row style
    {
      size = strlen(space);
      result |= (a_stream.Write(space, size) != size);
    }    

    size = MXMLUtf8Size(attribute->GetName());
    result |= (a_stream.Write(attribute->GetName(), size) != size);

    size = strlen(assign);
    result |= (a_stream.Write(assign, size) != size);

    const char* text = attribute->GetValue();
    size = MXMLUtf8Size(attribute->GetValue());
    MXMLEscapeEncodeBuffer(a_encoder, text, size, text, size);
    result |= (a_stream.Write(text, size) != size);

    size = strlen(closeAttr);
    result |= (a_stream.Write(closeAttr, size) != size);
    
    attribute = attribute->GetNextAttribute();
  }

  // if no children, write shorthand element
  if(GetFirstChild() == NULL)
  {
    size = strlen(closeA);
    result |= (a_stream.Write(closeA, size) != size);
  }
  else
  {
    size = strlen(closeB);
    result |= (a_stream.Write(closeB, size) != size);

    // write children
    for(const MXMLNode * child = GetFirstChild(); child != NULL; child = child->GetNextSibling())
    {
      if(!child->Write(a_stream, a_columnAttributes, a_encoder))
      {
        result |= 1;
        break;
      }
    }

    // write name close
    size = strlen(openB);
    result |= (a_stream.Write(openB, size) != size);

    size = MXMLUtf8Size(name);
    result |= (a_stream.Write(name, size) != size);

    size = strlen(closeB);
    result |= (a_stream.Write(closeB, size) != size);
  }

  return (result == 0);
}


MXMLElement::MXMLElement(MXMLDocument * a_document) : 
  MXMLNode(a_document),
  m_attributes(a_document)
{
  m_type |= T_ELEMENT;
  m_name = NULL;
  m_firstChild = NULL;
  m_lastChild = NULL;
}


MXMLElement::~MXMLElement()
{
  while(GetFirstChild())
  {
    GetFirstChild()->Dispose();
  }
}


MXMLDocument::MXMLDocument()
{
  Init(&MXMLDocument::DefaultAlloc, &MXMLDocument::DefaultFree, NULL, true);
}


MXMLDocument::MXMLDocument(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext, bool a_createRoot)
{
  Init(a_Alloc, a_Free, a_memContext, a_createRoot);
}


MXMLDocument::~MXMLDocument()
{
  Destroy();
}


void MXMLDocument::Init(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext, bool a_createRoot)
{
  m_uniques = NULL;
  m_sizeUniques = 233;

  m_buffer = NULL;
  m_bufferSize = 0;
  m_maxBufferSize = 0;

  m_Alloc = a_Alloc;
  m_Free = a_Free;
  m_memContext = a_memContext;

  m_document = NULL;
  if( a_createRoot )
  {
    // create a document element
    m_document = CreateElement();
  }
}


void MXMLDocument::Destroy()
{
  // dispose root
  if( m_document )
  {
    m_document->Dispose();
    m_document = NULL;
  }

  // clear uniques
  if( m_uniques )
  {
    UniqueNode * node;
    for(int i = 0; i < m_sizeUniques; ++i)
    {
      while(m_uniques[i])
      {
        node = m_uniques[i];
        m_uniques[i] = m_uniques[i]->m_next;
        Free((void *) node->m_string);
        Free(node);
      }
    }
    Free(m_uniques);
    m_uniques = NULL;
  }

  // free buffer
  if( m_buffer )
  {
    Free(m_buffer);
    m_buffer = NULL;
  }
}


void* MXMLDocument::DefaultAlloc(size_t a_size, void* a_context)
{
  return malloc(a_size);
}


void MXMLDocument::DefaultFree(void* a_mem, void* a_context)
{
  free(a_mem);
}


MXMLDeclaration * MXMLDocument::CreateDeclaration()
{
  return ::new(Alloc(sizeof(MXMLDeclaration))) MXMLDeclaration(this);
}


MXMLComment * MXMLDocument::CreateComment()
{
  return ::new(Alloc(sizeof(MXMLComment))) MXMLComment(this);
}


MXMLText * MXMLDocument::CreateText()
{
  return ::new(Alloc(sizeof(MXMLText))) MXMLText(this);
}


MXMLCDATA * MXMLDocument::CreateCDATA()
{
  return ::new(Alloc(sizeof(MXMLCDATA))) MXMLCDATA(this);
}


MXMLElement * MXMLDocument::CreateElement()
{
  return ::new(Alloc(sizeof(MXMLElement))) MXMLElement(this);
}


void MXMLDocument::ResetBuffer()
{
  m_bufferSize = 1;
  if(m_buffer == NULL)
  {
    m_maxBufferSize = 1024;
    m_buffer = (char *) Alloc(sizeof(char) * m_maxBufferSize);
  }
  m_buffer[0] = '\0';
}


void MXMLDocument::AppendBuffer(const char * a_text, size_t a_size)
{
  size_t size = (a_size) ? a_size : MXMLUtf8Size(a_text);
  size_t newSize = m_bufferSize + size;
  if(newSize >= m_maxBufferSize)
  {
    --newSize;

    newSize |= newSize >> 16;
    newSize |= newSize >> 8;
    newSize |= newSize >> 4;
    newSize |= newSize >> 2;
    newSize |= newSize >> 1;

    ++newSize;

    m_maxBufferSize = newSize;
    char * buffer = (char *) Alloc(sizeof(char) * m_maxBufferSize);
    memcpy(buffer, m_buffer, m_bufferSize);
    Free(m_buffer);
    m_buffer = buffer;
  }

  memcpy(m_buffer + m_bufferSize - 1, a_text, size);
  m_bufferSize += size;
  m_buffer[m_bufferSize - 1] = '\0';
}


const char * MXMLDocument::GetBuffer() const
{
  if(m_buffer)
  {
    return m_buffer;
  }
  return "";
}


struct MXMLUserData
{
  MXMLDocument * m_document;
  MXMLElement * m_root;
  MXMLElement * m_element;
};


static void MXMLStartElement(void * a_userData, const char * a_name, const char ** a_attributes)
{
  MXMLUserData * userData = (MXMLUserData *) a_userData;

  MXMLElement * element = userData->m_element->AppendElement(a_name);

  for(int i = 0; a_attributes[i]; i += 2)
  {
    element->GetAttributes().SetNamedItem(a_attributes[i], a_attributes[i + 1]);
  }

  userData->m_element = element;
}


static void MXMLEndElement(void * a_userData, const char * a_name)
{
  a_name;
  MXMLUserData * userData = (MXMLUserData *) a_userData;
  if(userData->m_element->GetParent())
  {
    userData->m_element = userData->m_element->GetParent();
  }
}


static void MXMLCharacterData(void * a_userData, const char * a_data, size_t a_length, MXMLCharacterDataType a_type)
{
  MXMLUserData * userData = (MXMLUserData *) a_userData;
  switch(a_type)
  {
    case MXML_CDT_DECLARATION :
      userData->m_element->AppendDeclaration(a_data, a_length);
      break;
    case MXML_CDT_COMMENT :
      userData->m_element->AppendComment(a_data, a_length);
      break;
    case MXML_CDT_TEXT :
      userData->m_element->AppendText(a_data, a_length);
      break;
    case MXML_CDT_CDATA :
      userData->m_element->AppendCDATA(a_data, a_length);
      break;
    default:
      break;
  }
}


bool MXMLDocument::Load(MXMLStream &a_stream, size_t a_bufferSize)
{
  unsigned char * buffer = (unsigned char *) Alloc(a_bufferSize);
  MXMLParser * parser = MXMLParserCreate(m_Alloc, m_Free, m_memContext);
  int done;
  
  MXMLUserData userData;
  userData.m_document = this;
  userData.m_root = CreateElement();
  userData.m_element = userData.m_root;

  parser->m_userData = &userData;
  parser->m_StartElementCallback = MXMLStartElement;
  parser->m_EndElementCallback = MXMLEndElement;
  parser->m_CharacterDataCallback = MXMLCharacterData;

  do 
  {
    size_t len = a_stream.Read(buffer, a_bufferSize);
    done = len < a_bufferSize;
    if (MXMLParserParse(parser, buffer, len, done) == MXML_RESULT_ERROR) 
    {
      userData.m_root->Dispose();

      ResetBuffer();
      AppendBuffer(MXMLParserGetErrorString(parser)); // Store error message in document buffer 

      // NOTE: Caller can retrieve error message from the document buffer
      //LOG(( "XML parse error: %s", this->m_buffer )); // Log error here while error string buffer is still valid
      
      MXMLParserDestroy(parser);
      Free(buffer);

      return false;
    }
  } while (!done);

  m_document->Dispose();
  m_document = userData.m_root;

  MXMLParserDestroy(parser);
  Free(buffer);

  return true;
}


bool MXMLDocument::Save(MXMLStream &a_stream, bool a_columnAttributes) const
{
  void* encoder = MXMLEscapeEncodeBegin(m_Alloc, m_Free, m_memContext);

  for(MXMLNode * node = m_document->GetFirstChild(); node; node = node->GetNextSibling())
  {
    node->Write(a_stream, a_columnAttributes, encoder);
  }
  
  MXMLEscapeEncodeEnd(encoder);
  
  return true;
}


const char * MXMLDocument::GetUniqueString(const char * a_string)
{
  // init the table if not yet initialized
  if(m_uniques == NULL)
  {
    m_uniques = (UniqueNode **) Alloc(sizeof(UniqueNode *) * m_sizeUniques);
    for(int i = 0; i < m_sizeUniques; ++i)
    {
      m_uniques[i] = NULL;
    }
  }

  // get hash slot
  unsigned int slot = MXMLUtf8Hash(a_string) % m_sizeUniques;

  // find insert string
  UniqueNode ** node = &m_uniques[slot];
  while(*node)
  {
    int compare = strcmp((*node)->m_string, a_string);
    if(compare == 0)
    {
      return (*node)->m_string;
    }
    else if(compare > 0)
    {
      break;
    }
    node = &((*node)->m_next);
  }

  UniqueNode * newNode = (UniqueNode *) Alloc(sizeof(UniqueNode));
  size_t size = MXMLUtf8Size(a_string);
  char * string = (char * ) Alloc(size + 1);
  memcpy(string, a_string, size + 1);
  newNode->m_string = string;
  newNode->m_next = *node;
  *node = newNode;

  return newNode->m_string;
}



void MXMLTextBase::CopyInternal(const MXMLNode* a_other)
{
  SetText( a_other->GetText() );
}


MXMLNode* MXMLDocument::AllocNodeCopyInternal(const MXMLNode* a_other)
{
  assert(a_other);
  
  if( a_other->m_type & MXMLNode::T_DECLARATION )
  {
    return CreateDeclaration();
  }
  else if( a_other->m_type & MXMLNode::T_COMMENT )
  {
    return CreateComment();
  }
  else if( a_other->m_type & MXMLNode::T_CDATA )
  {
    return CreateCDATA();
  }
  else if( a_other->m_type & MXMLNode::T_TEXT )
  {
    return CreateText();
  }
  else if( a_other->m_type & MXMLNode::T_ELEMENT )
  {
    return CreateElement();
  }

  assert(!"Unknown XML Node type");
  return NULL;
}


void MXMLElement::Attributes::CopyInternal(const Attributes& a_other)
{
  Attribute** newAtt = &m_attributes;
  const Attribute* curAtt = a_other.GetFirstAttribute();
  while( curAtt )
  {
    // Add new element to end of attribute list
    *newAtt = ::new(m_document->Alloc(sizeof(Attribute))) Attribute();

    // Copy name and value
    (*newAtt)->m_name = m_document->GetUniqueString(curAtt->m_name);
    size_t size = MXMLUtf8Size(curAtt->m_value);
    char * value = (char *) m_document->Alloc(size + 1);
    memcpy(value, curAtt->m_value, size + 1);
    (*newAtt)->m_value = value;

    curAtt = curAtt->m_next;
    newAtt = &((*newAtt)->m_next);
  }
}


void MXMLElement::CopyInternal(const MXMLNode* a_other)
{
  const MXMLElement* other = static_cast<const MXMLElement*>(a_other);

  // Set contents
  SetName(other->GetName());

  // Copy attributes
  m_attributes.CopyInternal( other->GetAttributes() );
  
  // Copy children
  const MXMLNode* curChild = other->GetFirstChild();
  while( curChild )
  {
    MXMLNode* newChild = m_document->AllocNodeCopyInternal(curChild);

    InsertLast(newChild);
    newChild->CopyInternal(curChild);
    
    curChild = curChild->GetNextSibling();
  }
}


void MXMLDocument::CopyFrom(const MXMLDocument& a_other)
{
  // Clear existing contents
  if( m_document )
  {
    m_document->Dispose();
  }

  // Copy nodes recursively
  m_document = CreateElement();
  m_document->CopyInternal(a_other.m_document);
}

