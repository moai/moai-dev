#ifndef _moai_lua_H
#define _moai_lua_H

char moai_lua_code[]="--==============================================================\n\
-- Copyright (c) 2010-2011 Zipline Games, Inc. \n\
-- All Rights Reserved. \n\
-- http://getmoai.com\n\
--==============================================================\n\
\n\
--============================================================--\n\
-- initTransform2DInterface\n\
--============================================================--\n\
local function initTransform2DInterface ( interface, superInterface )\n\
\n\
----------------------------------------------------------------\n\
function interface.addLoc ( self, xDelta, yDelta )\n\
superInterface.addLoc ( self, xDelta, yDelta, 0 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.addPiv ( self, xDelta, yDelta )\n\
superInterface.addPiv ( self, xDelta, yDelta, 0 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.addRot ( self, rDelta )\n\
superInterface.addRot ( self, 0, 0, rDelta )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.addScl ( self, xSclDelta, ySclDelta )\n\
superInterface.addScl ( self, xSclDelta, ySclDelta, 0 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.getLoc ( self )\n\
local x, y = superInterface.getLoc ( self )\n\
return x, y\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.getPiv ( self )\n\
local x, y = superInterface.getPiv ( self )\n\
return x, y\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.getRot ( self )\n\
local x, y, z = superInterface.getRot ( self )\n\
return z\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.getScl ( self )\n\
local x, y = superInterface.getScl ( self )\n\
return x, y\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.modelToWorld ( self, x, y )\n\
local x, y = superInterface.modelToWorld ( self, x, y, 0 )\n\
return x, y\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.move ( self, xDelta, yDelta, rDelta, xSclDelta, ySclDelta, length, mode )\n\
return superInterface.move ( self, xDelta, yDelta, 0, 0, 0, rDelta, xSclDelta, ySclDelta, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.moveLoc ( self, xDelta, yDelta, length, mode )\n\
return superInterface.moveLoc ( self, xDelta, yDelta, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.movePiv ( self, xDelta, yDelta, length, mode )\n\
return superInterface.movePiv ( self, xDelta, yDelta, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.moveRot ( self, rDelta, length, mode )\n\
return superInterface.moveRot ( self, 0, 0, rDelta, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.moveScl ( self, xSclDelta, ySclDelta, length, mode )\n\
return superInterface.moveScl ( self, xSclDelta, ySclDelta, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.seek ( self, xGoal, yGoal, rGoal, xSclGoal, ySclGoal, length, mode )\n\
return superInterface.seek ( self, xGoal, yGoal, 0, 0, 0, rGoal, xSclGoal, ySclGoal, 1, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.seekLoc ( self, xGoal, yGoal, length, mode )\n\
return superInterface.seekLoc ( self, xGoal, yGoal, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.seekPiv ( self, xGoal, yGoal, length, mode )\n\
return superInterface.seekPiv ( self, xGoal, yGoal, 0, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.seekRot ( self, rGoal, length, mode )\n\
return superInterface.seekRot ( self, 0, 0, rGoal, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.seekScl ( self, xSclGoal, ySclGoal, length, mode )\n\
return superInterface.seekScl ( self, xSclGoal, ySclGoal, 1, length, mode )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.setLoc ( self, x, y )\n\
superInterface.setLoc ( self, x, y, 0 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.setPiv ( self, x, y )\n\
superInterface.setPiv ( self, x, y, 0 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.setRot ( self, rot )\n\
superInterface.setRot ( self, 0, 0, rot )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.setScl ( self, x, y )\n\
superInterface.setScl ( self, x, y, 1 )\n\
end\n\
\n\
----------------------------------------------------------------\n\
function interface.worldToModel ( self, x, y )\n\
local x, y = superInterface.worldToModel ( self, x, y, 0 )\n\
return x, y\n\
end\n\
\n\
end\n\
\n\
--============================================================--\n\
-- moai2D.lua - version 1.0 Beta\n\
--============================================================--\n\
\n\
--============================================================--\n\
-- MOAIThread\n\
--============================================================--\n\
MOAIThread = MOAICoroutine\n\
\n\
--============================================================--\n\
-- MOAILayerBridge2D\n\
--============================================================--\n\
MOAILayerBridge2D = MOAILayerBridge\n\
\n\
--============================================================--\n\
-- MOAICamera\n\
--============================================================--\n\
MOAICamera.extend (\n\
\n\
'MOAICamera2D',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- extend the interface\n\
initTransform2DInterface ( interface, superInterface )\n\
\n\
interface.getFieldOfView = nil\n\
interface.getFocalLength = nil\n\
interface.setFieldOfView = nil\n\
interface.setOrtho = nil\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setFarPlane ( self, far )\n\
superInterface.setFarPlane ( self, far or -1 )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setNearPlane ( self, near )\n\
superInterface.setNearPlane ( self, near or 1 )\n\
end\n\
\n\
-- extend the class\n\
local new = class.new\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.new ()\n\
local self = new ()\n\
superInterface.setOrtho ( self, true )\n\
superInterface.setNearPlane ( self, 1 )\n\
superInterface.setFarPlane ( self, -1 )\n\
return self\n\
end\n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAIFreeTypeFontReader\n\
--============================================================--\n\
if MOAIFreeTypeFontReader then\n\
MOAIFont.extend (\n\
\n\
'MOAIFont',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
-- extend the interface\n\
function interface.loadFromTTF ( self, filename, charcodes, points, dpi )\n\
self:load ( filename )\n\
self:preloadGlyphs ( charcodes, points, dpi )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
-- extend the class\n\
local new = class.new\n\
\n\
function class.new ()\n\
local self = new ()\n\
superInterface.setCache ( self, MOAIGlyphCache.new ())\n\
superInterface.setReader ( self, MOAIFreeTypeFontReader.new ())\n\
return self\n\
end\n\
end\n\
)\n\
end\n\
\n\
--============================================================--\n\
-- MOAIHttpTask\n\
--============================================================--\n\
MOAIHttpTask = MOAIHttpTaskCurl or MOAIHttpNaCl\n\
\n\
--============================================================--\n\
-- MOAILayer\n\
--============================================================--\n\
MOAILayer.extend (\n\
\n\
'MOAILayer2D',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- extend the interface\n\
initTransform2DInterface ( interface, superInterface )\n\
\n\
interface.setPartitionCull2D = nil\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setSortScale ( self, x, y, priority )\n\
superInterface.setSortScale ( self, x, y, 0, priority )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.wndToWorld ( self, x, y )\n\
local x, y = superInterface.wndToWorld ( self, x, y, 0 )\n\
return x, y\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.worldToWnd ( self, x, y )\n\
local x, y = superInterface.worldToWnd ( self, x, y, 0 )\n\
return x, y\n\
end\n\
\n\
-- extend the class\n\
class.SORT_Z_ASCENDING = nil\n\
class.SORT_Z_DESCENDING = nil\n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAIProp\n\
--============================================================--\n\
MOAIProp.extend (\n\
\n\
'MOAIProp2D',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- extend the interface\n\
initTransform2DInterface ( interface, superInterface )\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setFrame ( self, xMin, yMin, xMax, yMax )\n\
superInterface.setBounds ( self, xMin, yMin, 0, xMax, yMax, 0 )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.getRect ( self )			\n\
print ( self )\n\
local xMin, yMin, zMin, xMax, yMax, zMax = superInterface.getBounds ( self )\n\
return xMin, yMin, xMax, yMax\n\
end\n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAIRenderMgr\n\
--============================================================--\n\
MOAIRenderMgr.extend (\n\
\n\
'MOAIRenderMgr',\n\
\n\
----------------------------------------------------------------\n\
function ( class, superClass )\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
-- extend the class\n\
function class.affirmRenderTable ()\n\
\n\
local renderTable = class.getRenderTable ()\n\
if not renderTable then\n\
renderTable = {}\n\
class.setRenderTable ( renderTable )\n\
end\n\
\n\
return renderTable\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.clearRenderStack ()\n\
class.setRenderTable ( nil )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.grabNextFrame ()\n\
local frameBuffer = MOAIGfxDevice.getFrameBuffer ()\n\
frameBuffer:grabNextFrame ()\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.getRenderTable ()\n\
local frameBuffer = MOAIGfxDevice.getFrameBuffer ()\n\
return frameBuffer:getRenderTable ()\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.popRenderPass ()\n\
\n\
local renderTable = class.affirmRenderTable ()\n\
table.remove ( renderTable )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.pushRenderPass ( pass )\n\
\n\
local renderTable = class.affirmRenderTable ()\n\
table.insert ( renderTable, pass )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.removeRenderPass ( pass )\n\
\n\
local renderTable = class.affirmRenderTable ()\n\
for i, cursor in ipairs ( renderTable ) do\n\
if cursor == pass then\n\
table.remove ( renderTable, i )\n\
break\n\
end\n\
end\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.setRenderTable ( renderTable )\n\
\n\
local frameBuffer = MOAIGfxDevice.getFrameBuffer ()\n\
frameBuffer:setRenderTable ( renderTable )\n\
end\n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAISim\n\
--============================================================--\n\
MOAISim.extend (\n\
\n\
'MOAISim',\n\
\n\
----------------------------------------------------------------\n\
function ( class, superClass )\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
-- extend the class\n\
function class.clearRenderStack ()\n\
MOAIRenderMgr.clearRenderStack ()\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.popRenderPass ()\n\
MOAIRenderMgr.popRenderPass ()\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.pushRenderPass ( pass )\n\
MOAIRenderMgr.pushRenderPass ( pass )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function class.removeRenderPass ( pass )\n\
MOAIRenderMgr.removeRenderPass ( pass )\n\
end \n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAITextBox\n\
--============================================================--\n\
MOAITextBox.extend (\n\
\n\
'MOAITextBox',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
-- extend the interface\n\
function interface.affirmStyle ( self )\n\
local style = self:getStyle ()\n\
if not style then\n\
style = MOAITextStyle.new ()\n\
self:setStyle ( style )\n\
end\n\
return style\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setFont ( self, font )\n\
local style = self:affirmStyle ()\n\
style:setFont ( font )\n\
end\n\
\n\
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -\n\
function interface.setTextSize ( self, points, dpi )\n\
local style = self:affirmStyle ()\n\
style:setSize ( points, dpi )\n\
end\n\
end\n\
)\n\
\n\
--============================================================--\n\
-- MOAITransform\n\
--============================================================--\n\
MOAITransform.extend (\n\
\n\
'MOAITransform2D',\n\
\n\
----------------------------------------------------------------\n\
function ( interface, class, superInterface, superClass )\n\
\n\
-- extend the interface\n\
initTransform2DInterface ( interface, superInterface )\n\
end\n\
)\n\
\n\
MOAIApp = MOAIAppAndroid or MOAIAppIOS\n\
MOAIDialog = MOAIDialogAndroid or MOAIDialogIOS\n\
MOAIMoviePlayer = MOAIMoviePlayerAndroid or MOAIMoviePlayerIOS\n\
\n\
-- Optional 3rd party extensions\n\
MOAIAdColony = MOAIAdColonyAndroid or MOAIAdColonyIOS\n\
MOAIBilling = MOAIBillingAndroid or MOAIBillingIOS\n\
MOAIChartBoost = MOAIChartBoostAndroid or MOAIChartBoostIOS\n\
MOAICrittercism = MOAICrittercismAndroid or MOAICrittercismIOS\n\
MOAIFacebook = MOAIFacebookAndroid or MOAIFacebookIOS\n\
MOAINotifications = MOAINotificationsAndroid or MOAINotificationsIOS\n\
MOAITapjoy = MOAITapjoyAndroid or MOAITapjoyIOS\n\
";


#endif
