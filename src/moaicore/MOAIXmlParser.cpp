// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIXmlParser.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	parseFile
	@text	Parses the contents of the specified file as XML.

	@in		MOAIXmlParser self
	@in		string filename			The path of the file to read the XML data from.
	@out	table data				A tree of tables representing the XML.
*/
int MOAIXmlParser::_parseFile ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "S" )) return 0;
	
	cc8* filename = lua_tostring ( state, 1 );
	
	if ( MOAILogMessages::CheckFileExists ( filename, L )) {
		TiXmlDocument doc;
		doc.LoadFile ( filename );
		MOAIXmlParser::Parse ( state, doc.RootElement ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	parseString
	@text	Parses the contents of the specified string as XML.

	@in		MOAIXmlParser self
	@in		string filename			The XML data stored in a string.
	@out	table data				A tree of tables representing the XML.
*/
int MOAIXmlParser::_parseString ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "S" )) return 0;
	
	cc8* xml = lua_tostring ( state, 1 );
	
	TiXmlDocument doc;
	doc.Parse ( xml );
	MOAIXmlParser::Parse ( state, doc.RootElement ());

	return 1;
}

//================================================================//
// MOAIXmlParser
//================================================================//

//----------------------------------------------------------------//
MOAIXmlParser::MOAIXmlParser () {
}

//----------------------------------------------------------------//
MOAIXmlParser::~MOAIXmlParser () {
}

//----------------------------------------------------------------//
void MOAIXmlParser::Parse ( MOAILuaState& state, TiXmlNode* node ) {

	if ( !node ) return;
	
	TiXmlElement* element = node->ToElement();
	if ( element ) {
		
		lua_newtable ( state );
		
		// element name
		lua_pushstring ( state, element->Value ());
		lua_setfield ( state, -2, "type" );
		
		// parse attributes
		
		TiXmlAttribute* attribute = element->FirstAttribute ();
		if ( attribute ) {
			lua_newtable ( state );
			for ( ; attribute; attribute = attribute->Next ()) {
				lua_pushstring ( state, attribute->Value ());
				lua_setfield ( state, -2, attribute->Name ());
			}
			lua_setfield ( state, -2, "attributes" );
		}
		
		// round up the children
		STLSet < string > children;
		TiXmlElement* childElement = node->FirstChildElement ();
		for ( ; childElement; childElement = childElement->NextSiblingElement ()) {
			children.affirm ( childElement->Value ());
		}
		
		if ( children.size ()) {
			lua_newtable ( state );
			STLSet < string >::iterator childrenIt = children.begin ();
			for ( ; childrenIt != children.end (); ++childrenIt ) {
				
				string name = *childrenIt;
				lua_newtable ( state );
				
				childElement = node->FirstChildElement ( name );
				for ( u32 count = 1; childElement; childElement = childElement->NextSiblingElement ( name ), ++count ) {
					MOAIXmlParser::Parse ( state, childElement );
					lua_rawseti ( state, -2, count );
				}
				lua_setfield ( state, -2, name.c_str ());
			}
			lua_setfield ( state, -2, "children" );
		}
		
		// handle text children
		TiXmlNode* child = node->FirstChild ();
		if ( child ) {
			TiXmlText* text = child->ToText ();
			if ( text ) {
				lua_pushstring ( state, text->Value ());
				lua_setfield ( state, -2, "value" );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIXmlParser::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "parseFile",				_parseFile },
		{ "parseString",			_parseString },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIXmlParser::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

