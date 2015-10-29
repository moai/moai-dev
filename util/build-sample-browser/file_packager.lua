local function generateRom(dir, skipList)

local function gen_uuid(romData)
  return crypto.digest("md5", romData)
end


local matchFunc = function(filename) 
    for i, p in ipairs ( skipList ) do
      if string.find ( filename, p ) then 
        return false 
      end
    end
    return true
end
  
local iterateFiles = function ( path, filterFunc )
  path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
  
  local doRecurse
  doRecurse = function ( dirname )

    local files = MOAIFileSystem.listFiles ( path .. dirname ) or {}
    for i, filename in ipairs ( files ) do
      if filterFunc(filename) then
        coroutine.yield ( string.format ( '%s%s', dirname, filename ), false)
      end
    end
  
    local dirs = MOAIFileSystem.listDirectories ( path .. dirname ) or {}
    for i, subdir in ipairs ( dirs ) do
      local thisdir = string.format ( '%s%s', dirname, subdir )
      coroutine.yield ( thisdir, true )
      doRecurse ( thisdir..'/' )
    end

  end
  
  local co = coroutine.create ( function ()
    doRecurse ( '' )
  end )
  
  return function ()
    local status, result, isdir = coroutine.resume ( co )
    if status then
      return result, isdir
    else
      return nil
    end
  end
end




local luaSrc=MOAIFileSystem.getAbsoluteDirectoryPath(dir)
local fileinfo = {}
local partial_dirs = {}

print ("Creating Rom")
local outmemstream = MOAIMemStream.new()
outmemstream:open(0, 250000)

local outstream = MOAIStreamAdapter.new()
outstream:openBase64Writer( outmemstream )

local file_start = 0


for file, isdir in iterateFiles(luaSrc, matchFunc) do
  print ("Processing "..file.."...")
  if isdir then 
    table.insert(partial_dirs, '/'..file)
  else
    local stream = MOAIFileStream.new()
    if stream:open(luaSrc..file, MOAIFileStream.READ) then
      
      --dump the data
      local data, size = stream:read()
      local outsize = outstream:write(data)
      if size ~= outsize then
        print("Write size did not match read size. Error creating package")
        os.exit(1)
      end
      
      --update file_info
      local file_end = file_start + size
      table.insert(fileinfo, { ['filename'] = '/'..file, ['start'] = file_start, ['end'] = file_end })
      file_start = file_end
     else
      print("error creating read stream",luaSrc..file)
    end
  end
end

outstream:close()

local romData = outmemstream:getString()
outmemstream:close()

--json dump
local uuid = gen_uuid(romData)

local jsonData =  MOAIJsonParser.encode({
   ['bundle_file'] = dumpfile , ['directories'] = partial_dirs, ['files'] = fileinfo, ['package_uuid'] = uuid
  })

return {
  romData = romData,
  jsonData = jsonData
}

end

return generateRom