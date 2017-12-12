--==============================================================
-- staging
--==============================================================

----------------------------------------------------------------
stage = function ()
	print ( 'staging' )
end

--==============================================================
-- testing
--==============================================================

----------------------------------------------------------------
test = function ()

	----------------------------------------------------------------
	MOAITestMgr.test ( 'Test A', function ()

		print ( 'test a' )
		MOAITestMgr.comment ( 'comment 1' )
		MOAITestMgr.comment ( 'comment 2' )
		MOAITestMgr.comment ( 'comment 3' )

	end )

	----------------------------------------------------------------
	MOAITestMgr.test ( 'Test B', function ()

		print ( 'test b' )
		MOAITestMgr.comment ( 'comment 1' )
		MOAITestMgr.comment ( 'comment 2' )
		MOAITestMgr.assert ( false, 'ERROR!' )

	end )

	----------------------------------------------------------------
	MOAITestMgr.test ( 'Test C', function ()

		print ( 'test c' )
		MOAITestMgr.comment ( 'foo' )
		local foo
		foo.bar = 'baz' -- error and stack trace

	end )

end
