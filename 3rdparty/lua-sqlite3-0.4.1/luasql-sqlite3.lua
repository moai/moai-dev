
--[[--------------------------------------------------------------------------

    Author: Michael Roth <mroth@nessie.de>

    Copyright (c) 2004 Michael Roth <mroth@nessie.de>

    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--]]--------------------------------------------------------------------------



if luasql == nil then
  luasql = { }
end

if type(luasql) ~= "table" then
  error("luasql needs to be a table, but is a "..type(luasql).." already", 2)
end



local env_class  = { }
local conn_class = { }
local cur_class  = { }

local env_class_mt  = { __index = env_class }
local conn_class_mt = { __index = conn_class }
local cur_class_mt  = { __index = cur_class }

local api, ERR, TYPE, AUTH

local function setref(t, v)	t.ref_counter = v end
local function ref(t)		t.ref_counter = t.ref_counter + 1 end
local function unref(t)		t.ref_counter = t.ref_counter - 1 end
local function isref(t)		return t.ref_counter > 0 end




function luasql.sqlite3()
  if not api then 
    api, ERR, TYPE, AUTH = load_libluasqlite3()
  end
  
  local env = setmetatable( {}, env_class_mt )
  setref(env, 0)
  
  env.closed = false
  return env
end


function env_class.close(env)
  if env.closed or isref(env) then return end
  env.closed = true
  return true
end


function env_class.connect_memory(env)
  return env_class.connect(env, ":memory:")
end


function env_class.connect(env, filename)
  if env.closed then return end
  
  local conn = setmetatable( {}, conn_class_mt )
  setref(conn, 0)
  
  local err, db = api.open(filename)
  
  if err ~= ERR.OK then error("api.open: "..err) end
  
  conn.env = env
  conn.db = db
  conn.closed = false
  conn.autocommit = true
  conn.ta_active = false
  
  ref(env)
  return conn
end



function conn_class.close(conn)
  if conn.closed or isref(conn) then return end
  
  api.close(conn.db)
  conn.db = nil
  
  unref(conn.env)
  conn.env = nil
  
  conn.closed = true
  return true
end



function conn_class.commit(conn)
  if conn.closed or conn.autocommit or not conn.ta_active then return end
  api.exec(conn.db, "COMMIT")
  conn.ta_active = false
  return true
end

function conn_class.rollback(conn)
  if conn.closed or conn.autocommit or not conn.ta_active then return end
  api.exec(conn.db, "ROLLBACK")
  conn.ta_active = false
  return true
end


function conn_class.setautocommit(conn, autocommit)
  if not conn.autocommit and autocommit then
    api.exec("END")
  end
  conn.autocommit = autocommit
  return true
end


function conn_class.execute(conn, sql)
  if conn.closed then return end
  
  if not conn.autocommit and not conn.ta_active then
    api.exec(conn.db, "BEGIN")
    conn.ta_active = true
  end
  
  local err, stmt = api.prepare(conn.db, sql)
  
  if err ~= ERR.OK then error("api.prepare: "..err) end
  
  if api.column_count(stmt) == 0 then
    -- normal excute, no cursor
    err = api.step(stmt)
    if err ~= ERR.DONE then
      error("api.step: "..err)
    end
    api.finalize(stmt)
    return api.changes(conn.db)
  else
    -- select, needs cursor
    local cur = setmetatable( {}, cur_class_mt )
    
    cur.stmt = stmt
    cur.finished = false
    cur.closed = false
    
    ref(conn)
    ref(conn.env)
    cur.conn = conn
    
    return cur
  end
  
end






function cur_class.close(cur)
  if cur.closed then return end
  
  if not cur.finished then 
    api.finalize(cur.stmt)
    cur.stmt = nil
  end
  
  unref(cur.conn.env)
  unref(cur.conn)
  cur.conn = nil
  
  cur.closed = true
  return true
end


function cur_class.fetch(cur, t, mode)
  if cur.closed or cur.finished then return end
  
  -- FIXME: write nice error wrapper...
  
  local err = api.step(cur.stmt)
  
  if err == ERR.DONE then
    api.finalize(cur.stmt)
    cur.stmt = nil
    cur.finished = true
    return
  elseif err ~= ERR.ROW then
    error("api.step: "..err)
  end
  
  -- FIXME: really ugly block follows....
  
  if t == nil and mode == nil then
    -- direct mode
    return api.drow(cur.stmt)
  else
    if type(t) == "string" and mode == nil then
      mode = t
      t = { }
    end
    if mode == nil then 
      mode = "n"
    end
    if type(t) ~= "table" or type(mode) ~= "string" then
      error("fetch usage error")
    end
    
    if mode == nil or mode == "n" then
      return api.irow(cur.stmt, t)
    elseif mode == "a" then
      return api.arow(cur.stmt, t)
    else
      error("unknown mode: "..mode)
    end
  end
end



local function get_col_desc(cur, desc, api_func)
  if cur.closed then return end
  if not cur[desc] then
    cur[desc] = { }
    for index = 1, api.column_count(cur.stmt) do
      table.insert( cur[desc], api_func(cur.stmt, index - 1) )
    end
  end
  return cur[desc]
end

function cur_class.getcolnames(cur)
  return get_col_desc(cur, "colnames", api.column_name)
end

function  cur_class.getcoltypes(cur)
  return get_col_desc(cur, "coltypes", api.column_decltype)
end




