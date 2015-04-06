---
title: "Moai Cloud Lua Environment"
---

Basic Lua Environment
---------------------

The basic Lua runtime environment available to Moai Cloud Services is similar to the standard Lua runtime environment, with some functionality removed for security and stability purposes. The basic Lua environment is defined as follows. Information on how to use each of these functions can be found in the [Lua 5.1 Reference Manual](http://www.lua.org/manual/5.1/).

`
  assert=assert,
  error=error,
  ipairs=ipairs,
  pairs=pairs,
  next=next,
  pcall=pcall,
  select=select,
  tonumber=tonumber,
  tostring=tostring,
  type=type,
  unpack=unpack,
  xpcall=xpcall,
  coroutine={
          create=coroutine.create,
          resume=coroutine.resume,
          running=coroutine.running,
          status=coroutine.status,
          wrap=coroutine.wrap,
          yield=coroutine.yield,
  },
  table={
          concat=table.concat,
          insert=table.insert,
          maxn=table.maxn,
          getn=table.getn,
          setn=table.setn,
          remove=table.remove,
          sort=table.sort,
  },
  string={
          byte=string.byte,
          char=string.char,
          find=string.find,
          format=string.format,
          gmatch=string.gmatch,
          gsub=string.gsub,
          len=string.len,
          lower=string.lower,
          match=string.match,
          rep=string.rep,
          reverse=string.reverse,
          sub=string.sub,
          upper=string.upper,
  },
  math={
          abs=math.abs,
          acos=math.acos,
          asin=math.asin,
          atan=math.atan,
          atan2=math.atan2,
          ceil=math.ceil,
          cos=math.cos,
          cosh=math.cosh,
          deg=math.deg,
          exp=math.exp,
          floor=math.floor,
          fmod=math.fmod,
          frexp=math.frexp,
          huge=math.huge,
          ldexp=math.ldexp,
          log=math.log,
          log10=math.log10,
          max=math.max,
          min=math.min,
          modf=math.modf,
          pi=math.pi,
          pow=math.pow,
          rad=math.rad,
          random=math.random, 
          randomseed=math.randomseed, 
          sin=math.sin,
          sinh=math.sinh,
          sqrt=math.sqrt,
          tan=math.tan,
          tanh=math.tanh,
  },
  mongo={
          Date=mongo.Date,
          Timestamp=mongo.Timestamp,
          ObjectId=mongo.ObjectId,
          NumberLong=mongo.NumberLong,
  },
  os={
          clock=os.clock,
          date=os.date, 
          difftime=os.difftime,
          time=os.time,
  },
  base64={
          encode=base64.encode,
          decode=base64.decode,
  },
  hmac=hmac,
  posix={
          sleep=posix.sleep,
  },
  loadstring = moaiLoadstring
`

Additional Libraries
--------------------

### curl

The Moai Cloud Lua environment contains full support for the [luacurl](http://luaforge.net/docman/view.php/117/136/luacurl.html) library and is defined as follows:

`
 curl={
       new=curl.new,
       setopt=curl.setopt,
       perform=curl.perform,
       close=curl.close
 },
 `\
`       `

#### Sample Code

The following demonstrates performing an HTTP POST of a JSON document to a given URL and retrieving the results as a string.

```lua
function curl_url(url)
  local writer = function(temp,s)
    temp[#temp+1] = s
    return s:len()
  end
  
  local post_data = {['arg1']='value1'}
  
  local buffer = {}
  local c = curl:new()
  c:setopt(curl.OPT_URL, url)
  c:setopt(curl.OPT_WRITEFUNCTION,writer)
  c:setopt(curl.OPT_WRITEDATA, buffer)
  c:setopt(curl.OPT_FOLLOWLOCATION, true)
  
  c:setopt(curl.OPT_POSTFIELDS, json.encode(post_data))
  c:setopt(curl.OPT_POSTFIELDSIZE, #json.encode(post_data))
 
  local result, msg = c:perform()
 
  return table.concat(buffer)
 end
 
```

### JSON

The Moai Cloud Lua environment contains full support for the [luajson](https://github.com/harningt/luajson) library and is defined as follows:

`
 json={
       encode=json.encode,
       isEncodable=json.isEncodable,
       decode=json.decode,
       util=json.util,
 },
 `

### HMAC

The Moai Cloud Lua environment contains support for the [Lua sha2](http://code.google.com/p/sha2/) library and is defined as follows:

`
 hmac={
       compute=hmac.compute,
       new=hmac.new,
       sha256=hmac.sha256,
       sha348=hmac.sha348,
       sha512=hmac.sha512,
 },
 `

### MD5

The Moai Cloud Lua environment contains partial support for the [Lua MD5](http://www.keplerproject.org/md5/) library and is defined as follows:

`
 md5={
       crypt=md5.cript,
       decrypt=md5.decrypt,
       exor=md5.exor,
       sum=md5.sum,
       sumhexa=md5.sumhexa,
 },
 `

### mongodb

All persistence in Moai Cloud is handled by MongoDB using a subset of the luamongo driver and the [luamongo project page](http://www.github.com/moai/luamongo).

`
 mongodb={
       count=mongodb.count,
       insert=mongodb.insert,
       insert_batch=mongodb.insert_batch,
       query=mongodb.query,
       remove=mongodb.remove,
       update=mongodb.update,
       drop_collection=drop_collection,
       list_collections=list_collections,
       distinct,
       find_and_modify,
       get_last_error,
       get_last_error_detailed,
 },
 `

### Client information

Moai Cloud provides a mechanism to allow a service callers to identify themselves using a unique public identifier. When a key is provided to a service call some additional information is available to the service.

An additional mongodb connection accessed through `client.db` that provides access to the caller's database is available. This connection is sandboxed within the namespace the user specified with the key. The typical mongodb connection has no knowledge of the namespace associated with the key.

Additionally a table of all the user-configurable settings for your service and the key, which can be used as a unique identifier of the caller is provided to your service.

`
  client={
    db,
    db.readonly,
    key,
    settings
  }
`

### GridFS File & Chunk

You can access a sandboxed version of the GridFS file and chunk objects through the [Moai File Store](moai-file-store.html) methods. We expose the following functions on those objects.

`
    function gridfile:chunk(chunk_num)
    function gridfile:chunk_size()
    function gridfile:content_length()
    function gridfile:exists()
    function gridfile:md5()
    function gridfile:num_chunks()
    function gridfile:upload_date()
    function gridfile:data()
    function gridfile:field(name)
    function gridfile:metadata()
 `

`
    function gridchunk:len()
    function gridchunk:data()
 `

Moai Cloud Specific
-------------------

In addition to the standard functions defined below, the Moai Cloud Lua environment also provides for some Moai Cloud specific variables and functions used for loading additional code, managing file and image resources and mongoDB database access. All additions to the standard Lua environment exist under an environment variable "moai" and are defined as follows.

`moai={
       app.settings= {}
       code={
         loadsource=loadsource,
         find_file=find_file,
         store_data=store_data,
         remove_file=remove_file,
         list=list,
         grid_file=grid_file
       },
       content={
         find_file=find_file,
         store_data=store_data,
         remove_file=remove_file,
         list=list
         grid_file=grid_file
       },
       log=moai.log, --DEPRECATED
       logger=moai.logger --See-[[Moai Logging|Logging With Moai Cloud]]
       table_print=moai.table_print,
       userid=moai.userid,
 },`

```
For more information about ``moai.code`` and ``moai.content`` see `[`Moai` `File` `Store`](moai-file-store.html)`.
```

### Function Reference

**`moai.appid`**` - the UUID of the current service`

**`moai.log(msg,` `level)`**` - Creates a log file entry for your application. The Moai Cloud internal`\
```
logger provides for a rolling log of 1000 messages of up to 1000 characters each. You can view 
the log output from the Moai Cloud dashboard. 
Note, all log messages across all of your services will be written to a common log file.
    `**`msg`**` - (string) - the message to log
    `**`level`**` - (string) - a string indicating the log level (e.g. DEBUG, INFO, WARN, ERROR, 
    FATAL, etc.)
```

**`moai.table_print(table)`**` - Returns an HTML formatted string representation of the specified `\
```
Lua table.
   `**`table`**` - (Lua table) - the table to stringify.
```

**`moai.userid`**` - your Moai Cloud ID`
