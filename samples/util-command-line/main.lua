print ( 'main.lua' )

print ( 'ARGS:' )

if arg then
	print ( arg [ -1 ])
	print ( arg [ 0 ])
	for i, v in ipairs ( arg ) do
		print ( i, v )
	end
end

params = function ( ... )
	for i, v in ipairs ( arg ) do
		print ( i, v )
	end
end

print ( 'PARAMS:' )
params ( ... )

