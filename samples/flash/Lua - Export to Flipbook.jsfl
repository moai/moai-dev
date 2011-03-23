fl.runScript ( fl.configURI + 'Commands/moai/LuaTable.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/SpriteDeckBuilder.jsfl' );

//----------------------------------------------------------------//
function exportToLua () {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	
	if ( folderPath &&  folderPath.length ) {
		
		var atlas = new TextureAtlas ();
		atlas.createTextureAtlas ();
		
		var lua = getDocumentLua ( document, atlas );
		
		var string = lua.toLuaLocalTable ( 'anim' );
		string += '\n';
		string += 'return anim\n';
		fl.trace ( string );
		
		var filePath = folderPath + document.name + '.lua';
		if ( !FLfile.write ( filePath, string )) {
			alert( "Error saving file" ); 
		}
	}
}

//----------------------------------------------------------------//
function getDocumentLua ( document, atlas ) {
	
	var timeline = document.getTimeline ();
	var frameRate = document.frameRate;
	
	return getTimelineLua ( atlas, timeline, frameRate );
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
	
	// elements
	var elements = lua.setTable ( 'elements' );
	
	for ( var i = 0; i < frame.elements.length; ++i ) {
		element = frame.elements [ i ];
		elements.push ( getElementLua ( element, i + 1 ));
	}

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
		
		// skip empty keyframes
		//if ( !frame.elements.length ) continue;
		
		frames.push ( getFrameLua ( atlas, frame, idx++, i ));
	}
	return lua;
}

//----------------------------------------------------------------//
function getTimelineLua ( atlas, timeline, frameRate ) {
	
	var lua = new LuaTable ();
	
	lua.set ( 'name', timeline.name );
	lua.set ( 'fps', frameRate );
	lua.set ( 'length', timeline.frameCount );
	
	var layers = lua.setTable ( 'layers' );
	
	for ( var i = 0; i < timeline.layers.length; ++i ) {
		layer = timeline.layers [ i ];
		
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

exportToLua ();
