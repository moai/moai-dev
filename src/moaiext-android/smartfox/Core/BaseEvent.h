// ===================================================================
//
// Description		
//		Contains the definition of BaseEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#ifndef __BaseEvent__
#define __BaseEvent__

#include "../Util/Common.h"
#include <stdio.h>

#if defined(_MSC_VER)
#pragma warning(disable:4786)			// STL library: disable warning 4786; this warning is generated due to a Microsoft bug
#endif
#include <string>				// STL library: string object
#include <map>					// STL library: map object
using namespace std;			// Declare the STL namespace

namespace Sfs2X {
namespace Core {

	// -------------------------------------------------------------------
	// Class BaseEvent
	// -------------------------------------------------------------------
	class DLLImportExport BaseEvent
	{
	public:

		// -------------------------------------------------------------------
		// Public methods
		// -------------------------------------------------------------------

		BaseEvent();
		BaseEvent(string type);
		BaseEvent(string type, map<string, void*>* args);
		~BaseEvent();

		string Type();
		void Type(string value);

		map<string, void*>* Params();
		void Params(map<string, void*>* value);

		void* Target();
		void Target(void* value);

		string ToString();
		BaseEvent* Clone();	

		// -------------------------------------------------------------------
		// Public members
		// -------------------------------------------------------------------

	protected:
		// -------------------------------------------------------------------
		// Protected methods
		// -------------------------------------------------------------------

		// -------------------------------------------------------------------
		// Protected members
		// -------------------------------------------------------------------

		map<string, void*>* arguments;
		string* type;
		void* target;

	private:

		// -------------------------------------------------------------------
		// Private methods
		// -------------------------------------------------------------------

		// -------------------------------------------------------------------
		// Private members
		// -------------------------------------------------------------------
	};

}	// namespace Core
}	// namespace Sfs2X

#endif
