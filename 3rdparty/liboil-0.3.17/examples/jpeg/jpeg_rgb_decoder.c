
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jpeg_internal.h"
#include "jpeg.h"

#include <liboil/liboil.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilcolorspace.h>

#define CLAMP(x,a,b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))

static int16_t jfif_matrix[24] = {
  0,      0,      -8192,   -8192,
  16384,  0,      0,       0,
  0,      16384,  16384,   16384,
  0,      0,      -5638,   29032,
  0,      22970,  -11700,  0,
  0, 0, 0, 0
};


unsigned char * get_argb_444 (JpegDecoder *dec);
unsigned char * get_argb_422 (JpegDecoder *dec);
unsigned char * get_argb_422v (JpegDecoder *dec);
unsigned char * get_argb_420 (JpegDecoder *dec);

#if 0
static void imagescale2h_u8 (unsigned char *dest, int d_rowstride,
    unsigned char *src, int src_rowstride, int width, int height);
static void imagescale2v_u8 (unsigned char *dest, int d_rowstride,
    unsigned char *src, int src_rowstride, int width, int height);
static void imagescale2h2v_u8 (unsigned char *dest, int d_rowstride,
    unsigned char *src, int src_rowstride, int width, int height);
static void scanlinescale2_u8 (unsigned char *dest, unsigned char *src,
    int len);
#endif


int jpeg_decode_argb (uint8_t *data, int length, uint32_t **image,
    int *width, int *height)
{
  JpegDecoder *dec;
  int ret;

  dec = jpeg_decoder_new();

  jpeg_decoder_addbits (dec, data, length);
  ret = jpeg_decoder_decode(dec);

  if (!ret) return FALSE;

  jpeg_decoder_get_image_size (dec, width, height);
  *image = (uint32_t *)jpeg_decoder_get_argb_image (dec);

  return TRUE;
}

unsigned char *
jpeg_decoder_get_argb_image (JpegDecoder *dec)
{

  if (dec->n_components == 3) {
    if (dec->components[0].h_subsample == 1 &&
        dec->components[0].v_subsample == 1 &&
        dec->components[1].h_subsample == dec->components[2].h_subsample &&
        dec->components[1].v_subsample == dec->components[2].v_subsample) {
      if (dec->components[1].h_subsample == 1 &&
          dec->components[1].v_subsample == 1) {
        return get_argb_444 (dec);
      } else if (dec->components[1].h_subsample == 2 &&
          dec->components[1].v_subsample == 1) {
        return get_argb_422 (dec);
      } else if (dec->components[1].h_subsample == 1 &&
          dec->components[1].v_subsample == 2) {
        return get_argb_422v (dec);
      } else if (dec->components[1].h_subsample == 2 &&
          dec->components[1].v_subsample == 2) {
        return get_argb_420 (dec);
      }
    }
  }

  return NULL;
}

static void
yuv_mux (uint32_t *dest, uint8_t *src_y, uint8_t *src_u, uint8_t *src_v,
    int n)
{
  int i;
  for (i = 0; i < n; i++) {
    dest[i] = oil_argb(255, src_y[i], src_u[i], src_v[i]);
  }
}

static void
upsample (uint8_t *d, uint8_t *s, int n)
{
  int i;

  d[0] = s[0];

  for (i = 0; i < n-3; i+=2) {
    d[i + 1] = (3*s[i/2] + s[i/2+1] + 2)>>2;
    d[i + 2] = (s[i/2] + 3*s[i/2+1] + 2)>>2;
  }

  if (n&1) {
    i = n-3;
    d[n-2] = s[n/2];
    d[n-1] = s[n/2];
  } else {
    d[n-1] = s[n/2-1];
  }

}

