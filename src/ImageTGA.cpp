/*******************************************************************\
*  Bezier Patch Editor  ---  ImageTGA                               *
*                                                                   *
*  Legge immagini TGA in formato non compresso.                     *
\*******************************************************************/

#include <memory.h>

#include "ImageTGA.h"

// Header delle immagini TGA
typedef struct
{
   byte numIden;
   byte colorMapType;
   byte imageType;
   byte colorMapSpec[5]; // not used, just here to take up space
   byte origX[2];
   byte origY[2];
   byte width[2];
   byte height[2];
   byte bpp;
   byte imageDes; // don't use, space eater
} tgaHeader_t;

ImageTGA::ImageTGA() : Image()
{
}

/////////////////////////////////////////////////////////////////////
// Legge l'immagine da file

int ImageTGA::readFromFile(FILE* f)
{	
   byte *buffer = 0;
	unsigned int fSize;
   
	// calcolo la dimensione del file
	fseek(f, 0,SEEK_END);
   fSize = ftell(f);
	fseek(f,0, SEEK_SET);

   if (!(buffer = new byte[fSize + 1]))
      return 0;
   
   fread(buffer, 1, fSize, f);
   
   int ret = parseBuffer(buffer);
   delete [] buffer;

   return ret;
}



ImageTGA::~ImageTGA()
{
	// ToDo: Add your specialized code here and/or call the base class
}

int ImageTGA::parseBuffer(byte *buffer)
{	
	int bits;
	int size;

   if (!buffer)
      return 0;

   byte *where = buffer;
   tgaHeader_t *header = (tgaHeader_t *) where;
   
   where += (sizeof(tgaHeader_t) + header->numIden);

   if (header->colorMapType != 0)
      return 0;

   if (header->imageType != 2 && header->imageType != 3)
      return 0;

   w = header->width[0] + header->width[1] * 256; 
   h = header->height[0] + header->height[1] * 256; 
   bits = header->bpp; 
   size = w * h; 

   // make sure we are loading a supported type 
   if (bits != 32 && bits != 24 && bits != 8)
      return 0; 

	allocate(w,h);
   grabData(where, size, bits);

   return 1;
}

void ImageTGA::getRGBA(byte *buff, int size)
{
   byte *rgba = new byte[size * 4];
   byte temp;
   int i;

   if (rgba == 0)
      return;

   if (!memcpy(rgba, buff, size * 4))
   {
      delete [] rgba;
      return;
   }

   for (i = 0; i < size * 4; i += 4)
   {
      temp = rgba[i];
      rgba[i] = rgba[i + 2];
      rgba[i + 2] = temp;
   }
  
}

void ImageTGA::getRGB(byte *buff, int size)
{   
   int i;
 	byte *p = data;

   for (i = 0; i < size * 3; i += 3)
   {
		p[0] = buff[i+2];
		p[1] = buff[i+1];
		p[2] = buff[i];
		p[3] = 255;
      
		p+=4;
   }
}

void ImageTGA::getGray(byte *buff, int size)
{
   int i;
 	byte *p = data;

   for (i = 0; i < size; i ++)
   {
		p[0] = buff[i];
		p[1] = buff[i];
		p[2] = buff[i];
		p[3] = 255;
      
		p+=4;
   }
   
}


void ImageTGA::grabData(byte *buff, int size, int bits)
{
   if (bits == 32)
      getRGBA (buff, size);
   else if (bits == 24)
      getRGB (buff, size);	
   else if (bits == 8)
      getGray (buff, size);
    
}

