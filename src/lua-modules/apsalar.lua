--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- apsalar.lua - version 1.0 Beta
----------------------------------------------------------------
module ( ..., package.seeall )

require "crypto"
require "luasql"
require "url"
require "util"

--==============================================================
-- database
--==============================================================
local DB_PATH = MOAIEnvironment.getDocumentDirectory () .. "/apsalar.sqlite3"
local needNewDatabase = not util.fileExists ( DB_PATH )

----------------------------------------------------------------
local function closeConnection ( ... )

	for i = 1, #arg do
		arg[ i ]:close ()
	end
end

----------------------------------------------------------------
local function connectToDb ()

	local sqlEnv = luasql.sqlite3 ()
	local sqlCon = sqlEnv:connect ( DB_PATH )
	sqlCon:setautocommit ( true )
	return sqlEnv, sqlCon
end

----------------------------------------------------------------
local function makeSqliteRecord ( sqliteTable )

	local sqliteRecord = {}
	local columns = {}
	
	--------------------------------
	sqliteRecord.insert = function ( self, sqlCon )

		local sqlStatement = "INSERT INTO '" .. sqliteTable:getName () .. "' ("

		for i,v in ipairs ( columns ) do
			sqlStatement = sqlStatement .. " \"" .. v.columnName .. "\","
		end
		
		sqlStatement = string.sub ( sqlStatement, 1, #sqlStatement - 1 ) .. ") VALUES ("
		
		for i,v in ipairs ( columns ) do
			if not (v.value == nil) then
				if v.columnName == "data" then
					v.value, num = v.value:gsub ("'", "")		
				end
				sqlStatement = sqlStatement .. " '" .. v.value .. "',"
			else
				sqlStatement = sqlStatement .. " '" .. "" .. "',"
			end
		end
		
		sqlStatement = string.sub ( sqlStatement, 1, #sqlStatement - 1 ) .. " )"
		sqlCon:execute ( sqlStatement )
	end
	
	--------------------------------
	sqliteRecord.setColumn = function ( self, columnName, value )
	
		table.insert ( columns, { columnName = columnName, value = value })
	end

	--------------------------------
	return sqliteRecord
end

----------------------------------------------------------------
local function makeSqliteTable ( name )

	local sqliteTable = {}
	local columns = {}

	--------------------------------
	sqliteTable.addColumn = function ( self, name, dataType )
	
		table.insert ( columns, { name = name, dataType = dataType })
	end
	
	--------------------------------
	sqliteTable.addRecord = function ( self )
	
		return makeSqliteRecord ( self )
	end
	
	--------------------------------
	sqliteTable.create = function ( self, sqlCon )
	
		local sqlStatement = "CREATE TABLE \"" .. name .. "\" ("
	
		for i,v in ipairs ( columns ) do
			sqlStatement = sqlStatement .. " \"" .. v.name .. "\" \"" .. v.dataType .. "\"," 
		end
		
		sqlStatement = string.sub ( sqlStatement, 1, #sqlStatement - 1 ) .. " )"
		
		sqlCon:execute ( sqlStatement )
	end
	
	--------------------------------
	sqliteTable.getName = function ( self )
	
		return name
	end
	
	--------------------------------
	return sqliteTable
end

----------------------------------------------------------------
local sqlEventsTable = makeSqliteTable ( "events" )

if needNewDatabase then

	-- open db connection
	local sqlEnv, sqlCon = connectToDb ()

	-- create events table
	sqlEventsTable:addColumn ( "name", "TEXT" )
	sqlEventsTable:addColumn ( "data", "TEXT" )
	sqlEventsTable:addColumn ( "time", "INTEGER" )
	sqlEventsTable:addColumn ( "sessionid", "TEXT")
	sqlEventsTable:addColumn ( "url", "TEXT" )
	sqlEventsTable:create ( sqlCon )
		
	-- close db connection
	closeConnection ( sqlCon, sqlEnv )
end

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

----------------------------------------------------------------
local function addSecurityHash ( queryString )

	local hash = crypto.evp.digest ( "sha1", mApiSecret .. "?" .. queryString )
	queryString = queryString .. "&h=" .. hash
	return queryString
end

--==============================================================
-- event queue functions
--==============================================================
local queueSingleton

----------------------------------------------------------------
local function makeQueue ()

	local queue = {}
	local EVENT_QUEUE_SELECT_SQL = "SELECT rowid, name, data, time, sessionid, url FROM events ORDER BY rowid"
	queue.numToSend = 0
	queue.waitingForCallback = false
	
	--------------------------------
	local function cleanUp ()
	
		closeConnection ( queue.sqlCur, queue.sqlCon, queue.sqlEnv )
		
		queue.active = false
		queue.sqlCur = nil
		queue.sqlCon = nil
		queue.sqlEnv = nil
		queue.numToSend = 0
		queue.waitingForCallback = false
	end
	
	--------------------------------
	local function finish ()
		
		if util.isFunc ( queue.finishCallback ) then
			queue.finishCallback ()
		end
		
		cleanUp ()
	end
	
	--------------------------------
	local function sendCallback ( task )

		if not queue.active then
			return
		end
		queue.waitingForCallback = false
		
		-- parse json response
		local result = MOAIJsonParser.decode ( task:getString () )
		
		if result and ( result.status == API_STATUS_OK ) then
		
			-- remove sent record from db
			queue.sqlCon:execute ( "DELETE FROM events WHERE rowid = " .. tostring ( task.record.rowid ))

			-- call progress callback
			if util.isFunc ( queue.progressCallback ) then
				queue.progressCallback ( task.record )
			end
			
			-- send next item, or clean up if we're only sending one
			queue.numToSend = queue.numToSend - 1
			if queue.numToSend > 0 then
				queue:send ( queue.numToSend )
			else
				finish ()
			end
		else
			
			-- call failure callback
			if util.isFunc ( queue.failureCallback ) then
				queue.failureCallback ( task.record, result )
			end
		end	
	end
		
	--------------------------------
	queue.getRecordCount = function ( self )
		
		if not self.sqlCur then
			self.sqlEnv, self.sqlCon = connectToDb ()
			self.sqlCur = self.sqlCon:execute ( "SELECT COUNT ( rowid ) FROM events" )
		else
			self.sqlCur:close ()
			self.sqlCur = self.sqlCon:execute ( "SELECT COUNT ( rowid ) FROM events" )
		end
		
		local countRecord = {}
		self.sqlCur:fetch ( countRecord, "n" )
		self.sqlCur:close ()
		self.sqlCur = nil
		
		return countRecord [ 1 ]
	end
	
	--------------------------------
	queue.send = function ( self, numRecords )
			
		if self.waitingForCallback then
			
			if numRecords then 
				self.numToSend = self.numToSend + numRecords 
			else
				self.numToSend = self.numToSend + self.getRecordCount ( self )
			end
			
			return
		end
		
		-- figure out how many items we are sending
		if not numRecords then
			self.numToSend = self.getRecordCount ( self )
		else
			self.numToSend = numRecords
		end
		
		-- either use the provided cursor, or create a new one
		if not self.sqlCur then
			self.sqlEnv, self.sqlCon = connectToDb ()
			self.sqlCur = self.sqlCon:execute ( EVENT_QUEUE_SELECT_SQL )
			self.active = true
		end
				
		-- get record from db
		local record = {}

		if not self.sqlCur:fetch ( record, "a" ) then
			finish ()
			return
		end
		
		-- update connection type for all events
		local connectionType = MOAIEnvironment.getConnectionType ()
		local connectionTypeStr
		
		if connectionType == MOAIEnvironment.CONNECTION_TYPE_WIFI then
			
			connectionTypeStr = "wifi"
			
		elseif connectionType == MOAIEnvironment.CONNECTION_TYPE_WWAN then
			
			connectionTypeStr = "wwan"
		
		elseif connectionType == MOAIEnvironment.CONNECTION_TYPE_NONE then
			
			if util.isFunc ( queue.failureCallback ) then
				queue.failureCallback ( record, "no connection (MOAIEnvironment.getConnectionType returned CONNECTION_TYPE_NONE)" )
			end
			
			return
		end
		
		record.url = record.url .. "&c=" .. connectionTypeStr
		
		-- update lag time for events that are not the start event
		if record.name ~= "start" then
			record.url = record.url .. "&lag=" .. os.time () - record.time
		end
		
		-- call api via httpTask
		self.waitingForCallback = true
		
		local task = MOAIHttpTask.new ()
		task.record = record
		task:setCallback ( sendCallback )
		task:httpGet ( record.url )
	end
	
	--------------------------------
	queue.getSendsRemaining = function ( self )
		
		return self.numToSend;
	end
	
	--------------------------------
	queue.setFailureCallback = function ( self, callback )
	
		self.failureCallback = callback
	end
	
	--------------------------------
	queue.setFinishCallback = function ( self, callback )
	
		self.finishCallback = callback
	end
	
	--------------------------------
	queue.setProgressCallback = function ( self, callback )
	
		self.progressCallback = callback
	end
	
	--------------------------------
	return queue
end

----------------------------------------------------------------
function getQueue ()

	queueSingleton = queueSingleton or makeQueue ()
	return queueSingleton
end

--==============================================================
-- event api call
--==============================================================
function event ( name, data )

	-- validate call
	if not mApiKey then
		error ( "apsalar.event called before apsalar.start", 2 )
	end

	-- open db connection
	local sqlEnv, sqlCon = connectToDb ()
		
	-- build url
	local curTime = os.time ()
	local jsonEncodedData = MOAIJsonParser.encode ( data )

	queryString = url.encode {
		a 	= mApiKey,
		e   = jsonEncodedData,
		i 	= MOAIEnvironment.getAppID (),
		n 	= name,
		p 	= mOSBrand,
		rt 	= "json",
		s 	= mSessionId,
		t 	= curTime - mSessionStartTime,
		u 	= MOAIEnvironment.getUDID (),
	}
	queryString = addSecurityHash ( queryString )
	local url = BASE_API_URI .. "event?" .. queryString
		
	-- insert record for the event
	local record = sqlEventsTable:addRecord ()
	record:setColumn ( "name", name )
	record:setColumn ( "time", curTime )
	record:setColumn ( "data", jsonEncodedData )
	record:setColumn ( "sessionID", mSessionId )
	record:setColumn ( "url", url )
	record:insert ( sqlCon )
	
	-- close db connection
	closeConnection ( sqlCon, sqlEnv )
end

--==============================================================
-- start api call
--==============================================================
function start ( apiKey, apiSecret )

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
	mSessionStartTime = os.time ()
	mSessionId = MOAIEnvironment.generateGUID ()
	
	local osBrand = MOAIEnvironment.getOSBrand ()
	if osBrand == MOAIEnvironment.OS_BRAND_IOS then
		mOSBrand = "iOS"
	elseif osBrand == MOAIEnvironment.OS_BRAND_ANDROID then
		mOSBrand = "Android"
	end
	
	-- open db connection
	local sqlEnv, sqlCon = connectToDb ()
	
	-- build url	
	local queryString = url.encode {
		a 	= mApiKey,
		av 	= MOAIEnvironment.getAppVersion (),
		i 	= MOAIEnvironment.getAppID (),
		n	= MOAIEnvironment.getAppDisplayName (),
		p 	= mOSBrand,
		rt 	= "json",
		s 	= mSessionId,
		u 	= MOAIEnvironment.getUDID (),
		v 	= MOAIEnvironment.getOSVersion (),
	}

	queryString = addSecurityHash ( queryString )

	local url = BASE_API_URI .. "start?" .. queryString
	
	-- insert record for the event
	local record = sqlEventsTable:addRecord ()
	record:setColumn ( "name", "start" )
	record:setColumn ( "time", mSessionStartTime )
	record:setColumn ( "data", "" )
	record:setColumn ( "sessionID", mSessionId )
	record:setColumn ( "url", url )
	record:insert ( sqlCon )
				
	-- close db connection
	closeConnection ( sqlCon, sqlEnv )
end