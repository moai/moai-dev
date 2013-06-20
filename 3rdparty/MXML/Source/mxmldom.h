/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml dom

 ****************************************************************************/

#ifndef _MXMLDOM_H_
#define _MXMLDOM_H_

#include <stdlib.h>
#include "mxmlutf8.h"
#include "mxml.h"


/**
 * \addtogroup MXMLDOM Micro XML Document Object Model
 * \ingroup MXML
 *
 * The Micro XML DOM provides a c++ Document Object to make working with XML documents easy.  The
 * Document supports loading and saving XML documents from a stream interface, and has memory override
 * hooks for the Document memory and parser memory.
 */

class MXMLDocument;
class MXMLElement;
class MXMLStream;

/// \ingroup MXMLDOM
/// \class MXMLNode
/// \brief abstract base class for XML Document nodes
///
/// Nodes can only be constructed by a MXMLDocument.  Call the Dispose method on a node
/// to delete it.
class MXMLNode
{
public:

  enum Type
  {
    T_NODE        = (1 << 0),
    T_DECLARATION = (1 << 1),
    T_COMMENT     = (1 << 2),
    T_TEXT        = (1 << 3),
    T_CDATA       = (1 << 4),
    T_ELEMENT     = (1 << 5),
  };

  /// \brief Delete this node and its children
  virtual void Dispose();

  /// \brief Get the owning document
  inline MXMLDocument * GetDocument() { return m_document; }
  inline const MXMLDocument * GetDocument() const { return m_document; }
  
  /// \brief Get the parent MXMLElement
  inline MXMLElement * GetParent() { return m_parent; }
  inline const MXMLElement * GetParent() const { return m_parent; }
  
  /// \brief Get the next sibling node
  inline MXMLNode * GetNextSibling() { return m_nextSibling; }
  inline const MXMLNode * GetNextSibling() const { return m_nextSibling; }
  MXMLNode * GetNextSibling(Type a_nodeType);
  const MXMLNode * GetNextSibling(Type a_nodeType) const;
  
  /// \brief Get the previous sibling node
  inline MXMLNode * GetPrevSibling() { return m_prevSibling; }
  inline const MXMLNode * GetPrevSibling()const { return m_prevSibling; }
  MXMLNode * GetPrevSibling(Type a_nodeType);
  const MXMLNode * GetPrevSibling(Type a_nodeType) const;
  
  /// \brief test to determine a node type to allow static cast to derived types
  inline bool IsType(Type a_type) const { return (m_type & a_type) == a_type; }

  /// \brief GetText will return text (UTF8) depending on the derived node type
  virtual const char * GetText() const { return NULL; }

  /// Copy node, for internal use only
  virtual void CopyInternal(const MXMLNode* a_other) = 0;

protected:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const = 0;

  MXMLNode(MXMLDocument * a_document);
  virtual ~MXMLNode() {}

  int m_type;
  MXMLDocument * m_document;
  MXMLElement * m_parent;
  MXMLNode * m_nextSibling;
  MXMLNode * m_prevSibling;

  friend class MXMLElement;
  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLTextBase
/// \brief Base class for nodes with text, abstract class
class MXMLTextBase : public MXMLNode
{
public:

  /// \brief Set the nodes text
  /// \param a_text (UTF8) is the nodes text
  /// \param a_size is the byte size of the text not including null character, or 0 for unknown
  void SetText(const char * a_text, size_t a_size = 0);
  /// \brief set text for this node similar to printf
  void SetTextFormatted(const char * a_format, ...);
  virtual const char * GetText() const { return (m_text) ? m_text : ""; }

  /// Copy node, for internal use only
  virtual void CopyInternal(const MXMLNode* a_other);

protected:

  MXMLTextBase(MXMLDocument * a_document);
  virtual ~MXMLTextBase();

  const char * m_text;
};


/// \ingroup MXMLDOM
/// \class MXMLDeclaration
/// \brief Node representing XML declaration
///
/// An XML declaration
/// \verbatim
/// <? contents not parsed by MXML ?>
/// \endverbatim
class MXMLDeclaration : public MXMLTextBase
{
private:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const;

  MXMLDeclaration(MXMLDocument * a_document);

  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLComment
/// \brief Node representing XML comment
///
/// An XML comment block 
/// \verbatim
/// <!-- COMMENT -->
/// \endverbatim
class MXMLComment : public MXMLTextBase
{
private:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const;

  MXMLComment(MXMLDocument * a_document);

  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLText
/// \brief Node representing XML text
///
/// A text block 
/// \verbatim
/// <Elem>TEXT</Elem>
/// \endverbatim
class MXMLText : public MXMLTextBase
{
protected:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const;

  MXMLText(MXMLDocument * a_document);

  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLCDATA
/// \brief Node representing XML CDATA
///
/// A CDATA block
/// \verbatim
/// <Elem><![CDATA[XXXX]]></Elem>
/// \endverbatim
class MXMLCDATA : public MXMLText
{
private:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const;

  MXMLCDATA(MXMLDocument * a_document);

  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLElement
/// \brief Node representing an XML element
///
/// XML elements contain child nodes.  They are named, and have child attributes.
class MXMLElement : public MXMLNode
{
public:

