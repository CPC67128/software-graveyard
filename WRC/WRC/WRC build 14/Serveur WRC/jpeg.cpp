/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : jpeg.cpp
 *** Description : 
 ***
 ***/

#include "include.h"

/* Amay Champaneria
 * 6.338 Final Project
 */



/*--------------
  A hack to hijack JPEG's innards to write into a memory buffer
----------------
/  this defines a new destination manager to store images in memory
/  derived by jdatadst.c */
typedef struct {
  struct jpeg_destination_mgr pub;      /* public fields */
  JOCTET *buffer;                                       /* start of buffer */
  int bufsize;                                          /* buffer size */
  int datacount;                                        /* finale data size */
} memory_destination_mgr;

typedef memory_destination_mgr *mem_dest_ptr;

/*----------------------------------------------------------------------------
  /  Initialize destination --- called by jpeg_start_compress before any data is actually written. */

METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->bufsize;
  dest->datacount=0;
}

/*----------------------------------------------------------------------------
  /  Empty the output buffer --- called whenever buffer fills up. */
METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->bufsize;

  return TRUE;
}

/*----------------------------------------------------------------------------
  /  Terminate destination --- called by jpeg_finish_compress
  /  after all data has been written.  Usually needs to flush buffer. */
METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
  /* expose the finale compressed image size */
  
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->datacount = dest->bufsize - dest->pub.free_in_buffer;
  
}

GLOBAL(void)
jpeg_memory_dest(j_compress_ptr cinfo, JOCTET *buffer,int bufsize)
{
  mem_dest_ptr dest;
  if (cinfo->dest == NULL) {    /* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                  sizeof(memory_destination_mgr));
  }
  
  dest = (mem_dest_ptr) cinfo->dest;
  dest->bufsize=bufsize;
  dest->buffer=buffer;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
}

int
jpeg_compress(char *dst, char *src, int width, int height, int dstsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char *dataRGB = (unsigned char *)src;
  JSAMPROW row_pointer=(JSAMPROW)dataRGB;
  JOCTET *jpgbuff;
  mem_dest_ptr dest;
  int csize=0;

  /* zero out the compresion info structures and
     allocate a new compressor handle */
  memset (&cinfo,0,sizeof(cinfo));
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
 
  /* Setup JPEG datastructures */
  cinfo.image_width = width;      /* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;   /* # of color components per pixel=3 RGB */
  cinfo.in_color_space = JCS_RGB;               
  jpgbuff = (JOCTET*)dst;

  /* Setup compression and do it */
  jpeg_memory_dest(&cinfo,jpgbuff,dstsize);
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  /* compress each scanline one-at-a-time */
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW)(dataRGB+(cinfo.next_scanline*3*width));
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  /* Now extract the size of the compressed buffer */
  dest=(mem_dest_ptr)cinfo.dest;
  csize=dest->datacount; /* the actual compressed datasize */
  /* destroy the compressor handle */
  jpeg_destroy_compress(&cinfo);
  return csize;
}

static void 
init_source(j_decompress_ptr cinfo)
{
    /* nothing to do */
}

static boolean
fill_input_buffer(j_decompress_ptr cinfo)
{
    /* can't fill */
    return FALSE; 
}

static void
skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    if ((size_t)num_bytes > cinfo->src->bytes_in_buffer) {
        cinfo->src->next_input_byte = NULL;
        cinfo->src->bytes_in_buffer = 0;
    } else {
        cinfo->src->next_input_byte += (size_t) num_bytes;
        cinfo->src->bytes_in_buffer -= (size_t) num_bytes;
    }
}

static void
term_source(j_decompress_ptr cinfo)
{
    /* nothing to do */
} 


/**
 * set momory-jpeg image to JPEG lib Info struct
 * @param cinfo  JPEG lib decompress infomation structure
 * @param ptr    JPEG image 
 * @param size   JPEG image size
 */
extern void
jpeg_memory_src(j_decompress_ptr cinfo, unsigned char *ptr, size_t size)
{
    struct jpeg_source_mgr *src;
    src = cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, 
                                    JPOOL_PERMANENT,
                                    sizeof(*src));
    src->init_source       = init_source;
    src->fill_input_buffer = fill_input_buffer;
    src->skip_input_data   = skip_input_data;
    src->resync_to_restart = jpeg_resync_to_restart;
    src->term_source     = term_source;
    src->next_input_byte = ptr;
    src->bytes_in_buffer = size;
}

void
jpeg_decompress(unsigned char *dst, unsigned char *src, int size, int *w, int *h) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  int line_size;
  unsigned int y;
  unsigned char *dstcur;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_memory_src(&cinfo, src, size);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  *w = cinfo.output_width;
  *h = cinfo.output_height;
  line_size = cinfo.output_width*cinfo.output_components;

  dstcur = dst;
  for (y = 0; y < cinfo.output_height ; y++) {
    jpeg_read_scanlines(&cinfo,(JSAMPARRAY) &dstcur,1);
    dstcur += line_size;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}