unsigned char *
get_argb_444 (JpegDecoder *dec)
{
  uint32_t *tmp;
  uint32_t *argb_image;
  uint8_t *yp, *up, *vp;
  uint32_t *argbp;
  int j;

  tmp = malloc (4 * dec->width * dec->height);
  argb_image = malloc (4 * dec->width * dec->height);

  yp = dec->components[0].image;
  up = dec->components[1].image;
  vp = dec->components[2].image;
  argbp = argb_image;
  for(j=0;j<dec->height;j++){
    yuv_mux (tmp, yp, up, vp, dec->width);
    oil_colorspace_argb(argbp, tmp, jfif_matrix, dec->width);
    yp += dec->components[0].rowstride;
    up += dec->components[1].rowstride;
    vp += dec->components[2].rowstride;
    argbp += dec->width;
  }
  free(tmp);
  return (unsigned char *)argb_image;
}

unsigned char *
get_argb_422 (JpegDecoder *dec)
{
  uint32_t *tmp;
  uint8_t *tmp_u;
  uint8_t *tmp_v;
  uint32_t *argb_image;
  uint8_t *yp, *up, *vp;
  uint32_t *argbp;
  int j;

  tmp = malloc (4 * dec->width * dec->height);
  tmp_u = malloc (dec->width);
  tmp_v = malloc (dec->width);
  argb_image = malloc (4 * dec->width * dec->height);

  yp = dec->components[0].image;
  up = dec->components[1].image;
  vp = dec->components[2].image;
  argbp = argb_image;
  for(j=0;j<dec->height;j++){
    upsample (tmp_u, up, dec->width);
    upsample (tmp_v, vp, dec->width);
    yuv_mux (tmp, yp, tmp_u, tmp_v, dec->width);
    oil_colorspace_argb(argbp, tmp, jfif_matrix, dec->width);
    yp += dec->components[0].rowstride;
    up += dec->components[1].rowstride;
    vp += dec->components[2].rowstride;
    argbp += dec->width;
  }
  free(tmp);
  free(tmp_u);
  free(tmp_v);
  return (unsigned char *)argb_image;
}

unsigned char *
get_argb_422v (JpegDecoder *dec)
{
  uint32_t *tmp;
  uint8_t *tmp_u;
  uint8_t *tmp_v;
  uint32_t *argb_image;
  uint8_t *yp, *up, *vp;
  uint32_t *argbp;
  int halfheight;
  int j;

OIL_ERROR("got here");
  tmp = malloc (4 * dec->width * dec->height);
  tmp_u = malloc (dec->width);
  tmp_v = malloc (dec->width);
  argb_image = malloc (4 * dec->width * dec->height);

  yp = dec->components[0].image;
  up = dec->components[1].image;
  vp = dec->components[2].image;
  argbp = argb_image;
  halfheight = (dec->height+1)>>1;
  for(j=0;j<dec->height;j++){
    uint32_t weight = 192 - 128*(j&1);

    oil_merge_linear_u8(tmp_u,
        up + dec->components[1].rowstride * CLAMP((j-1)/2,0,halfheight-1),
        up + dec->components[1].rowstride * CLAMP((j+1)/2,0,halfheight-1),
        &weight, dec->width);
    oil_merge_linear_u8(tmp_v,
        vp + dec->components[2].rowstride * CLAMP((j-1)/2,0,halfheight-1),
        vp + dec->components[2].rowstride * CLAMP((j+1)/2,0,halfheight-1),
        &weight, dec->width);

    yuv_mux (tmp, yp, tmp_u, tmp_v, dec->width);
    oil_colorspace_argb(argbp, tmp, jfif_matrix, dec->width);
    yp += dec->components[0].rowstride;
    argbp += dec->width;
  }
  free(tmp);
  free(tmp_u);
  free(tmp_v);
  return (unsigned char *)argb_image;
}

