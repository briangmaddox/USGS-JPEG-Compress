/**
 *Implementation file for the inmemory jpeg decompression file
 **/

#include "JPEGDecompression.h"
#include <new>

void JPDC_init_source(j_decompress_ptr cinfo) {}
boolean JPDC_fill_input_buffer(j_decompress_ptr cinfo) {return TRUE;}
void JPDC_skip_input_data(j_decompress_ptr cinfo, long num_bytes) {}
void JPDC_term_source(j_decompress_ptr cinfo) {}

//***********************************************************************
JPEGDecompression::JPEGDecompression() :  buffer(0), width(0), 
  height(0)
{
  pub.init_source = JPDC_init_source;
  pub.fill_input_buffer = JPDC_fill_input_buffer;
  pub.skip_input_data = JPDC_skip_input_data;
  pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  pub.term_source = JPDC_term_source;
  
}

//***********************************************************************
JPEGDecompression::~JPEGDecompression()
{
  if (buffer)
  {
    delete [] buffer;
  }
}

//***********************************************************************
bool JPEGDecompression::decompress(unsigned char * indata, 
                  const unsigned long int & length) throw()
{
  JSAMPROW row_pointer[1];
  int rows_stride(0);
  struct jpeg_error_mgr jerr;

  try
  {
    //create the jpeg compression
    cinfo.err = jpeg_std_error(&jerr);
    
    //create the decompress
    jpeg_create_decompress(&cinfo);
    cinfo.src = &pub;
    pub.next_input_byte = indata;
    pub.bytes_in_buffer = length;

    //read the header
    jpeg_read_header(&cinfo, TRUE);
    
    //start the decompression
    jpeg_start_decompress(&cinfo);
    width = cinfo.output_width;
    height = cinfo.output_height;

    rows_stride = width*3;

    if (buffer)
    {
      delete [] buffer;
      buffer = 0;
    }

    //create the buffer
    if (!(buffer = new (std::nothrow) unsigned char [width*height*3]))
      throw std::bad_alloc();
    
    
    while (cinfo.output_scanline < cinfo.output_height)
    {
      row_pointer[0] = &buffer[cinfo.output_scanline*width*3];
      jpeg_read_scanlines(&cinfo,row_pointer,1);
    }

    //finish the decompression
    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    return true;

  }
  catch(...)
  {
    delete [] buffer;
    buffer = 0;
    return false;
  }
}

//***********************************************************************
unsigned long int JPEGDecompression::getWidth() const throw()
{
  return width;
}

//***********************************************************************
unsigned long int JPEGDecompression::getHeight() const throw()
{
  return height;
}

//***********************************************************************
bool JPEGDecompression::getData(unsigned char * inarray) const throw()
{
  unsigned long int counter(0);

  if(!buffer)
    return false;
  
  for (counter = 0; counter < width*height*3; ++counter)
  {
    inarray[counter] = buffer[counter];
  }

  return true;
}

//***********************************************************************
const unsigned char * JPEGDecompression::getDatap() const throw()
{
  return buffer;
}
