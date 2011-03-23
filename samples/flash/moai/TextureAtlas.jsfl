//================================================================//
// TextureAtlas
//================================================================//
function TextureAtlas () {
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
		
		var loc = {};
		loc.x = rect.x0 + ( rect.width * 0.5 );
		loc.y = rect.y0 + ( rect.height * 0.5 );
		
		atlasDoc.addItem ( loc, item );
		
		if ( rect.isRotated ) {
			var element = atlasDoc.selection [ 0 ];
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
TextureAtlas.prototype.getPackListForLibrary = function () {

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
		
		var width = Math.ceil ( element.width );
		var height = Math.ceil ( element.height );
		
		this.addRect ( list, item.name, width + 2, height + 2 );
		
		scratchDoc.selectAll ();
		scratchDoc.deleteSelection ();
	}
	
	fl.closeDocument ( scratchDoc, false );
	
	return this.packRects ( list );
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

	list.sort ( this.sortRectByHeight );
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
	rect.x1 = bestX + rect.width;
	rect.y1 = bestY + rect.height;
	
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
TextureAtlas.prototype.sortRectByHeight = function ( a, b ) {

	if ( a.height == b.height ) return 0;
	return a.height < b.height ? -1 : 1;
}


