---------------------------------------------------------------------
-- LuaSQL JDBC driver implementation
-- @author Thiago Ponte
---------------------------------------------------------------------

---------------------------------------------------------------------
-- luasql table name
---------------------------------------------------------------------
local libName = "luasql"

local LUASQL_PREFIX = "LuaSQL: "

local Private = {}

luasql = type(_G[libName]) == "table" and _G[libName] or {}

Private.createJavaCursor = luajava.loadLib("org.keplerproject.luasql.jdbc.LuaSQLCursor", "open")

luasql._COPYRIGHT = "Copyright (C) 2003-2006 Kepler Project"
luasql._DESCRIPTION = "LuaSQL is a simple interface from Lua to a DBMS"
luasql._VERSION = "LuaSQL 2.0.2"

---------------------------------------------------------------------
-- function that returns a jdbc environment
---------------------------------------------------------------------
function luasql.jdbc(driver)

    if driver == nil then
        return nil, "Error. Argument #1 must be the jdbc driver class."
    end
    
    local cond, err = pcall(luajava.bindClass, driver)
    if not cond then
        return nil, err
    end
    
    return Private.createEnv()
end

---------------------------------------------------------------------
-- creates a jdbc environment
---------------------------------------------------------------------
function Private.createEnv()

    local isClosed  = false
    local openConns = {}
    openConns.n     = 0

    local env = {}
    
    local function closeConn(con)
    
        if not openConns[con] then
            return false
        end
        
        openConns[con] = nil
        openConns.n = openConns.n - 1
        
        return true
    end
    
    function env:close()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."environment expected")
        end
        
        if isClosed or openConns.n ~= 0 then
            return false
        end
        
        isClosed = true
        
        return true
    end
    
    function env:connect(sourcename, username, password)
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."environment expected")
        end

        if isClosed then
            error(LUASQL_PREFIX.."environment is closed")
            --return nil, "Environment closed."
        end
        if sourcename == nil then
            return nil, "Invalid sourcename."
        end

        local driverManager = luajava.bindClass("java.sql.DriverManager")
        
        local cond, con
        if username == nil and password == nil then
            cond, con = pcall(driverManager.getConnection, driverManager, sourcename)
        else
            cond, con = pcall(driverManager.getConnection, driverManager, sourcename, username or '', password or '')
        end
        
        if not cond then
            return nil, con
        end
        
        openConns[con] = true
        openConns.n = openConns.n + 1
        
        return Private.createConnection(con, closeConn)
    end
    
    -- For compatibility with other drivers
    setmetatable(env, {__metatable = LUASQL_PREFIX.."you're not allowed to get this metatable"})
    
    return env
end


---------------------------------------------------------------------
-- creates a jdbc connection
---------------------------------------------------------------------
function Private.createConnection(conObj, closeFunc)

    local openCursors = {}
    openCursors.n = 0
    
    local con = {}
    
    local function closeCursor(cursor)
    
        if not openCursors[cursor] then
            return false
        end
        
        openCursors[cursor] = nil
        openCursors.n = openCursors.n - 1
    end
    
    function con:close()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."connection expected")
        end

        if conObj:isClosed() or openCursors.n ~= 0 then
            return false
        end
        
        conObj:close()
        closeFunc(conObj)
        
        return true
    end
    
    function con:commit()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."connection expected")
        end

        local cond, err = pcall(conObj.commit, conObj)
        if not cond then
            return nil, err
        end
    end
    
    function con:execute(sql)
    
        if conObj:isClosed() then
            error(LUASQL_PREFIX.."connection is closed")
        end
        
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."connection expected")
        end

        local st = conObj:createStatement()

        local cond, isRS = pcall(st.execute, st, sql)
        if not cond then
            return nil, isRS
        end

        local res;
        if isRS then
            res = Private.createCursor(st:getResultSet(), st, closeCursor, con)
            openCursors[res] = true
            openCursors.n = openCursors.n + 1
        else
            res = st:getUpdateCount()
            st:close();
        end
        
        return res
    end
    
    function con:rollback()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."connection expected")
        end

        local cond, err = pcall(conObj.rollback, conObj)
        if not cond then
            return nil, err
        end
    end
    
    function con:setautocommit(bool)

        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."connection expected")
        end

        local cond, err = pcall(conObj.setAutoCommit, conObj, bool)
        if not cond then
            return nil, err
        end
    end
   
    -- For compatibility with other drivers
    setmetatable(con, {__metatable = LUASQL_PREFIX.."you're not allowed to get this metatable"})

    return con
end

---------------------------------------------------------------------
-- creates a jdbc cursor
---------------------------------------------------------------------
function Private.createCursor(rs, st, closeFunc, con)

    local isClosed = false
    local cursor = Private.createJavaCursor(rs)
    local res = {}
    local names
    local types
    
    res._con = con
    
    function res:close()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."cursor expected")
        end

        if isClosed then
            return false
        end
        
        rs:close()
        st:close()
        closeFunc(res)
        
        isClosed = true
        
        return true
    end
    
    function res:fetch(tb, modestring)
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."cursor expected")
        end

        if tb == nil or type(tb) ~= "table" then
            tb = {}
            local cond, tb = pcall(cursor.fetch, cursor, tb, "n")
            if not cond then
                error(LUASQ_PREFIX.."error fetching result")
            end
            
            if tb then 
                return unpack(tb) 
            else 
                return nil 
            end
        end
        
        if modestring == nil or type(modestring) ~= "string" then
            modestring = "n"
        end
        
        local cond, tb = pcall(cursor.fetch, cursor, tb, modestring)
        if not cond then
            return nil, tb
        end
        
        return tb
    end
    
    function res:getcolnames()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."cursor expected")
        end

        if names then
            return names
        end
        
        local cond, tb = pcall (cursor.getcolnames, cursor)
        if not cond then
            return cond, tb
        end
        
        names = tb
        
        return tb
    end
    
    function res:getcoltypes()
    
        -- For compatibility with other drivers
        if type(self) ~= "table" then
            error(LUASQL_PREFIX.."cursor expected")
        end
        
        if types then
            return types
        end

        local cond, tb = pcall(cursor.getcoltypes, cursor)
        if not cond then
            return nil, tb
        end
        
        types = tb
        
        return tb
    end
    
    -- For compatibility with other drivers
    setmetatable(res, {__metatable = LUASQL_PREFIX.."you're not allowed to get this metatable"})

    return res
end
