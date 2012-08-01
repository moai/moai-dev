-- Define invalid exception.
class invalid_type_exception
  function invalid_type_exception()
    self.message = "Attempted to add invalid type."
  end
end

-- Define new stack class.
class stack
  function stack(T)
    self.T = T
    self.backer = {}
  end

  function add(val)
    if (not (val is self.T)) then
      error(new invalid_type_exception())
    end
    self.backer[#self.backer+1] = val
  end

  function remove()
    if (#self.backer == 0) then
      error("Attempted to remove without end.")
    end
    self.backer[#self.backer] = nil
  end
end

-- Test.
local mystack = new stack(string)
try
  mystack:add("hello world")
  mystack:add(5)
catch (mystack) do
  if (e is invalid_type_exception) then
    print("blah")
  end
  print("catch handler not yet working.")
catch (invalid_type_exception) do
  print("Oh dear! I tried to add something that was the wrong type.")
end

--class Player
--  function 
--end
