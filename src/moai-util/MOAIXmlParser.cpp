// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStream.h>
#include <moai-util/MOAIXmlParser.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_TINYXML && MOAI_WITH_EXPAT
#include <tinyxml.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_getElementAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "US" )

	ZLXmlElement* element = self->mReader.GetElement ();
	if ( element ) {
		
		cc8* attribute = state.GetValue < cc8* >( 2, 0 );
		if ( attribute && element->HasAttribute ( attribute )) {
			state.Push ( element->GetAttribute ( attribute ));
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_getElementAttributes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )

	lua_newtable ( state );

	ZLXmlElement* element = self->mReader.GetElement ();
	if ( element ) {
		
		const ZLXmlElement::AttributeMap& attributes = element->GetAttributes ();
		
		ZLXmlElement::AttributeMapConstIt attrIt = attributes.begin ();
		for ( ; attrIt != attributes.end (); ++attrIt ) {
			state.SetField ( -1, attrIt->first, attrIt->second );
		}
		state.Push (( u32 )attributes.size ());
		return 2;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIXmlParser::_getElementLineNumber ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )
	
	state.Push ( self->mReader.GetLineNumber ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_getElementName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )

	ZLXmlElement* element = self->mReader.GetElement ();
	if ( element ) {
		state.Push ( element->GetName ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_getElementText ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )

	ZLXmlElement* element = self->mReader.GetElement ();
	if ( element && ( element->GetText ().size () > 0 )) {
		state.Push ( element->GetText ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	parseFile
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
/**	@lua	parseString
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

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_setStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )
	
	self->mStream.Set ( *self, state.GetLuaObject< MOAIStream >( 2, false ));
	
	if ( self->mStream ) {
		self->mReader.SetStream ( *self->mStream );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIXmlParser::_step ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIXmlParser, "U" )
	
	self->mReader.Parse ();
	state.Push ( self->mReader.GetEvent ());
	
	return 1;
}

//================================================================//
// MOAIXmlParser
//================================================================//

//----------------------------------------------------------------//
MOAIXmlParser::MOAIXmlParser () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIXmlParser::~MOAIXmlParser () {

	this->mStream.Set ( *this, 0 );
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

	state.SetField ( -1, "DONE",				( u32 )ZLXmlReader::DONE );
	state.SetField ( -1, "ELEMENT",				( u32 )ZLXmlReader::ELEMENT );
	state.SetField ( -1, "ELEMENT_BEGIN",		( u32 )ZLXmlReader::ELEMENT_BEGIN );
	state.SetField ( -1, "ELEMENT_END",			( u32 )ZLXmlReader::ELEMENT_END );
	state.SetField ( -1, "XML_ERROR",			( u32 )ZLXmlReader::XML_ERROR );

	luaL_Reg regTable[] = {
		{ "parseFile",				_parseFile },
		{ "parseString",			_parseString },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIXmlParser::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "getElementAttribute",	_getElementAttribute },
		{ "getElementAttributes",	_getElementAttributes },
		{ "getElementLineNumber",	_getElementLineNumber },
		{ "getElementName",			_getElementName },
		{ "getElementText",			_getElementText },
		{ "setStream",				_setStream },
		{ "step",					_step },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

#endif