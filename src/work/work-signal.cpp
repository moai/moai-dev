#include <uslscore/uslscore.h>
#include <signal.h>

//	#define SIGINT          2       /* interrupt */
//	#define SIGILL          4       /* illegal instruction - invalid function image */
//	#define SIGFPE          8       /* floating point exception */
//	#define SIGSEGV         11      /* segment violation */
//	#define SIGTERM         15      /* Software termination signal from kill */
//	#define SIGBREAK        21      /* Ctrl-Break sequence */
//	#define SIGABRT         22      /* abnormal termination triggered by abort call */

void _onSignal ( int signal ) {
    printf ( "Signal %d\n",signal );
    throw "caught signal";
}

//----------------------------------------------------------------//
int work_signal ( int argc, char **argv ) {

	//typedef void ( *SignalHandlerPointer )( int );

    //SignalHandlerPointer previousHandler;
    //previousHandler = signal ( SIGSEGV , SignalHandler );
    
	signal ( SIGINT, _onSignal );
	signal ( SIGILL, _onSignal );
	signal ( SIGFPE, _onSignal );
	signal ( SIGSEGV, _onSignal );
	signal ( SIGTERM, _onSignal );
	signal ( SIGBREAK, _onSignal );
	signal ( SIGABRT, _onSignal );

	try {
		int* foo = 0;
		( *foo ) = 1;
	}
	catch ( ... ) {
		printf ( "caught exception\n" );
	}
	
	try {
		// TODO: need to replace 'assert' via zlcore to raise ()
		// TODO: zlcore shoud handle SIGABRT and abort ()
		//assert ( false );
		raise ( SIGABRT );
	}
	catch ( ... ) {
		printf ( "caught exception\n" );
	}
	
	//try {
	//	char buffer [ 1 ];
	//	buffer [ 2 ] = 0;
	//}
	//catch ( ... ) {
	//	printf ( "caught exception\n" );
	//}
	
	return 0;
}
