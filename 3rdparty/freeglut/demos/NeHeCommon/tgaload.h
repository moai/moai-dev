
/*  Definitions for bitflags  */
typedef unsigned int tgaFLAG;

#define TGA_DEFAULT            0x0000000000000000   /* In case we don't want any parameters */
#define TGA_FREE               0x0000000000000001   /* Bit flag 0 */
#define TGA_NO_PASS            0x0000000000000010   /* Bit flag 1 */
#define TGA_ALPHA              0x0000000000000100   /* Bit flag 2 */
#define TGA_LUMINANCE          0x0000000000001000   /* Bit flag 3 */
#define TGA_NO_MIPMAPS         0x0000000000010000   /* Bit flag 4 */
#define TGA_LOW_QUALITY        0x0000000000100000   /* Bit flag 5 */
#define TGA_COMPRESS           0x0000000001000000   /* Bit flag 6 */


typedef struct {
   unsigned char id_length;
   unsigned char colour_map_type;
   unsigned char image_type;

   // colourmap spec.  5 bytes
   short int     colour_map_first_entry;  // Ignore
   short int     colour_map_length;       // Usually 256
   unsigned char colour_map_entry_size;   // Usually 24-bit

   // image spec.  10 bytes
   short int     x_origin;  // Ignore
   short int     y_origin;  // Ignore
   short int     width;
   short int     height;
   unsigned char pixel_depth;       // Usually 24 or 32
   unsigned char image_descriptor;  // Ignore

   // Added for 'compeletness' :)
   int   components;
   int   bytes;

   GLenum tgaColourType;

} tgaHeader_t;


typedef struct {
   tgaHeader_t          info;
   unsigned char        *data;      /* Image data */
} image_t;


/* 'Public' functions */
void   tgaLoad        ( char *file_name, image_t *p, tgaFLAG mode );
GLuint tgaLoadAndBind ( char *file_name, tgaFLAG mode );

void tgaSetTexParams  ( unsigned int min_filter, unsigned int mag_filter, unsigned int application );

void tgaFree ( image_t *p );
