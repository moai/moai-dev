require ( 'http' )
require ( 'util' )

print ( 'hello' )

local status, result = http.get ( 'https://api.github.com/users/conghuzhao/repos' )

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})
