//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

//----------------------------------------------------------------//
/*
function loadCommandScript ( script ) {
	fl.runScript( fl.configURI + 'Commands/' + script ); 
}

loadCommandScript ( 'moai/assert.jsfl' );
loadCommandScript ( 'moai/dom.jsfl' );
loadCommandScript ( 'moai/element.jsfl' );
loadCommandScript ( 'moai/layer.jsfl' );
loadCommandScript ( 'moai/matrix.jsfl' );
loadCommandScript ( 'moai/util.jsfl' );

loadCommandScript ( 'moai/ErrorStrings.jsfl' );
loadCommandScript ( 'moai/MotionXMLExporter.jsfl' );
*/

//----------------------------------------------------------------//
function exportToLua () {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	
	if ( folderPath &&  folderPath.length ) {
		
		var lua = getDocumentLuaAndExport ( document, folderPath );
		
		var filePath = folderPath + document.name + '.lua';
		if ( !FLfile.write ( filePath, lua )) {
			alert( "Error saving file" ); 
		}
	}
}

//----------------------------------------------------------------//
function getDocumentLuaAndExport ( document, pathURI ) {
	
	var timeline = document.getTimeline ();
	var frameRate = document.frameRate;
	
	var lua = '';
	lua += getTimelineLua ( timeline, frameRate, '' );
	lua += getLibraryLuaAndExport ( document.library, '', pathURI );
	lua += 'return library, timeline\n';
	
	return lua;
}

//----------------------------------------------------------------//
function getEaseCurveLua ( curve, name, indent ) {
	
	var lua = '';
	var indent2 = indent + '\t';
	
	lua += luaTableOpen ( name, indent );
	
	for ( var i = 0; i < curve.length; i++ ) {
		var point = curve [ i ];
		lua += indent2 + '[ ' + ( i + 1 ) + ' ] = {' + '[ \'x\' ] = ' + point.x + ', [ \'y\' ] = ' + point.y + ' },\n';
	}
	
	lua += luaTableClose ( indent );
	return lua;
}

//----------------------------------------------------------------//
function getElementLua ( element, idx, indent ) {

	var lua = '';
	var indent2 = indent + '\t';

	lua += luaHeading ( 'element', indent );
	lua += luaTableOpen ( idx, indent );
	
	var item = element.libraryItem;
	if ( item ) {
		lua += luaKeyVal ( 'name', item.name, indent2 );
	}
	
	lua += luaKeyVal ( 'x', element.x, indent2 );
	lua += luaKeyVal ( 'y', element.y, indent2 );
	lua += luaKeyVal ( 'r', element.rotation, indent2 );
	lua += luaKeyVal ( 'sx', element.scaleX, indent2 );
	lua += luaKeyVal ( 'sy', element.scaleY, indent2 );
	
	lua += luaTableClose ( indent );
	return lua;
}

//----------------------------------------------------------------//
function getFrameLua ( frame, idx, startIdx, indent ) {

	var lua = '';
	var indent2 = indent + '\t';
	var indent3 = indent2 + '\t';
	
	lua += luaHeading ( 'frame', indent );
	lua += luaTableOpen ( idx, indent );
	
	lua += luaKeyVal ( 'start', startIdx, indent2 );
	lua += luaKeyVal ( 'length', frame.duration, indent2 );
	
	// elements
	lua += luaTableOpen ( 'elements', indent2 );
	for ( var i = 0; i < frame.elements.length; ++i ) {
		element = frame.elements [ i ];
		lua += getElementLua ( element, i + 1, indent3 );
	}
	lua += luaTableClose ( indent2 );

	// ease
	if ( frame.hasCustomEase ) {
		if ( frame.useSingleEaseCurve ) {
			lua += getEaseCurveLua ( frame.getCustomEase (), 'easeCurve', indent2 );
		}
		else {
			lua += luaTableOpen ( 'easeCurves', indent2 );
			lua += getEaseCurveLua ( frame.getCustomEase ( 'position' ), 'position', indent3 );
			lua += getEaseCurveLua ( frame.getCustomEase ( 'rotation' ), 'rotation', indent3 );
			lua += getEaseCurveLua ( frame.getCustomEase ( 'scale' ), 'scale', indent3 );
			lua += getEaseCurveLua ( frame.getCustomEase ( 'color' ), 'color', indent3 );
			lua += luaTableClose ( indent2 );
		}
	}
	else {
		lua += luaKeyVal ( 'ease', frame.tweenEasing, indent2 );
	}

	lua += luaTableClose ( indent );
	return lua;
}

