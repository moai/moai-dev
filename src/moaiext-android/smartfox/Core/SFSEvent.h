// ===================================================================
//
// Description		
//		Contains the definition of SFSEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#ifndef __SFSEvent__
#define __SFSEvent__

#include "../Util/Common.h"
#include "../Core/BaseEvent.h"

namespace Sfs2X {
namespace Core {
	
	// -------------------------------------------------------------------
	// Class SFSEvent
	// -------------------------------------------------------------------
	class DLLImportExport SFSEvent : public BaseEvent
	{
	public:

		// -------------------------------------------------------------------
		// Public methods
		// -------------------------------------------------------------------

		/// <exclude/>
		SFSEvent(string type);

		/// <exclude/>
		SFSEvent(string type, map<string, void*>* data);

		// -------------------------------------------------------------------
		// Public members
		// -------------------------------------------------------------------

		/// <exclude/>
		static const string* HANDSHAKE;

		/// <summary>
		/// The Event notifies the result of the initial UDP handshake. 
		/// </summary>
		/// <remarks>
		/// The handshake is sent in order to establish a channel of communication between client and server.
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>success</term>
		///     <description>(<b>bool</b>) true if a connection was established, false otherwise.</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how initialize the UDP channel
		/// <code>
		/// 		sfs.AddEventListener(SFSEvent.UPD_INIT, OnUDPInit);
		/// 		sfs.InitUDP();
		/// 
		/// 		void OnUDPInit(BaseEvent evt) {
		/// 			if ((bool)evt.Params["success"] == false) {
		/// 				Console.WriteLine("Sorry, UDP is not available. Initialization failed");
		/// 			} else {
		/// 				// Init OK: Execute an extension call via UDP
		/// 				sfs.Send( new ExtensionRequest("udpTest", new SFSObject(), null, true) );
		/// 			}
		/// 		}
		/// </code>
		/// </example>
        /// <seealso cref="SmartFox.InitUDP(string, int)"/>
		static const string* UDP_INIT;

		/// <summary>
		/// The Event notifies the result of a connection attempt.
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
		///     <term>success</term>
		///     <description>(<b>bool</b>) true if a connection was established, false otherwise</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to start a connection
		/// <code>
        /// SmartFox smartFox = new SmartFox();
		/// smartFox.AddEventListener(SFSEvent.OnConnection, OnConnection);
		/// 
		/// void OnConnection(BaseEvent evt) {
        ///		if ((bool)evt.Params["success"])
		///			Console.WriteLine("Connection was established");
		///		else
		///			Console.WriteLine("Connection failed");
		///		}
		/// </code>
		/// </example>
		/// 
		/// <seealso cref="CONNECTION_RETRY"/>
		/// <seealso cref="CONNECTION_RESUME"/>
		/// <seealso cref="CONNECTION_LOST"/>
		static const string* CONNECTION;

		/**
		 * The <em>SFSEvent.PING_PONG</em> constant defines the value of the <em>type</em> property of the event object for a <em>pingPong</em> event.
		 * <p>The event is fired when measuring the lag between client and server</p>
		 * 
		 * <p>The properties of the <em>params</em> object contained in the event object have the following values:</p>
		 * <table class="innertable">
		 * <tr><th>Property</th><th>Type</th><th>Description</th></tr>
		 * <tr><td>lagValue</td><td><em>int</em></td><td>an average of the last 10 lag values, expressed in milliseconds</td></tr>
		 * </table>
		 */
		static const string* PING_PONG;

		/**
		 * The <em>SFSEvent.SOCKET_ERROR</em> constant defines the value of the <em>type</em> property of the event object for a <em>socketError</em> event.
		 * <p>The event is fired when an unexpected socket error occurs, typically a data error.</p>
		 * 
		 * <p>The properties of the <em>params</em> object contained in the event object have the following values:</p>
		 * <table class="innertable">
		 * <tr><th>Property</th><th>Type</th><th>Description</th></tr>
		 * <tr><td>errorMessage</td><td><em>int</em></td><td>the description of the problem</td></tr>
		 * </table>
		 */
		static const string* SFS_SOCKET_ERROR;

