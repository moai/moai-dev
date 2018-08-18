----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc.
-- All Rights Reserved.
-- http://getmoai.com
----------------------------------------------------------------

module ( 'matrix', package.seeall )

R1C1 = 1
R1C2 = 2
R1C3 = 3
R1C4 = 4
R2C1 = 5
R2C2 = 6
R2C3 = 7
R2C4 = 8
R3C1 = 9
R3C2 = 10
R3C3 = 11
R3C4 = 12
R4C1 = 13
R4C2 = 14
R4C3 = 15
R4C4 = 16

--==============================================================
-- interface
--==============================================================

			append						= nil
local		append_set					= nil
			create						= nil
			get							= nil
			ident						= nil
			invert						= nil
local		invert_set					= nil
			log							= nil
			multiply					= nil
			prepend						= nil
local		prepend_set					= nil
			rotateX						= nil
			rotateY						= nil
			rotateZ						= nil
			scale						= nil
			set							= nil
			translate					= nil
			transpose					= nil
local		transpose_set				= nil

--==============================================================
-- implementation
--==============================================================

----------------------------------------------------------------
append = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	return multiply (
		
		r1c1, r1c2, r1c3, r1c4,
		r2c1, r2c2, r2c3, r2c4,
		r3c1, r3c2, r3c3, r3c4,
		r4c1, r4c2, r4c3, r4c4,

		mtx [ R1C1 ], mtx [ R1C2 ], mtx [ R1C3 ], mtx [ R1C4 ],
		mtx [ R2C1 ], mtx [ R2C2 ], mtx [ R2C3 ], mtx [ R2C4 ],
		mtx [ R3C1 ], mtx [ R3C2 ], mtx [ R3C3 ], mtx [ R3C4 ],
		mtx [ R4C1 ], mtx [ R4C2 ], mtx [ R4C3 ], mtx [ R4C4 ]
	)
end

----------------------------------------------------------------
append_set = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	set ( mtx, append ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 ))
end

----------------------------------------------------------------
create = function ( r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	local mtx = {}

	if r1c1 then
		set ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )
	else
		set ( mtx, ident ())
	end

	mtx.append		= append_set
	mtx.get			= get
	mtx.invert		= invert_set
	mtx.log			= log
	mtx.prepend		= prepend_set
	mtx.set			= set
	mtx.transpose	= transpose_set

	return mtx
end

----------------------------------------------------------------
get = function ( mtx )

	return
		mtx [ R1C1 ], mtx [ R1C2 ], mtx [ R1C3 ], mtx [ R1C4 ],
		mtx [ R2C1 ], mtx [ R2C2 ], mtx [ R2C3 ], mtx [ R2C4 ],
		mtx [ R3C1 ], mtx [ R3C2 ], mtx [ R3C3 ], mtx [ R3C4 ],
		mtx [ R4C1 ], mtx [ R4C2 ], mtx [ R4C3 ], mtx [ R4C4 ]
end

----------------------------------------------------------------
ident = function ()

	return
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
end

