--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- utility functions
----------------------------------------------------------------
function table_slice (values,i1,i2)
  local res = {}
  local n = #values

  -- default values for range
  i1 = i1 or 1
  i2 = i2 or n

  if i2 < 0 then
    i2 = n + i2 + 1
  elseif i2 > n then
    i2 = n
  end

  if i1 < 1 or i1 > n then
    return {}
  end

  local k = 1
  for i = i1,i2 do
    res[k] = values[i]
    k = k + 1
  end

  return res
end

----------------------------------------------------------------
function clean_error(err)
  local error = err
  local s, i = error:find(':%d+: ')
  if i then
    error = error:sub(i+1)
  end
  
  return error
end

----------------------------------------------------------------
local function notFound(web)
  return web:page('{}', 404, 'Not Found',  {['content-type'] = 'application/json'})
end

----------------------------------------------------------------
local function badRequest(web, err)
  local msg = '{err:1}'
  if err then
    msg = '{err:1,msg:"' .. err .. '"}'
  end
  
  return web:page( msg, 400, "Bad Request", {['content-type'] = 'application/json'})
end
  
----------------------------------------------------------------
-- route functions
----------------------------------------------------------------
function newUser(web, req)

  -- Make sure there is a body
  if not req['body'] then
    return badRequest(web, "No object data found in POST body")
  end
  
  -- Make sure the body can be read
  local result, data = pcall(json.decode, req['body'])
  if not result then
    local err = clean_error(data)

    return badRequest(web, err)
  end

  -- Generate an id for this user
  local id = mongo.ObjectId()[1]
  data.id = nil
  data._id = id

  data.clicks = 0
  data.clickTime = 0
  
  data.created_date = os.time()
  data.last_modified = os.time()

  -- Place the new user in the database
  mongodb:insert("users", data)

  -- Calculate the time to the next click ( this case will be negative )
  local timeToClick = 60 - os.difftime(os.time(), data.clickTime)
  
  return web:json({id=id, clicks=data.clicks, timeToClick=timeToClick })
en

----------------------------------------------------------------
function updateUser(web, req, userId)

  -- Try to find the user
  local user = mongodb:query("users", {_id = userId}):next()
  if not user then
    return notFound(web)
  end

  -- Make sure there is a body
  if not req['body'] then
    return badRequest(web, "No object data found in POST body")
  end
  
  -- Make sure the body can be read
  local result, data = pcall(json.decode, req['body'])
  if not result then
    local err = clean_error(data)

    return badRequest(web, err)
  end

  user.id = nil
  user.last_modified = os.time()
  
  -- Save the time of the request
  local currentTime = os.time()
  
  local success = false
  
  -- Make sure the user intended to click ( could be expanded to more actions later )
  if data.click then	
  
	-- Make sure the 60 second timer period has actually expired ( don't want anyone easily hacking it )
	if os.difftime(currentTime, user.clickTime) > 60 then
		-- Update the number of clicks and set the new time
		user.clicks = user.clicks + 1
		user.clickTime = currentTime
		success = true
	end
	
  end

  -- Create the table to update the entry in the database
  local new_record = {}
  new_record.clicks = user.clicks
  new_record.clickTime = user.clickTime
  new_record.last_modified = user.last_modified
  
  -- Update the database entry for the user
  mongodb:update("users", {_id=userId}, {['$set'] = new_record})
  
  -- Calculate the time to the next click
  local timeToClick = 60 - os.difftime(currentTime, user.clickTime)

  return web:json({id=userId, clicks=user.clicks, timeToClick=timeToClick, success=success })
end

----------------------------------------------------------------
function getUser(web, req, userId)

  -- Try to find the user
  local user = mongodb:query("users", {_id = userId}):next()
  if not user then
    return notFound(web)
  end

  -- Calculate the time to the next click
  local timeToClick = 60 - os.difftime(os.time(), user.clickTime)
  
  return  web:json({id=user._id, clicks=user.clicks, timeToClick=timeToClick })
end

----------------------------------------------------------------
-- main function and routes
----------------------------------------------------------------
routes = {
  {url = "^/[^/]+/wolfclicker/users+$", POST=newUser },
  {url = "^/[^/]+/wolfclicker/user/([^/]*)$", POST=updateUser, GET=getUser},
}

----------------------------------------------------------------
function main(web, req)

  for _, route in pairs(routes) do
    local r = {string.find(web:path(), route.url)}
    if (nil ~= r[1]) then
      local args = table_slice(r, 3)

      local func = route[web:method()]

      if func then
        return func(web, req, unpack(args))
      end
    end
  end

  return badRequest(web, "Unknown route: " .. web:path() .. " method " .. web:method())
end