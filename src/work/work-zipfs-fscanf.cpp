#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zipfs_fscanf ( int argc, char **argv ) {
	
	int result;
	float f1, f2, f3;
	char s [ 256 ];
	u32 i1, i2;
	cc8* scanString = "%f,    %fabc%f,e-,%*s%d,%x";
	
	f1 = -1.0f;
	f2 = -1.0f;
	f3 = -1.0f;
	*s = 0;
	i1 = 0;
	i2 = 0;
	
	// try it straight
	FILE* file = fopen ( "scan.txt", "r" );
	result = fscanf ( file, scanString, &f1, &f2, &f3, &i1, &i2 );
	fclose ( file );
	
	// try it virtual
	zipfs_init ();
	
	f1 = -1.0f;
	f2 = -1.0f;
	f3 = -1.0f;
	*s = 0;
	i1 = 0;
	i2 = 0;
	
	ZIPFSFILE* zfile = zipfs_fopen ( "scan.txt", "r" );
	result = zipfs_fscanf ( zfile, scanString, &f1, &f2, &f3, &i1, &i2 );
	zipfs_fclose ( zfile );
	
	zipfs_cleanup ();
	return 0;
}
