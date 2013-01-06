/*******************************************************************\
*  Bezier Patch Editor  ---  Image                                  *
*                                                                   *
*  Una classe base per contenere immagini RGBA bitmap.              *
\*******************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Image.h"

/////////////////////////////////////////////////////////////////////
// Constructor / Destructor

Image::Image()
{
	w = 0;
	h = 0;
	data = NULL;
}



Image::~Image()
{
	if (data)
		free(data);
}

/////////////////////////////////////////////////////////////////////
// Restituisce la larghezza

int Image::getWidth()
{
	return w;
}

/////////////////////////////////////////////////////////////////////
// Restituisce l'altezza

int Image::getHeight()
{	
	return h;
}

/////////////////////////////////////////////////////////////////////
// Restituisce un puntatore ai dati dell'immagine

unsigned char *Image::getPixels()
{
	return data;
}

/////////////////////////////////////////////////////////////////////
// Legge l'immagine da file: deve essere implementato nelle classi
// derivate

int Image::readFromFile(FILE* f)
{
	return 0;
}

int Image::load(const char* fileName)
{
	FILE *in = fopen(fileName, "rb");
	if (!in)
		return 0;

	if (!readFromFile(in))
		return 0;

	fclose(in);
	return 1;
}

/////////////////////////////////////////////////////////////////////
// Crea una nuova immagine 8x8 che serve per prova.

Image *Image::makeDummy()
{
	Image *i = new Image();
	i->allocate(8,8);

	int x,y;
	for (y = 0 ;y<i->h; y++)
		for (x = 0 ;x<i->w; x++)
		{
			unsigned char v = (((x+y)&1)<<7) + 127;
			//i->putPixel(x,y,v,v,v,((x*255)>>3));
			i->putPixel(x,y,v,v,v,255);
		}
	
	return i;
}

/////////////////////////////////////////////////////////////////////
// Alloca lo spazio necessario per un'immagine width*height 

void Image::allocate(int width, int height)
{
	if (data)
		free(data);

	data = NULL;

	w = width;
	h = height;

	if (w <= 0 || h <= 0)
		return;

	data = (unsigned char*) malloc (w*h*4);
}

/////////////////////////////////////////////////////////////////////
// Setta il pixel alla posizione x, y

inline void Image::putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	assert(x >= 0 && x < w);
	assert(y >= 0 && y < h);

	unsigned char *pix = &(data[y*(w<<2)+(x<<2)]);
	pix[0] = r;
	pix[1] = g;
	pix[2] = b;
	pix[3] = a;

}


