// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDSLHANDLER_H
#define	MOAIABSTRACTDSLHANDLER_H

class MOAILuaObject;

//================================================================//
// MOAIAbstractDSLHandler
//================================================================//
// TODO: doxygen
class MOAIAbstractDSLHandler {
protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractDSLHandler_RetainObject			( MOAILuaObject* object );
	virtual void		MOAIAbstractDSLHandler_SubmitCommand		( u32 cmd, const void* param, ZLSize size ) = 0;
	
public:

	//----------------------------------------------------------------//
						MOAIAbstractDSLHandler			();
	virtual 			~MOAIAbstractDSLHandler			();
	void				RetainObject					( MOAILuaObject* object );
	void				SubmitCommand					( u32 cmd, const void* param = NULL, ZLSize size = 0 );
};

#endif
