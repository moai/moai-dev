--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

local mainMenu = {}
mainMenu.layerTable = nil
local mainLayer = nil

local playButton

----------------------------------------------------------------
mainMenu.StartGameCloud = function ( isNewGame )

	local userId
	local saveFile = savefiles.get ( "user" )
	local task
	local result
	local code
		
	--create a new user on the cloud if its a new game
	if isNewGame then
		task = cloud.createPostTask ( "users", { name = "Adam" } )
		result,code = task:waitFinish()
		
		if result then
			saveFile.data = result
			saveFile:saveGame ()
		else
			print ( "failed to create user, code " .. code )
		end 	
	end
		
	--get the user id for further cloud queries
	userId = saveFile.data.id
	
	--get the last click time
	task = cloud.createGetTask ( "user/"..userId, {} )
	result,code = task:waitFinish()
	
	if result then
		globalData.currentTime = os.time ()
		globalData.timeToClick = result.timeToClick
		globalData.clicks = result.clicks
	else
		print ( "failed to get user , code" .. code  )
	end 
	
	statemgr.push ( "states/state-game.lua" )
	
end

----------------------------------------------------------------
mainMenu.onFocus = function ( self )
	
	MOAIGfxDevice.setClearColor ( 0, 0, 0, 1 )
end	

----------------------------------------------------------------
mainMenu.onInput = function ( self )
	if inputmgr:up () then
		
		local x, y = mainLayer:wndToWorld ( inputmgr:getTouch ())
		
		playButton:updateClick ( false, x, y )
		
	elseif inputmgr:down () then
		
		local x, y = mainLayer:wndToWorld ( inputmgr:getTouch ())
		
		playButton:updateClick ( true, x, y )
	end

end

----------------------------------------------------------------
mainMenu.onLoad = function ( self )
		
	self.layerTable = {}
	local layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	mainMenu.layerTable [ 1 ] = { layer }
	
	local font =  MOAIFont.new ()
	font:loadFromTTF ( "arialbd.ttf", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?! ", 12, 163 )

	local textbox = {}
	textbox[1] = MOAITextBox.new ()
	textbox[1]:setFont ( font )
	textbox[1]:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox[1]:setYFlip ( true )
	textbox[1]:setRect ( -150, -230, 150, 230 )
	textbox[1]:setString ( "Wolf Clicker 1.0" )
	layer:insertProp ( textbox[1] )
	
	playButton = elements.makeTextButton ( font, "resources/button.png", 206, 150, 60 )
	
	playButton:setCallback ( function ( self )
		
		local thread = MOAIThread.new ()
		thread:run ( mainMenu.StartGameCloud, mainMenu.newGame )
		
	end )
	
	if savefiles.get ( "user" ).fileexist then
		playButton:setString ( "Continue" )
		playButton.newGame = false
	else
		playButton:setString ( "New Game" )
		playButton.newGame = true
	end
	
	layer:insertProp ( playButton.img )
	layer:insertProp ( playButton.txt )
	
	mainLayer = layer
	
end

----------------------------------------------------------------
mainMenu.onUnload = function ( self )
	
	for i, layerSet in ipairs ( self.layerTable ) do
		
		for j, layer in ipairs ( layerSet ) do
		
			layer = nil
		end
	end
	
	self.layerTable = nil
	mainLayer = nil
end

----------------------------------------------------------------
mainMenu.onUpdate = function ( self )
	
end

return mainMenu