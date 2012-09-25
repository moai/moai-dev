--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- apsalar.lua - version 1.2 Beta
----------------------------------------------------------------
module ( "apsalar", package.seeall )

require "crypto"

--==============================================================
-- database
--==============================================================
local docDir = MOAIEnvironment.documentDirectory
if docDir == nil then
	print "apsalar is not supported on this platform"
	USE_APSALAR = false
	return
end

print "initializing apsalar"

----------------------------------------------------------------
local mEventsToSend = nil
local mPreStartEvents = {}
local mWaitForCallback = false
local mSendingThread = nil
local EVENT_FILE_NAME = "apsalar-events.lua"
local mBackoff = 2
local mBackoffWait = 0

--==============================================================
-- apsalar api
--==============================================================
local BASE_API_URI = "http://e.apsalar.com/api/v1/"
local API_STATUS_OK = "ok"

local mApiKey
local mApiSecret
local mSessionId
local mSessionStartTime
local mOSBrand
local mCanonicalUID
local mCanonicalKeySpace
local mKnownIDs = {}

----------------------------------------------------------------
local function addSecurityHash ( queryString, apiSecret )

	if apiSecret then
		local hash = crypto.evp.digest ( "sha1", apiSecret .. "?" .. queryString )
		queryString = queryString .. "&h=" .. hash
		return queryString
	else
		local hash = crypto.evp.digest ( "sha1", mApiSecret .. "?" .. queryString )
		queryString = queryString .. "&h=" .. hash
		return queryString
	end
end

----------------------------------------------------------------
local function increaseBackoff ()
	
	mBackoff = mBackoff * mBackoff
end

----------------------------------------------------------------
local function resetBackoff ()

	mBackoff = 2
	mBackoffWait = mBackoff
end

----------------------------------------------------------------
local function waitForBackoff ()

	if mBackoffWait > 0 then
		mBackoffWait = mBackoffWait - 1
		return false
	end

	mBackoffWait = mBackoff
	return true
end

----------------------------------------------------------------
function loadEvents ( filename )

	local workingDir
	local events
	
	if DEVICE then
		workingDir = MOAIFileSystem.getWorkingDirectory () 
		MOAIFileSystem.setWorkingDirectory ( MOAIEnvironment.documentDirectory )
	end
	
	if MOAIFileSystem.checkFileExists ( filename ) then
		
		events = dofile ( filename )
	end

	if DEVICE then		
		MOAIFileSystem.setWorkingDirectory ( workingDir )
	end
	
	if events then return events.data else return nil end
end

----------------------------------------------------------------
function saveEvents ( events, filename )

	local workingDir
	local serializer = MOAISerializer.new ()
	local saveData = dofile ( "events-template.lua" )
	saveData.data = events 
	serializer:serialize ( saveData )
	local eventStr = serializer:exportToString ( )
	
	if not DEVICE then
		
		local file = assert ( io.open ( filename, 'wb' ) )
		file:write ( eventStr )
		file:close ()
		
	else
		workingDir = MOAIFileSystem.getWorkingDirectory () 
		MOAIFileSystem.setWorkingDirectory ( MOAIEnvironment.documentDirectory )		
		
		local file = assert ( io.open ( filename, 'wb' ) )
		file:write ( eventStr )
		file:close ()
		
		MOAIFileSystem.setWorkingDirectory ( workingDir )
	end
end

--------------------------------
local function sendEvent ( record, connType )
	
	record.query = record.query .. "&c=" .. connType

	-- update lag time for events that are not the start event
	if record.name ~= "" and record.name ~= "start" then
		record.query = record.query .. "&lag=" .. os.time () - record.curTime
	end

	local hashedQuery = addSecurityHash ( record.query )

	local sendCallback = function ( httpTask )
		
		local result = MOAIJsonParser.decode ( httpTask:getString ())
		if result and result.status == API_STATUS_OK then
			resetBackoff ()
			table.remove ( mEventsToSend, 1 )			
		else
		--	if httpTask:getResponseCode () ~= 500 or httpTask:getResponseCode () ~= 504 then
				increaseBackoff ()				
		--	end
		end
		mWaitForCallback = false
	end		
	
	local task = MOAIHttpTask.new ()
	task.name = record.name
	task.url = record.url .. hashedQuery
	task:setCallback ( sendCallback )
	task:setTimeout ( 0 )
	task:httpGet ( record.url .. hashedQuery )

end

