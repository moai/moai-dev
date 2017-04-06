module ( 'expression', package.seeall )

		add						= nil
		divide					= nil
		multiply				= nil
		reduce					= nil
		subtract				= nil
		toComponent				= nil

----------------------------------------------------------------
add = function ( lhs, rhs )

	local lhs = toComponent ( lhs )
	local rhs = toComponent ( rhs )

	if lhs == 0 and rhs == 0 then return 0 end
	if lhs == 0 then return rhs end
	if rhs == 0 then return lhs end

	return string.format ( '(%s + %s)', tostring ( lhs ), tostring ( rhs ))
end

----------------------------------------------------------------
divide = function ( lhs, rhs )

	local lhs = toComponent ( lhs )
	local rhs = toComponent ( rhs )

	if rhs == 1 then return lhs end

	return string.format ( '(%s / %s)', tostring ( lhs ), tostring ( rhs ))
end

----------------------------------------------------------------
multiply = function ( lhs, rhs )

	local lhs = toComponent ( lhs )
	local rhs = toComponent ( rhs )

	if lhs == 0 or rhs == 0 then return 0 end
	if lhs == 1 then return rhs end
	if rhs == 1 then return lhs end

	return string.format ( '(%s * %s)', tostring ( lhs ), tostring ( rhs ))
end

----------------------------------------------------------------
subtract = function ( lhs, rhs )

	local lhs = toComponent ( lhs )
	local rhs = toComponent ( rhs )

	if lhs == 0 and rhs == 0 then return 0 end
	if lhs == 0 then return string.format ( '-%s', tostring ( rhs )) end
	if rhs == 0 then return lhs end

	return string.format ( '(%s - %s)', tostring ( lhs ), tostring ( rhs ))
end

----------------------------------------------------------------
toComponent = function ( val )

	return val == nil and 0 or type ( val ) == 'number' and ( val == 0 or val == 1 ) and val or tostring ( val )
end
