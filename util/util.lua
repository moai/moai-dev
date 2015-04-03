module ( 'util', package.seeall )

require ( 'os' )
require ( 'string' )

local osx = MOAIEnvironment.osBrand == 'OSX'

			arrayToSet						= nil
			compile							= nil
			copy							= nil
			copyFiles						= nil
			dofileWithEnvironment			= nil
			escape							= nil
local		exec							= nil
			getAbsoluteDirPath				= nil
			getFilenameFromPath				= nil
			getFilenameExt					= nil
			getFolderFromPath				= nil
			hash							= nil
			hashDirectory					= nil
			hashFile						= nil
			isAbsPath						= nil
			isMember						= nil
			isSysPath						= nil
			iterateCommandLine				= nil
			iterateFiles					= nil
			iterateFilesAbsPath				= nil
local  		iterateFilesImplementation		= nil
			iterateSingleOrArray			= nil
			joinTables						= nil
			listDirectories					= nil
			listFiles						= nil
local		makeDlcResourceSig				= nil
			makeExecutable					= nil
			makeStoreEntryFunc				= nil
			mergeTables						= nil
			move							= nil
			onEntryCompile					= nil
			onEntryCopy						= nil
			onEntryStore					= nil
			pack							= nil
			package							= nil
			pairsByKeys						= nil
			powerIter						= nil
			printTable						= nil
			pruneEmptyDirs					= nil
			quantize						= nil
			readFileAll						= nil
			replaceInFile					= nil
			replaceInFiles					= nil
			saveTable						= nil
			scanFiles						= nil
			tokens							= nil
			tokenize						= nil
			trim							= nil
			wrap							= nil
			zip								= nil

----------------------------------------------------------------
arrayToSet = function ( array )

	local set = {}
	for i, entry in ipairs ( array or {}) do
		set [ entry ] = true
	end
	
	return set
end

----------------------------------------------------------------
compile = function ( dstpath, srcpath )

	local should = COMPILE

	if should then
		should = not isMember ( DONT_COMPILE, srcpath )
	end

	if should then
		
		local dstDir = getFolderFromPath ( dstpath )
		MOAIFileSystem.affirmPath ( dstDir );
		
		local cmd = osx and 'luac -s -o %s %s' or '..\\util\\luac\\win\\luac -s -o %s %s'
		exec ( cmd, dstpath, srcpath )
	else
		copy ( dstpath, srcpath )
	end
end

----------------------------------------------------------------
copy = function ( dstpath, srcpath )

	print ( string.format ( 'copying: %s -> %s', srcpath, dstpath ))
	MOAIFileSystem.copy ( srcpath, dstpath )
end

----------------------------------------------------------------
copyFiles = function ( srcRoot, dstRoot, paths, process )

	for dest, source in pairs ( paths ) do
		local srcpath = srcRoot .. ( type ( source ) == 'string' and source or dest )
		local dstpath = dstRoot .. dest
		process ( dstpath, srcpath )
	end
end

----------------------------------------------------------------
dofileWithEnvironment = function ( filename, env, ... )

	if MOAIFileSystem.checkFileExists ( filename ) then
		local chunk = assert ( loadfile ( filename ))
		setfenv ( chunk, env or getfenv ( 2 ))
		return chunk ( ... )
	end
end

----------------------------------------------------------------
escape = function ( path )
	return osx and path or string.gsub ( path, '/', '\\' )
end

----------------------------------------------------------------
exec = function ( cmd, path1, path2 )

	path1 = path1 and escape ( path1 )
	path2 = path2 and escape ( path2 )

	path1 = MOAIFileSystem.getAbsoluteFilePath ( path1 )
	path2 = MOAIFileSystem.getAbsoluteFilePath ( path2 )

	cmd = string.format ( cmd, path1, path2 )
	print ( cmd )
	os.execute ( cmd )
end

----------------------------------------------------------------
getAbsoluteDirPath = function ( path, base )

	if base and not ( string.match ( path, '^/' ) or string.match ( path, '^\\' )) then
		path = base .. path
	end

	return MOAIFileSystem.getAbsoluteDirectoryPath ( path )
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
hash = function ( path )

	if MOAIFileSystem.checkPathExists ( path ) then return hashDirectory ( path ) end
	if MOAIFileSystem.checkFileExists ( path ) then return hashFile ( path ) end
end

