#include <moaicore/moaicore.h>

//----------------------------------------------------------------//
void _write_png ( MOAIImage& image, cc8* filename ) {

	USFileStream out;
	out.OpenWrite ( filename );
	image.WritePNG ( out );
}

//----------------------------------------------------------------//
int work_font_mipmap ( int argc, char **argv ) {
	
	AKUCreateContext ();

	MOAIImage image;
	
	image.Load ( "../../src/work/font-mipmap/screebie32.png" );
	
	_write_png ( image, "1.png" );
	
	image.MipReduce ();
	_write_png ( image, "2.png" );
	
	image.MipReduce ();
	_write_png ( image, "3.png" );

	return 0;
}
