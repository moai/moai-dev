// These two are for MSVS 2005 security consciousness until safe std lib funcs are available
#pragma warning(disable : 4996) // Deprecated functions
#define _CRT_SECURE_NO_DEPRECATE // Allow old unsecure standard library functions, Disable some 'warning C4996 - function was deprecated'

#include "mxmlstream.h"
#include "mxmldom.h"

// Copy this text to a file 'test.xml' if not present
/*
<test>
  <!--
  Test xml file
  -->
  <market>
    <fruit color = "red">
    Strawberry
    </fruit>
    <fruit color = "yellow">
    Banana
    </fruit>
  </market>
</test>
*/

//
// Example usage for MXML
//
// The API is fairly straight forward however for advanced use you would likely want 
// to derive MXMLDocument into your own XmlDoc class and implement Alloc(), Free() from 
// a custom memory heap and implement MXMLStream to access your own IO stream.
// As well as this, there are a multitude of helper functions you might like to add, 
// to simplify what would otherwise be several operations into one, or adding parsers
// for common types like Floats, Ints, Bools and Vectors.
//

void TestMXML()
{
  MXMLDocument inDoc;
  MXMLDocument outDoc;
  
  MXMLStreamFile inFile("test.xml", "rb");
  if( inFile.IsOpen() )
  {
    bool readResult = inDoc.Load(inFile);
    outDoc.CopyFrom(inDoc); // Copying the document because we plan to modify and save the modification
    
    outDoc.GetDocumentElement()->Unformat(); // NOTE: Trimming white space from node text etc.
    //outDoc.GetDocumentElement()->Format(false);
    
    MXMLElement* node = outDoc.GetDocumentElement();
    if( node ) // Note: First node is whole document, not first element
    {
      node = node->GetFirstChildElement();
      if( node )
      {
        const char* docName = node->GetName();
        node = node->FindChild("market");
        if( node )
        {
          node = node->FindChild("fruit", 1);
          if( node )
          {
            // Text
            const char* text = node->GetText(); // NOTE: When you read a string like this, the value is only valid for this statement. Copy it to preserve it.
            
            // Note that text is stored in one or more child text nodes of the element.  
            //      We've already trimmed whitespace so the first text node contains the text we care about.
            // Note that if we were creating from scratch, we would use AppendText() instead
            MXMLText* textNode = static_cast<MXMLText *>( node->GetFirstChild(MXMLNode::T_TEXT) );
            if( textNode )
            {
              textNode->SetText("apple");
            }
          
            // Attributes
            const char* color = node->GetAttributes().GetNamedItem("color");
            node->GetAttributes().SetNamedItem("tasty", "true");
          }
        }
      }
    }

    MXMLStreamFile outFile("test2.xml", "wb");
    if( outFile.IsOpen() )
    {
      outDoc.GetDocumentElement()->Format(false); // Reformat nodes to look pretty and have vertical layout
      //bool writeResult = outDoc.Save(outFile, false);
      bool writeResult = outDoc.Save(outFile, true);
    }
  }
}

#if 1 // Make a test application

#include <crtdbg.h> // Check for memory leaks

#ifdef _DEBUG
  #define SET_CRT_DEBUG_FIELD(a) \
  _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
  #define CLEAR_CRT_DEBUG_FIELD(a) \
  _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
  #define SET_CRT_DEBUG_FIELD(a) ((void) 0)
  #define CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

void main()
{
  TestMXML();
  
  SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
}

#endif // Make a test application