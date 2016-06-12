/**
 *Implmentation file for the in memory jpeg compression
 *class.
 **/

#include "JPEGCompression.h"
#include <new>


void JPC_initInMemDest(j_compress_ptr cinfo){} //init the destition mgr
boolean JPC_emptyBuffer(j_compress_ptr cinfo)
{ return TRUE;}//never called
void JPC_termInMemDest(j_compress_ptr cinfo){}//called when ending compression

//**************************************************************************
JPEGCompression::JPEGCompression() : quality(75), buffer(0), buffer_size(0)
{
  pub.init_destination = JPC_initInMemDest;
  pub.empty_output_buffer = JPC_emptyBuffer;
  pub.term_destination = JPC_termInMemDest;
    
}

//**************************************************************************
JPEGCompression::~JPEGCompression()
{
  if (buffer)
  {
    delete [] buffer;   //nuke the buffer
  }   
}

//**************************************************************************
bool JPEGCompression::compress(unsigned char * indata,
                const unsigned long int & width, 
                const unsigned long int & height) throw()
{
  JSAMPROW row_pointer[1];
  int row_stride(0);
  struct jpeg_error_mgr jerr;
  try
  {
    //create the jpeg compression
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    //set up the memory manager
    cinfo.dest = &pub;
    
    if (buffer)
    {
      delete [] buffer;
      buffer = 0;
    }
    
    //create the buffer
    if (!(buffer = new (std::nothrow) JOCTET[width*height*3]))
      throw std::bad_alloc();

    
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
    //set the compression defaults
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    
    pub.next_output_byte = buffer;
    pub.free_in_buffer = width*height*3;

    //start the compression
    jpeg_start_compress(&cinfo, TRUE);
    
    row_stride = width*3;
    
    //do the deal
    while (cinfo.next_scanline < cinfo.image_height)
    {
      row_pointer[0] = &indata[cinfo.next_scanline*row_stride];
      jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
      
    jpeg_finish_compress(&cinfo);
    
    buffer_size = width*height*3 - pub.free_in_buffer;
    //release the cinfo stuff
    jpeg_destroy_compress(&cinfo);
    
    return true;
 
  }
  catch(...)
  {
    delete [] buffer;
    return false;
  } 

}

//**************************************************************************
void JPEGCompression::setQuality(const int & inQuality) throw()
{
  if (inQuality > 0 && inQuality <= 100)
    quality = inQuality;   //set the quality
}

//**************************************************************************
unsigned long int JPEGCompression::getDataSize() throw()
{
  return buffer_size;
}

//**************************************************************************
bool JPEGCompression::getData(unsigned char * indata) throw()
{
  unsigned long int counter(0);

  if(!buffer)
    return false;

  for (counter = 0; counter < buffer_size; ++counter)
  {
    indata[counter] = buffer[counter];
  }

  return true;
}

//**************************************************************************
const unsigned char * JPEGCompression::getDatap() throw()
{
  return buffer;
}