  /// \ingroup MXMLDOM
  /// \class Attributes
  /// \brief Attributes is a list of name value pair Attribute objects
  ///
  /// All Attribute modification is done via the Attributes class.  Attribute objects are
  /// read only.  Each MXMLElement has 1 Attributes object.
  class Attributes
  {
  public:

    /// \ingroup MXMLDOM
    /// \class Attribute
    /// \brief Attribute is a read only name value pair
    class Attribute
    {
    public:

      /// \brief Get the attribute name (UTF8)
      inline const char * GetName() const { return m_name; }

      /// \brief Get the attribute value (UTF8)
      inline const char * GetValue() const { return m_value; }

      /// \brief Get the next attribute in the owning Attributes object
      inline const Attribute * GetNextAttribute() const { return m_next; }

    private:

      Attribute() { m_name = NULL; m_value = NULL; m_next = NULL; }

      const char * m_name;
      const char * m_value;
      Attribute * m_next;

      friend class Attributes;
    };

    /// \brief Get the attribute value (UTF8) given name (UTF8)
    const char * GetNamedItem(const char * a_name, const char * a_default = "") const;

    /// \brief Set or Reset attribute by name value pair
    void SetNamedItem(const char * a_name, const char * a_value);

    /// \brief Scan named item
    int ScanNamedItem(const char * a_name, const char * a_format, ...);

    /// \brief Format named item
    void FormatNamedItem(const char * a_name, const char * a_format, ...);

    /// \brief Remove a named item 
    void RemoveNamedItem(const char * a_name);

    /// \brief Get the first attribute for iteration
    const Attribute * GetFirstAttribute() const { return m_attributes; }

    /// \brief get next attribute
    const Attribute * GetNextAttribute(const Attribute * a_attribute) const
    {
      return (m_attributes->GetNextAttribute()); 
    }

    /// Copy attributes, for internal use only
    void CopyInternal(const Attributes& a_other);

  private:

    Attributes(MXMLDocument * a_document);
    ~Attributes();

    Attribute * m_attributes;
    MXMLDocument * m_document;

    friend class MXMLElement;
  };

  /// \brief Get the elements attributes
  inline Attributes &GetAttributes() { return m_attributes; }
  inline const Attributes &GetAttributes() const { return m_attributes; }

  /// \brief Set the element name (UTF8)
  void SetName(const char * a_name);
  /// \brief Get the element name (UTF8)
  inline const char * GetName() const { return (m_name) ? m_name : ""; }
  /// \brief is the element named a_name (UTF8)
  bool IsName(const char * a_name) const;

  /// \brief Get all child text derived nodes appended
  ///
  /// This method uses the owning documents buffer to append child text nodes.
  virtual const char * GetText() const;

  /// \brief get the number of child element nodes
  int CountChildElements() const;
  /// \brief get the number of child elements nodes of this name
  int CountChildElements(const char * a_name) const;

  /// \brief Find the first named child element
  MXMLElement * FindChild(const char * a_name);
  const MXMLElement * FindChild(const char * a_name) const;
  
  /// \brief Find the nth named child element
  const MXMLElement * FindChild(const char * a_name, int a_index) const;
  MXMLElement * FindChild(const char * a_name, int a_index);

  /// \brief Get the first child node
  inline MXMLNode * GetFirstChild() { return m_firstChild; }
  inline const MXMLNode * GetFirstChild() const { return m_firstChild; }
  MXMLNode * GetFirstChild(MXMLNode::Type a_nodeType);
  const MXMLNode * GetFirstChild(MXMLNode::Type a_nodeType) const;
  
  /// \brief Get the last child node
  inline MXMLNode * GetLastChild() { return m_lastChild; }
  inline const MXMLNode * GetLastChild() const { return m_lastChild; }
  MXMLNode * GetLastChild(MXMLNode::Type a_nodeType);
  const MXMLNode * GetLastChild(MXMLNode::Type a_nodeType) const;

  /// \brief Get the first child element
  MXMLElement * GetFirstChildElement();
  const MXMLElement * GetFirstChildElement() const;

  /// \brief Get the next sibling element
  MXMLElement * GetNextSiblingElement();
  const MXMLElement * GetNextSiblingElement() const;

  /// \brief Remove a child node
  /// Once a node is removed, it must be either re-inserted or manually disposed
  bool RemoveChild(MXMLNode * a_node);
  /// \brief Remove all child nodes.
  /// Returns the number of immediate children removed
  int RemoveChildren();

  /// \brief Insert a node after the cursor
  bool InsertAfter(MXMLNode * a_cursor, MXMLNode * a_node);
  /// \brief Insert a node before the cursor
  bool InsertBefore(MXMLNode * a_cursor, MXMLNode * a_node);
  /// \brief Insert a node after the last child node
  void InsertLast(MXMLNode * a_node);

