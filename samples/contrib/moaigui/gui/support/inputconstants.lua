--[[
* The MIT License
* Copyright (C) 2011 Derick Dong (derickdong@hotmail.com).  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	FILE: inputconstants.lua
	DESCRIPTION: Constants used for input
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

_M.LEFT_MOUSE_BUTTON = 0
_M.MIDDLE_MOUSE_BUTTON = 1
_M.RIGHT_MOUSE_BUTTON = 2

local byte = string.byte

_M.K_A = byte("a")
_M.K_B = byte("b")
_M.K_C = byte("c")
_M.K_D = byte("d")
_M.K_E = byte("e")
_M.K_F = byte("f")
_M.K_G = byte("g")
_M.K_H = byte("h")
_M.K_I = byte("i")
_M.K_J = byte("j")
_M.K_K = byte("k")
_M.K_L = byte("l")
_M.K_M = byte("m")
_M.K_N = byte("n")
_M.K_O = byte("o")
_M.K_P = byte("p")
_M.K_Q = byte("q")
_M.K_R = byte("r")
_M.K_S = byte("s")
_M.K_T = byte("t")
_M.K_U = byte("u")
_M.K_V = byte("v")
_M.K_W = byte("w")
_M.K_X = byte("x")
_M.K_Y = byte("y")
_M.K_Z = byte("z")

_M.K_1 = byte("1")
_M.K_2 = byte("2")
_M.K_3 = byte("3")
_M.K_4 = byte("4")
_M.K_5 = byte("5")
_M.K_6 = byte("6")
_M.K_7 = byte("7")
_M.K_8 = byte("8")
_M.K_9 = byte("9")
_M.K_0 = byte("0")

_M.K_ENTER = 13
_M.K_RETURN = K_ENTER
_M.K_ESCAPE = 27

_M.K_SPACE = byte(" ")
_M.K_LEFT_PAREN = byte("(")
_M.K_RIGHT_PAREN = byte(")")
_M.K_ASTERISK = byte("*")
_M.K_AMPERSAND = byte("&")
_M.K_CARET = byte("^")
_M.K_PERCENT = byte("%")
_M.K_DOLLAR = byte("$")
_M.K_HASH = byte("#")
_M.K_POUND = K_HASH
_M.K_AT = byte("@")
_M.K_EXCLAIM = byte("!")

_M.K_LEFT_BRACKET = byte("[")
_M.K_RIGHT_BRACKET = byte("]")
_M.K_LEFT_BRACE = byte("{")
_M.K_RIGHT_BRACE = byte("}")
_M.K_PIPE = byte("|")
_M.K_BACKSLASH = byte("\\")
_M.K_SEMI_COLON = byte(";")
_M.K_COLON = byte(":")
_M.K_QUOTE = byte("\'")
_M.K_DOUBLE_QUOTE = byte("\"")
_M.K_COMMA = byte(",")
_M.K_PERIOD = byte(".")
_M.K_SLASH = byte("/")
_M.K_LESS_THAN = byte("<")
_M.K_RIGHT_THAN = byte(">")
_M.K_QUESTION = byte("?")
_M.K_MINUS = byte("-")
_M.K_UNDERSCORE = byte("_")
_M.K_EQUAL = byte("=")
_M.K_PLUS = byte("+")

return _M
