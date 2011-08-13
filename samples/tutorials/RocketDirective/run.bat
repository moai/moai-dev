::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off
setlocal

set MOAI="..\host-windows\moai"
REM set MOAI=%MOAI_BIN%

pushd lua
%MOAI% "main.lua"
popd
pause