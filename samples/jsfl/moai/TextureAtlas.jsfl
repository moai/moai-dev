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
/*
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
*/

//----------------------------------------------------------------//
TextureAtlas.prototype.createLuaBrushDeck = function () {

	var lua = new LuaTable ();
	
	for ( var i = 0; i < this.entries.length; i++ ) {
		var entry = this.entries [ i ];
		var uvRect = entry.uvRect;
		
		var brush = lua.pushTable ();
		
		brush.set ( 'u0', uvRect.x0 );
		brush.set ( 'v0', uvRect.y0 );
		brush.set ( 'u1', uvRect.x1 );
		brush.set ( 'v1', uvRect.y1 );
		
		brush.set ( 'w', entry.packRect.width );
		brush.set ( 'h', entry.packRect.height );
		
		if ( entry.uvRect.isRotated ) {
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
	
	this.entries = packed;
	this.map = {};

	for ( var i = 0; i < packed.length; ++i ) {
		
		var entry = packed [ i ];
		this.map [ entry.name ] = entry;
		entry.idx = i;
		
		var uvRect = {}; 
		
		uvRect.x0 = entry.packRect.x0 * xScale;
		uvRect.y0 = entry.packRect.y0 * yScale;
		uvRect.x1 = entry.packRect.x1 * xScale;
		uvRect.y1 = entry.packRect.y1 * yScale;
		
		uvRect.width = entry.packRect.width * xScale;
		uvRect.height = entry.packRect.height * yScale;
		
		uvRect.isRotated = entry.packRect.isRotated;
		
		entry.uvRect = uvRect;
	}
	return this;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.createTextureAtlasDoc = function () {
	
	var entries = this.entries;
	var size = this.getTextureSize ( entries );
	
	var document = fl.getDocumentDOM ();
	var library = document.library;
	var items = library.items;
	
	var atlasDoc = fl.createDocument ();
	
	atlasDoc.width = size.width;
	atlasDoc.height = size.height;
	
	for ( var i = 0; i < entries.length; i++ ) {
		var entry = entries [ i ];
		
		var xCenter = entry.packRect.x0 + ( entry.packRect.width * 0.5 );
		var yCenter = entry.packRect.y0 + ( entry.packRect.height * 0.5 );
		
		var idx = library.findItemIndex ( entry.name );
		var item = library.items [ idx ];
		
		atlasDoc.addItem ({ x:0, y:0 }, item );
		
		var element = atlasDoc.selection [ 0 ];
		
		element.x = xCenter;
		element.y = yCenter;
		
		// account for elements w/ off-center registration points
		element.x = xCenter - (( element.width * 0.5 ) - entry.modelRect.x );
		element.y = yCenter - (( element.height * 0.5 ) - entry.modelRect.y );
		
		if ( entry.packRect.isRotated ) {
			element.setTransformationPoint ({
				x:(( element.width * 0.5 ) - entry.modelRect.x ),
				y:(( element.height * 0.5 ) - entry.modelRect.y )
			});
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
// This generates a list of packed entries based on the current document's library.
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
		
		element.x = 0;
		element.y = 0;
		
		var width = element.width;
		var height = element.height;
		
		var entry = {};
		entry.name = item.name;
		
		entry.modelRect = {}; 
		
		entry.modelRect.width = width;
		entry.modelRect.height = height;
		
		entry.modelRect.x = element.x - element.left;
		entry.modelRect.y = element.y - element.top;
		
		entry.modelRect.x0 = element.left;
		entry.modelRect.y0 = element.top;
		entry.modelRect.x1 = element.left + element.width;
		entry.modelRect.y1 = element.top + element.height;
		
		entry.packRect = {}; 
		
		if ( height > width ) {
		
			entry.packRect.width = height;
			entry.packRect.height = width;
			
			entry.packRect.isRotated = true;
		}
		else {
		
			entry.packRect.width = width;
			entry.packRect.height = height;
		}
		
		// store the original width and height
		entry.packRect.originalWidth = entry.packRect.width;
		entry.packRect.originalHeight = entry.packRect.height;
		
		// pad the width and height to a pixel boundary
		entry.packRect.width = Math.ceil ( entry.packRect.width ) + gutter;
		entry.packRect.height = Math.ceil ( entry.packRect.height ) + gutter;
		
		list.push ( entry );
		
		scratchDoc.selectAll ();
		scratchDoc.deleteSelection ();
	}
	
	fl.closeDocument ( scratchDoc, false );
	
	// pack the entries
	var list1 = this.packRectsNoSort ( list );
	var list2 = this.packRectsSort ( list );
	if ( list1.textureSize > list2.textureSize ) {
		list = list2;
	}
	else {
		list = list1;
	}
	
	// remove the padding and adjust the final rect
	for ( var i = 0; i < list.length; i++ ) {
		var entry = list [ i ];
		
		// restore the original dimensions 
		entry.packRect.width = entry.packRect.originalWidth;
		entry.packRect.height = entry.packRect.originalHeight;
		
		// clear out the temp variables
		entry.packRect.originalWidth = null;
		entry.packRect.originalHeight = null;
		
		// offset the rect
		entry.packRect.x0 += padding;
		entry.packRect.y0 += padding;
		
		// set up the max dimensions
		entry.packRect.x1 = entry.packRect.x0 +entry.packRect.width;
		entry.packRect.y1 = entry.packRect.y0 + entry.packRect.height;
	}
	return list;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.getTextureSize = function ( list ) {

	var width = 0;
	var height = 0;
	
	for ( var i = 0; i < list.length; ++i ) {
		var entry = list [ i ];
		
		if ( entry.packRect.x1 > width ) {
			width = entry.packRect.x1;
		}
		
		if ( entry.packRect.y1 > height ) {
			height = entry.packRect.y1;
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
TextureAtlas.prototype.packRectsSort = function ( list ) {

	list.sort ( this.sortRectByWidth );
	list.reverse ();
	
	var size = 1;
	
	var packed = new Array ();
	packed.textureSize = 0;
	while ( packed.length < list.length ) {
	
		size = size * 2;
		
		var skyline = new Array ();
		skyline.push ( this.newSpan ( 0, size, 0 ));
		
		for ( var i = 0; i < list.length; ++i ) {
			var entry = list [ i ];
			
			if ( !this.placeRect ( skyline, entry, size )) {
				packed = new Array ();
				break;
			}
			skyline = this.rebuildTextureAtlas ( skyline, entry );
			packed.push ( entry );
		}
	}
	
	packed.textureSize = size;
	return packed;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.packRectsNoSort = function ( list ) {

	list.reverse ();
	
	var size = 1;
	
	var packed = new Array ();
	packed.textureSize = 0;
	while ( packed.length < list.length ) {
	
		size = size * 2;
		
		var skyline = new Array ();
		skyline.push ( this.newSpan ( 0, size, 0 ));
		
		for ( var i = 0; i < list.length; ++i ) {
			var entry = eval(uneval(list [ i ]));
			
			if ( !this.placeRect ( skyline, entry, size )) {
				packed = new Array ();
				break;
			}
			skyline = this.rebuildTextureAtlas ( skyline, entry );
			packed.push ( entry );
		}
	}
	
	list.reverse ();
	packed.textureSize = size;
	return packed;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.placeRect = function ( skyline, entry, size ) {
	
	var bias = 4;
	
	var bestX = 0;
	var bestY = this.getBaseHeight ( skyline, 0, entry.packRect.width );
	
	for ( i = 1; i < skyline.length; ++i ) {
		var span = skyline [ i ];
		
		if (( span.x0 + entry.packRect.width ) > size ) {
			break;
		}
		
		var y = this.getBaseHeight ( skyline, i, entry.packRect.width );
		
		if ( y < ( bestY - bias )) {
			bestX = span.x0;
			bestY = y;
		}
	}
	
	if (( bestX + entry.packRect.width ) > size ) {
		return false;
	}
	if (( bestY + entry.packRect.height ) > size ) {
		return false;
	}	
	entry.packRect.x0 = bestX;
	entry.packRect.y0 = bestY;
	entry.packRect.x1 = entry.packRect.x0 + entry.packRect.width;
	entry.packRect.y1 = entry.packRect.y0 + entry.packRect.height;
	
	return true;
}

//----------------------------------------------------------------//
TextureAtlas.prototype.rebuildTextureAtlas = function ( list, entry ) {
	
	var i = 0;
	var temp = new Array ();
	
	for ( ; i < list.length; ++i ) {
		var span = list [ i ];
		
		if ( span.x1 <= entry.packRect.x0 ) {
			temp.push ( span );
		}
		else if ( span.x1 > entry.packRect.x0 ) {
		
			if ( span.x0 < entry.packRect.x0 ) {
				temp.push ( this.newSpan ( span.x0, entry.packRect.x0, span.y ));
			}
			
			temp.push ( this.newSpan ( entry.packRect.x0, entry.packRect.x1, entry.packRect.y1 ));			
			break;
		}
	}
	
	for ( ; i < list.length; ++i ) {
		var span = list [ i ];
		
		if ( span.x0 >= entry.packRect.x1 ) {
			temp.push ( span );
		}
		else if ( span.x1 > entry.packRect.x1 ) {
			temp.push ( this.newSpan ( entry.packRect.x1, span.x1, span.y ));
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

	if ( a.packRect.width == b.packRect.width ) return 0;
	return a.packRect.width < b.packRect.width ? -1 : 1;
}