//----------------------------------------------------------------//
function getItemLuaAndExport ( item, indent, pathURI ) {
	
	var library = fl.getDocumentDOM().library;
	var document = fl.createDocument ();
	
	library.selectItem ( item );
	document.addItem ({ x:0, y:0 }, item );

	var timeline = document.getTimeline ();
	timeline.currentFrame = 0;
	
	var layer = timeline.layers [ 0 ];
	var frame = layer.frames [ 0 ];
	var element = frame.elements [ 0 ];
	
	document.width = Math.ceil ( element.width );
	document.height = Math.ceil ( element.height );
		
	element.x = document.width / 2;
	element.y = document.height / 2;
	
	var filename = item.name + '.png';
	document.exportPNG ( pathURI + filename, true, true );
	
	var lua = '';
	var indent2 = indent + '\t';
	
	lua += luaHeading ( 'item', indent );
	lua += luaTableOpen ( item.name, indent );
	lua += luaKeyVal ( 'filename', filename, indent2 );
	lua += luaKeyVal ( 'width', document.width, indent2 );
	lua += luaKeyVal ( 'height', document.height, indent2 );
	
	fl.closeDocument ( document, false );
	lua += luaTableClose ( indent );
	return lua;
}

//----------------------------------------------------------------//
function getLayerLua ( layer, idx, indent ) {

	var lua = '';
	var indent2 = indent + '\t';
	var indent3 = indent2 + '\t';

	lua += luaHeading ( 'layer', indent );
	lua += luaTableOpen ( idx, indent );
	
	lua += luaKeyVal ( 'name', layer.name, indent2 );
	lua += luaKeyVal ( 'length', layer.frames.length, indent2 );
	
	var idx = 1;
	lua += luaTableOpen ( 'frames', indent2 );
	for ( var i = 0; i < layer.frames.length; ++i ) {
		frame = layer.frames [ i ];
		
		// skip non-keyframes
		if ( frame.startFrame != i ) continue;

		lua += getFrameLua ( frame, idx++, i, indent3 );
	}
	lua += luaTableClose ( indent2 );
	
	lua += luaTableClose ( indent );
	return lua;
}

//----------------------------------------------------------------//
function getLibraryLuaAndExport ( library, indent, pathURI ) {
	
	var lua = '';
	var indent2 = indent + '\t';
	
	lua += indent + 'local library = {\n';
	
	for ( var i = 0; i < library.items.length; i++ ) {
		var item = library.items [ i ];
		lua += getItemLuaAndExport ( item, indent2, pathURI );
	}
	
	lua += indent + '}\n\n';
	return lua;
}

//----------------------------------------------------------------//
function getTimelineLua ( timeline, frameRate, indent ) {
	
	var lua = '';
	var indent2 = indent + '\t';
	var indent3 = indent2 + '\t';

	lua += indent + 'local timeline = {\n';
	
	lua += luaKeyVal ( 'name', timeline.name, indent2 );
	lua += luaKeyVal ( 'fps', frameRate, indent2 );
	lua += luaKeyVal ( 'length', timeline.frameCount, indent2 );
	
	lua += luaTableOpen ( 'layers', indent2 );
	for ( var i = 0; i < timeline.layers.length; ++i ) {
		layer = timeline.layers [ i ];
		
		if ( layer.layerType != 'normal' ) continue;
		lua += getLayerLua ( layer, i + 1, indent3 );
	}
	lua += luaTableClose ( indent2 );
	
	lua += indent + '}\n\n';
	return lua;
}

//----------------------------------------------------------------//
function luaComment ( comment, indent ) {

	return indent + '-- ' + comment + '\n';
}

//----------------------------------------------------------------//
function luaEscape ( val ) {

	if ( typeof val == 'string' ) {
		return '\'' + val + '\'';
	}
	return val;
}

//----------------------------------------------------------------//
function luaKeyVal ( key, val, indent ) {

	key = luaEscape ( key );
	val = luaEscape ( val );

	return indent + '[ ' + key + ' ] = ' + val + ',\n';
}

//----------------------------------------------------------------//
function luaTableClose ( indent ) {
	
	return indent + '},\n';
}

//----------------------------------------------------------------//
function luaTableOpen ( key, indent ) {
	
	key = luaEscape ( key );
	return indent + '[ ' + key + ' ] = {\n';
}

//----------------------------------------------------------------//
function luaHeading ( comment, indent ) {

	return '\n' + luaComment ( comment, indent );
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
