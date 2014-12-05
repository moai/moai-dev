--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

module ( "elements", package.seeall )

-- Helper functions for managing the 'buttons'
function makeButton ( texture, w, h )

	local button = {}
	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( texture )
	gfxQuad:setRect ( -w/2, -h/2, w/2, h/2 )
	
	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	prop:setPriority ( 0 )
	button.img =  prop
	
	button.func = nil
	
	button.hit = false
	
	-- updates the button based on Input status
	button.updateClick = function ( self, down, x, y )
		if down then
			--
			if self.img:inside ( x, y ) then
				self.hit = true
				self.img:setScl ( 1.2, 1.2 )
			else
				self.hit = false
				self.img:setScl ( 1.0, 1.0 )
			end
		else
			-- only process uphits if the button 
			if self.img:inside ( x, y ) and self.hit then
				self.img:setScl ( 1.0, 1.0 )
				if self.func then
					self:func ()
				end
			else
				playButtonHit = false
				self.img:setScl ( 1.0, 1.0 )
			end
		end
	end
	
	button.setCallback = function ( self, func )
		self.func = func
	end
	
	return button
end

function makeTextButton ( font, texture, w, h, textY )

	-- make a basic button
	local textButton = makeButton ( texture, w, h )
	
	-- add the text
	local textbox = MOAITextBox.new ()
	textbox:setFont ( font )
	textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox:setYFlip ( true )
	textbox:setRect ( -w/2, -h/2, w/2, h/2 )
	textbox:setLoc ( 0, -textY )
	textbox:setPriority ( 1 )
	textButton.txt = textbox
	
	-- allow the user to set the string
	textButton.setString = function ( self, text )
		self.txt:setString ( text )
	end
	
	
	return textButton
end