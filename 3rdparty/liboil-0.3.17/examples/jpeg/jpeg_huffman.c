
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <liboil/liboil.h>
#include <liboil/liboildebug.h>

#include "jpeg_huffman.h"
#include "jpeg_debug.h"

/* misc helper function definitions */

static char *sprintbits (char *str, unsigned int bits, int n);


#define TRUE 1
#define FALSE 0

void
huffman_table_dump (HuffmanTable * table)
{
  unsigned int n_bits;
  unsigned int code;
  char str[33];
  int i;
  HuffmanEntry *entry;

  OIL_DEBUG ("dumping huffman table %p", table);
  for (i = 0; i < table->len; i++) {
    entry = table->entries + i;
    n_bits = entry->n_bits;
    code = entry->symbol >> (16 - n_bits);
    sprintbits (str, code, n_bits);
    OIL_DEBUG ("%s --> %d", str, entry->value);
  }
}

void
huffman_table_init (HuffmanTable *table)
{
  memset (table, 0, sizeof(HuffmanTable));
}

void
huffman_table_add (HuffmanTable * table, uint32_t code, int n_bits, int value)
{
  HuffmanEntry *entry = table->entries + table->len;

  entry->value = value;
  entry->symbol = code << (16 - n_bits);
  entry->mask = 0xffff ^ (0xffff >> n_bits);
  entry->n_bits = n_bits;

  table->len++;
}

unsigned int
huffman_table_decode_jpeg (HuffmanTable * tab, JpegBits * bits)
{
  unsigned int code;
  int i;
  char str[33];
  HuffmanEntry *entry;

  code = peekbits (bits, 16);
  for (i = 0; i < tab->len; i++) {
    entry = tab->entries + i;
    if ((code & entry->mask) == entry->symbol) {
      code = getbits (bits, entry->n_bits);
      sprintbits (str, code, entry->n_bits);
      OIL_DEBUG ("%s --> %d", str, entry->value);
      return entry->value;
    }
  }
  printf ("huffman sync lost");

  return -1;
}

int
huffman_table_decode_macroblock (short *block, HuffmanTable * dc_tab,
    HuffmanTable * ac_tab, JpegBits * bits)
{
  int r, s, x, rs;
  int k;
  char str[33];

  memset (block, 0, sizeof (short) * 64);

  s = huffman_table_decode_jpeg (dc_tab, bits);
  if (s < 0)
    return -1;
  x = getbits (bits, s);
  if ((x >> (s - 1)) == 0) {
    x -= (1 << s) - 1;
  }
  OIL_DEBUG ("s=%d (block[0]=%d)", s, x);
  block[0] = x;

  for (k = 1; k < 64; k++) {
    rs = huffman_table_decode_jpeg (ac_tab, bits);
    if (rs < 0) {
      OIL_DEBUG ("huffman error");
      return -1;
    }
    if (bits->ptr > bits->end) {
      OIL_DEBUG ("overrun");
      return -1;
    }
    s = rs & 0xf;
    r = rs >> 4;
    if (s == 0) {
      if (r == 15) {
        OIL_DEBUG ("r=%d s=%d (skip 16)", r, s);
        k += 15;
      } else {
        OIL_DEBUG ("r=%d s=%d (eob)", r, s);
        break;
      }
    } else {
      k += r;
      if (k >= 64) {
        printf ("macroblock overrun");
        return -1;
      }
      x = getbits (bits, s);
      sprintbits (str, x, s);
      if ((x >> (s - 1)) == 0) {
        x -= (1 << s) - 1;
      }
      block[k] = x;
      OIL_DEBUG ("r=%d s=%d (%s -> block[%d]=%d)", r, s, str, k, x);
    }
  }
  return 0;
}

int
huffman_table_decode (HuffmanTable * dc_tab, HuffmanTable * ac_tab,
    JpegBits * bits)
{
  int16_t zz[64];
  int ret;

  while (bits->ptr < bits->end) {
    ret = huffman_table_decode_macroblock (zz, dc_tab, ac_tab, bits);
    if (ret < 0)
      return -1;
  }

  return 0;
}

/* misc helper functins */

static char *
sprintbits (char *str, unsigned int bits, int n)
{
  int i;
  int bit = 1 << (n - 1);

  for (i = 0; i < n; i++) {
    str[i] = (bits & bit) ? '1' : '0';
    bit >>= 1;
  }
  str[i] = 0;

  return str;
}
