/**
 *This is a inmemory jpeg decompression object.
 **/

#ifndef JPEGDECOMPRESSION_H
#define JPEGDECOMPRESSION_H

#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>

/**
 *These function really serve no purpose for 
 *a inmemory decompresser, however they are
 *called by libjpeg.
 **/
void JPDC_init_source(j_decompress_ptr cinfo);
boolean JPDC_fill_input_buffer(j_decompress_ptr cinfo);
void JPDC_skip_input_data(j_decompress_ptr cinfo, long num_bytes);
void JPDC_term_source(j_decompress_ptr cinfo);

/**
 *JPEG Decompression object
 **/
class JPEGDecompression
{
public:
  
  /**
   *Main class contructor.
   **/
  JPEGDecompression();

  /**
   *Class destructor
   **/
  virtual ~JPEGDecompression();

  /**
   *This function accepts a JPEG stream and decompresses
   *it interally.  Use getWidth, getHeight, and getData
   *to retrieve the compressed data. (only decopmresses 8 bit
   *3 sample data!)
   **/
  bool decompress(unsigned char * indata, 
                  const unsigned long int & length) throw();

  /**
   *This function returns the width of the compressed
   *image.
   **/
  unsigned long int getWidth() const throw();

  /**
   *This function returns the height of the compressed
   *image.
   **/
  unsigned long int getHeight() const throw();

  /**
   *This function copys the decompressed data
   *into a array.
   **/
  bool getData(unsigned char * inarray) const throw();

  /**
   *This function returns the decompressed data
   *as a const pointer
   **/
  const unsigned char * getDatap() const throw();

protected:

  unsigned char * buffer;               //buffer for storing decompressed data
  unsigned long int width, height;      //width and height
  struct jpeg_decompress_struct cinfo;  //jpeg data
  struct jpeg_source_mgr pub;           //source data manager
};


#endif
