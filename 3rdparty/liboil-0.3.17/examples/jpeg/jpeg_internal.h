
#ifndef _JPEG_INTERNAL_H_
#define _JPEG_INTERNAL_H_

#include "jpeg.h"
#include "jpeg_huffman.h"
#include "jpeg_bits.h"
#include "jpeg_debug.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#define JPEG_MAX_COMPONENTS 256

typedef struct _JpegScan JpegScan;
typedef struct _JpegQuantTable JpegQuantTable;

#define JPEG_ENTROPY_SEGMENT 0x0001
#define JPEG_LENGTH 0x0002

struct _JpegQuantTable {
  int pq;
  int16_t quantizer[64];
};

struct _JpegDecoder {
	int width;
	int height;
	int depth;
	int n_components;
	JpegBits bits;
        int error;
        char *error_message;

        int sof_type;

	int width_blocks;
	int height_blocks;

	int restart_interval;

	unsigned char *data;
	unsigned int data_len;

	struct{
		int id;
		int h_sample;
		int v_sample;
		int quant_table;

		int h_subsample;
		int v_subsample;
		unsigned char *image;
		int rowstride;
	} components[JPEG_MAX_COMPONENTS];

        JpegQuantTable quant_tables[4];
	HuffmanTable dc_huff_table[4];
	HuffmanTable ac_huff_table[4];

	int scan_list_length;
	struct{
		int component_index;
		int dc_table;
		int ac_table;
		int quant_table;
		int x;
		int y;
		int offset;
	}scan_list[10];
	int scan_h_subsample;
	int scan_v_subsample;

	/* scan state */
	int x,y;
	int dc[4];
};

struct _JpegScan {
	int length;
	
	int n_components;
	struct {
		int index;
		int dc_table;
		int ac_table;
	}block_list[10];
};


/* jpeg.c */

int jpeg_decoder_sof_baseline_dct(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_define_quant_table(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_define_huffman_table(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_sos(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_soi(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_eoi(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_application0(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_application_misc(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_comment(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_restart_interval(JpegDecoder *dec, JpegBits *bits);
int jpeg_decoder_restart(JpegDecoder *dec, JpegBits *bits);
void jpeg_decoder_decode_entropy_segment(JpegDecoder *dec);


#endif


