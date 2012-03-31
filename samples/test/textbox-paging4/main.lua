----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

font1 = MOAIFont.new ()
font1:load ( 'nanum_gothic_coding.ttf' )

continue = false

function setStyleTests ()
	while 1 do
	
		text = '<green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다.'
	
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setRect ( -150, -230, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		local style1 = MOAITextStyle.new ()
		style1:setColor ( 1, 1, 1, 1 )
		style1:setFont ( font1 )
		style1:setSize ( 12 )
		textbox:setStyle ( style1 )
		
		local style2 = MOAITextStyle.new ()
		style2:setColor ( 0, 1, 1, 1 )
		style2:setFont ( font1 )
		style2:setSize ( 20 )
		textbox:setStyle ( 'blue', style2 )
		
		local style3 = MOAITextStyle.new ()
		style3:setColor ( 0, 1, 0, 1 )
		style3:setFont ( font1 )
		style3:setSize ( 28 )
		textbox:setStyle ( 'green', style3 )
		
		continue = false
		repeat coroutine.yield () until continue
		
		textbox:nextPage ()
		
		continue = false
		repeat coroutine.yield () until continue
		
		textbox:nextPage ()
		
		continue = false
		repeat coroutine.yield () until continue
		textbox:setReveal ( 0 )
	end
end

-- tests
function onKeyboardEvent ( key, down )
	if down then
		if key == 32 then -- continue
			continue = true
		else
			return
		end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setStyleTests )
