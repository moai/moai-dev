
#ifndef __BITS_H__
#define __BITS_H__

typedef struct _JpegBits JpegBits;
struct _JpegBits {
	unsigned char *ptr;
	int idx;
	unsigned char *end;
        int error;
};

int jpeg_bits_error (JpegBits *bits);
int jpeg_bits_get_u8 (JpegBits *bits);
void jpeg_bits_skip (JpegBits *bits, int n);
int jpeg_bits_get_u16_be (JpegBits *bits);
int jpeg_bits_available (JpegBits *bits);

int bits_needbits(JpegBits *b, int n_bytes);
int getbit(JpegBits *b);
unsigned int getbits(JpegBits *b, int n);
unsigned int peekbits(JpegBits *b, int n);
int getsbits(JpegBits *b, int n);
unsigned int peek_u8(JpegBits *b);
unsigned int get_u8(JpegBits *b);
unsigned int get_u16(JpegBits *b);
unsigned int get_be_u16(JpegBits *b);
unsigned int get_u32(JpegBits *b);
void syncbits(JpegBits *b);

#endif

