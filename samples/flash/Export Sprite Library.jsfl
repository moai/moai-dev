//----------------------------------------------------------------//
function loadScript ( script ) {
	fl.runScript( fl.configURI + script ); 
}

loadScript ( 'Commands/moai/Skyline.jsfl' );
loadScript ( 'Commands/moai/LuaTable.jsfl' );

//----------------------------------------------------------------//
function exportSpriteLibrary () {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	if ( !( folderPath &&  folderPath.length )) return;
	
	var luaFilePath = folderPath + document.name + '.lua';
	var pngFilePath = folderPath + document.name + '.png';
	
	var atlas = createTextureAtlas ();
	
	var spriteLibs = new LuaTable ();
	spriteLibs.set ( 'texture', document.name + '.png' );
	
	var timelines = document.timelines;
	for ( var i = 0; i < timelines.length; ++i ) {
		var timeline = timelines [ i ];
		var layers = timeline.layers;
		
		var spriteLib = new LuaTable ();
		spriteLibs.push ( spriteLib );

		var uvRects = new LuaTable ();
		spriteLib.set ( 'uvRects', uvRects );
		
		var quads = new LuaTable ();
		spriteLib.set ( 'quads', quads );
		
		var prims = new LuaTable ();
		spriteLib.set ( 'prims', prims );
		
		var sprites = new LuaTable ();
		spriteLib.set ( 'sprites', sprites );
		
		var quadLib = {};
		
		for ( var frameIdx = 0; frameIdx < timeline.frameCount; ++frameIdx ) {
			
			var base = prims.numeric.length;
			
			for ( var layerIdx = layers.length - 1; layerIdx >= 0; --layerIdx ) {
				
				var layer = layers [ layerIdx ];
				var frames = layer.frames;
				
				if ( frameIdx >= frames.length ) continue;
				
				var locked = layer.locked;
				layer.locked = false
				var frame = layer.frames [ frameIdx ];
				
				if ( frame.elements.length ) {
					
					timeline.currentFrame = frameIdx;
					timeline.setSelectedLayers ( layerIdx, false );
					timeline.setSelectedFrames ( frameIdx, frameIdx );
					
					getSpritePrims ( document, frame, atlas, prims, uvRects, quads, quadLib );
				}
				layer.locked = locked;
			}
			
			var size = prims.numeric.length - base;
			
			var sprite = new LuaTable ();
			sprite.set ( 'base', base + 1 );
			sprite.set ( 'size', size );
			sprites.push ( sprite );
		}
	}
	
	var lua = spriteLibs.toLuaLocalTable ( 'spriteLibs' );
	lua += '\n';
	lua += 'return spriteLibs\n';
	fl.trace ( lua );
	
	if ( !FLfile.write ( luaFilePath, lua )) {
		alert( "Error saving file" ); 
	}
	
	atlasDoc = createTextureAtlasDoc ( atlas );
	atlasDoc.exportPNG ( pngFilePath, true, true );
	fl.closeDocument ( atlasDoc, false );
}

//----------------------------------------------------------------//
function getSpritePrims ( document, frame, atlas, prims, uvRects, screenQuads, quadLib ) {
	
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
					uvRect.set ( 'x0', rect.x0 );
					uvRect.set ( 'y0', rect.y1 );
					uvRect.set ( 'x1', rect.x1 );
					uvRect.set ( 'y1', rect.y0 );
					
					if ( rect.isRotated ) {
						uvRect.set ( 'r', true );
					}
					
					rect.id = uvRects.push ( uvRect );
				}
				
				var quadKey = getQuadKey ( element );
				var quadID = quadLib [ quadKey ];
				
				if ( quadID == null ) {
					var screenQuad = makeScreenQuad ( element, xOff, yOff );
					quadID = screenQuads.push ( screenQuad );
					quadLib [ quadKey ] = quadID;
				}
				
				var prim = new LuaTable ();
				prim.set ( 'uv', rect.id + 1 );
				prim.set ( 'q', quadID + 1 );
				prims.push ( prim );
			}
		}
	}
}

//----------------------------------------------------------------//
function getQuadKey ( element ) {

	var key = '';
	var m = element.matrix;
	
	key += round ( m.a, 3 );
	key += round ( m.b, 3 );
	key += round ( m.c, 3 );
	key += round ( m.d, 3 );
	key += round ( m.tx, 3 );
	key += round ( m.ty, 3 );
	key += round ( m.width, 3 );
	key += round ( m.height, 3 );
	
	return key;
}

//----------------------------------------------------------------//
function makeScreenQuad ( element, xOff, yOff ) {

	var hWidth = element.width * 0.5;
	var hHeight = element.height * 0.5;
	var m = element.matrix;
	
	var screenQuad = new LuaTable ();
	
	if ( m.b && m.c ) {
		screenQuad.set ( 'v0', newLuaVert ( m, -hWidth, hHeight, xOff, yOff ));
		screenQuad.set ( 'v1', newLuaVert ( m, hWidth, hHeight, xOff, yOff ));
		screenQuad.set ( 'v2', newLuaVert ( m, hWidth, -hHeight, xOff, yOff ));
		screenQuad.set ( 'v3', newLuaVert ( m, -hWidth, -hHeight, xOff, yOff ));
	}
	else {
		screenQuad.set ( 'x0', round (( -hWidth * m.a ) + m.tx + xOff, 3 ));
		screenQuad.set ( 'y0', round (( -hHeight * m.d ) + m.ty + yOff, 3 ));
		screenQuad.set ( 'x1', round (( hWidth * m.a ) + m.tx + xOff, 3 ));
		screenQuad.set ( 'y1', round (( hHeight * m.d ) + m.ty + yOff, 3 ));
	}
	return screenQuad;
}

//----------------------------------------------------------------//
function newLuaVert ( matrix, x, y, xOff, yOff ) {
	
	var v = {};
	v.x = x;
	v.y = y;
	transformVertex ( matrix, v );
	
	var luaVert = new LuaTable ();
	luaVert.set ( 'x', round ( v.x + xOff, 3 ));
	luaVert.set ( 'y', round ( v.y + yOff, 3 ));
	
	return luaVert;
}

//----------------------------------------------------------------//
function round ( n, p ) {

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
function transformVertex ( matrix, vertex ) {
	
	var x = vertex.x;
	var y = vertex.y;
	
	vertex.x = ( x * matrix.a ) + ( y * matrix.b ) + matrix.tx;
	vertex.y = ( x * matrix.c ) + ( y * matrix.d ) + matrix.ty;
}
	
exportSpriteLibrary ();
