print("start")

namespace A
	namespace B
		print("start of namespace")
		alpha = 4
	end
	
	test = 5

	namespace B
		print("inside namespace")
		beta = 3
	end

	print(alpha)
	print(B.alpha)
end

print(A)
print(test)
print(A.test)
print(alpha)
print(A.alpha)
print(A.B.alpha)
print(beta)
print(A.beta)
print(A.B.beta)

--print(A.B.C.test)

--[[namespace A
	namespace B
		namespace C
			print("hello world 1")
		end
		
--		print("hello world 2")
		
--		namespace D
--			print("hello world 3")
--		end
	end
	
	namespace B
		print("hello again")
	end
end]]

--[[namespace A.B.C
	a = 5

	function print(msg)
		_G["print"]("From C: " .. msg)
	end
end

A.B.C.print("test?")

namespace A.B.C.D.E.F.G
--namespace A
	print("nested")
end

namespace A.B.C
	print("nested 2")
end]]

--[[namespace A
	function test()
		print("test called")
		b()
	end
	
	function b()
		print("all good")
	end

	test()
end

A.test()]]

print("all done")
