MyClass = setmetatable({},
{
	__call = function()
		return setmetatable({},
		{
			__new = function()
				print("Hello World!")
				return {}
			end
		})
	end
})

-- Generics!
T = MyClass;
val = new T();
