--[[ 
        luaunit.lua

Description: A unit testing framework
Homepage: https://github.com/bluebird75/luaunit
Initial author: Ryu, Gwang (http://www.gpgstudy.com/gpgiki/LuaUnit)
Lot of improvements by Philippe Fremy <phil@freehackers.org>
License: BSD License, see LICENSE.txt
]]--

argv = arg

--[[ Some people like assertEquals( actual, expected ) and some people prefer 
assertEquals( expected, actual ).
]]--
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true

DEFAULT_VERBOSITY = 1

function assertError(f, ...)
    -- assert that calling f with the arguments will raise an error
    -- example: assertError( f, 1, 2 ) => f(1,2) should generate an error
    local has_error, error_msg = not pcall( f, ... )
    if has_error then return end 
    error( "Expected an error but no error generated", 2 )
end

function mytostring( v )
    if type(v) == 'string' then
        return '"'..v..'"'
    end
    if type(v) == 'table' then
        if v.__class__ then
            return string.gsub( tostring(v), 'table', v.__class__ )
        end
        return tostring(v)
    end
    return tostring(v)
end

function errormsg(actual, expected)
    local errorMsg
    if not USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS then
        expected, actual = actual, expected
    end
    if type(expected) == 'string' then
        errorMsg = "\nexpected: "..mytostring(expected).."\n"..
                         "actual  : "..mytostring(actual).."\n"
    else
        errorMsg = "expected: "..mytostring(expected)..", actual: "..mytostring(actual)
    end
    return errorMsg
end

function _table_contains(t, element)
    local _, value, v

    if t then
        for _, value in pairs(t) do
            if type(value) == type(element) then
                if type(element) == 'table' then
                    if _is_table_items_equals(v, expected) then
                        return true
                    end
                else
                    if value == element then
                        return true
                    end
                end
            end
        end
    end
    return false
end

function _is_table_items_equals(actual, expected, parent_key, items)
    if (type(actual) == 'table') and (type(expected) == 'table') then
        local k,v
        for k,v in pairs(actual) do
            if not _table_contains(expected, v) then
                return false
            end
        end
        return true
    elseif type(actual) ~= type(expected) then
        return false
    elseif actual == expected then
        return true
    end
    return false
end

function _is_table_equals(actual, expected)
    if (type(actual) == 'table') and (type(expected) == 'table') then
        local k,v
        for k,v in ipairs(actual) do
            if not _is_table_equals(v, expected[k]) then
                return false
            end
        end
        for k,v in pairs(actual) do
            if (type(k) ~= 'number') and (not _is_table_equals(v, expected[k])) then
                return false
            end
        end
        return true
    elseif type(actual) ~= type(expected) then
        return false
    elseif actual == expected then
        return true
    end
    return false
end

function assertEquals(actual, expected)
    if type(actual) == 'table' and type(expected) == 'table' then
        if not _is_table_equals(actual, expected) then
            error( errormsg(actual, expected), 2 )
        end
    elseif type(actual) ~= type(expected) then
        error( errormsg(actual, expected), 2 )
    elseif actual ~= expected then
        error( errormsg(actual, expected), 2 )
    end
end

function assertTrue(value)
    if not value then
        error("expected: true, actual: " ..mytostring(value), 2)
    end
end

function assertFalse(value)
    if value then
        error("expected: false, actual: " ..mytostring(value), 2)
    end
end

function assertNotEquals(actual, expected)
    if type(actual) == 'table' and type(expected) == 'table' then
        if _is_table_equals(actual, expected) then
            error( errormsg(actual, expected), 2 )
        end
    elseif type(actual) == type(expected) and actual == expected  then
        error( errormsg(actual, expected), 2 )
    end
end

function assertItemsEquals(actual, expected)
    if not _is_table_items_equals(actual, expected, true) then
        error( errormsg(actual, expected), 2 )
    end
end

function assertIsNumber(value)
    if type(value) ~= 'number' then
        error("expected: a number value, actual:" .. type(value))
    end
end

function assertIsString(value)
    if type(value) ~= "string" then
        error("expected: a string value, actual:" .. type(value))
    end
end

function assertIsTable(value)
    if type(value) ~= 'table' then
        error("expected: a table value, actual:" .. type(value))
    end
end

function assertIsBoolean(value)
    if type(value) ~= 'boolean' then
        error("expected: a boolean value, actual:" .. type(value))
    end
end

function assertIsNil(value)
    if type(value) ~= "nil" then
        error("expected: a nil value, actual:" .. type(value))
    end
end

function assertIsFunction(value)
    if type(value) ~= 'function' then
        error("expected: a function value, actual:" .. type(value))
    end
end

function assertIs(actual, expected)
    if actual ~= expected then
        error( errormsg(actual, expected), 2 )
    end
end

function assertNotIs(actual, expected)
    if actual == expected then
        error( errormsg(actual, expected), 2 )
    end
end

assert_equals = assertEquals
assert_not_equals = assertNotEquals
assert_error = assertError
assert_true = assertTrue
assert_false = assertFalse
assert_is_number = assertIsNumber
assert_is_string = assertIsString
assert_is_table = assertIsTable
assert_is_boolean = assertIsBoolean
assert_is_nil = assertIsNil
assert_is_function = assertIsFunction
assert_is = assertIs
assert_not_is = assertNotIs

function __genOrderedIndex( t )
    local orderedIndex = {}
    for key,_ in pairs(t) do
        table.insert( orderedIndex, key )
    end
    table.sort( orderedIndex )
    return orderedIndex
end

function orderedNext(t, state)
    -- Equivalent of the next() function of table iteration, but returns the
    -- keys in the alphabetic order. We use a temporary ordered key table that
    -- is stored in the table being iterated.

    --print("orderedNext: state = "..tostring(state) )
    local key
    if state == nil then
        -- the first time, generate the index
        t.__orderedIndex = nil
        t.__orderedIndex = __genOrderedIndex( t )
        key = t.__orderedIndex[1]
        return key, t[key]
    end
    -- fetch the next value
    key = nil
    for i = 1,#t.__orderedIndex do
        if t.__orderedIndex[i] == state then
            key = t.__orderedIndex[i+1]
        end
    end

    if key then
        return key, t[key]
    end

    -- no more value to return, cleanup
    t.__orderedIndex = nil
    return
end

function orderedPairs(t)
    -- Equivalent of the pairs() function on tables. Allows to iterate
    -- in order
    return orderedNext, t, nil
end

function strsplit(delimiter, text)
-- Split text into a list consisting of the strings in text,
-- separated by strings matching delimiter (which may be a pattern). 
-- example: strsplit(",%s*", "Anna, Bob, Charlie,Dolores")
    local list = {}
    local pos = 1
    if string.find("", delimiter, 1) then -- this would result in endless loops
        error("delimiter matches empty string!")
    end
    while 1 do
        local first, last = string.find(text, delimiter, pos)
        if first then -- found?
            table.insert(list, string.sub(text, pos, first-1))
            pos = last+1
        else
            table.insert(list, string.sub(text, pos))
            break
        end
    end
    return list
end


function prefixString( prefix, s )
    local t, s2
    t = strsplit('\n', s)
    s2 = prefix..table.concat(t, '\n'..prefix)
    return s2
end

----------------------------------------------------------------
--                     class TapOutput
----------------------------------------------------------------

TapOutput = { -- class
    __class__ = 'TapOutput',
    runner = nil,
    result = nil,
}
TapOutput_MT = { __index = TapOutput }

    function TapOutput:new()
        local t = {}
        t.verbosity = 0
        setmetatable( t, TapOutput_MT )
        return t
    end
    function TapOutput:startSuite() end
    function TapOutput:startClass(className) end
    function TapOutput:startTest(testName) end

    function TapOutput:addFailure( errorMsg, stackTrace )
       print(string.format("not ok %d\t%s", self.result.testCount, self.result.currentTestName ))
       if self.verbosity > 0 then
           print( prefixString( '    ', errorMsg ) )
        end
       if self.verbosity > 1 then
           print( prefixString( '    ', stackTrace ) )
        end
    end

    function TapOutput:endTest(testHasFailure)
       if not self.result.currentTestHasFailure then
          print(string.format("ok     %d\t%s", self.result.testCount, self.result.currentTestName ))
       end
    end

    function TapOutput:endClass() end

    function TapOutput:endSuite()
       print("1.."..self.result.testCount)
       return self.result.failureCount
    end


-- class TapOutput end

----------------------------------------------------------------
--                     class JUnitOutput
----------------------------------------------------------------

JUnitOutput = { -- class
    __class__ = 'JUnitOutput',
    runner = nil,
    result = nil,
    xmlFile = nil,
}
JUnitOutput_MT = { __index = JUnitOutput }

    function JUnitOutput:new()
        local t = {}
        t.verbosity = 0
        setmetatable( t, JUnitOutput_MT )
        return t
    end
    function JUnitOutput:startSuite() end
    function JUnitOutput:startClass(className) 
       xmlFile = io.open(string.lower(className) .. ".xml", "w")
       xmlFile:write('<testsuite name="' .. className .. '">\n')
    end
    function JUnitOutput:startTest(testName)
       if xmlFile then xmlFile:write('<testcase classname="' .. self.result.currentClassName .. '" name="'.. testName .. '">') end
    end

    function JUnitOutput:addFailure( errorMsg, stackTrace )
       if xmlFile then 
          xmlFile:write('<failure type="lua runtime error">' ..errorMsg .. '</failure>\n') 
          xmlFile:write('<system-err><![CDATA[' ..stackTrace .. ']]></system-err>\n')
       end
    end

    function JUnitOutput:endTest(testHasFailure)
       if xmlFile then xmlFile:write('</testcase>\n') end
    end

    function JUnitOutput:endClass() end

    function JUnitOutput:endSuite()
       if xmlFile then xmlFile:write('</testsuite>\n') end
       if xmlFile then xmlFile:close() end
       return self.result.failureCount
    end


-- class TapOutput end

----------------------------------------------------------------
--                     class TextOutput
----------------------------------------------------------------

TextOutput = { __class__ = 'TextOutput' }
TextOutput_MT = { -- class
    __index = TextOutput
}

    function TextOutput:new()
        local t = {}
        t.runner = nil
        t.result = nil
        t.errorList ={}
        t.verbosity = 1
        setmetatable( t, TextOutput_MT )
        return t
    end

    function TextOutput:startSuite()
    end

    function TextOutput:startClass(className)
        if self.verbosity > 0 then
            print( '>>>>>>>>> '.. self.result.currentClassName )
        end
    end

    function TextOutput:startTest(testName)
        if self.verbosity > 0 then
            print( ">>> ".. self.result.currentTestName )
        end
    end

    function TextOutput:addFailure( errorMsg, stackTrace )
        table.insert( self.errorList, { self.result.currentTestName, errorMsg, stackTrace } )
        if self.verbosity == 0 then
            io.stdout:write("F")
        end
        if self.verbosity > 0 then
            print( errorMsg )
            print( 'Failed' )
        end
    end

    function TextOutput:endTest(testHasFailure)
        if not testHasFailure then
            if self.verbosity > 0 then
                --print ("Ok" )
            else 
                io.stdout:write(".")
            end
        end
    end

    function TextOutput:endClass()
        if self.verbosity > 0 then
           print()
        end
    end

    function TextOutput:displayOneFailedTest( failure )
        testName, errorMsg, stackTrace = unpack( failure )
        print(">>> "..testName.." failed")
        print( errorMsg )
        if self.verbosity > 1 then
            print( stackTrace )
        end
    end

    function TextOutput:displayFailedTests()
        if #self.errorList == 0 then return end
        print("Failed tests:")
        print("-------------")
        for i,v in ipairs(self.errorList) do
            self:displayOneFailedTest( v )
        end
        print()
    end

    function TextOutput:endSuite()
        if self.verbosity == 0 then
            print()
        else
            print("=========================================================")
        end
        self:displayFailedTests()
        local successPercent, successCount
        successCount = self.result.testCount - self.result.failureCount
        if self.result.testCount == 0 then
            successPercent = 100
        else
            successPercent = math.ceil( 100 * successCount / self.result.testCount )
        end
        print( string.format("Success : %d%% - %d / %d",
            successPercent, successCount, self.result.testCount) )
    end


-- class TextOutput end


----------------------------------------------------------------
--                     class NilOutput
----------------------------------------------------------------

function nopCallable() 
    --print(42) 
    return nopCallable
end

NilOutput = {
    __class__ = 'NilOuptut',    
}
NilOutput_MT = {
    __index = nopCallable,
}
function NilOutput:new()
    local t = {}
    t.__class__ = 'NilOutput'
    setmetatable( t, NilOutput_MT )
    return t 
end

----------------------------------------------------------------
--                     class LuaUnit
----------------------------------------------------------------

LuaUnit = {
    outputType = TextOutput,
    verbosity = DEFAULT_VERBOSITY,
    __class__ = 'LuaUnit'
}
LuaUnit_MT = { __index = LuaUnit }

    function LuaUnit:new()
        local t = {}
        setmetatable( t, LuaUnit_MT )
        return t
    end

    -----------------[[ Utility methods ]]---------------------

    function LuaUnit.isFunction(aObject) 
        return 'function' == type(aObject)
    end

    --------------[[ Output methods ]]-------------------------

    function LuaUnit:ensureSuiteStarted( )
        if self.result and self.result.suiteStarted then
            return
        end
        self:startSuite()
    end

    function LuaUnit:startSuite()
        self.result = {}
        self.result.failureCount = 0
        self.result.testCount = 0
        self.result.currentTestName = ""
        self.result.currentClassName = ""
        self.result.currentTestHasFailure = false
        self.result.suiteStarted = true
        self.outputType = self.outputType or TextOutput
        self.output = self.outputType:new()
        self.output.runner = self
        self.output.result = self.result
        self.output.verbosity = self.verbosity
        self.output:startSuite()
    end

    function LuaUnit:startClass( className )
        self.result.currentClassName = className
        self.output:startClass( className )
    end

    function LuaUnit:startTest( testName  )
        self.result.currentTestName = testName
        self.result.testCount = self.result.testCount + 1
        self.result.currentTestHasFailure = false
        self.output:startTest( testName )
    end

    function LuaUnit:addFailure( errorMsg, stackTrace )
        if not self.result.currentTestHasFailure then
            self.result.failureCount = self.result.failureCount + 1
            self.result.currentTestHasFailure = true
        end
        self.output:addFailure( errorMsg, stackTrace )
    end

    function LuaUnit:endTest()
        self.output:endTest( self.result.currentTestHasFailure )
        self.result.currentTestName = ""
        self.result.currentTestHasFailure = false
    end

    function LuaUnit:endClass()
        self.output:endClass()
    end

    function LuaUnit:endSuite()
        self.result.suiteStarted = false
        self.output:endSuite()
    end

    function LuaUnit:setOutputType(outputType)
        -- default to text
        -- tap produces results according to TAP format
        if outputType:upper() == "NIL" then
            self.outputType = NilOutput
            return
        end
        if outputType:upper() == "TAP" then
            self.outputType = TapOutput
            return
        end 
        if outputType:upper() == "JUNIT" then
            self.outputType = JUnitOutput
            return
        end 
        if outputType:upper() == "TEXT" then
            self.outputType = TextOutput
            return
        end
        error( 'No such format: '..outputType)
    end

    function LuaUnit:setVerbosity( verbosity )
        self.verbosity = verbosity
    end

    --------------[[ Runner ]]-----------------

    SPLITTER = '\n>----------<\n'

    function LuaUnit:protectedCall( classInstance , methodInstance)
        -- if classInstance is nil, this is just a function run
        local function err_handler(e)
            return debug.traceback(e..SPLITTER, 4)
        end

        local ok=true, errorMsg, stackTrace
        if classInstance then
            -- stupid Lua < 5.2 does not allow xpcall with arguments so let's live with that
            ok, errorMsg = xpcall( function () methodInstance(classInstance) end, err_handler )
        else
            ok, errorMsg = xpcall( function () methodInstance() end, err_handler )
        end
        if not ok then
            t = strsplit( SPLITTER, errorMsg )
            stackTrace = string.sub(t[2],2)
            self:addFailure( t[1], stackTrace )
        end

        return ok
    end


    function LuaUnit:_runTestMethod(className, methodName, classInstance, methodInstance)
        -- When executing a class method, all parameters are set
        -- When executing a test function, className and classInstance are nil

        if type(methodInstance) ~= 'function' then
            error( tostring(methodName)..'must be a function, not '..type(methodInstance))
        end

        if className == nil then
            className = '<TestFunction>'
        end

        if self.lastClassName ~= className then
            if self.lastClassName ~= nil then
                self:endClass()
            end
            self:startClass( className )
            self.lastClassName = className
        end

        self:startTest(className..':'..methodName)

        -- run setUp first(if any)
        if classInstance and self.isFunction( classInstance.setUp ) then
            self:protectedCall( classInstance, classInstance.setUp)
        end

        -- run testMethod()
        if not self.result.currentTestHasFailure then
            self:protectedCall( classInstance, methodInstance)
        end

        -- lastly, run tearDown(if any)
        if classInstance and self.isFunction(classInstance.tearDown) then
            self:protectedCall( classInstance, classInstance.tearDown)
        end

        self:endTest()
    end

    function LuaUnit:runSomeTest( someName, someInstance )
        -- name is mandatory
        -- if instance is not given, it's looked up in the global namespace
        -- name can be a test class, a test function, or a test class + test method
        -- instance can be a test class or a test function
        -- example: runSomeTest( 'TestToto' )
        -- example: runSomeTest( 'TestToto', TestToto )
        -- example: runSomeTest( 'TestToto:testTiti' )
        -- example: runSomeTest( 'TestToto:testTiti', TestToto )
        -- example: runSomeTest( 'testFunction' )
        -- example: runSomeTest( 'testFunction' , testFunction )

        self:ensureSuiteStarted()

        local hasMethod, methodName, methodInstance, className, classInstance
        if someName == nil or someName == '' then
            error( 'Name is required!')
        end

        hasMethod = string.find(someName, ':' )

        -- name is class + method
        if hasMethod then
            methodName = string.sub(someName, hasMethod+1)
            className = string.sub(someName,1,hasMethod-1)
            classInstance = someInstance

            classInstance = classInstance or _G[className]
            if classInstance == nil then
                error( "No such class: "..className )
            end

            if type(classInstance) ~= 'table' then
                error( 'Instance must be a table')
            end

            methodInstance = classInstance[methodName]
            if methodInstance == nil then
                error( "Could not find method in class "..tostring(className).." for method "..tostring(methodName) )
            end

            self:_runTestMethod( className, methodName, classInstance, methodInstance )
            return
        end

        if someInstance == nil then
            someInstance = _G[someName]
            if not someInstance then
                error( "No such variable: "..someName )
            end
        end

        if (type(someInstance) ~= 'table' and type(someInstance) ~= 'function') then
            error( 'Instance must be function or table')
        end

        -- name is either a function or a class
        if type(someInstance) == 'table' then
            -- run all test methods of the class
            className = someName
            classInstance = someInstance

            for methodName, methodInstance in orderedPairs(classInstance) do
                if LuaUnit.isFunction(methodInstance) and string.sub(methodName, 1, 4) == "test" then
                    self:_runTestMethod( className, methodName, classInstance, methodInstance )
                end
            end
            return
        end

        if type(someInstance) == 'function' then
            self:_runTestMethod( nil, someName, nil, someInstance )
            return
        end

        error( 'Should never be reached...')

    end

    function LuaUnit:run(...)
        -- Run some specific test classes.
        -- If no arguments are passed, run the class names specified on the
        -- command line. If no class name is specified on the command line
        -- run all classes whose name starts with 'Test'
        --
        -- If arguments are passed, they must be strings of the class names 
        -- that you want to run
        local runner = self:new()
        local outputType = os.getenv("outputType")
        if outputType then LuaUnit:setOutputType(outputType) end
        return runner:runSuite(...)
    end

    function LuaUnit:runSuite(...)
        self:startSuite()

        args={...};
        if #args == 0 then
            args = argv
        end

        if #args == 0 then
            -- create the list if classes to run now ! If not, you can
            -- not iterate over _G while modifying it.
            args = {}
            for key, val in pairs(_G) do 
                if string.sub(key,1,4):lower() == 'test' then 
                    table.insert( args, key )
                end
            end
            table.sort( args )
        end

        for i,testName in ipairs( args ) do
            self:runSomeTest( testName )
        end

        if self.lastClassName ~= nil then
            self:endClass()
        end
        self:endSuite()
        return self.result.failureCount
    end
-- class LuaUnit

