--==============================================================
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

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
					getErrorString	= function () return parser:getErrorString () end,
					getLineNumber	= function () return parser:getLineNumber () end,
					getName			= function () return parser:getElementName () end,
					getText			= function () return parser:getElementText () end,
				}
				
				return function ()
					if more then
						local event = parser:step ()
						more = ( event ~= MOAIXmlParser.XML_ERROR ) and ( event ~= MOAIXmlParser.DONE )
						return event, element
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
MOAIProp			= MOAIGraphicsProp

MOAIStreamReader	= MOAIStreamAdapter
MOAIStreamWriter	= MOAIStreamAdapter

--============================================================--
-- Cross Platform
--============================================================--

MOAIApp = MOAIApp or MOAIAppAndroid or MOAIAppIOS
MOAIAudioSampler = MOAIAudioSampler or MOAIAudioSamplerCocoa or MOAIAudioSamplerAndroid
MOAIBrowser = MOAIBrowser or MOAIBrowserAndroid or MOAIBrowserIOS
MOAISafariIOS = MOAIBrowserIOS
MOAIDialog = MOAIDialog or MOAIDialogAndroid or MOAIDialogIOS or MOAIDialogSDL
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