		/// <summary>
		/// The Event notifies the disconnection from the Server.
		/// </summary>
		/// 
		/// <remarks>
		/// When possible the API will also provide a reason for the disconnection:
		/// <ul>
		/// <li><b>Idle</b>: the connection was dropped because the User was inactive for too long</li>
		/// <li><b>Manual</b>: the User requested the disconnection</li>
		/// <li><b>Kick</b>: the connection was dropped because the User was kicked</li>
		/// <li><b>Ban</b>: the connection was dropped because the User was banned</li>
		/// <li><b>Unknown</b>: the connection was lost for reasons outside of the API (slow or bad connection, network problems etc...)</li>
		/// </ul>
        /// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>reason</term>
		///     <description>(<b>String</b>) the reason of the disconnection.</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a disconnection
		/// <code>
		/// SmartFox smartFox = new SmartFox();
		/// smartFox.AddEventListener(SFSEvent.OnConnectionLost, OnConnectionLost);
		/// 
		/// void OnConnectionLost(BaseEvent evt) {
        ///		Console.WriteLine("Connection was lost, Reason: " + (string)evt.Params["reason"]);
		///		}
		/// </code>
		/// </example>
		/// 
		/// <seealso cref="CONNECTION_RETRY"/>
		/// <seealso cref="CONNECTION_RESUME"/>
		/// <seealso cref="CONNECTION"/>
		static const string* CONNECTION_LOST;

		/// <summary>
		/// The Event notifies the disconnection from the Server.
		/// </summary>
		/// 
		/// <remarks>
		/// SmartFoxServer 2X provides a powerful new feature that allows a broken connection to be re-established transparently
		/// without loosing any of the current application state. This allows any player to get back in the game without fear of loosing 
		/// the match because of a sloppy internet connection. 
		/// <para/>
		/// When this event is dispatched the API enter a "freeze" mode where no new requests can be sent until the reconnection is 
		/// successfully performed. It is highly recommended to handle this event and freeze your application interface accordingly until the
		/// <see cref="CONNECTION_RESUME"/> event is fired.
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a reconnection
		/// <code>
        ///	SmartFox smartFox = new SmartFox();
		///	smartFox.AddEventListener(SFSEvent.CONNECTION_RETRY, OnConnectionRetry);
		///	smartFox.AddEventListener(SFSEvent.CONNECTION_RESUME, OnConnectionResume);
		///			
		///			void OnConnectionRetry(BaseEvent evt) {
		///				// Freeze your GUI and provide some feedback to the Player
		///			}
		///
		///			void OnConnectionResume(BaseEvent evt) {
		///				// Unfreeze the GUI and let the player continue with the game...
		///			}
		/// </code>
		/// </example>
		/// 
		/// <seealso cref="CONNECTION_LOST"/>
		/// <seealso cref="CONNECTION_RESUME"/>
		/// <seealso cref="CONNECTION"/>
		static const string* CONNECTION_RETRY;

		/// <summary>
		/// The Event is dispatched after a temporary disconnection, when the connection to the Server has been re-establisehd.
		/// </summary>
		/// 
		/// <remarks>
		/// SmartFoxServer 2X provides a powerful new feature that allows a broken connection to be re-established transparently
		/// without loosing any of the current application state. This allows any player to get back in the game without fear of loosing 
		/// the match because of a sloppy internet connection or a sudden disconnection. 
		/// <para/>
		/// In case the re-connection attempts fails an <see cref="CONNECTION_LOST"/> event will be fired.
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a reconnection
		/// 			<code>
		/// 			SmartFox smartFox = new SmartFox();
		/// 			smartFox.AddEventListener(SFSEvent.OnConnectionRetry, OnConnectionRetry);
		/// 			smartFox.AddEventListener(SFSEvent.OnConnectionResumed, OnConnectionResumed);
		/// 			smartFox.AddEventListener(SFSEvent.OnConnectionResumed, OnConnectionLost);
		/// 			
		/// 			void OnConnectionRetry(BaseEvent evt) {
		/// 				// Freeze your GUI and provide some feedback to the Player
		/// 			}
		/// 
		/// 			void OnConnectionResumed(BaseEvent evt) {
		/// 				// Unfreeze the GUI and let the player continue with the game...
		/// 			}
		/// 
		/// 			void OnConnectionLost(BaseEvent evt) {
        /// 				Console.WriteLine("Ouch, connection was lost. Reason: " + (string)evt.Params["reason"]);
		/// 			}
		/// 			</code>
		/// </example>
		/// 
		/// <seealso cref="CONNECTION_LOST"/>
		/// <seealso cref="CONNECTION_RESUME"/>
		/// <seealso cref="CONNECTION"/>
		static const string* CONNECTION_RESUME;

