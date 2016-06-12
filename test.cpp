/**
 *A test to see how long it takes to compress
 *a image to jpeg in memory
 **/

#include "ImageLib/TIFFImageIFile.h"  //for image input
#include "ImageLib/TIFFImageOFile.h"  //for image output
#include "JPEGDecompression.h"        //for memory decompression
#include "JPEGCompression.h"          //in memory compression wrapper
#include "MiscUtils/MicroTimer.h"     //for time values

#define NUM_RUNS 1000


//main entry point
int main(int argc, char ** argv)
{
  float averagetime(0.0f);               //average compression time
  JPEGCompression compobj;               //compression object
  JPEGDecompression decompobj;           //decompression object
  USGSImageLib::TIFFImageIFile * tif(0); //input image
  USGSImageLib::TIFFImageOFile * otif(0);//output image
  unsigned char * buffer(0);             //input buffer
  long int width(0), height(0);          //image metrics
  int spp(3);                            //samples per pixel
  unsigned long int temptime(0);
  long int counter(0);
  MiscUtils::MicroTimer timer;           //for timings
  std::string file;
  if (argc != 2)
  {
    std::cout << "Please enter the test image file as a arg" << std::endl;
    return 0;
  }

 
  try
  {
    file = argv[1];
    if (!(tif = new (std::nothrow) USGSImageLib::TIFFImageIFile
          (file)))
      throw std::bad_alloc();
    
    //get the image metrics
    tif->getWidth(width);
    tif->getHeight(height);
    tif->getSamplesPerPixel(spp);

    if (spp != 3)
      throw std::bad_alloc(); //throw a stupid exception
    
    //try to create the input buffer
    if (!(buffer = new (std::nothrow) unsigned char [width*height*spp]))
      throw std::bad_alloc();
    
    //now read the image
    for(counter = 0; counter < height; ++counter)
    {
      tif->getRawScanline(counter, &buffer[counter*width*spp]);
    }
    
    compobj.setQuality(50);

    //now compress the thing
    for (counter = 0; counter < NUM_RUNS; ++counter)
    {
      timer.start();
      
      if(!compobj.compress(buffer, width, height))
        throw std::bad_alloc();

      timer.stop(temptime);
      averagetime += temptime;
    }

    std::cout << "Average compression time " << averagetime/NUM_RUNS 
              << std::endl;

    averagetime = 0.0f;
    compobj.getData(buffer); //get the data
    //now uncompress the thing
    for (counter = 0; counter < NUM_RUNS; ++counter)
    {
      timer.start();
      
      if (!decompobj.decompress(buffer, compobj.getDataSize()))
        throw std::bad_alloc();
      
      timer.stop(temptime);
      averagetime += temptime;
    }

    std::cout << "Average decompression time " << averagetime/NUM_RUNS
              << std::endl;
    
    std::string outfile = "out.tif";
    
    if(!(otif = new (std::nothrow) USGSImageLib::TIFFImageOFile
         (outfile, decompobj.getWidth(), decompobj.getHeight(), 
          PHOTO_RGB)))
      throw std::bad_alloc();
    
    otif->setSamplesPerPixel(3);
    otif->setBitsPerSample(8);
    otif->setPlanarConfig(1);
    otif->setSubfileType(0);
    otif->setCompression(COMPRESSION_NONE);

    decompobj.getData(buffer);

    for(counter = 0; counter < decompobj.getHeight(); counter++)
    {
      otif->putRawScanline(counter, &(buffer[counter*width*3]));
    }

    delete otif;
    delete tif;
    delete [] buffer;
  }
  catch(...)
  {
    std::cout << "Shazbot!" << std::endl;
    delete tif;
    delete [] buffer;
  }
}
    
    
    
    
  
  
  

  
