--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--============================================================--
-- initTransform2DInterface
--============================================================--
local function initTransform2DInterface ( interface, superInterface )

	----------------------------------------------------------------
	function interface.addLoc ( self, xDelta, yDelta )
		superInterface.addLoc ( self, xDelta, yDelta, 0 )
	end

	----------------------------------------------------------------
	function interface.addPiv ( self, xDelta, yDelta )
		superInterface.addPiv ( self, xDelta, yDelta, 0 )
	end
	
	----------------------------------------------------------------
	function interface.addRot ( self, rDelta )
		superInterface.addRot ( self, 0, 0, rDelta )
	end
	
	----------------------------------------------------------------
	function interface.addScl ( self, xSclDelta, ySclDelta )
		superInterface.addScl ( self, xSclDelta, ySclDelta, 0 )
	end
	
	----------------------------------------------------------------
	function interface.getLoc ( self )
		local x, y = superInterface.getLoc ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.getPiv ( self )
		local x, y = superInterface.getPiv ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.getRot ( self )
		local x, y, z = superInterface.getRot ( self )
		return z
	end

	----------------------------------------------------------------
	function interface.getScl ( self )
		local x, y = superInterface.getScl ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.modelToWorld ( self, x, y )
		local x, y = superInterface.modelToWorld ( self, x, y, 0 )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.move ( self, xDelta, yDelta, rDelta, xSclDelta, ySclDelta, length, mode )
		return superInterface.move ( self, xDelta, yDelta, 0, 0, 0, rDelta, xSclDelta, ySclDelta, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.moveLoc ( self, xDelta, yDelta, length, mode )
		return superInterface.moveLoc ( self, xDelta, yDelta, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.movePiv ( self, xDelta, yDelta, length, mode )
		return superInterface.movePiv ( self, xDelta, yDelta, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.moveRot ( self, rDelta, length, mode )
		return superInterface.moveRot ( self, 0, 0, rDelta, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.moveScl ( self, xSclDelta, ySclDelta, length, mode )
		return superInterface.moveScl ( self, xSclDelta, ySclDelta, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seek ( self, xGoal, yGoal, rGoal, xSclGoal, ySclGoal, length, mode )
		return superInterface.seek ( self, xGoal, yGoal, 0, 0, 0, rGoal, xSclGoal, ySclGoal, 1, length, mode )
	end

	----------------------------------------------------------------
	function interface.seekLoc ( self, xGoal, yGoal, length, mode )
		return superInterface.seekLoc ( self, xGoal, yGoal, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.seekPiv ( self, xGoal, yGoal, length, mode )
		return superInterface.seekPiv ( self, xGoal, yGoal, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seekRot ( self, rGoal, length, mode )
		return superInterface.seekRot ( self, 0, 0, rGoal, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seekScl ( self, xSclGoal, ySclGoal, length, mode )
		return superInterface.seekScl ( self, xSclGoal, ySclGoal, 1, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.setLoc ( self, x, y )
		superInterface.setLoc ( self, x, y, 0 )
	end
	
	----------------------------------------------------------------
	function interface.setPiv ( self, x, y )
		superInterface.setPiv ( self, x, y, 0 )
	end
	
	----------------------------------------------------------------
	function interface.setRot ( self, rot )
		superInterface.setRot ( self, 0, 0, rot )
	end
	
	----------------------------------------------------------------
	function interface.setScl ( self, x, y )
		superInterface.setScl ( self, x, y, 1 )
	end
	
	----------------------------------------------------------------
	function interface.worldToModel ( self, x, y )
		local x, y = superInterface.worldToModel ( self, x, y, 0 )
		return x, y
	end
end

--============================================================--
-- initTextLabelInterface
--============================================================--
local function initTextLabelInterface ( interface, superInterface )

	----------------------------------------------------------------
	function interface.affirmStyle ( self )
		local style = self:getStyle ()
		if not style then
			style = MOAITextStyle.new ()
			self:setStyle ( style )
		end
		return style
	end
	
	----------------------------------------------------------------
	function interface.setFont ( self, font )
		local style = self:affirmStyle ()
		style:setFont ( font )
	end
	
	----------------------------------------------------------------
	function interface.setTextSize ( self, points, dpi )
		local style = self:affirmStyle ()
		style:setSize ( points, dpi )
	end
end

--============================================================--
-- moai2D.lua - version 1.0 Beta
--============================================================--

--============================================================--
-- MOAICamera
--============================================================--
MOAICamera.extend (

	'MOAICamera2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		interface.getFieldOfView = nil
		interface.getFocalLength = nil
		interface.setFieldOfView = nil
		interface.setOrtho = nil
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFarPlane ( self, far )
			superInterface.setFarPlane ( self, far or -1 )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setNearPlane ( self, near )
			superInterface.setNearPlane ( self, near or 1 )
		end
		
		-- extend the class
		local new = class.new
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.new ()
			local self = new ()
			superInterface.setOrtho ( self, true )
			superInterface.setNearPlane ( self, 1 )
			superInterface.setFarPlane ( self, -1 )
			return self
		end
	end
)

--============================================================--
-- MOAIFreeTypeFontReader
--============================================================--
if MOAIFreeTypeFontReader then
	MOAIFont.extend (
	
		'MOAIFont',
		
		----------------------------------------------------------------
		function ( interface, class, superInterface, superClass )
			
			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the interface
			function interface.loadFromTTF ( self, filename, charcodes, points, dpi )
				self:load ( filename )
				self:preloadGlyphs ( charcodes, points, dpi )
			end
		
			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the class
			local new = class.new
			
			function class.new ()
				local self = new ()
				superInterface.setCache ( self, MOAIDynamicGlyphCache.new ())
				superInterface.setReader ( self, MOAIFreeTypeFontReader.new ())
				return self
			end
		end
	)
end

--============================================================--
-- MOAIHttpTask
--============================================================--
MOAIHttpTask = MOAIHttpTaskNSURL or MOAIHttpTaskNaCl or MOAIHttpTaskCurl 

--============================================================--
-- MOAIJsonParser
--============================================================--
if MOAIJsonParser then

	MOAIJsonParser.extend (

		'MOAIJsonParser',
		
		----------------------------------------------------------------
		function ( class, superClass )

			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the class
			function class.decodeFromFile ( filename )

				local json = MOAIFileSystem.loadFile ( filename )
				return json and MOAIJsonParser.decode ( json )
			end

			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the class
			function class.encodeToFile ( filename, table, flags )

				flags = flags or ( MOAIJsonParser.JSON_INDENT + MOAIJsonParser.JSON_SORT_KEYS )
				local json = MOAIJsonParser.encode ( table, flags )
				MOAIFileSystem.saveFile ( filename, json )
			end
		end
	)
end

--============================================================--
-- MOAILayer
--============================================================--
MOAILayer.extend (

	'MOAILayer2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		interface.setPartitionCull2D = nil
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setSortScale ( self, x, y, priority )
			superInterface.setSortScale ( self, x, y, 0, priority )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.wndToWorld ( self, x, y )
			local x, y = superInterface.wndToWorld ( self, x, y, 0 )
			return x, y
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.worldToWnd ( self, x, y )
			local x, y = superInterface.worldToWnd ( self, x, y, 0 )
			return x, y
		end
		
		-- extend the class
		class.SORT_Z_ASCENDING = nil
		class.SORT_Z_DESCENDING = nil
	end
)

--============================================================--
-- MOAIGraphicsProp
--============================================================--
MOAIGraphicsProp.extend (

	'MOAIGraphicsProp2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFrame ( self, xMin, yMin, xMax, yMax )
			superInterface.setBounds ( self, xMin, yMin, 0, xMax, yMax, 0 )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.getRect ( self )			
			print ( self )
			local xMin, yMin, zMin, xMax, yMax, zMax = superInterface.getBounds ( self )
			return xMin, yMin, xMax, yMax
		end
	end
)

--============================================================--
-- MOAIRenderMgr
--============================================================--
MOAIRenderMgr.extend (

	'MOAIRenderMgr',
	
	----------------------------------------------------------------
	function ( class, superClass )
	
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		function class.affirmRenderTable ()
		
			local renderTable = class.getRenderTable ()
			if not renderTable then
				renderTable = {}
				class.setRenderTable ( renderTable )
			end
			
			return renderTable
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.clearRenderStack ()
			class.setRenderTable ( nil )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.grabNextFrame (image, callback)
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			frameBuffer:grabNextFrame (image, callback)
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.getRenderTable ()
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			return frameBuffer:getRenderTable ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.popRenderPass ()

			local renderTable = class.affirmRenderTable ()
			table.remove ( renderTable )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.pushRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			table.insert ( renderTable, pass )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.removeRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			for i, cursor in ipairs ( renderTable ) do
				if cursor == pass then
					table.remove ( renderTable, i )
					break
				end
			end
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setRenderTable ( renderTable )
		
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			frameBuffer:setRenderTable ( renderTable )
		end
	end
)

--============================================================--
-- MOAISim
--============================================================--
MOAISim.extend (

	'MOAISim',
	
	----------------------------------------------------------------
	function ( class, superClass )

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		function class.clearRenderStack ()
			MOAIRenderMgr.clearRenderStack ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.popRenderPass ()
			MOAIRenderMgr.popRenderPass ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.pushRenderPass ( pass )
			MOAIRenderMgr.pushRenderPass ( pass )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.removeRenderPass ( pass )
			MOAIRenderMgr.removeRenderPass ( pass )
		end 
	end
)

--============================================================--
-- MOAIScriptNode
--============================================================--
MOAIScriptNode.extend (
	'MOAIScriptNode',

	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.connect ( self, srcField, node, dstField )
			local attrSrc = self.__attrNames [ srcField ]
			local attrDst = node.__attrNames [ dstField ]
			if attrSrc and attrDst then
				interface.setAttrLink ( self, attrSrc, node, attrDst )
			end
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		local new = class.new
		
		function class.new ()
			local self = new ()

			self.__attrNames = {}
			local ref = superInterface.getRefTable ( self )
			local mem = superInterface.getMemberTable ( self )

			ref.__newindex = function ( self, key, value )
				if self.__attrNames [ key ] then
					mem [ key ] = value
					superInterface.scheduleUpdate ( self )
				else
					mem [ key ] = value
				end
			end

			return self
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setAttrName ( self, attrId, attrName )
			superInterface.setAttrName ( self, attrId, attrName )
			self.__attrNames [ attrName ] = attrId
		end
	end
)

--============================================================--
-- MOAITextLabel
--============================================================--
MOAITextLabel.extend (

	'MOAITextLabel',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		initTextLabelInterface ( interface, superInterface )
		
		interface.getString = superInterface.getText
		interface.getStringBounds = superInterface.getTextBounds
		interface.setString = superInterface.setText
	end
)

--============================================================--
-- MOAITransform
--============================================================--
MOAITransform.extend (

	'MOAITransform2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
	end
)

--============================================================--
-- MOAIGfxDevice
--============================================================--
MOAIGfxDevice.extend (

	'MOAIGfxDevice',
	
	----------------------------------------------------------------
	function ( class, superClass )

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		function class.setClearColor ( ... )
			MOAIGfxDevice.getFrameBuffer():setClearColor ( ... )
		end
	end
)

--============================================================--
-- MOAIXmlParser
--============================================================--
if MOAIXmlParser then
	MOAIXmlParser.extend (

		'MOAIXmlParser',
		
		----------------------------------------------------------------
		function ( interface, class, superInterface, superClass )

			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the class
			function class.events ( stream )
			
				local parser = MOAIXmlParser.new ()
				parser:setStream ( stream )
				local more = true
		
				local element = {
					getAttribute	= function ( name ) return parser:getElementAttribute ( name ) end,
					getAttributes	= function () return parser:getElementAttributes () end,
					getLineNumber	= function () return parser:getElementLineNumber () end,
					getName			= function () return parser:getElementName () end,
					getText			= function () return parser:getElementText () end,
				}
		
				return function ()
					if more then
						local event = parser:step ()
						more = ( event ~= MOAIXmlParser.XML_ERROR ) and ( event ~= MOAIXmlParser.DONE )
						return event, more and element
					end
				end
			end
		end
	)
end

--============================================================--
-- renames
--============================================================--

MOAIHashWriter		= MOAIHashWriterCrypto or MOAIHashWriter
MOAITextBox			= MOAITextLabel
MOAIThread			= MOAICoroutine

MOAIProp			= MOAIGraphicsProp
MOAIProp2D			= MOAIGraphicsProp2D

MOAIStreamReader	= MOAIStreamAdapter
MOAIStreamWriter	= MOAIStreamAdapter

--============================================================--
-- Cross Platform
--============================================================--

if MOAISim.forceGC then
    MOAISim.forceGarbageCollection = MOAISim.forceGC
end

MOAIApp = MOAIApp or MOAIAppAndroid or MOAIAppIOS
MOAIAudioSampler = MOAIAudioSampler or MOAIAudioSamplerCocoa or MOAIAudioSamplerAndroid
MOAIBrowser = MOAIBrowser or MOAIBrowserAndroid or MOAIBrowserIOS
MOAISafariIOS = MOAIBrowserIOS
MOAIDialog = MOAIDialog or MOAIDialogAndroid or MOAIDialogIOS
MOAIMoviePlayer = MOAIMoviePlayer or MOAIMoviePlayerAndroid or MOAIMoviePlayerIOS

-- Optional 3rd party extensions
MOAIAdColony = MOAIAdColonyAndroid or MOAIAdColonyIOS
MOAIBilling = MOAIBilling or MOAIBillingAndroid or MOAIBillingIOS
MOAIChartBoost = MOAIChartBoostAndroid or MOAIChartBoostIOS
MOAICrittercism = MOAICrittercismAndroid or MOAICrittercismIOS
MOAIFacebook = MOAIFacebookAndroid or MOAIFacebookIOS
MOAINotifications = MOAINotificationsAndroid or MOAINotificationsIOS
MOAITapjoy = MOAITapjoyAndroid or MOAITapjoyIOS
MOAITwitter = MOAITwitterAndroid or MOAITwitterIOS

-- Compatibility

if MOAITwitterIOS then
    MOAITwitter.sendTweet = function(text, url) MOAITwitterIOS.composeTweet(text, url) end

    MOAITwitter.setAccessToken = function()  end

    MOAITwitter.init = function() end

    MOAITwitter.isLoggedIn = function () return MOAITwitterIOS.canTweet() end

    MOAITwitter.SESSION_DID_LOGIN = MOAITwitter.TWEET_SUCCESSFUL + 1
    MOAITwitter.SESSION_DID_NOT_LOGIN = MOAITwitter.SESSION_DID_LOGIN + 1

    MOAITwitter.login = function()
        local listener = MOAITwitter.loginListeners[MOAITwitter.SESSION_DID_NOT_LOGIN] 
        if MOAITwitter.isLoggedIn() then
            listener = MOAITwitter.loginListeners[MOAITwitter.SESSION_DID_LOGIN] 
        end
        listener()
    end

    MOAITwitter._oldSetListener = MOAITwitter.setListener

    MOAITwitter.setListener = function(event, listener)
        if event == MOAITwitter.SESSION_DID_LOGIN or event == MOAITwitter.SESSION_DID_NOT_LOGIN then
            MOAITwitter.loginListeners = MOAITwitter.loginListeners or {}
            MOAITwitter.loginListeners[event] = listener
        else
            MOAITwitter._oldSetListener(event, listener)
        end
    end
end

--============================================================--
-- Managers
--============================================================--

local wrapGlobal = function ( obj, func )
	return function ( ... ) return func ( obj, ... ) end
end

MOAIActionMgr = MOAISim.getActionMgr ()

MOAIActionMgr.getRoot					= wrapGlobal ( MOAIActionMgr, MOAIActionMgr.getRoot )
MOAIActionMgr.setProfilingEnabled		= wrapGlobal ( MOAIActionMgr, MOAIActionMgr.setProfilingEnabled )
MOAIActionMgr.setRoot					= wrapGlobal ( MOAIActionMgr, MOAIActionMgr.setRoot )
MOAIActionMgr.setThreadInfoEnabled		= wrapGlobal ( MOAIActionMgr, MOAIActionMgr.setThreadInfoEnabled )

if MOAIApp then

	MOAIApp.lifecycle = {}

	MOAIApp.addLifecycleListener = function ( eventID, listener )

		local listeners = MOAIApp.lifecycle [ eventID ] or {}
		MOAIApp.lifecycle [ eventID ] = listeners

		table.insert ( listeners, listener )
	end

	MOAIApp.invokeLifecycleListener = function ( eventID, ... )

		local listeners = MOAIApp.lifecycle [ eventID ]
		if listeners then
			for i, listener in pairs ( listeners ) do
				listener ( ... )
			end
		end
	end
end
