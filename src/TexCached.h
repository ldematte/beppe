/*******************************************************************\
*  Bezier Patch Editor  ---  TexCached                              *
*                                                                   *
*  Rappresenta una texture OpenGl caricata in cache, cioè nella     *
*  memoria dell'acceleratore 3d.                                    *
\*******************************************************************/


#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TEXCACHED_3CB5DB0B00E6_INCLUDED
#define _INC_TEXCACHED_3CB5DB0B00E6_INCLUDED

class TexCached 
{
public:
	TexCached(unsigned int id);
	virtual ~TexCached();

	unsigned int texId;

	//Il codice assegnato alla texture da OpenGl.
	unsigned int glTexName;

	//Dimensione in bytes della texture.
	unsigned int size;
};

#endif /* _INC_TEXCACHED_3CB5DB0B00E6_INCLUDED */