		/**
		 * The <em>SFSEvent.CONNECTION_ATTEMPT_HTTP</em> constant defines the value of the <em>type</em> property of the event object for a <em>connectionAttemptHttp</em> event.
		 * 
		 * <p>No parameters are available for this event object.</p>
		 * 
		 * @example	See the example provided in the <em>CONNECTION_ATTEMPT_HTTP</em> constant description.
		 * 
		 * @eventType	connectionAttemptHttp
		 * 
		 * @see		#CONNECTION
		 * @see		#CONNECTION_LOST
		 */	
		static const string* CONNECTION_ATTEMPT_HTTP;

		/// <summary>
		/// Dispatched when the external SmartFox configuration file has been loaded successfully.
		/// </summary>
		/// <remarks>
		/// This event is dispatched only if the <i>connectOnSuccess</i> parameter of the <see cref="SmartFox.LoadConfig(string, bool)"/> method is set to <c>false</c> otherwise the connection is made and the related <see cref="CONNECTION"/> event is fired.
		/// <para/>
		/// No parameters are provided.
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a successful configuration loading.
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.CONFIG_LOAD_SUCCESS, OnConfigLoadSuccess);
		/// 			smartFox.LoadConfig("testEnvironmentConfig.xml", false);
		/// 			
		/// 			void OnConfigLoadSuccessHandler(BaseEvent evt) {
		/// 				Console.WriteLine("Config file loaded, now connecting...");
		/// 				smartFox.Connect(smartFox.CurrentIp, smartFox.CurrentPort);
		/// 			}
		/// 			</code>
		/// 
		/// </example>
		/// 
		/// <seealso cref="CONFIG_LOAD_FAILURE"/>
		/// <seealso cref="SmartFox.LoadConfig(string, bool)"/>
		static const string* CONFIG_LOAD_SUCCESS;

		/// <summary>
		/// Dispatched when an error occurs while loading the external SmartFox configuration file.
		/// </summary>
		/// <remarks>
		/// Typically this event is received when the configuration is not found or its not accessible (no read permissions)
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a potential error in configuration loading.
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.CONFIG_LOAD_FAILURE, OnConfigLoadFailure);
		/// 			smartFox.LoadConfig("testEnvironmentConfig.xml");
		/// 			
		/// 			void OnConfigLoadFailureHandler(BaseEvent evt) {
        /// 				Console.WriteLine("Failed loading config file.);
		/// 			}
		/// 			</code>
		/// </example>
		/// 
		/// <seealso cref="CONFIG_LOAD_SUCCESS"/>
		/// <seealso cref="SmartFox.LoadConfig(string, bool)"/>
		static const string* CONFIG_LOAD_FAILURE;

		/// <summary>
		/// The Event is dispatched in response to a login request.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User object</description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>SFSObject</b>) a custom object with extra params (if you have added custom server side logic)</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// 
		/// <example>
		/// The following example shows how to handle a login event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.LOGIN, OnLogin);
		/// 			
		/// 			void OnLogin(BaseEvent evt) {
        /// 				Console.WriteLine("Hi, I have just logged in as: " + ((User)evt.Params["user"]).Name);
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="LOGIN_ERROR"/>
        /// <seealso cref="Requests.LoginRequest"/>
		static const string* LOGIN;

