//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

fl.runScript ( fl.configURI + 'Commands/moai/Settings.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/LuaTable.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/SpriteDeckBuilder.jsfl' );

//----------------------------------------------------------------//
function exportToLua ( padding, forceSquare ) {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	
	if ( folderPath &&  folderPath.length ) {
		
		var atlas = new TextureAtlas ( padding, forceSquare );
		atlas.createTextureAtlas ();
		
		var lua = new LuaTable ();
		
		lua.set ( 'texture', document.name + '.png' );
		lua.set ( 'fps', document.frameRate );
		lua.set ( 'width', document.width );
		lua.set ( 'height', document.height );
		
		lua.set ( 'brushDeck', atlas.createLuaBrushDeck ());
		
		var timeline = document.getTimeline ();
		lua.set ( 'timeline', getTimelineLua ( atlas, timeline ));
		
		var string = lua.toLuaLocalTable ( 'anim' );
		string += '\n';
		string += 'return anim\n';
		fl.trace ( string );
		
		var luaFilePath = folderPath + document.name + '.lua';
		if ( !FLfile.write ( luaFilePath, string )) {
			alert( "Error saving file" ); 
		}
		
		var pngFilePath = folderPath + document.name + '.png';
		atlasDoc = atlas.createTextureAtlasDoc ();
		atlasDoc.exportPNG ( pngFilePath, true, true );
		fl.closeDocument ( atlasDoc, false );
	}
}

//----------------------------------------------------------------//
function getEaseCurveLua ( curve, name ) {
	
	var lua = new LuaTable ();
	
	for ( var i = 0; i < curve.length; i++ ) {
		var point = curve [ i ];
		
		var luaPoint = lua.pushTable ();
		
		luaPoint.set ( 'x', point.x );
		luaPoint.set ( 'y', point.y );
	}
	return lua;
}

//----------------------------------------------------------------//
function getElementLua ( element, idx ) {

	var lua = new LuaTable ();
	
	var item = element.libraryItem;
	if ( item ) {
		lua.set ( 'name', item.name );
	}
	
	lua.set ( 'x', element.x );
	lua.set ( 'y', element.y );
	lua.set ( 'r', element.rotation );
	lua.set ( 'sx', element.scaleX );
	lua.set ( 'sy', element.scaleY );
	
	return lua;
}

//----------------------------------------------------------------//
function getFrameLua ( atlas, frame, idx, startIdx ) {
	
	var lua = new LuaTable ();
	
	lua.set ( 'start', startIdx );
	lua.set ( 'length', frame.duration );
	
	// we only care about the first element
	var element;
	if ( frame.elements.length ) {
		element = frame.elements [ 0 ];
	}
	
	if ( element && element.libraryItem ) {
	
		var entry = atlas.map [ element.libraryItem.name ];
		if ( !entry ) return;
		
		lua.set ( 'id', entry.idx + 1 );
		
		var transform = lua.setTable ( 'transform' );
	
		transform.set ( 'x', element.x );
		transform.set ( 'y', element.y );
		transform.set ( 'r', element.rotation );
		transform.set ( 'sx', element.scaleX );
		transform.set ( 'sy', element.scaleY );
		
		// ease
		if ( frame.hasCustomEase ) {
			if ( frame.useSingleEaseCurve ) {
				var easeCurve = getEaseCurveLua ( frame.getCustomEase (), 'easeCurve' );
				lua.set ( 'easeCurve', easeCurve );
			}
			else {
				var easeCurves = lua.setTable ( 'easeCurves' );
				
				easeCurves.push ( getEaseCurveLua ( frame.getCustomEase ( 'position' ), 'position' ));
				easeCurves.push ( getEaseCurveLua ( frame.getCustomEase ( 'rotation' ), 'rotation' ));
				easeCurves.push ( getEaseCurveLua ( frame.getCustomEase ( 'scale' ), 'scale' ));
				easeCurves.push ( getEaseCurveLua ( frame.getCustomEase ( 'color' ), 'color' ));
			}
		}
		else {
			lua.set ( 'ease', frame.tweenEasing );
		}
	}
	else {
		lua.set ( 'id', -1 );
	}

	return lua;
}

//----------------------------------------------------------------//
function getLayerLua ( atlas, layer, idx ) {

	var lua = new LuaTable ();
	
	lua.set ( 'name', layer.name );
	lua.set ( 'length', layer.frames.length );
	
	var frames = lua.setTable ( 'frames' );
	
	var idx = 1;
	for ( var i = 0; i < layer.frames.length; ++i ) {
		frame = layer.frames [ i ];
		
		// skip non-keyframes
		if ( frame.startFrame != i ) continue;
		
		var frameLua = getFrameLua ( atlas, frame, idx++, i );
		if ( frameLua ) {
			frames.push ( getFrameLua ( atlas, frame, idx++, i ));
		}
	}
	return lua;
}

//----------------------------------------------------------------//
function getTimelineLua ( atlas, timeline ) {
	
	var lua = new LuaTable ();
	
	lua.set ( 'name', timeline.name );
	lua.set ( 'length', timeline.frameCount );
	
	var layers = lua.setTable ( 'layers' );
	
	for ( var i = 0; i < timeline.layers.length; ++i ) {
		layer = timeline.layers [ timeline.layers.length - i - 1 ];
		
		if ( layer.layerType != 'normal' ) continue;
		layers.push ( getLayerLua ( atlas, layer, i + 1 ));
	}
	return lua;
}

//----------------------------------------------------------------//
function reflect ( objName, obj ) {
	
	fl.trace ( objName );
	
	for ( var member in obj ){
		fl.trace ( '\t' + member + ': ' + obj [ member ]);
	}
	fl.trace ( '\n' );
}

exportToLua ( MOAI_TEXTURE_ATLAS_PADDING, MOAI_TEXTURE_ATLAS_FORCE_SQUARE );
