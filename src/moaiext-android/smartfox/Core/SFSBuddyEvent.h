// ===================================================================
//
// Description		
//		Contains the definition of SFSBuddyEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#ifndef __SFSBuddyEvent__
#define __SFSBuddyEvent__

#include "../Util/Common.h"
#include "../Core/BaseEvent.h"

#if defined(_MSC_VER)
#pragma warning(disable:4786)			// STL library: disable warning 4786; this warning is generated due to a Microsoft bug
#endif
#include <map>							// STL library: map object
#include <string>						// STL library: string object
using namespace std;					// STL library: declare the STL namespace

namespace Sfs2X {
namespace Core {
	
	/// <summary>
	/// This class contains all the Buddy List API related events
	/// </summary>
	class SFSBuddyEvent : public BaseEvent
	{
	public:

		// -------------------------------------------------------------------
		// Public methods
		// -------------------------------------------------------------------
		SFSBuddyEvent(string type);
		SFSBuddyEvent(string type, map<string, void*>* args);

		// -------------------------------------------------------------------
		// Public members
		// -------------------------------------------------------------------
		
		/// <summary>
		/// The Event is fired in response to a InitBuddyListRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// After the Buddy List initialization the User will return to his previous Buddy state. His online/offline status, the Buddy state
		/// and his persistent Buddy Variables will all be loaded and broadcast in the system. In particular the online status determines if the
		/// User will appear online or not to other Users that have added him/her to their lists.
		/// <para/>
		/// At this stage the custom <b>Buddy states</b> are also loaded and populated locally by the API.
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddyList</term>
		///     <description>(<b>Array</b>) a list of Buddy objects</description>
		///   </item>
		///   <item>
		///     <term>myVariables</term>
		///     <description>(<b>Array</b>) a list of the User's BuddyVariable(s)</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <example>
		/// You can access them like this:
		/// <code lang="C#">
		/// 	Console.WriteLine("Buddy States: " + smartFox.BuddyManager.BuddyStates);
		/// </code>
		/// </example>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
        /// <seealso cref="Entities.Variables.SFSBuddyVariable"/>
		/// <seealso cref="Requests.InitBuddyListRequest"/>
		static const string* BUDDY_LIST_INIT;

		/// <summary>
		/// The Event is fired in response to a AddBuddyRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// After the Buddy List initialization the User will return to his previous Buddy state. His online/offline status, the Buddy state
		/// and his persistent Buddy Variables will all be loaded and broadcast in the system. In particular the online status determines if the
		/// User will appear online or not to other Users that have added him/her to their lists.
		/// <para/>
		/// At this stage the custom <b>Buddy states</b> are also loaded and populated locally by the API.
        /// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that was added</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
        /// <seealso cref="Requests.AddBuddyRequest"/>
		static const string* BUDDY_ADD;

		/// <summary>
		/// The Event is fired in response to a RemoveBuddyRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that was removed</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
		/// <seealso cref="Requests.RemoveBuddyRequest"/>
		static const string* BUDDY_REMOVE;

		/// <summary>
		/// The Event is fired in response to a BlockBuddyRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that was blocked/unblocked</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
		/// <seealso cref="Requests.BlockBuddyRequest"/>
		static const string* BUDDY_BLOCK;

		/// <summary>
		/// The Event is fired in response to a failed Buddy request, such as AddBuddy, RemoveBuddy etc...
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>errorMessage</term>
		///     <description>(<b>string</b>) the error message</description>
		///   </item>
		///   <item>
		///     <term>errorCode</term>
		///     <description>(<b>short</b>) the error code</description>
		///   </item>
		/// </list>
		/// </remarks>
		static const string* BUDDY_ERROR;

		/// <summary>
		/// The Event is fired in response to a GoOnlineRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that was has changed his online status</description>
		///   </item>
		///   <item>
		///     <term>isItMe</term>
		///     <description>(<b>bool</b>) true if the Buddy is actually the current User</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
		/// <seealso cref="Requests.GoOnlineRequest"/>
		static const string* BUDDY_ONLINE_STATE_UPDATE;

		/// <summary>
		/// The Event is fired in response to a SetBuddyVariablesRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that updated the BuddyVariables</description>
		///   </item>
		///   <item>
		///     <term>changedVars</term>
		///     <description>(<b>Array</b>) a list with the names of the Variables that where changed/set</description>
		///   </item>
		///   <item>
		///     <term>isItMe</term>
		///     <description>(<b>bool</b>) true if the Buddy is actually the current User</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
		/// <seealso cref="Requests.SetBuddyVariablesRequest"/>
		static const string* BUDDY_VARIABLES_UPDATE;

		/// <summary>
		/// The Event is fired in response to a BuddyMessageRequest.
		/// </summary>
		/// 
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>buddy</term>
		///     <description>(<b>Buddy</b>) the Buddy that updated the BuddyVariables</description>
		///   </item>
		///   <item>
		///     <term>isItMe</term>
		///     <description>(<b>bool</b>) true if the Buddy is actually the current User</description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>string</b>) a list with the names of the Variables that where changed/set</description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) optional custom parameters</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <seealso cref="Entities.SFSBuddy"/>
		/// <seealso cref="Requests.BuddyMessageRequest"/>
		static const string* BUDDY_MESSAGE;

	protected:

		// -------------------------------------------------------------------
		// Protected methods
		// -------------------------------------------------------------------

		// -------------------------------------------------------------------
		// Protected members
		// -------------------------------------------------------------------

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
