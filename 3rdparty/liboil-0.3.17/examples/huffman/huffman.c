
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "huffman.h"



Huffman *
huffman_new (void)
{
  Huffman *huff;

  huff = malloc (sizeof (Huffman));
  memset (huff, 0, sizeof(Huffman));

  return huff;
}


void
huffman_add_code (Huffman *huff, unsigned int code, int n_bits,
    int value)
{
  huff->codes = realloc(huff->codes,
      sizeof(HuffmanCode) * (huff->n_codes + 1));
  huff->codes[huff->n_codes].value = value;
  huff->codes[huff->n_codes].code = code << (32 - n_bits);
  huff->codes[huff->n_codes].mask = 0xffffffff << (32 - n_bits);
  huff->codes[huff->n_codes].n_bits = n_bits;

  huff->n_codes++;
}


void huffman_decode_iterate (Huffman *huff)
{
  unsigned int bits;
  int i;

  if (huff->state == HUFF_NEED_BYTE) {
    huff->state_bits = (huff->state_bits << 8) | huff->next_byte;
    huff->state_n_bits += 8;
  }

  bits = huff->state_bits << (32 - huff->state_n_bits);

  for(i=0;i<huff->n_codes;i++){
    if (huff->codes[i].n_bits <= huff->state_n_bits &&
        huff->codes[i].code == (bits & huff->codes[i].mask)) {
      huff->out_value = huff->codes[i].value;
      break;
    }
  }
  if (i == huff->n_codes) {
    huff->state = HUFF_NEED_BYTE;
  } else {
    huff->state = HUFF_OK;
  }

}

void huffman_decode_ref (Huffman *huff, int *dest, unsigned char *src, int n)
{
  int i;

  i = 0;
  while(n>0 || huff->state == HUFF_OK){
    printf("STATE %d\n", huff->state);
    if (huff->state == HUFF_NEED_BYTE) {
      printf("pushing byte %02x\n", *src);
      huff->next_byte = *src;
      src++;
      n--;
    }
    huffman_decode_iterate (huff);
    if (huff->state == HUFF_OK) {
      printf("got value %d\n", huff->out_value);
      dest[i] = huff->out_value;
    }
  }
}