----------------------------------------------------------------
hashDirectory = function ( path )

	path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )

	local allFiles = {}

	local scanRecurse
	scanRecurse = function ( localPath )
		
		local files = MOAIFileSystem.listFiles ( localPath ) or {}
		for k, entry in pairs ( files ) do
			local fullpath = string.format ( '%s/%s', localPath, entry )
			if not isMember ( FILTER, fullpath ) then
				table.insert ( allFiles, fullpath )
			end
		end
		
		local dirs = MOAIFileSystem.listDirectories ( localPath ) or {}
		for k, entry in pairs ( dirs ) do
			local fullpath = string.format ( '%s%s/', localPath, entry )
			if not ( isMember ( FILTER, fullpath ) or isSysPath ( fullpath )) then
				scanRecurse ( fullpath )
			end
		end
	end

	scanRecurse ( path )
	table.sort ( allFiles )

	local writer = MOAIHashWriter.new ()
	writer:openMD5 ()
	
	for i, filename in ipairs ( allFiles ) do
		local file = MOAIFileStream.new ()
		file:open ( filename )
		writer:write ( file:read ())
		file:close ()
	end
	
	writer:close ()
	
	return writer:getHashHex ()
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
isAbsPath = function ( path )

	local c = string.byte ( path )
	return ( c == 0x5C ) or ( c == 0x2F ) -- hex codes for '/' and '\'
end

----------------------------------------------------------------
isMember = function ( array, str )

	if array then
		for i, entry in ipairs ( array ) do
			if string.find ( str, entry, 1, true ) then
				return true
			end
		end
	end
	
	return false
end

----------------------------------------------------------------
isSysPath = function ( str )

	if str == '.' then return true end
	if str == '..' then return true end

	return false
end

----------------------------------------------------------------
iterateCommandLine = function ( arg )

	local commands = function ()

		local currentEscape
		local iter = powerIter ( ipairs ( arg ))

		iter.next = function ()
			local i, v = iter ()
			return v
		end

		for i, v in iter do
		
			local escape = string.match ( v, '^%-%-([%w-]+)' )
		
			if escape then
				currentEscape = escape
				coroutine.yield ( currentEscape, nil, iter )
			else
				local escapeStr = string.match ( v, '^%-(%a+)' )
		
				if escapeStr then
					for j = 1, #escapeStr do
						currentEscape = string.sub ( escapeStr, j, j )
						coroutine.yield ( currentEscape, nil, iter )
					end
				else
					coroutine.yield ( currentEscape, v, iter )
				end
			end
		end
	end
	
	local co = coroutine.wrap ( commands )
	
	local c = 0
	
	return function ()
		local escape, param, iter = co ()
		if escape or param then
			c = c + 1
			return c, escape, param, iter
		end
	end
end

----------------------------------------------------------------
iterateFiles = function ( path, fileFilter, recurse )

	return iterateFilesImplementation ( path, fileFilter, false, recurse )
end

----------------------------------------------------------------
iterateFilesAbsPath = function ( path, fileFilter )

	return iterateFilesImplementation ( path, fileFilter, true, recurse )
end

----------------------------------------------------------------
iterateFilesImplementation = function ( path, fileFilter, absPath, recurse )

	recurse = recurse or true

	path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
	local prefix = absPath and path or ''
	
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
				coroutine.yield ( string.format ( '%s%s%s', prefix, dirname, filename ))
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
iterateSingleOrArray = function ( item )

	if type ( item ) == 'table' then
		return ipairs ( item )
	end

	return function ()
		if item then
			local temp = item
			item = nil
			return 1, temp
		end 
	end
end

----------------------------------------------------------------
joinTables = function ( t1, t2 )
	
	local t = {}
	
	for i, v in ipairs ( t1 or {}) do
		table.insert ( t, v )
	end
	
	for i, v in ipairs ( t2 or {}) do
		table.insert ( t, v )
	end
	
	return t
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
makeExecutable = function ( path )
	if MOAIEnvironment.osBrand ~= 'Windows' then
		os.execute("chmod a+x "..path)
	end
end 


----------------------------------------------------------------
makeDlcResourceSig = function ( path, md5 )
	
	local path = string.gsub ( path, '[\\/]', '.' )
	return string.lower ( string.format ( '%s~%s', path, md5 ))
end

----------------------------------------------------------------
makeEnvironment = function ( env )

	env = env or {}
	setmetatable ( env, { __index = getfenv ( 2 ), __newindex = rawset })
	return env
end

----------------------------------------------------------------
makeStoreEntryFunc = function ( packingList )

	local func = function ( indir, outdir, entry )
		return onEntryStore ( packingList, indir, outdir, entry )
	end

	return func
end

----------------------------------------------------------------
mergeTables = function ( t1, t2 )

	t1 = t1 or {}
	if t2 then
		for k, v in pairs ( t2 ) do
			t1 [ k ] = v
		end
	end
	return t1
end

----------------------------------------------------------------
move = function ( dstpath, srcpath )

	local cmd = osx and 'mv -f %s %s' or 'move /y %s %s'
	exec ( cmd, srcpath, dstpath )
end

----------------------------------------------------------------
pack = function ( ... )
	return { ... }
end

----------------------------------------------------------------
package = function ( dstpath, srcpath )
	
	if string.match ( srcpath, '%.lua' ) then
		util.compile ( dstpath, srcpath )
	elseif string.match ( srcpath, '%.png' ) then
		util.quantize ( dstpath, srcpath ) 
	else
		util.copy ( dstpath, srcpath )
	end
end

