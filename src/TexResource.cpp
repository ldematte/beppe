/*******************************************************************\
*  Bezier Patch Editor  ---  TexResource                            *
*                                                                   *
*  Memorizza tutti i parametri di una texture OpenGl.               *
*  I parametri vengono attivati con una serie di comandi opengl     *
*  chiamando la funzione applyAllOptions().                         *
\*******************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "TexResource.h"

TexResource::TexResource(const char* file, const char* n)
{
	assert(file);
	assert(n);

	filename = (char*)malloc(strlen(file)+1);
	strcpy(filename,file);

	name = NULL;
	setName(n);

	toCache = NULL;
	locked = false;
	setDefaultOptions();
}

TexResource::~TexResource()
{
	free(filename);
	free(name);
}

void TexResource::setName(const char *n)
{
	if (name)
		free(name);

	name = (char*)malloc(strlen(n)+1);
	strcpy(name,n);
}

void TexResource::setDefaultOptions()
{
	options[TEX_WRAP_S_MODE]	= WRAP_REPEAT;
	options[TEX_WRAP_T_MODE]	= WRAP_REPEAT;
	options[TEX_ENV_MODE]		= ENV_MODULATE;
	options[TEX_QUALITY]			= QUALITY_BILINEAR;
	options[TEX_MIPMAPPED]		= 0;	// false
	options[TEX_COLOR_MODE]    = COL_RGB;
}



void TexResource::setOption(int name, int value)
{
	options[name] = value;

	// se la texture è in cache la seleziono ed applico la modifica
	if (toCache)
	{
		TexCached *tc = toCache->Elem;
		glBindTexture (GL_TEXTURE_2D, tc->glTexName);
		applyOption(name);
	}
}


void TexResource::applyOption(int name)
{
	int value = options[name];
	GLenum pname = 0;

	if (name == TEX_MIPMAPPED ||
		 name == TEX_COLOR_MODE)
		 return;								// non possono essere modificati al volo	
	
	if (name == TEX_WRAP_S_MODE ||
		 name == TEX_WRAP_T_MODE)
	{
		pname = GL_TEXTURE_WRAP_S;
		if (name == TEX_WRAP_T_MODE)
			pname = GL_TEXTURE_WRAP_T;
		glTexParameteri(GL_TEXTURE_2D, pname, value);
		return;
	}

	if (name == TEX_ENV_MODE)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, value);
		return;
	}

	if (name == TEX_QUALITY)
	{	
		if (options[TEX_MIPMAPPED] == 0)	// texture senza mipmaps
		{
			switch(value)
			{
			case QUALITY_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case QUALITY_LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			case QUALITY_BILINEAR:
			case QUALITY_TRILINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
		else	// texture con mipmaps!
		{
		
			switch(value)
			{
			case QUALITY_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			
			case QUALITY_LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				break;
		
			case QUALITY_BILINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);				
				break;
		
			case QUALITY_TRILINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
			}
		}
		return;
	}


	// name deve essere TEX_NUM_MIPMAPS
}



void TexResource::applyAllOptions()
{	
	applyOption(TEX_WRAP_S_MODE);
	applyOption(TEX_WRAP_T_MODE);
	applyOption(TEX_ENV_MODE);
	applyOption(TEX_QUALITY);
	//applyOption(TEX_MIPMAPPED);
	//applyOption(TEX_COLOR_MODE);
}