----------------------------------------------------------------
invert = function ( r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	if type ( r1c1 ) == 'table' then

		return invert ( get ( r1c1 ))
	end

	-- 2x2 determinants
	local fA1 = ( r1c1 * r2c2 ) - ( r1c2 * r2c1 );
	local fA2 = ( r1c1 * r2c3 ) - ( r1c3 * r2c1 );
	local fA3 = ( r1c1 * r2c4 ) - ( r1c4 * r2c1 );
	local fA4 = ( r1c2 * r2c3 ) - ( r1c3 * r2c2 );
	local fA5 = ( r1c2 * r2c4 ) - ( r1c4 * r2c2 );
	local fA6 = ( r1c3 * r2c4 ) - ( r1c4 * r2c3 );
	local fB1 = ( r3c1 * r4c2 ) - ( r3c2 * r4c1 );
	local fB2 = ( r3c1 * r4c3 ) - ( r3c3 * r4c1 );
	local fB3 = ( r3c1 * r4c4 ) - ( r3c4 * r4c1 );
	local fB4 = ( r3c2 * r4c3 ) - ( r3c3 * r4c2 );
	local fB5 = ( r3c2 * r4c4 ) - ( r3c4 * r4c2 );
	local fB6 = ( r3c3 * r4c4 ) - ( r3c4 * r4c3 );

	-- 3x3 determinants
	local det = ( fA1 * fB6 ) - ( fA2 * fB5 ) + ( fA3 * fB4 ) + ( fA4 * fB3 ) - ( fA5 * fB2 ) + ( fA6 * fB1 )

	-- if determinant == 0.0, there is no inverse
	if det == 0.0 then return nil end

	local invDet = 1.0 / det

	-- transpose & 3x3 determinants * inverse determinant
	return
		(  ( r2c2 * fB6 ) - ( r2c3 * fB5 ) + ( r2c4 * fB4 )) * invDet,
		( -( r1c2 * fB6 ) + ( r1c3 * fB5 ) - ( r1c4 * fB4 )) * invDet,
		(  ( r4c2 * fA6 ) - ( r4c3 * fA5 ) + ( r4c4 * fA4 )) * invDet,
		( -( r3c2 * fA6 ) + ( r3c3 * fA5 ) - ( r3c4 * fA4 )) * invDet,

		( -( r2c1 * fB6 ) + ( r2c3 * fB3 ) - ( r2c4 * fB2 )) * invDet,
		(  ( r1c1 * fB6 ) - ( r1c3 * fB3 ) + ( r1c4 * fB2 )) * invDet,
		( -( r4c1 * fA6 ) + ( r4c3 * fA3 ) - ( r4c4 * fA2 )) * invDet,
		(  ( r3c1 * fA6 ) - ( r3c3 * fA3 ) + ( r3c4 * fA2 )) * invDet,

		(  ( r2c1 * fB5 ) - ( r2c2 * fB3 ) + ( r2c4 * fB1 )) * invDet,
		( -( r1c1 * fB5 ) + ( r1c2 * fB3 ) - ( r1c4 * fB1 )) * invDet,
		(  ( r4c1 * fA5 ) - ( r4c2 * fA3 ) + ( r4c4 * fA1 )) * invDet,
		( -( r3c1 * fA5 ) + ( r3c2 * fA3 ) - ( r3c4 * fA1 )) * invDet,

		( -( r2c1 * fB4 ) + ( r2c2 * fB2 ) - ( r2c3 * fB1 )) * invDet,
		(  ( r1c1 * fB4 ) - ( r1c2 * fB2 ) + ( r1c3 * fB1 )) * invDet,
		( -( r4c1 * fA4 ) + ( r4c2 * fA2 ) - ( r4c3 * fA1 )) * invDet,
		(  ( r3c1 * fA4 ) - ( r3c2 * fA2 ) + ( r3c3 * fA1 )) * invDet
end

----------------------------------------------------------------
invert_set = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	set ( mtx, invert ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 ))
end

