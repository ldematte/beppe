/*******************************************************************\
*  Bezier Patch Editor  ---  TexResource                            *
*                                                                   *
*  Memorizza tutti i parametri di una texture OpenGl.               *
*  I parametri vengono attivati con una serie di comandi opengl     *
*  chiamando la funzione applyAllOptions().                         *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TEXRESOURCE_3CB5D7780186_INCLUDED
#define _INC_TEXRESOURCE_3CB5D7780186_INCLUDED

#ifdef	WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "zList.h"
#include "TexCached.h"

// --------------------- texOptions: ---------------------
#define	TEX_WRAP_S_MODE			1
#define	TEX_WRAP_T_MODE			2
#define	TEX_ENV_MODE				3
#define	TEX_QUALITY					4

#define	TEX_MIPMAPPED				5		// 0 o 1
#define	TEX_COLOR_MODE				6


// ------------------ texOptions values ------------------

// applicabili a TEX_WRAP_S_MODE e TEX_WRAP_T_MODE
#define	WRAP_CLAMP					GL_CLAMP
#define	WRAP_REPEAT					GL_REPEAT

// applicabili a TEX_ENV_MODE
#define	ENV_REPLACE					GL_REPLACE
#define	ENV_DECAL					GL_DECAL
#define	ENV_MODULATE				GL_MODULATE

// applicabili a TEX_QUALITY
#define	QUALITY_NEAREST			1		// qualità DOOM  (MIN = nearest, MAX = nearest)
#define	QUALITY_LINEAR				2		// qualità bassa (MIN = nearest, MAX = linear)
#define	QUALITY_BILINEAR			3		// default       (MIN = linear , MAX = linear)
#define	QUALITY_TRILINEAR			4		// (utilizzapile solo con mipmapping)

// applicabili a TEX_COLOR_MODE
#define	COL_GRAY						GL_ALPHA	// scala di grigi
#define	COL_RGB						GL_RGB
#define	COL_RGBA						GL_RGBA




//Questa è una classe di uso interno al TextureManager. Tutti 
//gli attributi sono pubblici per ragioni di efficienza.
class TexResource 
{
private:


public:
	// Esegue i comandi OpenGL per applicare tutti parametri
	void applyAllOptions();

	TexResource(const char* file, const char* n);
	virtual ~TexResource();

	void setName(const char *n);
	void setOption(int name, int value);
	void setDefaultOptions();

	//Esegue un comando OpenGL per rendere attiva l'opzione.
	void applyOption(int name);
	
	char* filename;		// filename della texture
	char* name;				// nome utente della tex
	bool locked;			// vero se la texture e' bloccata in cache
	int options[8];		// opzioni (parametri)

	//Il puntatore toCache è settato a NULL se la risorsa non 
	//è presente in cache.
	zList<TexCached>::zNode *toCache;

};

#endif /* _INC_TEXRESOURCE_3CB5D7780186_INCLUDED */