		/// <summary>
		/// The Event is dispatched in response to a failed login request.
		/// </summary>
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
		/// <example>
		/// The following example shows how to handle a login failure event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.LOGIN_ERROR, OnLoginError);
		/// 			
		/// 			void OnLoginError(BaseEvent evt) {
        /// 				Console.WriteLine("Login Failed. Reason: " + (string)evt.Params["errorMessage"]);
		/// 			}
		/// 			</code>
		/// </example> 
		/// <seealso cref="LOGIN"/>
        /// <seealso cref="Requests.LoginRequest"/>
		static const string* LOGIN_ERROR;

		/// <summary>
		/// The Event is dispatched in response to a logout request.
		/// </summary>
		/// <remarks>
		/// Usually this event needs to be handled when you want to logout from the current Zone and join another one.
		/// <para/>
        /// In order to perform the operation correctly you need to do as follows:
		/// <ul>
		/// 	<li>Send a LogoutRequest to the Server</li>
		/// 	<li>Handle the OnLogout event on the client</li>
		/// 	<li>Perform a new login in the new Zone</li>
		/// </ul>
		/// <para/>
		/// No parameters are passed for this event
		/// </remarks>
		/// <example>
		/// The following example shows how to handle a logout event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.LOGOUT, OnLogout);
		/// 			
		/// 			void OnLogout(BaseEvent evt) {
		/// 				Console.WriteLine("Logout was successful");
		/// 				// Proceed with more logic here...
		/// 			}
		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.LogoutRequest"/>
        /// <seealso cref="Requests.LoginRequest"/>
		static const string* LOGOUT;

		/// <summary>
		/// The Event is dispatched when a new Room was created inside the Zone under any of the Room Groups that the User is listening for.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the room object</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.ROOM_ADD, OnRoomAdded);
		/// 			
		/// 			void OnRoomAdded(BaseEvent evt) {
        /// 				Console.WriteLine("A new Room was added: " + (Room)evt.Params["room"]);
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="ROOM_REMOVE"/>
        /// <seealso cref="Requests.CreateRoomRequest"/>
		static const string* ROOM_ADD;

		/// <summary>
		/// The Event is dispatched whenever a Room is removed from the Zone, under any of the Room Groups that the User is listening for.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the room object</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.ROOM_REMOVE, OnRoomRemoved);
		/// 			
		/// 			void OnRoomRemoved(BaseEvent evt) {
        /// 				Console.WriteLine("A new Room was removed: " + (Room)evt.Params["room"]);
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="ROOM_ADD"/>
		static const string* ROOM_REMOVE;

