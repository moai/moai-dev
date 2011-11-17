-- The MIT License (MIT)
-- Copyright (c) 2011 Andrew Scott
-- Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
-- (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
-- merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
-- subject to the following conditions:
-- The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
-- DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
-- SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

module(..., package.seeall)

function newCpRect(image, xStart, yStart, xEnd, yEnd, mass, posX, posY)

	texture = MOAIGfxQuad2D.new ()
	texture:setTexture ( image )
	texture:setRect (xStart, yStart, xEnd, yEnd )

	sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	layer:insertProp ( sprite )

	if mass then
		mass = mass
		moment = MOAICpShape.momentForRect ( mass, xStart, yStart, xEnd, yEnd )
		body = MOAICpBody.new ( mass, moment )
		space:insertPrim ( body )

		shape = body:addRect (xStart, yStart, xEnd, yEnd )
		shape:setElasticity ( 0 )
		shape:setFriction ( 0.8 )
		shape:setType ( 1 )
		shape.name = "thing"
		space:insertPrim ( shape )

		sprite:setParent ( body )
		if posX then
			body:setPos(posX,posY)
		end
	end
end	
