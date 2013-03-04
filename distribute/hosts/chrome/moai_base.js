
//constructor
Moai.Base = function () {

	this.uid = "";

	if ( localStorage.getItem('uid') ) {
		this.uid = localStorage.getItem('uid');
	}

	window.onbeforeunload = function () { return "Leaving this page will erase any current progress."; };
	window.onload = this.Init;
	
	this.naclMessageHandler = this.naclMessageHandler.bind ( this );
}

//message handlers
Moai.Base.prototype.naclMessageHandler = function ( message_event ) {
	
	if ( message_event.data.search ( 'URL:Open:' ) != -1 ) {
		var startFirstArg = 'URL:Open:'.length;
		var endFirstArg = message_event.data.length;
		var url = message_event.data.substr ( startFirstArg, endFirstArg - startFirstArg );

		window.open ( url );
	}
	else if ( message_event.data.search ( 'URL:OpenPop:' ) != -1 ) {
		var startFirstArg = 'URL:OpenPop:'.length;
		var endFirstArg = message_event.data.length;
		var url = message_event.data.substr ( startFirstArg, endFirstArg - startFirstArg );

		window.open ( url );
	}
	else if ( message_event.data.search ( 'ALERT:' ) != -1 ) {
		var startFirstArg = 'ALERT:'.length;
		var endFirstArg = message_event.data.length;
		var alertMessage = message_event.data.substr ( startFirstArg, endFirstArg - startFirstArg );
		
		if ( message_event.data.search ( 'OPENGL' ) != -1 ) {
			alert ( 'Error: Unable to create OpenGL Context, please make sure you can use WebGL.' );
			window.open ( "http://www.google.com/support/chrome/bin/answer.py?hl=en&answer=1220892" );
		}
		else if ( message_event.data.search ( 'STORE' ) != -1 ) {
			alert ( "Error: Unable to verify purchase. Please try again." );
		}
	}
	else if ( message_event.data.search ( 'UID' ) != -1 ) {
		
		if ( this.uid == "" ) {

			this.uid = this.guidGenerator() + new Date().getTime();
			localStorage.setItem('uid', this.uid);
		}
		
		console.log ( 'posting: UID:' + this.uid );
		MoaiModule.postMessage ( 'UID:' + this.uid );
	}
}

//functions
Moai.Base.prototype.Init = function () {
	
	//default size is 1024x768, resize if height is too small
	var scaleH = window.innerHeight / 800;
	var scaleW = window.innerWidth / 1000;
	
	var scale;
	if ( scaleW < scaleH ) { 
		scale = scaleW;
	}
	else {
		scale = scaleH;
	}
	
	if( scale  > 1.0 ) {
		scale = 1.0;
	}
	
	var moai = document.getElementById ( "moai" );	
	if( moai ) {
		
		moai.width = scale * 960;
		moai.height = scale * 640;
	}
	
	var game = document.getElementById ( "game" );
	if( game ) {
		
		game.style.width = ( scale * 960 ) + 'px';
		game.style.height = ( scale * 640 ) + 'px';
	}
	
	var banner = document.getElementById ( "moai_image" );
	if( banner ) {
	
		banner.width = ( scale * 967 );// + "px";
		banner.height = ( scale * 157 );// + "px";
	}
	
	var toggleButton = document.getElementById('toggle');
	
	if ( toggleButton ) {
		toggleButton.onclick = function() {
			if (screen.height === window.outerHeight) {
				document.webkitCancelFullScreen();
			}
			else {
				document.documentElement.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
			}
		}
	}
}
	
Moai.Base.prototype.guidGenerator = function () {
	var S4 = function() {
	   return ( ((1+Math.random())*0x10000)|0 ).toString ( 16 ).substring ( 1 );
	};
	//return ( (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4()+"-") + (new Date().getTime()) );
	//going to use these as fake UDIDs, no non-alpha numberic
	return ( ( S4()+S4()+S4()+S4()+S4()+S4()+S4()+S4() ) + ( new Date().getTime() ) );
}