		/// <summary>
		/// The Event is dispatched in response to a failed create room request.
		/// </summary>
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
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.ROOM_CREATION_ERROR, OnRoomCreationError);
		/// 			
		/// 			void OnRoomCreationError(BaseEvent evt) {
        /// 				Console.WriteLine("Room Creation Error: " + (string)evt.Params["errorMessage"]);
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="ROOM_ADD"/>
        /// <seealso cref="Requests.CreateRoomRequest"/>
		static const string* ROOM_CREATION_ERROR;

		/// <summary>
		/// The Event is dispatched in response to a JoinRoomRequest.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the joined Room </description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.ROOM_JOIN, OnJoinRoom);
		/// 			
		/// 			void OnJoinRoom(BaseEvent evt) {
        /// 				Room room = (Room)evt.Params["room"];
		/// 				Console.WriteLine("The Room " + room.Name + " was succesfully joined!");
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="ROOM_JOIN_ERROR"/>
        /// <seealso cref="Requests.JoinRoomRequest"/>
		static const string* ROOM_JOIN;

		/// <summary>
		/// The Event is dispatched in response to a failed JoinRoomRequest.
		/// </summary>
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
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.ROOM_JOIN_ERROR, OnJoinRoomError);
		/// 			
		/// 			void OnJoinRoomError(BaseEvent evt) {
        /// 				Console.WriteLine("Join Room Failure: " + (string)evt.Params["errorMessage"]);
		/// 			}
		/// 			</code>
		/// </example>
		/// <seealso cref="ROOM_JOIN"/>
        /// <seealso cref="Requests.JoinRoomRequest"/>
		static const string* ROOM_JOIN_ERROR;

		/// <summary>
		/// The Event is fired whenever a User enters one of the Rooms where the current User is joined.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User object</description>
		///   </item>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room object</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.USER_ENTER_ROOM, OnUserEnterRoom);
		/// 			
		/// 			void OnUserEnterRoom(BaseEvent evt) {
        /// 				Room room = (Room)evt.Params["room"];
        /// 				User user = (User)evt.Params["user"];
		/// 
		/// 				Console.WriteLine("User: " + user.Name + " has just joined Room: " + room.Name);
		/// 			}
		/// 			</code>
		/// </example>	
		/// <seealso cref="USER_EXIT_ROOM"/>
		/// <seealso cref="ROOM_JOIN"/>
        /// <seealso cref="Requests.JoinRoomRequest"/>
		static const string* USER_ENTER_ROOM;

		/// <summary>
		/// The Event is fired whenever a User leaves one of the Rooms where the current User is joined.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User object</description>
		///   </item>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room object</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
		/// 			smartFox.AddEventListener(SFSEvent.USER_EXIT_ROOM, OnUserExitRoom);
		/// 			
		/// 			void OnUserExitRoom(BaseEvent evt) {
        /// 				Room room = (Room)evt.Params["room"];
        /// 				User user = (User)evt.Params["user"];
		/// 
		/// 				Console.WriteLine("User: " + user.Name + " has just left Room: " + room.Name);
		/// 			}
		/// 			</code>
		/// </example>	
		/// <seealso cref="USER_ENTER_ROOM"/>
		/// <seealso cref="ROOM_JOIN"/>
        /// <seealso cref="Requests.LeaveRoomRequest"/> 
		static const string* USER_EXIT_ROOM;

		/// <summary>
		/// The Event is fired whenever the number of users/spectators in a Room changes.
		/// </summary>
		/// <remarks>
		/// The client receives updates for all Rooms inside the Room Groups that he is listening to.
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room  </description>
		///   </item>
		///   <item>
		///     <term>uCount</term>
		///     <description>(<b>int</b>) the new user count</description>
		///   </item>
		///   <item>
		///     <term>sCount</term>
		///     <description>(<b>int</b>) the new spectator count</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.USER_COUNT_CHANGE, OnUserCountChange);
		/// 			
		/// 			void OnUserCountChange(BaseEvent evt) {
        /// 				Room room = (Room)evt.Params["room"];
        /// 				int uCount = (int)evt.Params["uCount"];
        /// 				int sCount = (int)evt.Params["sCount"];
		/// 
		/// 				Console.WriteLine("Room: " + room.Name + " contains " + uCount + " users and " + sCount + " spectators");
		/// 			}
		/// 			</code>
		/// </example>
		static const string* USER_COUNT_CHANGE;

		/// <summary>
		/// The Event is fired when a User sends a public message.
		/// </summary>
		/// <remarks>
		/// Also the User receives this event every time he sends a public message. 
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the target Room</description>
		///   </item>
		///   <item>
		///     <term>sender</term>
		///     <description>(<b>User</b>) the sender</description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>String</b>) the message</description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) an object with custom data</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.PUBLIC_MESSAGE, OnPublicMessage);
		/// 			
		/// 			void OnPublicMessage(BaseEvent evt) {
        /// 				User sender = (User)evt.Params["sender"];
        /// 				Console.WriteLine(sender.Name + ": " + evt.Params["message"]);
		/// 			}
		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.PublicMessageRequest"/>
		static const string* PUBLIC_MESSAGE;

		/// <summary>
		/// The Event is fired when a User receives a private message.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>sender</term>
		///     <description>(<b>User</b>) the sender of the message </description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>String</b>) the message </description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) an object with custom data </description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.PRIVATE_MESSAGE, OnPrivateMessage);
		/// 			
		/// 			void OnPrivateMessage(BaseEvent evt) {
        /// 				User sender = (User)evt.Params["sender"];
        /// 				Console.WriteLine("PM: " + (string)evt.Params["message"] + ", from: " + sender.Name);
		/// 			}
		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.PrivateMessageRequest"/>
		static const string* PRIVATE_MESSAGE;

		/// <summary>
		/// The Event is fired when a User receives a Moderator message.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>sender</term>
		///     <description>(<b>User</b>) the sender of the message </description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>String</b>) the message </description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) an object with custom data</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.MODERATOR_MESSAGE, OnModeratorMessage);
		/// 			
		/// 			void OnModeratorMessage(BaseEvent evt) {
        /// 				User sender = (User)evt.Params["sender"];
        /// 				Console.WriteLine("Moderator says: " + (string)evt.Params["message"] + ", from: " + sender.Name);
		/// 			}
		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.ModeratorMessageRequest"/>
		static const string* MODERATOR_MESSAGE;

		/// <summary>
		/// The Event is fired when a User receives an Admin message.
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>sender</term>
		///     <description>(<b>User</b>) the sender of the message  </description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>String</b>) the message</description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) an object with custom data </description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.ADMIN_MESSAGE, OnAdminMessage);
		/// 			
		/// 			void OnAdminMessage(BaseEvent evt) {
        /// 				Console.WriteLine("Admin says: " + (string)evt.Params["message"]);
		/// 			}
		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.AdminMessageRequest"/>
		static const string* ADMIN_MESSAGE;

		/// <summary>
		/// This Event is received when a User has sent an ObjectMessage request
		/// </summary>
		/// <remarks>
		/// The object sent by the sender can contain custom data, such as a game move etc...
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>sender</term>
		///     <description>(<b>User</b>) the sender of the message  </description>
		///   </item>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>ISFSObject</b>) the object data </description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// The following example shows how to handle the event
 		/// 			<code>
        /// 			smartFox.AddEventListener(SFSEvent.OBJECT_MESSAGE, OnObjectMessage);
 		/// 			
 		/// 			void OnObjectMessage(BaseEvent evt) {
        /// 				ISFSObject obj = (SFSObject)evt.Params["message"];
 		/// 				// dumps the data object
 		/// 				Console.WriteLine("Received data: " + obj.GetDump());
 		/// 			}
 		/// 			</code>
		/// </example>
        /// <seealso cref="Requests.ObjectMessageRequest"/>
 		static const string* OBJECT_MESSAGE;

		/// <summary>
		/// The Event is fired every time a server-side Extension sends a message, usually in response to an ExtensionRequest. 
		/// </summary>
		/// <remarks>
		/// A server Extension can send messages at any moment, not just in response to a specific client request
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>cmd</term>
		///     <description>(<b>String</b>) the name of the "command" (or "action id") associated with the message </description>
		///   </item>
		///   <item>
		///     <term>sourceRoom</term>
		///     <description>(<b>int</b>) The id of the room where the server extension is attached to.</description>
		///   </item>
		///   <item>
		///     <term>params</term>
		///     <description>(<b>ISFSObject</b>) custom parameters</description>
		///   </item>
		///   <item>
		///     <term>packetId</term>
		///     <description>(<b>long</b>) (optional) the id of the packet for an UDP message, the id is an auto increment value sent by the server. It can be used to detect out of order UDP packets.</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.ExtensionRequest"/>
		/// <seealso cref="Entities.Data.SFSObject"/>
		static const string* EXTENSION_RESPONSE;

		/// <summary>
		/// The Event is fired in response to a SetRoomVariablesRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room in which the event occurred</description>
		///   </item>
		///   <item>
		///     <term>changedVars</term>
		///     <description>(<b>Array</b>) the list of variable names that where modified or created</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <seealso cref="Requests.SetRoomVariablesRequest"/>
		/// <seealso cref="Entities.Variables.SFSRoomVariable"/>
		static const string* ROOM_VARIABLES_UPDATE;

		/// <summary>
		/// The Event is fired in response to a SetUserVariablesRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User that created or modified the Variables</description>
		///   </item>
		///   <item>
		///     <term>changedVars</term>
		///     <description>(<b>Array</b>) the list of variable names that where modified or created</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.SetUserVariablesRequest"/>
        /// <seealso cref="Entities.Variables.SFSUserVariable"/>
		static const string* USER_VARIABLES_UPDATE;

		/// <summary>
		/// The Event is fired in response to a SubscribeRoomGroupRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>groupId</term>
		///     <description>(<b>String</b>) the name of the group that was subscribed</description>
		///   </item>
		///   <item>
		///     <term>newRooms</term>
		///     <description>(<b>Array</b>) the list of rooms contained in the new group </description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.SubscribeRoomGroupRequest"/>
		static const string* ROOM_GROUP_SUBSCRIBE;

		/// <summary>
		/// The Event is fired in response to an UnsubscribeRoomGroupRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>groupId</term>
		///     <description>(<b>String</b>) the name of thr group that was unsubscribed</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.UnsubscribeRoomGroupRequest"/>
		static const string* ROOM_GROUP_UNSUBSCRIBE;

		/// <summary>
		/// The Event is fired in response to a failed SubscribeRoomGroupRequest
		/// </summary>
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
        /// <seealso cref="Requests.SubscribeRoomGroupRequest"/>
		static const string* ROOM_GROUP_SUBSCRIBE_ERROR;

		/// <summary>
		/// The Event is fired in response to a failed UnsubscribeRoomGroupRequest
		/// </summary>
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
        /// <seealso cref="Requests.UnsubscribeRoomGroupRequest"/>
		static const string* ROOM_GROUP_UNSUBSCRIBE_ERROR;

		/// <summary>
		/// The Event is fired in response to a successful SpectatorToPlayerRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room where the spectator became a player</description>
		///   </item>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User who became a player</description>
		///   </item>
		///   <item>
		///     <term>playerId</term>
		///     <description>(<b>int</b>) the new playerId of the User</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.SpectatorToPlayerRequest"/>
		static const string* SPECTATOR_TO_PLAYER;

		/// <summary>
		/// The Event is fired in response to a failed PlayerToSpectatorRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room where the player became a spectator</description>
		///   </item>
		///   <item>
		///     <term>user</term>
		///     <description>(<b>User</b>) the User who became a spectator</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.PlayerToSpectatorRequest"/>
        static const string* PLAYER_TO_SPECTATOR;

		/// <summary>
		/// The Event is fired in response to a failed SpectatorToPlayerRequest
		/// </summary>
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
        /// <seealso cref="Requests.SpectatorToPlayerRequest"/>
		static const string* SPECTATOR_TO_PLAYER_ERROR;

		/// <summary>
		/// The Event is fired in response to a failed UnsubscribeRoomGroupRequest
		/// </summary>
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
        /// <seealso cref="Requests.PlayerToSpectatorRequest"/>
		static const string* PLAYER_TO_SPECTATOR_ERROR;

		/// <summary>
		/// The Event is fired in response to a ChangeRoomNameRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room where the change occurred</description>
		///   </item>
		///   <item>
		///     <term>oldName</term>
		///     <description>(<b>String</b>) the old Room name</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.ChangeRoomNameRequest"/>
		static const string* ROOM_NAME_CHANGE;

		/// <summary>
		/// The Event is fired in response to a failed ChangeRoomNameRequest
		/// </summary>
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
        /// <seealso cref="Requests.ChangeRoomNameRequest"/>
		static const string* ROOM_NAME_CHANGE_ERROR;

		/// <summary>
		/// The Event is fired in response to a ChangeRoomPasswordStateRequest.
		/// </summary>
		/// <remarks>
		/// In particular this event when the <b>password state</b> is changed. For example when a non-password protected Room is assigned a password, or vice-versa 
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room where the change occurred</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.ChangeRoomPasswordStateRequest"/>
		static const string* ROOM_PASSWORD_STATE_CHANGE;

		/// <summary>
		/// The Event is fired in response to a failed ChangeRoomPasswordStateRequest
		/// </summary>
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
        /// <seealso cref="Requests.ChangeRoomPasswordStateRequest"/>
		static const string* ROOM_PASSWORD_STATE_CHANGE_ERROR;

		/// <summary>
		/// The Event is fired in response to a ChangeRoomCapacityRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>room</term>
		///     <description>(<b>Room</b>) the Room where the change occurred</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.ChangeRoomCapacityRequest"/>
		static const string* ROOM_CAPACITY_CHANGE;

		/// <summary>
		/// The Event is fired in response to a failed ChangeRoomCapacityRequest
		/// </summary>
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
        /// <seealso cref="Requests.ChangeRoomCapacityRequest"/>
		static const string* ROOM_CAPACITY_CHANGE_ERROR;

		/// <summary>
		/// The Event is fired in response to a FindRoomsRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>rooms</term>
		///     <description>(<b>List&lt;Room&gt;</b>) a list of Rooms matching the request criteria. If no Room was found the list will be empty.</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.FindRoomsRequest"/>
		static const string* ROOM_FIND_RESULT;

		/// <summary>
		/// The Event is fired in response to a FindUsersRequest
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>rooms</term>
		///     <description>(<b>Array</b>) a list of Users matching the request criteria. If no User was found the list will be empty.</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.FindUsersRequest"/>
		static const string* USER_FIND_RESULT;

		/// <summary>
		/// The Event is fired when another User has sent you an Invitation
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>invitation</term>
		///     <description>(<b>Invitation</b>) the invitation object</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Requests.Game.InviteUsersRequest"/>
		/// <seealso cref="Entities.Invitation.SFSInvitation"/>
		static const string* INVITATION;

		/// <summary>
		/// The Event is fired when a User sends a reply to your Invitation
		/// </summary>
		/// <remarks>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>invitee</term>
		///     <description>(<b>User</b>) the User that was invited</description>
		///   </item>
		///   <item>
		///     <term>reply</term>
		///     <description>(<b>int</b>) the <i>reply code</i>. It can be either InvitationReply.ACCEPT or InvitationReply.REFUSE</description>
		///   </item>
		///   <item>
		///     <term>data</term>
		///     <description>(<b>ISFSObject</b>) additional custom paramters (e.g. a reason for the refusal, a message etc...)</description>
		///   </item>
		/// </list>
		/// </remarks>
        /// <seealso cref="Entities.Invitation.InvitationReply"/>
        /// <seealso cref="Requests.Game.InviteUsersRequest"/>
		static const string* INVITATION_REPLY;

		/// <summary>
		/// The Event is fired in response to a failed InvitationReplyRequest
		/// </summary>
		/// <remarks>
		/// The failure is usually caused by a late reply which doesn't make it to the server within the expected time
		/// <para/>
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
		/// 
        /// <seealso cref="Requests.Game.InvitationReplyRequest"/>
		/// <seealso cref="Requests.Game.InviteUsersRequest"/>
		static const string* INVITATION_REPLY_ERROR;

		/// <summary>
		/// Event fired for all log messages 
		/// </summary>
		/// <remarks>
		/// Unity API only
		/// <para/>
		/// The <see cref="BaseEvent.Params"/> object contains the following parameters.
		/// <list type="table">
		///   <listheader>
		///     <term>parameter</term>
		///     <description>description</description>
		///   </listheader>
		///   <item>
		///     <term>message</term>
		///     <description>(<b>String</b>) the log message</description>
		///   </item>
		/// </list>
		/// </remarks>
		/// <example>
		/// <code>
		/// 	smartFox.AddLogListener(LogLevel.DEBUG, OnDebugMessage);
		/// 
		/// 	void OnDebugMessage(BaseEvent evt) {
		///			string message = (string)evt.Params["message"];
		///			Console.WriteLine("[SFS DEBUG] " + message);
		///		}
		/// </code>
		/// </example>
		static const string* DEBUG_MESSAGE;

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
