module ( 'util', package.seeall )

require ( 'os' )
require ( 'string' )

	dofileWithEnvironment		= nil
	getFilenameFromPath			= nil
	getFilenameExt				= nil
	getFolderFromPath			= nil
	hashFile					= nil
	iterateFiles				= nil
	listDirectories				= nil
	listFiles					= nil
	loadFileAsString			= nil
	replaceInFile				= nil
	replaceInFiles				= nil
	saveTable					= nil
	tokens						= nil
	trim						= nil
	wrap						= nil
	zip							= nil

----------------------------------------------------------------
dofileWithEnvironment = function ( filename, env, ... )

	if MOAIFileSystem.checkFileExists ( filename ) then
		local chunk = assert ( loadfile ( filename ))
		setfenv ( chunk, env or getfenv ( 2 ))
		return chunk ( ... )
	end
end

----------------------------------------------------------------
getFilenameFromPath = function ( path )

	return string.match ( path, "[/]?([^/]+)$" )
end

----------------------------------------------------------------
getFilenameExt = function ( path )
	return string.match ( path, '.([^%.\\/]*)$' ) or ''
end

----------------------------------------------------------------
getFolderFromPath = function ( path )

	return string.sub ( path, 1, string.len ( path ) - string.len ( getFilenameFromPath ( path )))
end

----------------------------------------------------------------
hashFile = function ( filename )
	
	local file = MOAIFileStream.new ()
	file:open ( filename )
	local data = file:read ()
	file:close ()
	
	local writer = MOAIHashWriter.new ()
	writer:openMD5 ()
	writer:write ( data )
	writer:close ()
	
	return writer:getHashHex ()
end

----------------------------------------------------------------
iterateFiles = function ( path, fileFilter, recurse )

	recurse = recurse or true
	
	path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
	
	local match = function () return true end
	
	if type ( fileFilter ) == 'string' then
		match = function ( filename ) return string.find ( filename, fileFilter ) end
	end
	
	if type ( fileFilter ) == 'table' then
		match = function ( filename )
			for i, p in ipairs ( fileFilter ) do
				if string.find ( filename, p ) then return true end
			end
		end
	end
	
	local doRecurse
	doRecurse = function ( dirname )
		
		local files = MOAIFileSystem.listFiles ( path .. dirname ) or {}
		for i, filename in ipairs ( files ) do
			if match ( filename ) then
				coroutine.yield ( string.format ( '%s%s', dirname, filename ))
			end
		end
		
		if recurse then
			local dirs = MOAIFileSystem.listDirectories ( path .. dirname ) or {}
			for i, subdir in ipairs ( dirs ) do
				doRecurse ( string.format ( '%s%s/', dirname, subdir ))
			end
		end
	end
	
	local co = coroutine.create ( function ()
		doRecurse ( '' )
	end )
	
	return function ()
		local status, result = coroutine.resume ( co )
		return status and result or nil
	end
end

----------------------------------------------------------------
listDirectories = function ( path )
	
	return MOAIFileSystem.listDirectories ( path )
end

----------------------------------------------------------------
listFiles = function ( path, ... )

	local extensions = { ... }

	local files = MOAIFileSystem.listFiles ( path )	
	assert ( files, string.format ( 'listFiles (): NO SUCH PATH %s', path ))
	
	if #extensions > 0 then
	
		for i, extension in ipairs ( extensions ) do
			extensions [ i ] = string.format ( '.%s$', extension )
		end
	
		local filtered = {}
		for i, filename in ipairs ( files ) do
			for j, extension in ipairs ( extensions ) do
				if string.find ( filename, extension ) then
					table.insert ( filtered, filename )
				end
			end
		end
		return filtered
	end

	return files
end

----------------------------------------------------------------
loadFileAsString = function ( filename )
	
	local str
	
	local fp = io.open ( filename, 'r' )
	if fp then
		str = fp:read ( "*all" )
		fp:close ()
	end
	
	return str
end

----------------------------------------------------------------
replaceInFile = function ( filename, commands )

	local fp = io.open ( filename, "r" )
	local str = fp:read ( "*all" )
	fp:close ()
	
	local dirty = false
 
	for find, replace in pairs ( commands ) do
		local n
		str, n = string.gsub ( str, find, replace )
		dirty = dirty or n > 0
	end

	if dirty == true then
		fp = io.open ( filename, "w+" )
		fp:write ( str )
		fp:close ()
	end
end

----------------------------------------------------------------
replaceInFiles = function ( files )

	for key, commands in pairs ( files ) do
		
		local iter
		
		if type ( key ) == 'string' then
			iter = function ()
				return pairs ({[ key ] = key })
			end
		end
		
		if type ( key ) == 'function' then
			iter = key
		end
		
		if iter then
			for filename in iter () do
				replaceInFile ( filename, commands )
			end
		end
	end
end

----------------------------------------------------------------
saveTable = function ( filename, table, raw )
	
	local bytes = MOAISerializer.serializeToString ( table )
	
	if not raw then
		bytes = string.dump ( loadstring ( bytes, '' )) -- compile to lua bytecode
	end
	
	local fileStream = MOAIFileStream.new ()
	
	fileStream:open ( filename, MOAIFileStream.READ_WRITE_NEW )
	fileStream:write ( bytes )
	fileStream:close ()
end

----------------------------------------------------------------
tokens = function ( s, patterns )

	if type ( patterns ) == 'string' then
		patterns = { patterns }
	end

	local start, finish, id

	local nextCapture = function ()
	
		if s and start == nil then
	
			for i, pattern in ipairs ( patterns ) do
			
				local ts, tf = string.find ( s, pattern )
				if ts and (( start == nil ) or ( ts < start )) then
					start, finish = ts, tf
					id = i
				end
		
				if start == 1 then break end
			end
		end
		
		if start then
			if start > 1 then
				local result = string.sub ( s, 1, start - 1 )
				s = string.sub ( s, start )
			
				finish = finish - start + 1
				start = 1
			
				return result
			end

			local result = string.sub ( s, 1, finish )
			s = string.sub ( s, finish + 1 )
			start, finish = nil, nil
			return result, id
		end
		
		local result = s
		s = nil
		return result
	end
	
	return nextCapture
end

----------------------------------------------------------------
function trim ( str, p1, p2 )

	local wsp = '[%s]*'

	p1 = p1 or wsp
	p2 = p2 or wsp

	local s, f
	
	if p1 ~= '' then
		s, f = string.find ( str, '^' .. p1 )
		if s then str = string.sub ( str, f + 1 ) end
	end
	
	if p2 ~= '' then
		s, f = string.find ( str, p2 .. '$' )
		if s then str = string.sub ( str, 1, s - 1 ) end
	end
	
	return str
end

----------------------------------------------------------------
wrap = function ( f, ... )
	
	local params = { ... }

	return function ()
		return f ( unpack ( params ))
	end
end
