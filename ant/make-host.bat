::================================================================::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::================================================================::

	@echo off
	setlocal
	set CYGWIN=nodosfilewarning
	bash make-host.sh %*
	@pause
	