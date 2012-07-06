----------------------------------------------------------------
--
--  Lua-SQLite3 Moai Sample main.lua
--  This sample querying a database for software localization.
--  It uses the Lua-SQLite3 binding for communication with the
--  database.  The Moai framework is used to display the
--  results.
--
--  Written in 2012 by Brendan A R Sechter <bsechter@sennue.com>
--
--  To the extent possible under law, the author(s) have
--  dedicated all copyright and related and neighboring rights
--  to this software to the public domain worldwide. This
--  software is distributed without any warranty.
--
--  You should have received a copy of the CC0 Public Domain
--  Dedication along with this software. If not, see
--  <http://creativecommons.org/publicdomain/zero/1.0/>.
--
----------------------------------------------------------------


----------------------------------------------------------------
-- SQLite Code
----------------------------------------------------------------
require "luasql"

-- Ideally the language would be auto detected.
-- Languages in the database:
--   ar de en es fr ja ko ru zh_Hans zh_Hant default
language = "en"

-- Create Environment
environment = assert ( luasql.sqlite3 () )
connection  = assert ( environment:connect ( "message.db" ) )

-- Prepare Query
message = ""
query = "SELECT text FROM `" .. language .. "` WHERE id = "

-- Query Messages (printed at end of boilerplate)
cursor = assert ( connection:execute( query .. "'HELLO_WORLD'" ))
row    = cursor:fetch( {}, "a" )
while row do
  message = message .. "\n" .. row.text
  row = cursor:fetch( row, "a" )
end
cursor:close()
cursor = assert ( connection:execute( query .. "'GOOD_BYE'" ))
row    = cursor:fetch( {}, "a" )
while row do
  message = message .. "\n" .. row.text
  row = cursor:fetch( row, "a" )
end
cursor:close()

-- Close Everything
query = nil
connection:close()
environment:close()

-- Documentation can be found here:
--   http://www.keplerproject.org/luasql/


----------------------------------------------------------------
-- Boilerplate Code
----------------------------------------------------------------

-- Simulation Window
programName          = "Lua-SQLite3 Sample"
horizontalResolution = MOAIEnvironment.horizontalResolution or 320
verticalResolution   = MOAIEnvironment.verticalResolution   or 480
MOAISim.openWindow ( programName, horizontalResolution, verticalResolution )

-- Viewport
viewport = MOAIViewport.new ()
viewport:setSize  ( horizontalResolution, verticalResolution )
viewport:setScale ( horizontalResolution, verticalResolution )

-- Layer
layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- Font
fontName = "unifont-5.1.20080907.ttf"
fontSize = horizontalResolution / 10
glyphCache = MOAIGlyphCache.new ()
fontReader = MOAIFreeTypeFontReader.new ()
font = MOAIFont.new ()
font:load ( fontName )
font:setCache( glyphCache )
font:setReader( fontReader )
font:setDefaultSize( fontSize )

-- Textbox
textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setRect ( -horizontalResolution / 2, -verticalResolution / 2, horizontalResolution / 2, verticalResolution / 2)
textbox:setLoc ( 0, 0 )
textbox:setYFlip ( true )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( textbox )

-- Output
font:preloadGlyphs( message, fontSize )
textbox:setString ( message );
textbox:spool ()

