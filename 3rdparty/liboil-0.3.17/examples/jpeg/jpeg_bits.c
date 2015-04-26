
#include <stdio.h>

#include "jpeg_bits.h"

/* FIXME */
#define TRUE 1
#define FALSE 0

int jpeg_bits_error (JpegBits *bits)
{
  return bits->error;
}

int jpeg_bits_get_u8 (JpegBits *bits)
{
  if (bits->ptr < bits->end) {
    return *bits->ptr++;
  } 
  bits->error = TRUE;
  return 0;
}

void jpeg_bits_skip (JpegBits *bits, int n)
{
  bits->ptr += n;
  if (bits->ptr > bits->end) {
    bits->error = TRUE;
    bits->ptr = bits->end;
  }
}

int jpeg_bits_get_u16_be (JpegBits *bits)
{
  int x;

  x = jpeg_bits_get_u8 (bits) << 8;
  x |= jpeg_bits_get_u8 (bits);

  return x;
}

int jpeg_bits_available (JpegBits *bits)
{
  return bits->end - bits->ptr;
}

int bits_needbits(JpegBits *b, int n_bytes)
{
  if(b->ptr==NULL)return 1;
  if(b->ptr + n_bytes > b->end)return 1;

  return 0;
}

int getbit(JpegBits *b)
{
	int r;

	r = ((*b->ptr)>>(7-b->idx))&1;

	b->idx++;
	if(b->idx>=8){
		b->ptr++;
		b->idx = 0;
	}

	return r;
}

unsigned int getbits(JpegBits *b, int n)
{
	unsigned long r = 0;
	int i;

	for(i=0;i<n;i++){
		r <<=1;
		r |= getbit(b);
	}

	return r;
}

unsigned int peekbits(JpegBits *b, int n)
{
	JpegBits tmp = *b;

	return getbits(&tmp, n);
}

int getsbits(JpegBits *b, int n)
{
	unsigned long r = 0;
	int i;

	if(n==0)return 0;
	r = -getbit(b);
	for(i=1;i<n;i++){
		r <<=1;
		r |= getbit(b);
	}

	return r;
}

unsigned int peek_u8(JpegBits *b)
{
	return *b->ptr;
}

unsigned int get_u8(JpegBits *b)
{
	return *b->ptr++;
}

unsigned int get_u16(JpegBits *b)
{
	unsigned int r;

	r = b->ptr[0] | (b->ptr[1]<<8);
	b->ptr+=2;

	return r;
}

unsigned int get_be_u16(JpegBits *b)
{
	unsigned int r;

	r = (b->ptr[0]<<8) | b->ptr[1];
	b->ptr+=2;

	return r;
}

unsigned int get_u32(JpegBits *b)
{
	unsigned int r;

	r = b->ptr[0] | (b->ptr[1]<<8) | (b->ptr[2]<<16) | (b->ptr[3]<<24);
	b->ptr+=4;

	return r;
}

void syncbits(JpegBits *b)
{
	if(b->idx){
		b->ptr++;
		b->idx=0;
	}

}

