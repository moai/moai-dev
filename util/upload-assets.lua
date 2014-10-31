require ( 'http' )
require ( 'util' )

local status, result = http.get ( 'https://api.github.com/repos/ConghuZhao/testRelease/releases')

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )



--https://uploads.github.com/repos/ConghuZhao/testRelease/releases/652472/assets{?name



http_headers = {
	Authorization='',
	['Content-Type']="application/zip"
}


local file = MOAIFileStream.new ()
file:open ("./foo.zip")
local data = file:read ()
file:close ()
print(data)
status, result = http.post ( 'https://uploads.github.com/repos/ConghuZhao/testRelease/releases/652472/assets?name=foo.zip',nil ,data,http_headers)

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})




local file = MOAIFileStream.new ()
file:open ("./boo.zip")
local data = file:read ()
file:close ()
print(data)
status, result = http.post ( 'https://uploads.github.com/repos/ConghuZhao/testRelease/releases/652472/assets?name=boo.zip',nil ,data,http_headers)

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})