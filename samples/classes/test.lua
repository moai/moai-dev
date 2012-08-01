class Exception
  function Exception(msg)
    self.message = msg
  end
end

class InvalidTypeException inherits Exception
  function InvalidTypeException()
    base("Attempted to add invalid type.")
  end
end

class Stack
  function Stack(T)
    self.T = T
    self.backer = {}
  end

  function Add(val)
    if (!(val is self.T)) then
      error(new InvalidTypeException())
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

mystack = new Stack(string)
--try
  mystack:add("hello world")
  mystack:add(5)
--catch (invalid_type_exception)
--  print("Oh dear! I tried to add something that was the wrong type.")
--end


--[[namespace MyNamespace.NestedNamespace
	print("In nested namespace.")
end

namespace MyNamespace
	namespace ExplicitNestedNamespace
		print("In explicitly nested namespace.")
	end
end

class GlobalClass
	print("In globally defined class.")

	function GlobalClass()
		print("In globally defined class constructor.")
	end

	function test()
		print("In globally defined class function.")
	end
end

namespace MyNamespace
	class MyClass
		print("In namespace scoped class.")
		
		function MyClass()
			print("In namespace scoped class constructor.")
		end

		function test()
			print("In namespace scoped class function.")
		end

		class NestedClass
			print("In nested class.")
			
			function NestedClass()
				print("In nested class constructor.")
			end

			function test()
				print("In nested class function.")
			end
		end
	end
end]]
