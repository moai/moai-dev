----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- Read access token from disk
-- @returns access token as a associative table with token and tokenSecret keys
function readAccessTokenFromDisk()
    local file, err = io.open(MOAIEnvironment.documentDirectory .. "/twitter.dat", "r")
    local accessToken
    if err then
        return nil, err
    else
        accessToken = MOAIJsonParser.decode(file:read("*all")) or {} 
        file:close()
    end
 
    return accessToken 
end

-- Write the access token as a associative table with token and tokenSecret keys
-- @returns boolean true if write succeeded
-- @returns [error] Error that occured 
function writeAccessTokenToDisk(token, tokenSecret)
    local accessToken = {
        token = token,
        tokenSecret = tokenSecret
    }
    local file, err = io.open(MOAIEnvironment.documentDirectory .."/twitter.dat", "w")
    if err then
       return false, err
    end
    file:write(MOAIJsonParser.encode(accessToken))
    file:close()
    return true
end
