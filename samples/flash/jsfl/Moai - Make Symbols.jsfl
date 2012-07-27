//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

//----------------------------------------------------------------//
function makeSymbolsForDocument () {
	
	var document = fl.getDocumentDOM ();
	
	var timelines = document.timelines;
	for ( var i = 0; i < timelines.length; ++i ) {
		var timeline = timelines [ i ];
		var layers = timeline.layers;
		
		for ( var frameIdx = 0; frameIdx < timeline.frameCount; ++frameIdx ) {
			for ( var layerIdx = 0; layerIdx < layers.length; ++layerIdx ) {
				
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
					makeSymbolsForFrame ( document, frame );
				}
				layer.locked = locked;
			}
		}
	}
}

//----------------------------------------------------------------//
function makeSymbolsForFrame ( document, frame ) {
	
	document.selectNone ();
	
	var elements = frame.elements;
	for ( var i = 0; i < elements.length; ++i ) {
		var element = elements [ i ];
		
		if ( element.elementType == 'shape' ) {
			
			element.selected = true;
			
			//var selection = document.selection;
			//for ( var j = 0; j < selection.length; ++j ) {
			//	fl.trace ( 'selection!' );
			//}
			document.convertToSymbol ( 'graphic', '', 'center' );
			document.arrange ( 'back' );
			document.library.selectNone ();
			document.selectNone ();
		}
		else if ( element.elementType == 'text' ) {
			
			var depth = element.depth;
			
			element.selected = true;
			document.convertToSymbol ( 'graphic', '', 'center' );
			
			element = document.selection [ 0 ];
			while ( element.depth < depth ) {
				document.arrange ( 'backward' );
			}
		}
	}
}

makeSymbolsForDocument ();
