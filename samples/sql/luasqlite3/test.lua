require "sqlite3"

-- Languages in the database:
--   ar de en es fr ja ko ru zh_Hans zh_Hant default
language = "en"

sqlitedb = sqlite3.open("message.db")

query = "SELECT text FROM `" .. language .. "` WHERE id = ?"
statement = sqlitedb:prepare( query )
if not statement then
  query = "SELECT text FROM `default` WHERE id = ?"
  statement = sqlitedb:prepare( query )
end

message = statement:bind( "HELLO_WORLD" ):first_cols()
message = message .. "\n" .. statement:bind( "GOOD_BYE" ):first_cols()
print ( message )

sqlitedb:close()