----------------------------------------------------------------
pairsByKeys = function ( t, f )

	local a = {}
	for n in pairs ( t ) do table.insert ( a, n ) end
	table.sort ( a, f )

	local i = 0      -- iterator variable
	local iter = function ()   -- iterator function
		i = i + 1
		if a [ i ] == nil then
			return nil
		else
			return a [ i ], t [ a [ i ]]
		end
	end

	return iter
end

----------------------------------------------------------------
powerIter = function ( iter, state, var )

	local c = 0
	local step = 1
	local entries = {}
	local done = false
	
	local cursor
	local next
	local set
	
	cursor = function ()
		return c
	end
	
	next = function ()
		set ( c + step )
		local entry = entries [ c ]
		if entry then return unpack ( entry ) end
	end
	
	set = function ( j, s )
		
		step = s or step
		
		if j > #entries and not done then
			for k = #entries + 1, j do
				
				local entry = pack ( iter ( state, var ))
				var = entry [ 1 ]
				
				if var then
					entries [ k ] = entry
				else
					done = true
					break
				end
			end
		end
		
		c = j
	end
	
	local wrapper = {
		cursor	= cursor,
		next	= next,
		set		= set,
	}
	
	setmetatable ( wrapper, { __call = next })
	return wrapper
end

----------------------------------------------------------------
printTable = function ( t, d )

	local depth = d or 0
	local indent = ""
	for i = 1, depth do indent = indent .. "> " end
	for i, item in pairs ( t ) do

		io.write ( indent )
		print ( i, item )
		if type ( item ) == "table" then
			printTable ( item, depth + 1 )
		end
	end
end

----------------------------------------------------------------
pruneEmptyDirectories = function ( dir )

	local totalFiles = 0

	local files = MOAIFileSystem.listFiles ( dir ) or {}
	local totalFiles = #files

	local dirs = MOAIFileSystem.listDirectories ( dir ) or {}
	for k, entry in pairs ( dirs ) do
		totalFiles = totalFiles + pruneEmptyDirectories ( dir .. entry .. '/' )
	end
	
	if totalFiles == 0 then
		MOAIFileSystem.deleteDirectory ( dir )
	end
	
	return totalFiles
end

----------------------------------------------------------------
quantize = function ( dstpath, srcpath )

	local should = COMPILE
	
	if should then
		should = not isMember ( DONT_COMPILE, srcpath )
	end

	if should then
		
		local tmp = dstpath .. '.tmp'
		copy ( tmp, srcpath )
		
		local cmd = osx and '../util/pngnq/mac/pngnq -vf -Qf -s1 -e.png %s' or '..\\util\\pngnq\\win\\pngnqi -vf -Qf -s1 -e.png %s'
		exec ( cmd, tmp )
		
		MOAIFileSystem.deleteFile ( tmp )
		MOAIFileSystem.rename ( tmp .. '.png', dstpath )
	else
		copy ( dstpath, srcpath )
	end
end

----------------------------------------------------------------
readFileAll = function ( filename )

	if MOAIFileSystem.checkFileExists ( filename ) then
		local fp = io.open ( filename, "r" )
		local str = fp:read ( "*all" )
		fp:close ()
		return str
	end
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
scanFiles = function ( srcRoot, dstRoot, exclude, process, localPath )

	localPath = localPath or ''

	local indir = srcRoot .. localPath
	local outdir = dstRoot .. localPath
	
	local files = MOAIFileSystem.listFiles ( indir ) or {}
	for k, entry in pairs ( files ) do
		local fullpath = localPath .. entry
		if not ( isMember ( FILTER, fullpath ) or isMember ( exclude, fullpath )) then
			process ( outdir .. entry, indir .. entry )
		end
	end
	
	local dirs = MOAIFileSystem.listDirectories ( indir ) or {}
	for k, entry in pairs ( dirs ) do
		local fullpath = localPath .. entry .. '/'
		if not ( isMember ( FILTER, fullpath ) or isSysPath ( fullpath )) then
			scanFiles ( srcRoot, dstRoot, exclude, process, localPath .. entry .. '/' )
		end
	end
end

---------------------------------------------------------------
tokenize = function ( str, sep )

	local sep, fields = sep or ' ', {}
	local pattern = str.format ( "([^%s]+)", sep )
	str:gsub ( pattern, function ( c ) fields [ #fields + 1 ] = c end )
	return fields
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

----------------------------------------------------------------
zip = function ( dstpath, srcpath )

	if not MOAIFileSystem.checkPathExists ( srcpath ) then return end

	srcpath = escape ( srcpath )

	local fullpath = escape ( MOAIFileSystem.getAbsoluteFilePath ( dstpath ))
	local cwd = escape ( MOAIFileSystem.getWorkingDirectory ())
	MOAIFileSystem.setWorkingDirectory ( srcpath )

	local cmd = osx and 'zip -r %s *' or '7z a -tzip -r %s .\\*'
	exec ( cmd, fullpath )
	
	MOAIFileSystem.setWorkingDirectory ( cwd )
end