----------------------------------------------------------------
log = function ( r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	if type ( r1c1 ) == 'table' then

		log ( get ( r1c1 ))
	else

		print ( string.format ( '[ %f %f %f %f ]', r1c1, r1c2, r1c3, r1c4 ))
		print ( string.format ( '[ %f %f %f %f ]', r2c1, r2c2, r2c3, r2c4 ))
		print ( string.format ( '[ %f %f %f %f ]', r3c1, r3c2, r3c3, r3c4 ))
		print ( string.format ( '[ %f %f %f %f ]', r4c1, r4c2, r4c3, r4c4 ))
	end
end

----------------------------------------------------------------
multiply = function (
	a_r1c1, a_r1c2, a_r1c3, a_r1c4, a_r2c1, a_r2c2, a_r2c3, a_r2c4, a_r3c1, a_r3c2, a_r3c3, a_r3c4, a_r4c1, a_r4c2, a_r4c3, a_r4c4,
	b_r1c1, b_r1c2, b_r1c3, b_r1c4, b_r2c1, b_r2c2, b_r2c3, b_r2c4, b_r3c1, b_r3c2, b_r3c3, b_r3c4, b_r4c1, b_r4c2, b_r4c3, b_r4c4 )

	return 
		( a_r1c1  *  b_r1c1 )	+ ( a_r1c2  *  b_r2c1 ) + ( a_r1c3  *  b_r3c1 ) + ( a_r1c4  *  b_r4c1 ),	-- R1C1
		( a_r1c1  *  b_r1c2 )	+ ( a_r1c2  *  b_r2c2 ) + ( a_r1c3  *  b_r3c2 ) + ( a_r1c4  *  b_r4c2 ),	-- R1C2
		( a_r1c1  *  b_r1c3 )	+ ( a_r1c2  *  b_r2c3 ) + ( a_r1c3  *  b_r3c3 ) + ( a_r1c4  *  b_r4c3 ),	-- R1C3
		( a_r1c1  *  b_r1c4 )	+ ( a_r1c2  *  b_r2c4 ) + ( a_r1c3  *  b_r3c4 ) + ( a_r1c4  *  b_r4c4 ),	-- R1C4

		( a_r2c1  *  b_r1c1 )	+ ( a_r2c2  *  b_r2c1 ) + ( a_r2c3  *  b_r3c1 ) + ( a_r2c4  *  b_r4c1 ),	-- R2C1
		( a_r2c1  *  b_r1c2 )	+ ( a_r2c2  *  b_r2c2 ) + ( a_r2c3  *  b_r3c2 ) + ( a_r2c4  *  b_r4c2 ),	-- R2C2
		( a_r2c1  *  b_r1c3 )	+ ( a_r2c2  *  b_r2c3 ) + ( a_r2c3  *  b_r3c3 ) + ( a_r2c4  *  b_r4c3 ),	-- R2C3
		( a_r2c1  *  b_r1c4 )	+ ( a_r2c2  *  b_r2c4 ) + ( a_r2c3  *  b_r3c4 ) + ( a_r2c4  *  b_r4c4 ),	-- R2C4

		( a_r3c1  *  b_r1c1 )	+ ( a_r3c2  *  b_r2c1 ) + ( a_r3c3  *  b_r3c1 ) + ( a_r3c4  *  b_r4c1 ),	-- R3C1
		( a_r3c1  *  b_r1c2 )	+ ( a_r3c2  *  b_r2c2 ) + ( a_r3c3  *  b_r3c2 ) + ( a_r3c4  *  b_r4c2 ),	-- R3C2
		( a_r3c1  *  b_r1c3 )	+ ( a_r3c2  *  b_r2c3 ) + ( a_r3c3  *  b_r3c3 ) + ( a_r3c4  *  b_r4c3 ),	-- R3C3
		( a_r3c1  *  b_r1c4 )	+ ( a_r3c2  *  b_r2c4 ) + ( a_r3c3  *  b_r3c4 ) + ( a_r3c4  *  b_r4c4 ),	-- R3C4

		( a_r4c1  *  b_r1c1 )	+ ( a_r4c2  *  b_r2c1 ) + ( a_r4c3  *  b_r3c1 ) + ( a_r4c4  *  b_r4c1 ),	-- R4C1
		( a_r4c1  *  b_r1c2 )	+ ( a_r4c2  *  b_r2c2 ) + ( a_r4c3  *  b_r3c2 ) + ( a_r4c4  *  b_r4c2 ),	-- R4C2
		( a_r4c1  *  b_r1c3 )	+ ( a_r4c2  *  b_r2c3 ) + ( a_r4c3  *  b_r3c3 ) + ( a_r4c4  *  b_r4c3 ),	-- R4C3
		( a_r4c1  *  b_r1c4 )	+ ( a_r4c2  *  b_r2c4 ) + ( a_r4c3  *  b_r3c4 ) + ( a_r4c4  *  b_r4c4 )		-- R4C4
end

----------------------------------------------------------------
prepend = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	return multiply (

		mtx [ R1C1 ], mtx [ R1C2 ], mtx [ R1C3 ], mtx [ R1C4 ],
		mtx [ R2C1 ], mtx [ R2C2 ], mtx [ R2C3 ], mtx [ R2C4 ],
		mtx [ R3C1 ], mtx [ R3C2 ], mtx [ R3C3 ], mtx [ R3C4 ],
		mtx [ R4C1 ], mtx [ R4C2 ], mtx [ R4C3 ], mtx [ R4C4 ],

		r1c1, r1c2, r1c3, r1c4,
		r2c1, r2c2, r2c3, r2c4,
		r3c1, r3c2, r3c3, r3c4,
		r4c1, r4c2, r4c3, r4c4
	)
end

----------------------------------------------------------------
prepend_set = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	set ( mtx, prepend ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 ))
end

