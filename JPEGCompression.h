/**
 *This is a in memory jpeg compression class
 **/

#ifndef JPEGCOMPRESSION_H_
#define JPEGCOMPRESSION_H_

#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>

/**
 *These are the handling function that are called by lib jpeg
 **/
void JPC_initInMemDest(j_compress_ptr cinfo); //init the destition mgr
boolean JPC_emptyBuffer(j_compress_ptr cinfo);//never called
void JPC_termInMemDest(j_compress_ptr cinfo);//called when ending compression


/**
 *The in memory jpeg compression class
 **/
class JPEGCompression
{
public:

  /**
   *Main constructor for the class
   **/
  JPEGCompression();

  /**
   *Class destructor
   **/
  ~JPEGCompression();

  /**
   *This function actually starts the 
   *compression. Use getdata and getdatasize
   *to retreive the results. This function 
   *accepts only 24 bit RGB color.
   **/
  bool compress(unsigned char * indata,
                const unsigned long int & width, 
                const unsigned long int & height) throw();

  /**
   *This function sets the quality used 
   *during compression. Quality defaults
   *to 75
   **/
  void setQuality(const int & inQuality) throw();


  /**
   *This function returns the size of
   *the compressed data sotred in the class
   **/
  unsigned long int getDataSize() throw();

  /**
   *This function returns a copy of the 
   *data.
   **/
  bool getData(unsigned char * indata) throw();

  /**
   *This function is a alternate to the prev function
   *which returns a constant pointer to the 
   *compress data.
   **/
  const unsigned char * getDatap() throw();


protected:
  
  int quality;                        //jpeg compression quality
  struct jpeg_compress_struct cinfo;  //jpeg compression struct
  struct jpeg_destination_mgr pub;    //pointers to jpeg handling funcs
  JOCTET * buffer;                    //the start of the buffer
  unsigned int buffer_size;           //the size of the buffer 
};


#endif
