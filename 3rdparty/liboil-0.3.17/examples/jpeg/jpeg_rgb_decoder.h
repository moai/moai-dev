
#ifndef _JPEG_RGB_DECODER_H_
#define _JPEG_RGB_DECODER_H_

typedef struct jpeg_rgb_decoder_struct JpegRGBDecoder;


JpegRGBDecoder *jpeg_rgb_decoder_new(void);
void jpeg_rgb_decoder_free(JpegRGBDecoder *dec);
int jpeg_rgb_decoder_addbits(JpegRGBDecoder *dec, unsigned char *data, unsigned int len);
int jpeg_rgb_decoder_parse(JpegRGBDecoder *dec);
int jpeg_rgb_decoder_get_image(JpegRGBDecoder *dec,
	unsigned char **image, int *rowstride, int *width, int *height);



#endif

