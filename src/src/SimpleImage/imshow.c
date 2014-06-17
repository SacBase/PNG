#include "imagesac.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <png.h>

void imshow( SAC_ND_PARAM_out_nodesc( res_nt, int),
	     SAC_ND_PARAM_in( ar_nt, int))
{
  char *fname;
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep  *row_pointers;
  png_bytep   img_data;
  png_int_32 *bptr;
  int height, width;
  int x, y, i, aroffset, ioffset;
  char *sys;
  
  fname = tempnam( NULL, "img_");
  fp = fopen( fname, "wb");
  if ( fp == NULL) {
    SAC_RuntimeError( "Failed to create temporary image file");
  }   

  png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if ( png_ptr == NULL) {
    SAC_RuntimeError( "Failed to create png structure");
  } 
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr,
			     (png_infopp)NULL);
    SAC_RuntimeError( "Failed to create png info structure");
  }

  height = SAC_ND_A_DESC_SHAPE( ar_nt, 0);
  width  = SAC_ND_A_DESC_SHAPE( ar_nt, 1);

  png_set_IHDR( png_ptr, info_ptr, 
		width,
		height,
		8,                              /* bit depth */
		PNG_COLOR_TYPE_RGB,             /* color type */
		PNG_INTERLACE_NONE,             /* interlace type */
		PNG_COMPRESSION_TYPE_DEFAULT,   /* compression type */
		PNG_FILTER_TYPE_DEFAULT);       /* filter type */

  row_pointers = malloc( height*sizeof( png_bytep));
  img_data = malloc( height*width*4);
  for ( i = 0; i < height; i++) {
    row_pointers[i] = img_data + (4*i*width);
  }

  /*
   * draw
   */
  bptr = (png_int_32 *)img_data;
  aroffset = 0;
  ioffset = 0;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      bptr[ioffset] = 
	(((png_int_32)SAC_ND_A_FIELD( ar_nt)[aroffset])     << 16
	 | ((png_int_32)SAC_ND_A_FIELD( ar_nt)[aroffset+1]) << 8
	 | ((png_int_32)SAC_ND_A_FIELD( ar_nt)[aroffset+2]));
      ioffset +=1;
      aroffset+=3;
    }
  }
  png_set_rows(png_ptr, info_ptr, row_pointers);

  /*
   * Write the image
   */
  png_init_io( png_ptr, fp);
  png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_FILLER, NULL);

  /*
   * Clean up
   */
  free( img_data);
  free( row_pointers);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose( fp);
  

  /*
   * show image
   */
  sys = malloc( 1024*sizeof( char));
  sprintf( sys, "(display \"%s\" && rm -f \"%s\") > /dev/null 2>&1 &", 
	   fname, fname);
  system( sys);

  free( sys);
  free( fname);


  /*
   * return 0
   */
  *SAC_NAMEP( SAC_ND_A_FIELD( res_nt)) = 0;

  SAC_ND_DEC_RC( ar_nt, 1)  
}
           
