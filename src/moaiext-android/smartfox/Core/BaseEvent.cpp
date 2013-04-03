// ===================================================================
//
// Description		
//		Contains the implementation of BaseEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#include "BaseEvent.h"

namespace Sfs2X {
namespace Core {

// -------------------------------------------------------------------
// Type
// -------------------------------------------------------------------
string BaseEvent::Type()
{
	return (*type);
}

// -------------------------------------------------------------------
// Type
// -------------------------------------------------------------------
void BaseEvent::Type(string value)
{
	type = new string(value);
}

// -------------------------------------------------------------------
// Params
// -------------------------------------------------------------------
map<string, void*>* BaseEvent::Params()
{
	return arguments;
}

// -------------------------------------------------------------------
// Params
// -------------------------------------------------------------------
void BaseEvent::Params(map<string, void*>* value)
{
	arguments = value;
}

// -------------------------------------------------------------------
// Target
// -------------------------------------------------------------------
void* BaseEvent::Target()
{
	return target;
}

// -------------------------------------------------------------------
// Target
// -------------------------------------------------------------------
void BaseEvent::Target(void* value)
{
	target = value;
}

// -------------------------------------------------------------------
// ToString
// -------------------------------------------------------------------
string BaseEvent::ToString()
{
	string log;

	if (target == NULL)
	{
		char buffer[100];
		sprintf (buffer, "%s [ null]", type->c_str());

		log = buffer;
	}
	else
	{
		char buffer[100];
		sprintf (buffer, "%s [ 0x%x]", type->c_str(), target);

		log = buffer;
	}

	return log;
}


// -------------------------------------------------------------------
// Clone
// -------------------------------------------------------------------
BaseEvent* BaseEvent::Clone()
{
	return new BaseEvent(*type, arguments);
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
BaseEvent::BaseEvent()
{
	type = new string();
	arguments = new map<string, void*>();
	target = NULL;
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
BaseEvent::BaseEvent(string type)
{
	this->target = NULL;
	this->type = new string(type);
	if (arguments == NULL) arguments = new map<string, void*>();
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
BaseEvent::BaseEvent(string type, map<string, void*>* args)
{
	this->target = NULL;
	this->type = new string(type);
	arguments = args;
	if (arguments == NULL) arguments = new map<string, void*>();
}

// -------------------------------------------------------------------
// Destructor
// -------------------------------------------------------------------
BaseEvent::~BaseEvent()
{
	delete type;
	type = NULL;

	if (arguments != NULL)
	{
		arguments->clear();
		delete arguments;
		arguments = NULL;
	}

}


}	// namespace Core
}	// namespace Sfs2X