--------------------------------
local function sendThreadFunction ()

	while true do
		
		coroutine.yield ()
		-------------------------------------

		-- update connection type for all events
		local connectionType = MOAIEnvironment.connectionType
		local connectionTypeStr

		if connectionType == MOAIEnvironment.CONNECTION_TYPE_WIFI then
	
			connectionTypeStr = "wifi"
	
		elseif connectionType == MOAIEnvironment.CONNECTION_TYPE_WWAN then
	
			connectionTypeStr = "wwan"

		elseif connectionType == MOAIEnvironment.CONNECTION_TYPE_NONE or connectionType == nil then
	
			print "no connection (MOAIEnvironment.getConnectionType returned CONNECTION_TYPE_NONE)"
			return
		end
		
		-------------------------------------
		if waitForBackoff () then
		
			if #mEventsToSend > 0 and not mWaitForCallback then
				sendEvent ( mEventsToSend [ 1 ], connectionTypeStr )
				mWaitForCallback = true
			end
		end
	end
end

--==============================================================
-- end session api call
--==============================================================
function endSession ( )
	
	if mSendingThread then mSendingThread:stop () end
	
	-- save off any remaining events
	if mEventsToSend and #mEventsToSend > 0 then
		
		saveEvents ( mEventsToSend, EVENT_FILE_NAME )
	end
	
end

--==============================================================
-- event api call
--==============================================================
function event ( name, data )

	local preStart = false
	if DEVICE and not mCanonicalUID then return end
	-- validate call
	if not mApiKey or not mApiSecret then
		preStart = true
	end
		
	-- build url
	local curTime = os.time ()
	local jsonEncodedData = MOAIJsonParser.encode ( data )

	local params = {
		a 	= mApiKey,
		e   = jsonEncodedData,
		i 	= MOAIEnvironment.appID,
		n 	= name,
		p	= MOAIEnvironment.osBrand,
		rt 	= "json",
		s 	= mSessionId,
		t 	= curTime - mSessionStartTime,
		u 	= mCanonicalUID,
		k 	= mCanonicalKeySpace
	}
			
	if ANDROID then
		params.ab = MOAIEnvironment.cpuabi
        params.br = MOAIEnvironment.devBrand 
        params.de = MOAIEnvironment.devName
        params.ma = MOAIEnvironment.devManufacturer
        params.mo = MOAIEnvironment.devModel
        params.pr = MOAIEnvironment.devProduct
	end
	
	local queryString = cloud.encode ( params )
	
	--queryString = addSecurityHash ( queryString )	
	
	local url = BASE_API_URI .. "event?"
	
	local event = {}		
	event.name = name
	event.curTime = curTime
	--event.jsonEncodedData = data
	--event.mSessionId = mSessionId
	event.url = url
	event.query = queryString 
	
	if preStart then
		table.insert ( mPreStartEvents, event )
	else
		table.insert ( mEventsToSend, event )
	end
end

--==============================================================
-- start api call
--==============================================================
function start ( apiKey, apiSecret, ids )

	-- validate parameters
	if apiKey == nil then
		error ( "apsalar.start is missing required \"apiKey\" parameter", 2 )
	end

	if apiSecret == nil then
		error ( "apsalar.start is missing required \"apiSecret\" parameter", 2 )
	end

	-- init module level variables
	mApiKey = apiKey
	mApiSecret = apiSecret
	
	if not ids then
		mSessionStartTime = os.time ()
		mSessionId = MOAIEnvironment.generateGUID ()
	end
		
	-- build url	
	local params = {
		a 	= mApiKey,
		av 	= MOAIEnvironment.appVersion,
		i 	= MOAIEnvironment.appID,
		n	= MOAIEnvironment.appDisplayName,
		p	= MOAIEnvironment.osBrand,
		rt 	= "json",
		s 	= mSessionId,
		u 	= mCanonicalUID,	
		v 	= MOAIEnvironment.osVersion,
		k 	= mCanonicalKeySpace
	}
	
	if ids then
		
		params.dk = MOAIJsonParser.encode ( ids )
	end
	
	if ANDROID then
		params.ab = MOAIEnvironment.cpuabi
        params.br = MOAIEnvironment.devBrand 
        params.de = MOAIEnvironment.devName
        params.ma = MOAIEnvironment.devManufacturer
        params.mo = MOAIEnvironment.devModel
        params.pr = MOAIEnvironment.devProduct
	end
	
	local queryString = cloud.encode ( params )

	--queryString = addSecurityHash ( queryString )

	local url = BASE_API_URI .. "start?"
	local event = {}		
	event.name = "start"
	event.curTime = curTime
	event.jsonEncodedData = jsonEncodedData
	event.mSessionId = mSessionId
	event.url = url
	event.query = queryString
			
	-- start up apsalar 
	mEventsToSend = loadEvents ( EVENT_FILE_NAME )
	if mEventsToSend == nil then 
		mEventsToSend = {} 
	end
		
	table.insert ( mEventsToSend, event )
	
	if #mPreStartEvents > 0 then
		for k, v in pairs ( mPreStartEvents ) do
			
			table.insert ( mEventsToSend, v )
		end
		
		mPreStartEvents = {}
	end	
	
	-- Launch the event sending thread
	mSendingThread = MOAIThread.new ()
	mSendingThread:run ( sendThreadFunction )
