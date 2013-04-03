// ===================================================================
//
// Description		
//		Contains the implementation of SFSBuddyEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#include "SFSBuddyEvent.h"

namespace Sfs2X {
namespace Core {

const string* SFSBuddyEvent::BUDDY_LIST_INIT (new string("buddyListInit"));
const string* SFSBuddyEvent::BUDDY_ADD (new string("buddyAdd"));
const string* SFSBuddyEvent::BUDDY_REMOVE (new string("buddyRemove"));
const string* SFSBuddyEvent::BUDDY_BLOCK (new string("buddyBlock"));
const string* SFSBuddyEvent::BUDDY_ERROR (new string("buddyError"));
const string* SFSBuddyEvent::BUDDY_ONLINE_STATE_UPDATE (new string("buddyOnlineStateChange"));
const string* SFSBuddyEvent::BUDDY_VARIABLES_UPDATE (new string("buddyVariablesUpdate"));
const string* SFSBuddyEvent::BUDDY_MESSAGE (new string("buddyMessage"));

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
SFSBuddyEvent::SFSBuddyEvent(string type)
	: BaseEvent (type, NULL)
{
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
SFSBuddyEvent::SFSBuddyEvent(string type, map<string, void*>* args)
	: BaseEvent (type, args)
{
}

}	// namespace Core
}	// namespace Sfs2X