unsigned char *
get_argb_420 (JpegDecoder *dec)
{
  uint32_t *tmp;
  uint8_t *tmp_u;
  uint8_t *tmp_v;
  uint8_t *tmp1;
  uint32_t *argb_image;
  uint8_t *yp, *up, *vp;
  uint32_t *argbp;
  int j;
  int halfwidth;
  int halfheight;

  halfwidth = (dec->width + 1)>>1;
  tmp = malloc (4 * dec->width * dec->height);
  tmp_u = malloc (dec->width);
  tmp_v = malloc (dec->width);
  tmp1 = malloc (halfwidth);
  argb_image = malloc (4 * dec->width * dec->height);

  yp = dec->components[0].image;
  up = dec->components[1].image;
  vp = dec->components[2].image;
  argbp = argb_image;
  halfheight = (dec->height+1)>>1;
  for(j=0;j<dec->height;j++){
    uint32_t weight = 192 - 128*(j&1);

    oil_merge_linear_u8(tmp1,
        up + dec->components[1].rowstride * CLAMP((j-1)/2,0,halfheight-1),
        up + dec->components[1].rowstride * CLAMP((j+1)/2,0,halfheight-1),
        &weight, halfwidth);
    upsample (tmp_u, tmp1, dec->width);
    oil_merge_linear_u8(tmp1,
        vp + dec->components[2].rowstride * CLAMP((j-1)/2,0,halfheight-1),
        vp + dec->components[2].rowstride * CLAMP((j+1)/2,0,halfheight-1),
        &weight, halfwidth);
    upsample (tmp_v, tmp1, dec->width);

    yuv_mux (tmp, yp, tmp_u, tmp_v, dec->width);
    oil_colorspace_argb(argbp, tmp, jfif_matrix, dec->width);
    yp += dec->components[0].rowstride;
    argbp += dec->width;
  }
  free(tmp);
  free(tmp_u);
  free(tmp_v);
  free(tmp1);
  return (unsigned char *)argb_image;
}

#if 0
int
jpeg_rgb_decoder_get_image (JpegRGBDecoder * rgbdec,
    unsigned char **image, int *rowstride, int *width, int *height)
{
  int i;

  jpeg_decoder_get_image_size (rgbdec->dec, &rgbdec->width, &rgbdec->height);
  for (i = 0; i < 3; i++) {
    jpeg_decoder_get_component_ptr (rgbdec->dec, i + 1,
        &rgbdec->component[i].image, &rgbdec->component[i].rowstride);
    jpeg_decoder_get_component_subsampling (rgbdec->dec, i + 1,
        &rgbdec->component[i].h_subsample, &rgbdec->component[i].v_subsample);
    rgbdec->component[i].alloc = 0;
    if (rgbdec->component[i].h_subsample > 1 ||
        rgbdec->component[i].v_subsample > 1) {
      unsigned char *dest;

      dest = malloc (rgbdec->width * rgbdec->height);
      if (rgbdec->component[i].v_subsample > 1) {
        if (rgbdec->component[i].h_subsample > 1) {
          imagescale2h2v_u8 (dest,
              rgbdec->width,
              rgbdec->component[i].image,
              rgbdec->component[i].rowstride, rgbdec->width, rgbdec->height);
        } else {
          imagescale2v_u8 (dest,
              rgbdec->width,
              rgbdec->component[i].image,
              rgbdec->component[i].rowstride, rgbdec->width, rgbdec->height);
        }
      } else {
        imagescale2h_u8 (dest,
            rgbdec->width,
            rgbdec->component[i].image,
            rgbdec->component[i].rowstride, rgbdec->width, rgbdec->height);
      }
      rgbdec->component[i].alloc = 1;
      rgbdec->component[i].image = dest;
      rgbdec->component[i].rowstride = rgbdec->width;
      rgbdec->component[i].h_subsample = 1;
      rgbdec->component[i].v_subsample = 1;
    }
  }

  rgbdec->image = malloc (rgbdec->width * rgbdec->height * 4);

  convert (rgbdec);

  if (image)
    *image = rgbdec->image;
  if (rowstride)
    *rowstride = rgbdec->width * 4;
  if (width)
    *width = rgbdec->width;
  if (height)
    *height = rgbdec->height;

  return 0;
}
#endif


