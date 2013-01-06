/*******************************************************************\
*  Bezier Patch Editor  ---  ImageTGA                               *
*                                                                   *
*  Legge immagini TGA in formato non compresso.                     *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_IMAGETGA_3CB5CA9B033E_INCLUDED
#define _INC_IMAGETGA_3CB5CA9B033E_INCLUDED

#include "Image.h"

class ImageTGA 
: public Image
{
public:
	virtual ~ImageTGA();

	ImageTGA();

protected:
	int readFromFile(FILE* f);
	int parseBuffer(byte *buffer);

	void grabData(byte *buff, int size, int bits);
	void getRGBA(byte *buff, int size);
	void getRGB(byte *buff, int size);
	void getGray(byte *buff, int size);

};

#endif /* _INC_IMAGETGA_3CB5CA9B033E_INCLUDED */

