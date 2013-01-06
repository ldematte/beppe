/*******************************************************************\
*  Bezier Patch Editor  ---  Image                                  *
*                                                                   *
*  Una classe base per contenere immagini RGBA bitmap.              *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_IMAGE_3CB5C90C0320_INCLUDED
#define _INC_IMAGE_3CB5C90C0320_INCLUDED

#include <stdio.h>

typedef unsigned char byte;

class Image 
{
public:
		
	virtual ~Image();

  // Restituisce la larghezza
	int getWidth();

  // Restituisce l'altezza
	int getHeight();

  // Restituisce un puntatore ai dati dell'immagine
	unsigned char *getPixels();

	int load(const char* fileName);

	// Crea una nuova immagine 8x8 che serve per prova.
	static Image *makeDummy();


protected:

	Image();

  // Legge l'immagine da file
	virtual int readFromFile(FILE* f);

	int w;
	int h;
	unsigned char* data;

  // Alloca lo spazio necessario per un'immagine width*height 
	void allocate(int width, int height);

  // Setta il pixel alla posizione x, y
	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
	

};

#endif /* _INC_IMAGE_3CB5C90C0320_INCLUDED */

