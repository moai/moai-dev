require ( 'expression' )

exp = expression

module ( 'matrix', package.seeall )

		chain					= nil
local	get						= nil
local	idx						= nil
		matrix					= nil
		matrix4x4				= nil
		multiply				= nil
local	printColumns			= nil
		rotateX					= nil
		rotateY					= nil
		rotateZ					= nil
		scale					= nil
local	set						= nil
		shear					= nil
		translate				= nil

----------------------------------------------------------------
chain = function ( ... )

	local mtx1 = arg [ 1 ]
	local mtx2 = arg [ 2 ]

	local mtx = multiply ( mtx2, mtx1 )

	for i = 3, #arg do
		mtx = multiply ( arg [ i ], mtx )
	end

	return mtx
end

----------------------------------------------------------------
get = function ( self, c, r )
	return self [ self:idx ( c, r )]
end

----------------------------------------------------------------
idx = function ( self, c, r )
	c = c - 1
	r = r - 1
	return 1 + ( c * self.rows ) + r
end

----------------------------------------------------------------
matrix = function ( cols, rows )

	local mtx = {}

	for col = 1, cols do
		for row = 1, rows do
			mtx [( col * cols ) + row ] = col == row and 1 or 0
		end
	end

	mtx.cols = cols
	mtx.rows = rows

	mtx.get				= get
	mtx.idx				= idx
	mtx.printColumns	= printColumns
	mtx.set				= set

	return mtx
end

----------------------------------------------------------------
matrix4x4 = function ( ... )

	local mtx = matrix ( 4, 4 )

	for col = 1, 4 do
		for row = 1, 4 do

			local c = col - 1
			local r = row - 1

			mtx:set ( col, row, arg [ 1 + ( r * 4 ) + c ] or col == row and 1 or 0 )
		end
	end

	return mtx
end

----------------------------------------------------------------
multiply = function ( mtx1, mtx2 )

	assert ( mtx1.cols == mtx2.rows )

	local rows = mtx1.rows
	local cols = mtx2.cols

	local n = mtx1.cols

	local mtx = matrix ( rows, cols )

	for col = 1, cols do
		for row = 1, rows do

			local v = 0
			for i = 1, n do
				v = exp.add ( v, exp.multiply ( mtx1:get ( i, row ), mtx2:get ( col, i )))
			end
			mtx:set ( col, row, v )
		end
	end

	return mtx
end

----------------------------------------------------------------
printColumns = function ( mtx, labels )

	local cols = mtx.cols
	local rows = mtx.rows

	print ()

	for col = 1, cols do
		for row = 1, rows do

			local v = tostring ( mtx:get ( col, row ))

			if labels then
				local label = labels [ mtx:idx ( col, row )]
				if label then
					print ( string.format ( '%s = %s', label, v ))
				end
			else
				print ( v )
			end
		end
		print ()
	end
end

----------------------------------------------------------------
rotateX = function ( r )

	local mtx = matrix4x4 ()

	mtx [ 1 ]	= 1
	mtx [ 2 ]	= 0
	mtx [ 3 ]	= 0
	mtx [ 4 ]	= 0

	mtx [ 5 ]	= 0
	mtx [ 6 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 7 ]	= string.format ( 'sin(%s)', tostring ( r ))
	mtx [ 8 ]	= 0

	mtx [ 9 ]	= 0
	mtx [ 10 ]	= string.format ( '-sin(%s)', tostring ( r ))
	mtx [ 11 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 12 ]	= 0

	mtx [ 13 ]	= 0
	mtx [ 14 ]	= 0
	mtx [ 15 ]	= 0
	mtx [ 16 ]	= 1

	return mtx
end

----------------------------------------------------------------
rotateY = function ( r )

	local mtx = matrix4x4 ()

	mtx [ 1 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 2 ]	= 0
	mtx [ 3 ]	= string.format ( '-sin(%s)', tostring ( r ))
	mtx [ 4 ]	= 0

	mtx [ 5 ]	= 0
	mtx [ 6 ]	= 1
	mtx [ 7 ]	= 0
	mtx [ 8 ]	= 0

	mtx [ 9 ]	= string.format ( 'sin(%s)', tostring ( r ))
	mtx [ 10 ]	= 0
	mtx [ 11 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 12 ]	= 0

	mtx [ 13 ]	= 0
	mtx [ 14 ]	= 0
	mtx [ 15 ]	= 0
	mtx [ 16 ]	= 1

	return mtx
end

----------------------------------------------------------------
rotateZ = function ( r )

	local mtx = matrix4x4 ()

	mtx [ 1 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 2 ]	= string.format ( 'sin(%s)', tostring ( r ))
	mtx [ 3 ]	= 0
	mtx [ 4 ]	= 0

	mtx [ 5 ]	= string.format ( '-sin(%s)', tostring ( r ))
	mtx [ 6 ]	= string.format ( 'cos(%s)', tostring ( r ))
	mtx [ 7 ]	= 0
	mtx [ 8 ]	= 0

	mtx [ 9 ]	= 0
	mtx [ 10 ]	= 0
	mtx [ 11 ]	= 1
	mtx [ 12 ]	= 0

	mtx [ 13 ]	= 0
	mtx [ 14 ]	= 0
	mtx [ 15 ]	= 0
	mtx [ 16 ]	= 1

	return mtx
end

----------------------------------------------------------------
scale = function ( x, y, z )

	return matrix4x4 (
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	)
end

----------------------------------------------------------------
set = function ( self, c, r, v )
	self [ self:idx ( c, r )] = v
end

----------------------------------------------------------------
shear = function ( x, yx, zx, y, xy, zy, z, xz, yz )

	return matrix4x4 (
		x,  xy, xz, 0,
		yx, y,  yz, 0,
		zx, zy, z,  0,
		0,  0,  0,  1
	)
end

----------------------------------------------------------------
translate = function ( x, y, z )

	return matrix4x4 (
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	)
end
