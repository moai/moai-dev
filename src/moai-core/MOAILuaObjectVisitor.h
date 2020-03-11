// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECTVISITOR_H
#define	MOAILUAOBJECTVISITOR_H

#include <moai-core/MOAILuaObject.h>

//================================================================//
// MOAIAbstractLuaRegistrationVisitor
//================================================================//
class MOAIAbstractLuaRegistrationVisitor {
public:

	//----------------------------------------------------------------//
	virtual ~MOAIAbstractLuaRegistrationVisitor () {
	}

	//----------------------------------------------------------------//
	virtual void	RegisterLuaClass		( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const = 0;
	virtual void	RegisterLuaFuncs		( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const = 0;
};

//================================================================//
// MOAILuaObjectVisitor
//================================================================//
template < typename TYPE >
class MOAILuaRegistrationVisitor :
	public MOAIAbstractLuaRegistrationVisitor {
public:

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const {
		self.AsType < TYPE >()->TYPE::_RegisterLuaClass ( history, state );
	}
	
	//----------------------------------------------------------------//
	void RegisterLuaFuncs ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const {
		self.AsType < TYPE >()->TYPE::_RegisterLuaFuncs ( history, state );
	}
};

//================================================================//
// MOAIAbstractLuaSerializationVisitor
//================================================================//
class MOAIAbstractLuaSerializationVisitor {
public:

	//----------------------------------------------------------------//
	virtual ~MOAIAbstractLuaSerializationVisitor () {
	}

	//----------------------------------------------------------------//
	virtual void 	SerializeIn 			( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) const = 0;
	virtual void 	SerializeOut 			( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) const = 0;
};

//================================================================//
// MOAILuaSerializationVisitor
//================================================================//
template < typename TYPE >
class MOAILuaSerializationVisitor :
	public MOAIAbstractLuaSerializationVisitor {
public:

	//----------------------------------------------------------------//
	void SerializeIn ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) const {
		self.AsType < TYPE >()->TYPE::_SerializeIn ( history, state, serializer );
	}
	
	//----------------------------------------------------------------//
	void SerializeOut ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) const {
		self.AsType < TYPE >()->TYPE::_SerializeOut ( history, state, serializer );
	}
};

#endif
