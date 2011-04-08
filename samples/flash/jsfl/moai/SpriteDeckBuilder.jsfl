//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//


//================================================================//
// SpriteDeckBuilder
//================================================================//
function SpriteDeckBuilder () {
	
	this.spriteDeck = new LuaTable ();

	this.uvRects = new LuaTable ();
	this.spriteDeck.set ( 'uvRects', this.uvRects );
	
	this.quads = new LuaTable ();
	this.spriteDeck.set ( 'quads', this.quads );
	
	this.prims = new LuaTable ();
	this.spriteDeck.set ( 'prims', this.prims );
	
	this.sprites = new LuaTable ();
	this.spriteDeck.set ( 'sprites', this.sprites );
	
	this.quadLib = {};
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.addSpriteForFrameIdx = function ( document, timeline, atlas, frameIdx ) {
	
	var base = this.prims.numeric.length;
	
	var layers = timeline.layers;
	for ( var layerIdx = layers.length - 1; layerIdx >= 0; --layerIdx ) {
		
		var layer = layers [ layerIdx ];
		var frames = layer.frames;
		
		if ( frameIdx >= frames.length ) continue;
		
		var frame = layer.frames [ frameIdx ];
		if ( !frame ) continue;
		
		var locked = layer.locked;
		layer.locked = false
		
		if ( frame.elements.length ) {
			
			timeline.currentFrame = frameIdx;
			timeline.setSelectedLayers ( layerIdx, false );
			timeline.setSelectedFrames ( frameIdx, frameIdx );
			
			this.addSpritePrimsForFrame ( document, atlas, frame );
		}
		layer.locked = locked;
	}
	
	var size = this.prims.numeric.length - base;
	
	var sprite = new LuaTable ();
	sprite.set ( 'base', base + 1 );
	sprite.set ( 'size', size );
	
	return this.sprites.push ( sprite );
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.addSpritePrimsForFrame = function ( document, atlas, frame ) {
	
	var xOff = -document.width * 0.5;
	var yOff = -document.height * 0.5;
	
	var elements = frame.elements;
	for ( var i = 0; i < elements.length; ++i ) {
		var element = elements [ i ];
		
		if ( element.elementType == 'instance' ) {
			
			item = element.libraryItem;
			var rect = atlas.map [ item.name ];
			
			if ( rect ) {
				
				if ( rect.id == null ) {
					
					var uvRect = new LuaTable ();
					uvRect.set ( 'x0', rect.uvRect.x0 );
					uvRect.set ( 'y0', rect.uvRect.y0 );
					uvRect.set ( 'x1', rect.uvRect.x1 );
					uvRect.set ( 'y1', rect.uvRect.y1 );
					
					if ( rect.isRotated ) {
						uvRect.set ( 'r', true );
					}
					rect.id = this.uvRects.push ( uvRect );
				}
				
				var quadKey = this.getQuadKey ( element );
				var quadID = this.quadLib [ quadKey ];
				
				if ( quadID == null ) {
					
					var screenQuad
					
					if ( rect.isRotated ) {
						screenQuad = this.makeScreenQuad ( element.matrix, rect.height, rect.width, xOff, yOff );
					}
					else {
						screenQuad = this.makeScreenQuad ( element.matrix, rect.width, rect.height, xOff, yOff );
					}
					quadID = this.quads.push ( screenQuad );
					this.quadLib [ quadKey ] = quadID;
				}
				
				var prim = new LuaTable ();
				prim.set ( 'uv', rect.id + 1 );
				prim.set ( 'q', quadID + 1 );
				this.prims.push ( prim );
			}
		}
	}
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.getQuadKey = function ( element ) {

	var key = '';
	var m = element.matrix;
	
	key += this.round ( m.a, 3 );
	key += this.round ( m.b, 3 );
	key += this.round ( m.c, 3 );
	key += this.round ( m.d, 3 );
	key += this.round ( m.tx, 3 );
	key += this.round ( m.ty, 3 );
	key += this.round ( m.width, 3 );
	key += this.round ( m.height, 3 );
	
	return key;
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.makeScreenQuad = function ( m, width, height, xOff, yOff ) {
	
	var hWidth	= width * 0.5;
	var hHeight	= height * 0.5;
	
	var screenQuad = new LuaTable ();
	
	if ( m.b && m.c ) {		
		screenQuad.set ( 'v0', this.newLuaVert ( m, -hWidth, hHeight, xOff, yOff ));
		screenQuad.set ( 'v1', this.newLuaVert ( m, hWidth, hHeight, xOff, yOff ));
		screenQuad.set ( 'v2', this.newLuaVert ( m, hWidth, -hHeight, xOff, yOff ));
		screenQuad.set ( 'v3', this.newLuaVert ( m, -hWidth, -hHeight, xOff, yOff ));
	}
	else {
		screenQuad.set ( 'x0', this.round (( -hWidth * m.a ) + m.tx + xOff, 3 ));
		screenQuad.set ( 'y0', this.round (( -hHeight * m.d ) + m.ty + yOff, 3 ));
		screenQuad.set ( 'x1', this.round (( hWidth * m.a ) + m.tx + xOff, 3 ));
		screenQuad.set ( 'y1', this.round (( hHeight * m.d ) + m.ty + yOff, 3 ));
	}
	return screenQuad;
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.newLuaVert = function ( matrix, x, y, xOff, yOff ) {
	
	var v	= {};
	v.x		= x;
	v.y		= y;
	this.transformVertex ( matrix, v );
	
	var luaVert = new LuaTable ();
	luaVert.set ( 'x', this.round ( v.x + xOff, 3 ));
	luaVert.set ( 'y', this.round ( v.y + yOff, 3 ));
	
	return luaVert;
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.round = function ( n, p ) {

	var s = 1;	
	if ( p > 0 ) {
		s = 10;
		for ( var i = 1; i < p; ++i ) {
			s *= 10;
		}
	}
	return Math.floor ( n * s ) / s;
}

//----------------------------------------------------------------//
SpriteDeckBuilder.prototype.transformVertex = function ( matrix, vertex ) {
	
	var x = vertex.x;
	var y = vertex.y;
	
	vertex.x = ( x * matrix.a ) + ( y * matrix.c ) + matrix.tx;
	vertex.y = ( x * matrix.b ) + ( y * matrix.d ) + matrix.ty;
}

