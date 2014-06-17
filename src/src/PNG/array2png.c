#include "imagesac.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <png.h>

void SAC_PNG_array2png(SAC_ND_PARAM_out_nodesc(res_nt, int),
                       SAC_ND_PARAM_in_nodesc(string_nt, string),
                       SAC_ND_PARAM_in(ar_nt, int))
{
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *row_pointers;
  png_bytep img_data;
  int x, y;
  int height = SAC_ND_A_DESC_SHAPE(ar_nt, 0);
  int width = SAC_ND_A_DESC_SHAPE(ar_nt, 1);
  int *arr_data = SAC_ND_A_FIELD(ar_nt);

  fp = fopen(NT_NAME(string_nt), "wb");
  if (fp == NULL)
  {
    SAC_RuntimeError("SAC_PNG_array2png: Failed to open image file '%s': %s",
                     NT_NAME(string_nt), strerror(errno));
    return;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
  {
    SAC_RuntimeError("SAC_PNG_array2png: Failed to create png structure: %s",
                    strerror(errno));
    return;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
    SAC_RuntimeError("SAC_PNG_array2png: Failed to create png info structure: %s",
                    strerror(errno));
    return;
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    SAC_RuntimeError("SAC_PNG_array2png: Failed to read PGN image");
    return;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, width, height, 8,     /* bit depth */
               PNG_COLOR_TYPE_RGB,      /* color type */
               PNG_INTERLACE_NONE,      /* interlace type */
               PNG_COMPRESSION_TYPE_DEFAULT,    /* compression type */
               PNG_FILTER_TYPE_DEFAULT);        /* filter type */

  row_pointers = malloc(height * sizeof(png_bytep));
  img_data = malloc(height * width * 3);
  for (y = 0; y < height; y++)
  {
    row_pointers[y] = img_data + (y * width * 3);
  }

  /*
   * draw
   */
  for (y = 0; y < height; y++)
  {
    unsigned char *bptr = (unsigned char *) row_pointers[y];
    for (x = 0; x < width; x++)
    {
      *bptr++ = *arr_data++;
      *bptr++ = *arr_data++;
      *bptr++ = *arr_data++;
    }
  }
  png_set_rows(png_ptr, info_ptr, row_pointers);

  /*
   * Write the image
   */
  png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  png_write_end(png_ptr, info_ptr);

  /*
   * Clean up
   */
  free(row_pointers);
  free(img_data);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);

  /*
   * return 0
   */
  *SAC_NAMEP(SAC_ND_A_FIELD(res_nt)) = 0;
  SAC_ND_DEC_RC_FREE(ar_nt, 1,)
}