  /// \brief Create and append an MXMLDeclaration node
  MXMLDeclaration * AppendDeclaration(const char * a_declaration = NULL, size_t a_size = 0);
  /// \brief Create and append an MXMLComment node
  MXMLComment * AppendComment(const char * a_comment = NULL, size_t a_size = 0);
  /// \brief Create and append an MXMLText node
  MXMLText * AppendText(const char * a_text = NULL, size_t a_size = 0);
  /// \brief Create and append an MXMLText node using format
  MXMLText * AppendTextFormatted(const char * a_format, ...);
  /// \brief Create and append an MXMLCDATA node
  MXMLCDATA * AppendCDATA(const char * a_cdata = NULL, size_t a_size = 0);
  /// \brief Create and append an MXMLElement node
  MXMLElement * AppendElement(const char * a_name = NULL);

  /// \brief Reformat the element with white space
  /// Format will trim all current document whitespace and re format the document with 
  /// the said number of tabs (tabs == 2 spaces). Optionaly trims whitespace around text, full whitespace is always removed.
  virtual void Format(bool a_trim = true, int a_tabs = 0);
  
  /// Simply strips whitespace and empty TextNodes from the document.  Optionaly trims whitespace around text, full whitespace is always removed.
  virtual void Unformat(bool a_trim = true);

  /// Copy node, for internal use only
  virtual void CopyInternal(const MXMLNode* a_other);

private:

  virtual bool Write(MXMLStream &a_stream, bool a_columnAttributes, void* a_encoder) const;

  MXMLElement(MXMLDocument * a_document);
  virtual ~MXMLElement();

  const char * m_name;

  Attributes m_attributes;

  MXMLNode * m_firstChild;
  MXMLNode * m_lastChild;
  friend class MXMLDocument;
};


/// \ingroup MXMLDOM
/// \class MXMLStream
/// \brief Stream interface 
///
/// MXMLStream derived objects may be used with MXMLDocument to load and save XML documents.
class MXMLStream
{
public:
  virtual ~MXMLStream() {}

  /// \brief Read from the stream
  /// \param a_buffer is the buffer to read into
  /// \param a_bytes is the number of bytes to read
  /// \return the number of bytes read from the stream into a_buffer
  virtual size_t Read(void * a_buffer, size_t a_bytes) = 0;

  /// \brief Write to the stream
  /// \param a_buffer is the buffer to write from
  /// \param a_bytes is the number of bytes to write
  /// \return the number of bytes written to the stream from a_buffer
  virtual size_t Write(const void * a_buffer, size_t a_bytes) = 0;
};


/// \ingroup MXMLDOM
/// \class MXMLDocument
/// \brief XML Document
///
/// MXMLDocument is used to load and save XML documents into a MXMLElement tree.
class MXMLDocument
{
public:

  MXMLDocument();
  MXMLDocument(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext = NULL, bool a_createRoot = true);
  virtual ~MXMLDocument();

  inline void * Alloc(size_t a_size) { return m_Alloc(a_size, m_memContext); }
  inline void Free(void * a_ptr) { m_Free(a_ptr, m_memContext); }

  MXMLDeclaration * CreateDeclaration();
  MXMLComment * CreateComment();
  MXMLText * CreateText();
  MXMLCDATA * CreateCDATA();
  MXMLElement * CreateElement();

  void ResetBuffer();
  void AppendBuffer(const char * a_text, size_t a_size = 0);
  const char * GetBuffer() const;

  /// \brief Load an xml document
  /// \param a_stream is a MXMLStream derived object ready for reading
  /// \param a_bufferSize is used with MXMLParserParse to load the document
  /// \return true on success, otherwise false and the document buffer has the error message
  bool Load(MXMLStream &a_stream, size_t a_bufferSize = 4096);

  /// \brief Save an xml document
  /// \param a_stream is a MXMLStream derived object ready for writing
  /// \param a_columnAttributes format attributes as column for human readabilty / editing
  /// \return true on success
  bool Save(MXMLStream &a_stream, bool a_columnAttributes = true) const;

  const char * GetUniqueString(const char * a_string);

  /// \brief Get the document level element, should have 1 root element and possible declarations.
  inline MXMLElement * GetDocumentElement() { return m_document; }
  inline const MXMLElement * GetDocumentElement() const { return m_document; }

  /// Fill this from other document
  void CopyFrom(const MXMLDocument& a_other);
  
protected:

  struct UniqueNode
  {
    const char * m_string;
    UniqueNode * m_next;
  };

  UniqueNode ** m_uniques;
  int m_sizeUniques;

  char * m_buffer;
  size_t m_bufferSize;
  size_t m_maxBufferSize;

  MXMLElement * m_document;

  MXMLAlloc m_Alloc;
  MXMLFree m_Free;
  void* m_memContext;
  
  static void* DefaultAlloc(size_t a_size, void* a_context);                    // Default alloc(), with context
  static void DefaultFree(void* a_mem, void* a_context);                        // Default free(), with context
  MXMLNode* AllocNodeCopyInternal(const MXMLNode* a_other);                     // Allocate a copy of this type of node
  void Init(MXMLAlloc a_Alloc, MXMLFree a_Free, void* a_memContext, bool a_createRoot); // Init document and resources
  void Destroy();                                                               // Destroy document and resources
  
  friend class MXMLElement;                                                     // Access internal helpers
};

#endif