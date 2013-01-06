/*******************************************************************\
*  Bezier Patch Editor  ---  MaterialInfo                           *
*                                                                   *
*  Racchiude in maniera semplice tutte le impostazioni di un        *
*  materiale OpenGl.                                                *
\*******************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "MaterialInfo.h"

//////////////////////////////////////////////////
// Contructor / Destructor

MaterialInfo::MaterialInfo(const char* txt)
{
	name = NULL;
	setName(txt);
	setDefaultParameters();
}

MaterialInfo::~MaterialInfo()
{
	free(name);
}

//////////////////////////////////////////////////
// Imposta tutti i parametri di default.

void MaterialInfo::setDefaultParameters()
{		

	// un lieve riflesso ambientale
	glAmbient[0] = 0.2f;
	glAmbient[1] = 0.2f;
	glAmbient[2] = 0.2f;
	glAmbient[3] = 1.0f;
	luminosity = 64;

	// colore grigio
	glDiffuse[0] = 0.8f;
	glDiffuse[1] = 0.8f;
	glDiffuse[2] = 0.8f;
	glDiffuse[3] = 1.0f;

	// niente specular
	glSpecular[0] = 0.0f;
	glSpecular[1] = 0.0f;
	glSpecular[2] = 0.0f;
	glSpecular[3] = 1.0f;
	specular = 0;

	glShininess = 0.0f;
	shininess = 0;

	// niente emission
	glEmission[0] = 0.0f;
	glEmission[1] = 0.0f;
	glEmission[2] = 0.0f;
	glEmission[3] = 1.0f;	
	emission = 0;
}

//////////////////////////////////////////////////
// Cambia il nome al materiale.

void MaterialInfo::setName(const char* txt)
{
	assert(txt != NULL);

	if (!name)
		free(name);
	
	name = (char*)malloc(strlen(txt)+1);
	strcpy(name,txt);
}

//////////////////////////////////////////////////
// Esegue i comandi OpenGL per applicare tutti i 
// parametri

void MaterialInfo::applyAllParameters()
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT	, glAmbient);
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE	, glDiffuse);
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR	, glSpecular);
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION	, glEmission);
	glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, glShininess);

	glColor4fv(glDiffuse);	// nel caso che l'illuminazione sia spenta
}


