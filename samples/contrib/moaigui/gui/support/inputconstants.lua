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
]]

module(..., package.seeall)

LEFT_MOUSE_BUTTON = 0
MIDDLE_MOUSE_BUTTON = 1
RIGHT_MOUSE_BUTTON = 2

local byte = string.byte

K_A = byte("a")
K_B = byte("b")
K_C = byte("c")
K_D = byte("d")
K_E = byte("e")
K_F = byte("f")
K_G = byte("g")
K_H = byte("h")
K_I = byte("i")
K_J = byte("j")
K_K = byte("k")
K_L = byte("l")
K_M = byte("m")
K_N = byte("n")
K_O = byte("o")
K_P = byte("p")
K_Q = byte("q")
K_R = byte("r")
K_S = byte("s")
K_T = byte("t")
K_U = byte("u")
K_V = byte("v")
K_W = byte("w")
K_X = byte("x")
K_Y = byte("y")
K_Z = byte("z")

K_1 = byte("1")
K_2 = byte("2")
K_3 = byte("3")
K_4 = byte("4")
K_5 = byte("5")
K_6 = byte("6")
K_7 = byte("7")
K_8 = byte("8")
K_9 = byte("9")
K_0 = byte("0")

K_ENTER = 13
K_RETURN = K_ENTER
K_ESCAPE = 27

K_SPACE = byte(" ")
K_LEFT_PAREN = byte("(")
K_RIGHT_PAREN = byte(")")
K_ASTERISK = byte("*")
K_AMPERSAND = byte("&")
K_CARET = byte("^")
K_PERCENT = byte("%")
K_DOLLAR = byte("$")
K_HASH = byte("#")
K_POUND = K_HASH
K_AT = byte("@")
K_EXCLAIM = byte("!")

K_LEFT_BRACKET = byte("[")
K_RIGHT_BRACKET = byte("]")
K_LEFT_BRACE = byte("{")
K_RIGHT_BRACE = byte("}")
K_PIPE = byte("|")
K_BACKSLASH = byte("\\")
K_SEMI_COLON = byte(";")
K_COLON = byte(":")
K_QUOTE = byte("\'")
K_DOUBLE_QUOTE = byte("\"")
K_COMMA = byte(",")
K_PERIOD = byte(".")
K_SLASH = byte("/")
K_LESS_THAN = byte("<")
K_RIGHT_THAN = byte(">")
K_QUESTION = byte("?")
K_MINUS = byte("-")
K_UNDERSCORE = byte("_")
K_EQUAL = byte("=")
K_PLUS = byte("+")
