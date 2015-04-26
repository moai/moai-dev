
#ifndef _JPEG_DECODER_H_
#define _JPEG_DECODER_H_

#include <stdint.h>

#define JPEG_MARKER_STUFFED		0x00
#define JPEG_MARKER_TEM			0x01
#define JPEG_MARKER_RES			0x02

#define JPEG_MARKER_SOF_0		0xc0
#define JPEG_MARKER_SOF_1		0xc1
#define JPEG_MARKER_SOF_2		0xc2
#define JPEG_MARKER_SOF_3		0xc3
#define JPEG_MARKER_DEFINE_HUFFMAN_TABLES		0xc4
#define JPEG_MARKER_SOF_5		0xc5
#define JPEG_MARKER_SOF_6		0xc6
#define JPEG_MARKER_SOF_7		0xc7
#define JPEG_MARKER_JPG			0xc8
#define JPEG_MARKER_SOF_9		0xc9
#define JPEG_MARKER_SOF_10		0xca
#define JPEG_MARKER_SOF_11		0xcb
#define JPEG_MARKER_DEFINE_ARITHMETIC_CONDITIONING	0xcc
#define JPEG_MARKER_SOF_13		0xcd
#define JPEG_MARKER_SOF_14		0xce
#define JPEG_MARKER_SOF_15		0xcf

#define JPEG_MARKER_RST_0		0xd0
#define JPEG_MARKER_RST_1		0xd1
#define JPEG_MARKER_RST_2		0xd2
#define JPEG_MARKER_RST_3		0xd3
#define JPEG_MARKER_RST_4		0xd4
#define JPEG_MARKER_RST_5		0xd5
#define JPEG_MARKER_RST_6		0xd6
#define JPEG_MARKER_RST_7		0xd7

#define JPEG_MARKER_SOI			0xd8
#define JPEG_MARKER_EOI			0xd9
#define JPEG_MARKER_SOS			0xda
#define JPEG_MARKER_DEFINE_QUANTIZATION_TABLES		0xdb
#define JPEG_MARKER_DNL			0xdc
#define JPEG_MARKER_DEFINE_RESTART_INTERVAL		0xdd
#define JPEG_MARKER_DHP			0xde
#define JPEG_MARKER_EXP			0xdf
#define JPEG_MARKER_APP(x)		(0xe0 + (x))
#define JPEG_MARKER_JPG_(x)		(0xf0 + (x))
#define JPEG_MARKER_COMMENT                             0xfe

#define JPEG_MARKER_JFIF		JPEG_MARKER_APP(0)

#define JPEG_MARKER_IS_START_OF_FRAME(x) ((x)>=0xc0 && (x) <= 0xcf && (x)!=0xc4 && (x)!=0xc8 && (x)!=0xcc)
#define JPEG_MARKER_IS_APP(x) ((x)>=0xe0 && (x) <= 0xef)
#define JPEG_MARKER_IS_RESET(x) ((x)>=0xd0 && (x)<=0xd7)


typedef struct _JpegDecoder JpegDecoder;


JpegDecoder *jpeg_decoder_new(void);
void jpeg_decoder_free(JpegDecoder *dec);
int jpeg_decoder_addbits(JpegDecoder *dec, unsigned char *data, unsigned int len);
int jpeg_decoder_decode (JpegDecoder *dec);
int jpeg_decoder_get_image_size(JpegDecoder *dec, int *width, int *height);
int jpeg_decoder_get_component_size(JpegDecoder *dec, int id,
	int *width, int *height);
int jpeg_decoder_get_component_subsampling(JpegDecoder *dec, int id,
	int *h_subsample, int *v_subsample);
int jpeg_decoder_get_component_ptr(JpegDecoder *dec, int id,
	unsigned char **image, int *rowstride);

unsigned char *jpeg_decoder_get_argb_image (JpegDecoder *dec);
int jpeg_decode_argb (uint8_t *data, int length, uint32_t **image,
    int *width, int *height);

#endif

