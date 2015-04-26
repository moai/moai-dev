
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <liboil/liboil.h>
#include <liboil/liboilcolorspace.h>

#include "jpeg.h"

/* getfile */

void *getfile (char *path, int *n_bytes);
static void dump_pnm (uint32_t *ptr, int rowstride, int width, int height);

int
main (int argc, char *argv[])
{
  unsigned char *data;
  int len;
  char *fn;
  uint32_t *image = NULL;
  int width;
  int height;
  int ret;

  if (argc < 2) {
    printf("jpeg_rgb_test <file.jpg>\n");
    exit(1);
  }
  fn = argv[1];
  data = getfile (fn, &len);

  if (data == NULL) {
    printf("cannot read file %s\n", fn);
    exit(1);
  }

  ret = jpeg_decode_argb (data, len, &image, &width, &height);
  if (ret) {
    dump_pnm (image, width*4, width, height);
  }

  if (image) free (image);

  free (data);

  return 0;
}



/* getfile */

void *
getfile (char *path, int *n_bytes)
{
  int fd;
  struct stat st;
  void *ptr = NULL;
  int ret;

  fd = open (path, O_RDONLY);
  if (!fd)
    return NULL;

  ret = fstat (fd, &st);
  if (ret < 0) {
    close (fd);
    return NULL;
  }

  ptr = malloc (st.st_size);
  if (!ptr) {
    close (fd);
    return NULL;
  }

  ret = read (fd, ptr, st.st_size);
  if (ret != st.st_size) {
    free (ptr);
    close (fd);
    return NULL;
  }

  if (n_bytes)
    *n_bytes = st.st_size;

  close (fd);
  return ptr;
}

static void
dump_pnm (uint32_t *ptr, int rowstride, int width, int height)
{
  int x, y;

  printf ("P3\n");
  printf ("%d %d\n", width, height);
  printf ("255\n");

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      printf ("%d ", oil_argb_R(ptr[x]));
      printf ("%d ", oil_argb_G(ptr[x]));
      printf ("%d ", oil_argb_B(ptr[x]));
      if ((x & 15) == 15) {
        printf ("\n");
      }
    }
    printf ("\n");
    ptr += rowstride/4;
  }
}
