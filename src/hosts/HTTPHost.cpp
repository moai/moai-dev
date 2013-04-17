//#include <HTTPHost.h>
//
//extern "C" {
//	#include <mongoose.h>
//}
//
////----------------------------------------------------------------//
//int main () {
//
//	printf ( "Hello Web!\n" );
//	return 1;
//}

#define _CRT_SECURE_NO_WARNINGS  // Disable deprecation warning in VS2005

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>

#include "mongoose.h"

#include <windows.h>

#ifndef S_ISDIR
#define S_ISDIR(x) ((x) & _S_IFDIR)
#endif

#define DIRSEP '\\'
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) * 1000)
#define WINCDECL __cdecl

#define MAX_OPTIONS 40
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

static int exit_flag;
static char server_name[40];        // Set by init_server_name()
static char config_file[PATH_MAX];  // Set by process_command_line_arguments()

#if !defined(CONFIG_FILE)
#define CONFIG_FILE "mongoose.conf"
#endif /* !CONFIG_FILE */

//----------------------------------------------------------------//
static void WINCDECL signal_handler(int sig_num) {
  exit_flag = sig_num;
}

//----------------------------------------------------------------//
static void die(const char *fmt, ...) {
  va_list ap;
  char msg[200];

  va_start(ap, fmt);
  vsnprintf(msg, sizeof(msg), fmt, ap);
  va_end(ap);

  exit(EXIT_FAILURE);
}

//----------------------------------------------------------------//
static void init_server_name ( void ) {
  snprintf ( server_name, sizeof ( server_name ), "Mongoose web server v. %s", mg_version ());
}

//----------------------------------------------------------------//
static int log_message ( const struct mg_connection *conn, const char *message ) {
  ( void ) conn;
  printf ( "%s\n", message );
  return 0;
}

//----------------------------------------------------------------//
static int begin_request ( mg_connection* connection ) {

	// http://localhost:8080/foo/bar/baz?a=blarg&b=zowie

	printf ( "//----------------------------------------------------------------//\n" );
	printf ( "begin_request\n" );
	
	//const char *request_method; // "GET", "POST", etc
	//const char *uri;            // URL-decoded URI
	//const char *http_version;   // E.g. "1.0", "1.1"
	//const char *query_string;   // URL part after '?', not including '?', or NULL
	//const char *remote_user;    // Authenticated user, or NULL if no auth used
	//long remote_ip;             // Client's IP address
	//int remote_port;            // Client's port
	//int is_ssl;                 // 1 if SSL-ed, 0 if not
	//void *user_data;            // User data pointer passed to mg_start()

	//int num_headers;            // Number of HTTP headers
	//struct mg_header {
	//	const char *name;         // HTTP header name
	//	const char *value;        // HTTP header value
	//} http_headers[64];         // Maximum 64 headers
	
	mg_request_info* info = mg_get_request_info ( connection );
	
	printf ( "method:  %s\n", info->request_method );
	printf ( "uri:     %s\n", info->uri );
	printf ( "version: %s\n", info->http_version );
	printf ( "query:   %s\n", info->query_string );
	printf ( "user:    %s\n", info->remote_user );
	
	char content[100];

	// Prepare the message we're going to send
	int content_length = snprintf (
		content,
		sizeof ( content ),
		"Hello from mongoose! Remote port: %d",
		info->remote_port
	);

	// Send HTTP reply to the client
	mg_printf (
		connection,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n"		// Always set Content-Length
		"\r\n"
		"%s",
		content_length, content
	);

	// Returning non-zero tells mongoose that our function has replied to
	// the client, and mongoose should not send client any more data.
	return 1;
}

//----------------------------------------------------------------//
static void end_request ( const mg_connection* connection, int reply_status_code ) {

	printf ( "\n" );
}

//----------------------------------------------------------------//
static mg_context* start_mongoose ( int argc, char *argv []) {

	mg_context* ctx;
	struct mg_callbacks callbacks;

	/* Setup signal handler: quit on Ctrl-C */
	signal ( SIGTERM, signal_handler );
	signal ( SIGINT, signal_handler );

	/* Start Mongoose */
	memset ( &callbacks, 0, sizeof ( callbacks ));
	callbacks.begin_request = &begin_request;
	callbacks.end_request = &end_request;
	callbacks.log_message = &log_message;
	//ctx = mg_start ( &callbacks, NULL, ( const char** )options );
	ctx = mg_start ( &callbacks, NULL, NULL );

	if ( ctx == NULL ) {
		die ( "%s", "Failed to start Mongoose." );
	}
	
	return ctx;
}

//----------------------------------------------------------------//
int main ( int argc, char *argv []) {

	init_server_name();

	mg_context* ctx = start_mongoose ( argc, argv );
	
	printf (
		"%s started on port(s) %s with web root [%s]\n",
		server_name,
		mg_get_option ( ctx, "listening_ports" ),
		mg_get_option ( ctx, "document_root" )
	);

	while ( exit_flag == 0 ) {
		sleep ( 1 );
	}
	printf ( "Exiting on signal %d, waiting for all threads to finish...", exit_flag );
	fflush ( stdout );
	mg_stop ( ctx );
	printf ( "%s", " done.\n" );

	return EXIT_SUCCESS;
}