end

--==============================================================
-- resolve and bind api calls
--==============================================================
local function bindCallback ( task )

	local result = MOAIJsonParser.decode ( task:getString ())
	if result then
		if result.status ~= "ok" then
			print "error in bind!"
			util.printTable ( result )
		else 
			
		end
	end
end

local function bindSHA1 ( canID, canKey )
	
	-- mac sha1 first
	-- build url	
	local params = {
		a 	= mApiKey,
		k 	= "MAC1",
		p	= MOAIEnvironment.osBrand,
		u	= crypto.evp.digest ( "sha1", MOAIEnvironment.getMACAddress ()),
		v 	= MOAIEnvironment.osVersion,
		rt 	= "json",
		ck	= canKey,
		ci 	= canID
	}
	
	local queryString = cloud.encode ( params )
	queryString = addSecurityHash ( queryString, apiSecret )
		
	local url = BASE_API_URI .. "bind?" .. queryString	
	local task = MOAIHttpTask.new ()
	task:setCallback ( bindCallback )
	task:setFollowRedirects ( true )
	task:httpGet ( url )
	
	table.insert ( mKnownIDs, { k = "MAC1", v = params.u })
end

local function bindMP5 ( canID )
	
	-- mac sha1 first
	-- build url	
	local params = {
		a 	= mApiKey,
		k 	= "MAC5",
		p	= MOAIEnvironment.osBrand,
		u	= crypto.evp.digest ( "md5", MOAIEnvironment.getMACAddress ()),
		v 	= MOAIEnvironment.osVersion,
		rt 	= "json",
		ck	= canKey,
		ci 	= canID
	}
	
	local queryString = cloud.encode ( params )
	queryString = addSecurityHash ( queryString, apiSecret )
		
	local url = BASE_API_URI .. "bind?" .. queryString	
	local task = MOAIHttpTask.new ()
	task:setCallback ( bindCallback )
	task:setFollowRedirects ( true )
	task:httpGet ( url )

	table.insert ( mKnownIDs, { k = "MAC5", v = params.u })
end

function resolve ( apiKey, apiSecret )
	
	-- validate parameters
	if apiKey == nil then
		error ( "apsalar.start is missing required \"apiKey\" parameter", 2 )
	end

	if apiSecret == nil then
		error ( "apsalar.start is missing required \"apiSecret\" parameter", 2 )
	end
	mApiKey = apiKey
	mApiSecret = apiSecret
	mSessionStartTime = os.time ()
	mSessionId = MOAIEnvironment.generateGUID ()
	
	local keySpace = ( MOAIEnvironment.osBrand == "iOS" ) and "OPNU" or "UDID"
	local id = ( MOAIEnvironment.osBrand == "iOS" ) and MOAIEnvironment.openUdid or MOAIEnvironment.udid
		
	-- build url	
	local params = {
		a 	= mApiKey,
		k 	= keySpace,
		p	= MOAIEnvironment.osBrand,
		u	= id,
		v 	= MOAIEnvironment.osVersion,
		rt 	= "json"
	}
	
	local queryString = cloud.encode ( params )
	queryString = addSecurityHash ( queryString, apiSecret )
	
	----------------------
	local function resolveCallback ( task )
	
		local result = task:getString ()
		if result then
			local returnVal = MOAIJsonParser.decode ( result )
			if returnVal.u then 
				mCanonicalUID = returnVal.u 
				mCanonicalKeySpace = returnVal.k
			end
			
			bindSHA1 ( mCanonicalUID, mCanonicalKeySpace )
			bindMP5 ( mCanonicalUID, mCanonicalKeySpace )
			apsalar.start ( mApiKey, mApiSecret, mKnownIDs )
			apsalar.event ( "Slots Tycoon session start" )
		else
			print ( "resolve nil-ed out", task:getResponseCode ())
			if task:getResponseCode () == 500 then
				resolve ( apiKey, apiSecret )
			end
		end
	end	
	
	local url = BASE_API_URI .. "resolve?" .. queryString	
	local task = MOAIHttpTask.new ()
	task.record = record
	task:setCallback ( resolveCallback )
	task:setFollowRedirects ( true )
	task:httpGet ( url )
	
	
end

function setConID ( idTable )
	util.printTable ( idTable )
	mCanonicalUID = idTable.id
	mCanonicalKeySpace = idTable.key
end

function getConID ()
	if mCanonicalUID == nil then return nil end
	local idTable = { id = mCanonicalUID, key = mCanonicalKeySpace }
	return idTable
end