require ( 'http' )
require ( 'util' )




getCurrenttag = function ()
    str = util.loadFileAsString ( '../src/config/moai_version_major.h' )


    type="MOAI_SDK_VERSION_MAJOR %d"
    VERSION = string.match(string.sub(str, string.find(str, type)),"%d+")

    str = util.loadFileAsString ( '../src/config/moai_version_minor.h' )
    type="MOAI_SDK_VERSION_MINOR %d"
    SUBVERSION= string.match(string.sub(str, string.find(str, type)),"%d+")
    type="MOAI_SDK_VERSION_REVISION %d"
    REVISION= string.match(string.sub(str, string.find(str, type)),"%d+")
    return VERSION , SUBVERSION ,REVISION
end


------------------------------------------------
--main
----------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
  
  if param then

    if escape == 'd'  then
      
      RELEASE_BODY = param
    end
    if escape == 'n'  then
      
      RELEASE_NAME = param
    end
    if escape == 'r'  then
      RELEASE_TYPE = param
      end
  end
end

if RELEASE_BODY == nil then 
  print("please enter a Description for the release")
  do return end
end


config = {}

util.dofileWithEnvironment ( './upload-info.lua', config )


if RELEASE_NAME == nil then 
  RELEASE_NAME = config['RELEASE_NAME']
  do return end
end


http_headers = {
	Authorization=config['RELEASE_AUTHORIZATION'],
}

local VERSION ,SUBVERSION,REVISION = getCurrenttag()

http_body = 
{
  tag_name= string.format("v%i.%i.%i",VERSION,SUBVERSION,REVISION),
  target_commitish = config['RELEASE_TRAGET_COMMITISH'],
  name= RELEASE_NAME,
  body= RELEASE_BODY,
  draft= config['RELEASE_DRAFT'],
  prerelease= config['RELEASE_PRERELEASE']
}
 util.printTable ( http_body or {})
http_body = MOAIJsonParser.encode ( http_body )


local status, result = http.post ( config['RELEASE_REPO'],nil ,http_body,http_headers)
--local status, result = http.get ( 'https://api.github.com/repos/ConghuZhao/testRelease/releases')

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})


assets_url = string.format("%s/%s/assets",config['UPLOAD_REPO'],json["id"])


http_param =
{
	name="foo.zip"
} 
http_headers = {
	Authorization=config['RELEASE_AUTHORIZATION'],
	['Content-Type']="application/zip"
}


local file = MOAIFileStream.new ()
file:open ( config[ 'RELEASE_BINARY' ] )
local data = file:read ()
file:close ()

status, result = http.post ( assets_url,http_param ,data,http_headers)

print ( status )
json = http.is200 ( status ) and result and MOAIJsonParser.decode ( result )

util.printTable ( json or {})

--updatesaveReleaseVersion(VERSION,SUBVERSION,REVISION)
