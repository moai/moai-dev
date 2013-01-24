
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




-------------------------------------------------
-- This is the luaSql compatible sqlite driver --
-------------------------------------------------
require "luasql-sqlite3"

require "lunit"

lunit.setprivfenv()
lunit.import "asserts"




lunit.wrap("Driver Interface", function()
  assert_table( luasql )
  assert_function( luasql.sqlite3 )
  local env = assert_table( luasql.sqlite3() )
  assert_function( env.connect )
  assert_function( env.connect_memory )	-- sqlite3 extension
  assert_function( env.close )
  assert_true( env:close(),  "Closing an unused environment must return 'true'" )
  assert_nil( env:close(), "Closing a closed environment must return 'nil'" )
end)



lunit.wrap("Connection Interface", function()
  local env = assert_table( luasql.sqlite3() )
  local con = assert_table( env:connect_memory() )
  assert_function( con.close )
  assert_function( con.execute )
  assert_function( con.rollback )
  assert_function( con.commit )
  assert_function( con.setautocommit )
  assert_true( con:close(), "Closing an open connection must return 'true'" )
  assert_nil( con:close(), "Closing a closed connection must return 'nil'" )
  assert_true( env:close() )
end)



lunit.wrap("Simple connection usage", function()
  local env = assert_table( luasql.sqlite3() )
  local con = assert_table( env:connect_memory() )
  assert_equal( 0, con:execute("CREATE TABLE test (id, name)") )
  assert_equal( 1, con:execute("INSERT INTO test VALUES (1, 'Hello World')") )
  assert_true( con:close() )
  assert_true( env:close() )
end)



lunit.wrap("Cursor Interface", function()
  local env = assert_table( luasql.sqlite3() )
  local con = assert_table( env:connect_memory() )
  assert_equal( 0, con:execute("CREATE TABLE test (id, name)") )
  local cur = assert_table( con:execute("SELECT * FROM test") )
  assert_function( cur.close )
  assert_function( cur.fetch )
  assert_function( cur.getcolnames )
  assert_function( cur.getcoltypes )
  assert_true( cur:close(), "Closing an open cursor must return 'true'" )
  assert_nil( cur:close(), "Closing a closed cursor must return 'nil'" )
  assert_true( con:close() )
  assert_true( env:close() )
end)



local scu = lunit.TestCase("Simple cursor usage")

function scu:setup()
  -- open database
  self.env = assert_table( luasql.sqlite3() )
  self.con = assert_table( self.env:connect_memory() )

  -- prepare database
  assert_equal( 0, self.con:execute("CREATE TABLE test (id INTEGER, item TEXT)") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (1, 'Hello World')") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (2, 'Hello Lua')") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (3, 'Hello sqlite3')") )
  
  -- open cursor
  self.cur = assert_table( self.con:execute("SELECT * FROM test ORDER BY id") )
end

function scu:teardown()
  assert_true( self.cur:close() )
  assert_true( self.con:close() )
  assert_true( self.env:close() )
end

function scu:test_fetch_direct()
  local id, item
  id, item = self.cur:fetch(); assert_equal(1, id); assert_equal("Hello World", item)
  id, item = self.cur:fetch(); assert_equal(2, id); assert_equal("Hello Lua", item)
  id, item = self.cur:fetch(); assert_equal(3, id); assert_equal("Hello sqlite3", item)
  assert_nil( self.cur:fetch() )
end

function scu.check(key_id, id, key_item, item, row)
  assert_table(row)
  assert_equal(id, row[key_id])
  assert_equal(item, row[key_item])
end

function scu:test_fetch_default()
  self.check(1, 1, 2, "Hello World",   self.cur:fetch({}) )
  self.check(1, 2, 2, "Hello Lua",     self.cur:fetch({}) )
  self.check(1, 3, 2, "Hello sqlite3", self.cur:fetch({}) )
  assert_nil( self.cur:fetch({}) )
end
  
function scu:test_fetch_numeric()
  self.check(1, 1, 2, "Hello World",   self.cur:fetch({}, "n") )
  self.check(1, 2, 2, "Hello Lua",     self.cur:fetch({}, "n") )
  self.check(1, 3, 2, "Hello sqlite3", self.cur:fetch({}, "n") )
  assert_nil( self.cur:fetch({}, "n") )
end

function scu:test_fetch_alphanumeric()
  self.check("id", 1, "item", "Hello World",   self.cur:fetch({}, "a"))
  self.check("id", 2, "item", "Hello Lua",     self.cur:fetch({}, "a"))
  self.check("id", 3, "item", "Hello sqlite3", self.cur:fetch({}, "a"))
  assert_nil( self.cur:fetch({}, "a") )
end

function scu:test_getcolnames()
  local names = assert_table( self.cur:getcolnames() )
  assert_equal(2, table.getn(names) )
  assert_equal("id", names[1])
  assert_equal("item", names[2])
end

function scu:test_getcoltypes()
  local types = assert_table( self.cur:getcoltypes() )
  assert_equal(2, table.getn(types) )
  assert_equal("INTEGER", types[1])
  assert_equal("TEXT", types[2])
end



local ta = lunit.TestCase("Transaction Tests")

function ta:setup()
  -- open database
  self.env = assert_table( luasql.sqlite3() )
  self.con = assert_table( self.env:connect_memory() )

  -- prepare database
  assert_equal( 0, self.con:execute("CREATE TABLE test (id INTEGER, item TEXT)") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (1, 'Hello World')") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (2, 'Hello Lua')") )
  assert_equal( 1, self.con:execute("INSERT INTO test VALUES (3, 'Hello sqlite3')") )
  
  -- switch to manual transaction controll
  assert_true( self.con:setautocommit(false) )
end

function ta:teardown()
  assert_true( self.con:close() )
  assert_true( self.env:close() )
end

function ta:insert(id, item)
  assert_equal(1, self.con:execute("INSERT INTO test VALUES ("..id..", '"..item.."')") )
end

function ta:update(id, item)
  assert_equal(1, self.con:execute("UPDATE test SET item = '"..item.."' WHERE id = "..id) )
end

function ta:check(expected)
  assert_table(expected)
  
  local cur = assert_table( self.con:execute("SELECT * FROM test ORDER BY id") )
  local id = 0
  local row = cur:fetch({}, "a")
  while row do
    assert_table(row)
    id = id + 1
    assert_equal(id, row.id, "Unexpected 'id' read (wrong row?)")
    assert( id <= table.getn(expected), "'Id' read to large (to many rows?)")
    assert_equal(expected[id], row.item, "Invalid content in row")
    row = cur:fetch({}, "a")
  end
  assert_equal(id,  table.getn(expected), "To less rows read")
  assert_true( cur:close() )
end

function ta:test_prepared_content()
  self:check { "Hello World", "Hello Lua", "Hello sqlite3" }
end

function ta:test_transactions()
  self:insert(4, "Hello again")
  self:insert(5, "Goodbye")
  self:check { "Hello World", "Hello Lua", "Hello sqlite3", "Hello again", "Goodbye" }
  assert_true( self.con:commit() )
  self:update(1, "Good morning")
  self:insert(6, "Foobar")
  self:check { "Good morning", "Hello Lua", "Hello sqlite3", "Hello again", "Goodbye", "Foobar" }
  assert_true( self.con:rollback() )
  self:check { "Hello World", "Hello Lua", "Hello sqlite3", "Hello again", "Goodbye" }
end