----------------------------------------------------------------
rotateX = function ( d )

	local r = math.rad ( d )

	local c = math.cos ( r )
	local s = math.sin ( r )

	return
		1,  0,  0,  0,
		0,  c, -s,  0,
		0,  s,  c,  0,
		0,  0,  0,  1
end

----------------------------------------------------------------
rotateY = function ( d )

	local r = math.rad ( d )

	local c = math.cos ( r )
	local s = math.sin ( r )

	return
		c,  0,  s,  0,
		0,  1,  0,  0,
		-s, 0,  c,  0,
		0,  0,  0,  1
end

----------------------------------------------------------------
rotateZ = function ( d )

	local r = math.rad ( d )

	local c = math.cos ( r )
	local s = math.sin ( r )

	return
		c,  -s, 0,  0,
		s,  c,  0,  0,
		0,  0,  1,  0,
		0,  0,  0,  1
end

----------------------------------------------------------------
scale = function ( x, y, z )

	return
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
end

----------------------------------------------------------------
set = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	if type ( r1c1 ) == 'table' then

		set ( mtx, get ( r1c1 ))
	else

		mtx [ R1C1 ] = r1c1
		mtx [ R1C2 ] = r1c2
		mtx [ R1C3 ] = r1c3
		mtx [ R1C4 ] = r1c4
		mtx [ R2C1 ] = r2c1
		mtx [ R2C2 ] = r2c2
		mtx [ R2C3 ] = r2c3
		mtx [ R2C4 ] = r2c4
		mtx [ R3C1 ] = r3c1
		mtx [ R3C2 ] = r3c2
		mtx [ R3C3 ] = r3c3
		mtx [ R3C4 ] = r3c4
		mtx [ R4C1 ] = r4c1
		mtx [ R4C2 ] = r4c2
		mtx [ R4C3 ] = r4c3
		mtx [ R4C4 ] = r4c4
	end
end

----------------------------------------------------------------
translate = function ( x, y, z )

	return
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
end

----------------------------------------------------------------
transpose = function ( r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	if type ( r1c1 ) == 'table' then
		
		return transpose ( get ( r1c1 ))
	end

	return
		r1c1, r2c1, r3c1, r4c1,
		r1c2, r2c2, r3c2, r4c2,
		r1c3, r2c3, r3c3, r4c3,
		r1c4, r2c4, r3c4, r4c4
end

----------------------------------------------------------------
transpose_set = function ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 )

	set ( mtx, transpose ( mtx, r1c1, r1c2, r1c3, r1c4, r2c1, r2c2, r2c3, r2c4, r3c1, r3c2, r3c3, r3c4, r4c1, r4c2, r4c3, r4c4 ))
end

