--[[
* The MIT License
* Copyright (C) 2012 Derick Dong (derickdong@hotmail.com).  All rights reserved.
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

	FILE: layermgr.lua
	DESCRIPTION: Maintains a list of layers
	AUTHOR: Derick Dong
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
	CREATED: 4-14-12
]]

local _M = {}

local layers = {}
local visibleLayers = {}

local function Layer()
	local t = {}
	t.name = ""
	t.priority = 0
	t.moaiLayer = nil

	return t
end

local function refreshRenderTable(layerList)
	MOAIRenderMgr.setRenderTable(layerList)
end

local function addLayerToRendering(layer)
	local flag = false
	for i, v in ipairs(visibleLayers) do
		if (layer.priority < v.priority) then
			table.insert(visibleLayers, i, layer)
			flag = true
			break
		end
	end

	if (not flag) then
		visibleLayers[#visibleLayers + 1] = layer
	end

	local moaiLayers = {}
	for i, v in ipairs(visibleLayers) do
		moaiLayers[#moaiLayers + 1] = v.moaiLayer
	end

	refreshRenderTable(moaiLayers)
end

function _M.addLayer(name, priority, moaiLayer)
	local layer = layers[name]
	if (nil == layer) then
		layer = Layer()
		layers[name] = layer
	end

	layer.name = name
	layer.priority = priority
	layer.moaiLayer = moaiLayer

	addLayerToRendering(layer)
end

function _M.createLayer(name, priority)
	local layer = MOAILayer.new()

	_M.addLayer(name, priority, layer)

	return layer
end

function _M.getLayer(name)
	return layers[name]
end

function _M.showLayer(name)
	local layer = layers[name]
	if (nil == layer) then return end

	for i, v in ipairs(visibleLayers) do
		if (v == layer) then return end
	end

	addLayerToRendering(layer)
end

function _M.hideLayer(name)
	local flag = false

	for i, v in ipairs(visibleLayers) do
		if (v.name == name) then
			flag = true
			table.remove(visibleLayers, i)
			break
		end
	end

	if (false == flag) then return end

	refreshRenderTable(visibleLayers)
end

return _M
