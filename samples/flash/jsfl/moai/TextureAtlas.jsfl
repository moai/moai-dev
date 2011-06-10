//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

//================================================================//
// TextureAtlas
//================================================================//
function TextureAtlas ( padding, forceSquare ) {

	this.padding = padding;
	this.forceSquare = forceSquare;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.addRect = function ( list, name, width, height ) {

	var rect = {};
	
	if ( height > width ) {
		rect.width = height;
		rect.height = width;
		rect.isRotated = true;
	}
	else {
		rect.width = width;
		rect.height = height;
	}
	
	rect.name = name;
	list.push ( rect );
	
	return rect;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.createLuaBrushDeck = function () {

	var lua = new LuaTable ();
	
	for ( var i = 0; i < this.rects.length; i++ ) {
		var rect = this.rects [ i ];
		var uvRect = rect.uvRect;
		
		var brush = lua.pushTable ();
		
		brush.set ( 'u0', uvRect.x0 );
		brush.set ( 'v0', uvRect.y0 );
		brush.set ( 'u1', uvRect.x1 );
		brush.set ( 'v1', uvRect.y1 );
		
		brush.set ( 'w', rect.width );
		brush.set ( 'h', rect.height );
		
		if ( rect.isRotated ) {
			brush.set ( 'r', true );
		}
	}
	
	return lua;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.createTextureAtlas = function () {
	
	var packed = this.getPackListForLibrary ();
	var size = this.getTextureSize ( packed );
	
	var xScale = 1 / size.width;
	var yScale = 1 / size.height;
	
	this.rects = packed;
	this.map = {};

	for ( var i = 0; i < packed.length; ++i ) {
		
		var rect = packed [ i ];
		this.map [ rect.name ] = rect;
		rect.idx = i;
		
		var uvRect = {}; 
		
		uvRect.x0 = rect.x0 * xScale;
		uvRect.y0 = rect.y0 * yScale;
		uvRect.x1 = rect.x1 * xScale;
		uvRect.y1 = rect.y1 * yScale;
		
		uvRect.width = rect.width * xScale;
		uvRect.height = rect.height * yScale;
		
		rect.uvRect = uvRect;
	}
	return this;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.createTextureAtlasDoc = function () {
	
	var packed = this.rects;
	var size = this.getTextureSize ( packed );
	
	var document = fl.getDocumentDOM ();
	var library = document.library;
	var items = library.items;
	
	var atlasDoc = fl.createDocument ();
	
	atlasDoc.width = size.width;
	atlasDoc.height = size.height;
	
	for ( var i = 0; i < packed.length; i++ ) {
		var rect = packed [ i ];
		
		var idx = library.findItemIndex ( rect.name );
		var item = library.items [ idx ];
		
		atlasDoc.addItem ({ x:0, y:0 }, item );
		
		var element = atlasDoc.selection [ 0 ];
		element.x = rect.x0 + ( rect.width * 0.5 );
		element.y = rect.y0 + ( rect.height * 0.5 );
		
		if ( rect.isRotated ) {
			element.setTransformationPoint ({ x:0, y:0 });
			element.rotation = 90;
		}
	}
	
	return atlasDoc;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.getBaseHeight = function ( skyline, i, width ) {

	var span = skyline [ i++ ];
	
	var x1 = span.x0 + width;
	var y0 = span.y;
	
	for ( ; i < skyline.length; ++i ) {
		span = skyline [ i ];
		
		// done checking spans
		if ( span.x0 >= x1 ) {
			break;
		}
		
		// span is above rect base
		if ( span.y > y0 ) {
			y0 = span.y;
		}
	}
	return y0;
}

//----------------------------------------------------------------//
// This generates a list of packed rects based on the current document's library.
// The rects represent the library items as laid out into a pow2 image.
TextureAtlas.prototype.getPackListForLibrary = function () {

	var padding = this.padding;
	var gutter = padding * 2;

	var list = new Array ();
	
	var document = fl.getDocumentDOM ();
	var library = document.library;
	var items = library.items;
	
	var scratchDoc = fl.createDocument ();
	
	for ( var i = 0; i < items.length; i++ ) {
		var item = items [ i ];
		
		if ( item.itemType != 'graphic' ) continue;
		
		scratchDoc.addItem ({ x:0, y:0 }, item );
		
		var timeline = scratchDoc.getTimeline ();
		var layer = timeline.layers [ 0 ];
		var frame = layer.frames [ 0 ];
		var element = frame.elements [ 0 ];
		
		var width = element.width;
		var height = element.height;
		
		var rect = {};
		rect.name = item.name;
		
		if ( height > width ) {
			rect.width = height;
			rect.height = width;
			rect.isRotated = true;
		}
		else {
			rect.width = width;
			rect.height = height;
		}
		
		// store the original width and height
		rect.originalWidth = rect.width;
		rect.originalHeight = rect.height;
		
		// pad the width and height to a pixel boundary
		rect.width = Math.ceil ( rect.width ) + gutter;
		rect.height = Math.ceil ( rect.height ) + gutter;
		
		list.push ( rect );
		
		scratchDoc.selectAll ();
		scratchDoc.deleteSelection ();
	}
	
	fl.closeDocument ( scratchDoc, false );
	
	// pack the rects
	list = this.packRects ( list );
	
	// remove the padding and adjust the final rect
	for ( var i = 0; i < list.length; i++ ) {
		var rect = list [ i ];
		
		// restore the original dimensions 
		rect.width = rect.originalWidth;
		rect.height = rect.originalHeight;
		
		// clear out the temp variables
		rect.originalWidth = null;
		rect.originalHeight = null;
		
		// offset the rect
		rect.x0 += padding;
		rect.y0 += padding;
		
		// set up the max dimensions
		rect.x1 = rect.x0 + rect.width;
		rect.y1 = rect.y0 + rect.height;
	}
	return list;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.getTextureSize = function ( list ) {

	var width = 0;
	var height = 0;
	
	for ( var i = 0; i < list.length; ++i ) {
		var rect = list [ i ];
		
		if ( rect.x1 > width ) {
			width = rect.x1;
		}
		
		if ( rect.y1 > height ) {
			height = rect.y1;
		}
	}
	
	var pow2Width = 1;
	var pow2Height = 1;
	
	while ( pow2Width < width ) {
		pow2Width = pow2Width * 2;
	}
	
	while ( pow2Height < height ) {
		pow2Height = pow2Height * 2;
	}
	
	if ( this.forceSquare ) {
	
		if ( pow2Height < pow2Width ) {
			pow2Height = pow2Width;
		}
	
		if ( pow2Width < pow2Height ) {
			pow2Width = pow2Height;
		}
	}
	
	var size = {};
	size.width = pow2Width;
	size.height = pow2Height;
	
	return size;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.newSpan = function ( x0, x1, y ) {

	var span = {};
	span.x0 = x0;
	span.x1 = x1;
	span.y = y;
	
	return span;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.packRects = function ( list ) {

	list.sort ( this.sortRectByWidth );
	list.reverse ();
	
	var size = 1;
	
	var packed = new Array ();
	while ( packed.length < list.length ) {
	
		size = size * 2;
		fl.trace ( 'trying size: ' + size );
		
		var skyline = new Array ();
		skyline.push ( this.newSpan ( 0, size, 0 ));
		
		for ( var i = 0; i < list.length; ++i ) {
			var rect = list [ i ];
			
			if ( !this.placeRect ( skyline, rect, size )) {
				packed = new Array ();
				break;
			}
			skyline = this.rebuildTextureAtlas ( skyline, rect );
			packed.push ( rect );
		}
	}
	return packed;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.placeRect = function ( skyline, rect, size ) {
	
	var bias = 4;
	
	var bestX = 0;
	var bestY = this.getBaseHeight ( skyline, 0, rect.width );
	
	for ( i = 1; i < skyline.length; ++i ) {
		var span = skyline [ i ];
		
		if (( span.x0 + rect.width ) > size ) {
			break;
		}
		
		var y = this.getBaseHeight ( skyline, i, rect.width );
		
		if ( y < ( bestY - bias )) {
			bestX = span.x0;
			bestY = y;
		}
	}
	
	if (( bestX + rect.width ) > size ) return false;
	if (( bestY + rect.height ) > size ) return false;
		
	rect.x0 = bestX;
	rect.y0 = bestY;
	rect.x1 = rect.x0 + rect.width;
	rect.y1 = rect.y0 + rect.height;
	
	return true;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.rebuildTextureAtlas = function ( list, rect ) {
	
	var i = 0;
	var temp = new Array ();
	
	for ( ; i < list.length; ++i ) {
		var span = list [ i ];
		
		if ( span.x1 <= rect.x0 ) {
			temp.push ( span );
		}
		else if ( span.x1 > rect.x0 ) {
		
			if ( span.x0 < rect.x0 ) {
				temp.push ( this.newSpan ( span.x0, rect.x0, span.y ));
			}
			
			temp.push ( this.newSpan ( rect.x0, rect.x1, rect.y1 ));			
			break;
		}
	}
	
	for ( ; i < list.length; ++i ) {
		var span = list [ i ];
		
		if ( span.x0 >= rect.x1 ) {
			temp.push ( span );
		}
		else if ( span.x1 > rect.x1 ) {
			temp.push ( this.newSpan ( rect.x1, span.x1, span.y ));
		}
	}
	
	var skyline = new Array ();
	var prevSpan;
	
	for ( i = 0; i < temp.length; ++i ) {
		var span = temp [ i ];
		
		if (( prevSpan ) && ( prevSpan.y == span.y )) {
			prevSpan.x1 = span.x1;
		}
		else {
			skyline.push ( span );
			prevSpan = span;
		}
	}
	return skyline;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.sortRectByWidth = function ( a, b ) {

	if ( a.width == b.width ) return 0;
	return a.width < b.width ? -1 : 1;
}
