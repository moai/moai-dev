----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- All diacritics (thai vowels) should be rendered. 
-- See this thread: http://getmoai.com/forums/post10786.html
-- 
-- Fix: 
-- void MOAITextDesigner::BuildLayout ()
--      delete line:    if ( glyph->mAdvanceX == 0.0f ) continue;
-- 
-- Thai diactric marks have zero AdvanceX so they were discarded

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

font = MOAIFont.new ()
font:load ( 'Thonburi.ttf' )

text = 'Thai characters display test ชาติ ศาสนา พระมหากษัตริย์'

local textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 24 )
textbox:setYFlip( true )
textbox:setRect ( -150, -230, 150, 230 )
layer:insertProp ( textbox )
