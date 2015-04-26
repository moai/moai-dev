
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

enum {
  HUFF_OK = 0,
  HUFF_NEED_BYTE
};

typedef struct _Huffman Huffman;
typedef struct _HuffmanCode HuffmanCode;
typedef struct _HuffmanTable HuffmanTable;
typedef struct _HuffmanTableEntry HuffmanTableEntry;

struct _Huffman {
  int n_codes;
  HuffmanCode *codes;
  HuffmanTable *tables;

  int state;
  unsigned int state_bits;
  int state_n_bits;

  unsigned char next_byte;
  int out_value;
};

struct _HuffmanCode {
  unsigned int code;
  unsigned int mask;
  int n_bits;
  int value;
};

struct _HuffmanTableEntry {
  unsigned int codes[8];
  int n_codes;
  struct _HuffmanTable *next_table;
};

struct _HuffmanTable {
  int n_bits;
  unsigned int bits;
  HuffmanTableEntry entries[256];
};


Huffman * huffman_new (void);
void huffman_add_code (Huffman *huff, unsigned int code, int n_bits, int value);
void huffman_decode_ref (Huffman *huff, int *dest, unsigned char *src, int n);

#endif

