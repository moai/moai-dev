require ( 'http' )
require ( 'util' )

local status, result = http.get ( 'https://api.github.com/repos/ConghuZhao/testRelease/releases')

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})

config = {}

util.dofileWithEnvironment ( './upload-info.lua', config )


http_headers = {
	Authorization=config['RELEASE_AUTHORIZATION'],
	['Content-Type']="application/zip"
}


assets_url = string.format("%s/%s/assets",config['UPLOAD_REPO'],json["id"])

local file = MOAIFileStream.new ()
file:open ("./boo.zip")
local data = file:read ()
file:close ()
print(data)
status, result = http.post ( assets_url,nil ,data,http_headers)

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})