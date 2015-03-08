iterateFiles = function ( path )
  path = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
  
  local doRecurse
  doRecurse = function ( dirname )

    local files = MOAIFileSystem.listFiles ( path .. dirname ) or {}
    for i, filename in ipairs ( files ) do
      coroutine.yield ( string.format ( '%s%s', dirname, filename ), false)
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

local function uuid()
    local template ='xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'
    return string.gsub(template, '[xy]', function (c)
        local v = (c == 'x') and math.random(0, 0xf) or math.random(8, 0xb)
        return string.format('%x', v)
    end)
end


local luaSrc=MOAIFileSystem.getAbsoluteDirectoryPath(arg[1])
local destRom=arg[2]
local dumpfile = string.format("%s.rom",destRom)
local jsonfile = string.format("%s.json",dumpfile)
local fileinfo = {}
local partial_dirs = {}

print ("Creating ", dumpfile)
local outstream = MOAIFileStream.new()
outstream:open(dumpfile, MOAIFileStream.READ_WRITE_NEW)
local file_start = 0


for file, isdir in iterateFiles(luaSrc) do
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
      outstream:flush()
      
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

--json dump
local jsonstream = MOAIFileStream.new()
jsonstream:open(jsonfile,MOAIFileStream.READ_WRITE_NEW)
jsonstream:write(
  MOAIJsonParser.encode({
   ['bundle_file'] = dumpfile , ['directories'] = partial_dirs, ['files'] = fileinfo, ['package_uuid'] = uuid() 
  })
)
jsonstream:flush()
jsonstream:close()

print ("Complete.")