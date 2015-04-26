
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <liboil/liboil-stdint.h>

#include "jpeg_bits.h"

typedef struct _HuffmanEntry HuffmanEntry;
typedef struct _HuffmanTable HuffmanTable;

struct _HuffmanEntry {
  unsigned int symbol;
  unsigned int mask;
  int n_bits;
  unsigned char value;
};

struct _HuffmanTable {
  int len;
  HuffmanEntry entries[256];
};


void huffman_table_init(HuffmanTable *table);

void huffman_table_dump(HuffmanTable *table);
void huffman_table_add(HuffmanTable *table, uint32_t code, int n_bits,
	int value);
unsigned int huffman_table_decode_jpeg(HuffmanTable *tab, JpegBits *bits);
int huffman_table_decode_macroblock(short *block, HuffmanTable *dc_tab,
	HuffmanTable *ac_tab, JpegBits *bits);
int huffman_table_decode(HuffmanTable *dc_tab, HuffmanTable *ac_tab, JpegBits *bits);


#endif

