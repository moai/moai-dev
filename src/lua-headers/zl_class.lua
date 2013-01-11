
----------------------------------------------------------------
local function shallowCopy ( tDest, tSource )

	for vKey, vValue in pairs ( tSource or {}) do
		tDest [ vKey ] = vValue
	end
end

----------------------------------------------------------------
local function makeRootClass ( nRootClassName )
	
	local tRootClass = {}
	_G [ nRootClassName ] = tRootClass
	
	-- map classes onto metatada
	local tClassMetadata = {}
	
	-- map public instances onto private representations
	-- keys and values are both *weak*: __mode - 'kv'
	local tPublicToPrivate = setmetatable ({}, { __mode = 'kv' })
	
	local tMethods = {}
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tMethods.affirmScopeTables ( tClass )
	
		tClass.public		= tClass.public or {}
		tClass.private		= tClass.private or {}
		tClass.abstract		= tClass.abstract or {}
		tClass.static		= tClass.static or {}
	end
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tMethods.getClassInfo ( tClass )
		return tClassMetadata [ tClass ] or {}
	end
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tMethods.prepare ( tClass )
		
		tMethods.affirmScopeTables ( tClass )
		
		local tInfo = tMethods.getClassInfo ( tClass )
		
		local tPrivateMethods	= {} -- private *and* public methods
		local tPublicMethods	= {} -- public methods only
		local tAbstractMethods	= {} -- abstract methods only
			
		local tPrivateMembers	= {} -- private members
		local tPublicMembers	= {} -- public members
		
		local tStatic			= {} -- public members
		
		tInfo.tPrivateMethods	= tPrivateMethods
		tInfo.tPublicMethods	= tPublicMethods
		tInfo.tAbstractMethods	= tAbstractMethods
		
		tInfo.tPrivateMembers	= tPrivateMembers
		tInfo.tPublicMembers	= tPublicMembers
		tInfo.tStatic			= tStatic
		
		-- this is the environment for all internal methods (public and private)
		local tEnvironment = setmetatable ( {}, { __index = _G }) -- expose globals
		
		-- 'this' method accepts a public instance and returns the corresponding private representation
		-- if no private is found, just return the original public instance
		function tEnvironment.this ( vPublic )
			return tPublicToPrivate [ vPublic ] or vPublic
		end
		
		local tSuperInfo = tClassMetadata [ tInfo.tSuperClass ]
		if tSuperInfo then
			
			tInfo.tSuperInfo = tSuperInfo
			
			local tSuperClassInterface = {}
			
			if tSuperInfo.tPrivateMethods then 
				setmetatable ( tSuperClassInterface, { __index = tSuperInfo.tPrivateMethods })
			end
			
			tSuperClassInterface.super = tSuperInfo.tSuperClassInterface
			tInfo.tSuperClassInterface = tSuperClassInterface
			
			tEnvironment.super = tSuperClassInterface
			
			shallowCopy ( tPrivateMethods, tSuperInfo.tPrivateMethods )
			shallowCopy ( tPublicMethods, tSuperInfo.tPublicMethods )
			shallowCopy ( tAbstractMethods, tSuperInfo.tAbstractMethods )
		
			shallowCopy ( tPrivateMembers, tSuperInfo.tPrivateMembers )
			shallowCopy ( tPublicMembers, tSuperInfo.tPublicMembers )
			
			shallowCopy ( tStatic, tSuperInfo.tStatic )
		end
		
		-- statics are a verbatim copy, after inheritance
		shallowCopy ( tStatic, tClass.static )
		
		-- iterate through abstract declarations and sort them into method or member tables
		for vKey, vValue in pairs ( tClass.abstract ) do
		
			if type ( vValue ) ~= 'function' then
				print ( string.format ( 'illegal declaration of abstract non-function \n%s\n', vKey ))
			else
				tAbstractMethods [ vKey ] = vValue
			end
		end
		
		-- iterate through private declarations and sort them into method or member tables
		for vKey, vValue in pairs ( tClass.private ) do
		
			if type ( vValue ) == 'function' then
				-- store function in the method table and set the environment (to expose 'this' getter)
				tPrivateMethods [ vKey ] = vValue
				tAbstractMethods [ vKey ] = nil
				setfenv ( vValue, tEnvironment )
			else
				tPrivateMembers [ vKey ] = vValue
			end
		end
		
		-- iterate through public declarations and sort them into method or member tables
		for vKey, vValue in pairs ( tClass.public ) do
		
			if type ( vValue ) == 'function' then
				-- store function in *both* method tables and set the environment (to expose 'this' getter)
				tPrivateMethods [ vKey ] = vValue
				tPublicMethods [ vKey ] = vValue
				tAbstractMethods [ vKey ] = nil
				setfenv ( vValue, tEnvironment )
			else
				tPublicMembers [ vKey ] = vValue
			end
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- here's where we set up the class itself
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function tClass.isA ( tOther )
			
			local vItr = tInfo
			while vItr ~= nil do
				if vItr.tClass == tOther then return true end
				vItr = vItr.tSuperInfo
			end
			
			return false
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function tClass.makeSubclass ( sSubClassName )
			tMethods.makeSubclass ( tClass, sSubClassName )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- define the class instance constructor
		function tClass.new ()
			
			local bAbstract = false
			for vKey, vValue in pairs ( tInfo.tAbstractMethods ) do
				print ( string.format ( 'missing abstract method \'%s\'', vKey ))
				bAbstract = true
			end
			
			if bAbstract == true then
				print ( 'cannot instantiate abstract class' )
				return nil
			end
			
			local tPublicInstance = {} -- public instance
			local tPublicInstanceMT = {} -- public instance metatable
			setmetatable ( tPublicInstance, tPublicInstanceMT )
			
			local tPrivateInstance = {} -- private representation
			local tPrivateInstanceMT = {} -- private representation metatable
			setmetatable ( tPrivateInstance, tPrivateInstanceMT )
			
			-- private __index - look in the private table, then the public table, then finally the private methods
			function tPrivateInstanceMT.__index ( tTable, vKey )
				return rawget ( tPrivateInstance, vKey ) or rawget ( tPublicInstance, vKey ) or rawget ( tPrivateMethods, vKey )
			end
			
			tPublicInstanceMT.__index = tInfo.tPublicMethods -- public __index - look only in the public methods
			tPublicInstanceMT.__metatable = 0 -- thwart calls to getmetatable
			tPublicInstanceMT.tPrivateInstance = tPrivateInstance -- strong ref to private representation (as tPublicToPrivate table is weak)
			
			-- register the private instance
			tPublicToPrivate [ tPublicInstance ] = tPrivateInstance -- local weak ref to private via closure
			
			-- initialize the public members
			shallowCopy ( tPublicInstance, tInfo.tPublicMembers )
			shallowCopy ( tPrivateInstance, tInfo.tPrivateMembers )
			
			-- return the instance
			return tPublicInstance
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		if tInfo.tPublicMethods.isA == nil then
			function tInfo.tPublicMethods:isA ( tOther )
				return tClass.isA ( tOther )
			end
		end
		
		tClass.prepare = nil
		
		tClass.public = nil
		tClass.private = nil
		tClass.abstract = nil
		tClass.static = nil
		
		tClassMT = {}
		setmetatable ( tClass, tClassMT )
		
		tClassMT.__index = tStatic
		tClassMT.__metatable = 0
		
		function tClassMT.__newindex ()
			print ( 'attempt to augment sealed class' )
		end
	end
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tMethods.makeSubclass ( tSuperClass, sSubClassName )
		
		local tSub = {}
		tMethods.affirmScopeTables ( tSub )
		
		local tInfo = {}
		tClassMetadata [ tSub ] = tInfo
		
		tInfo.sName = sSubClassName
		tInfo.tSuperClass = tSuperClass
		tInfo.tClass = tSub
		
		function tSub.prepare ()
			return tMethods.prepare ( tSub )
		end
		
		_G [ sSubClassName ] = tSub
	end
	
	local tRootClassInfo = {}
	tClassMetadata [ tRootClass ] = tRootClassInfo
	
	tRootClassInfo.sName = tRootClassName
	tRootClassInfo.tClass = tRootClass
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tRootClass.isA ( tOther )
		return tRootClass == tOther
	end
	
	-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	function tRootClass.makeSubclass ( sSubClassName )
		tMethods.makeSubclass ( tRootClass, sSubClassName )
	end
	
	return tRootClass
end

makeRootClass ( 'ZLClass' )
