/*******************************************************************\
*  Bezier Patch Editor  ---  MaterialInfo                           *
*                                                                   *
*  Racchiude in maniera semplice tutte le impostazioni di un        *
*  materiale OpenGl.                                                *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_MATERIALINFO_3CC9964F01F4_INCLUDED
#define _INC_MATERIALINFO_3CC9964F01F4_INCLUDED

#ifdef	WIN32
#include <windows.h>
#endif

#include "GL/gl.h"

// Contiene le informazioni di un materiale. Questa classe è 
// di uso interno al Material Manager.
class MaterialInfo 
{
public:
	
  // Contructor / Destructor
	MaterialInfo(const char* txt);
  virtual ~MaterialInfo();

	// Imposta tutti i parametri di default.
	void setDefaultParameters();

	// Cambia il nome al materiale.
	void setName(const char* txt);

	// Esegue i comandi OpenGL per applicare tutti i parametri.
	void applyAllParameters();

	// Il nome dato al materiale.
	char* name;

  // Gli array OpenGL per i vari parametri del materiale
	GLfloat glAmbient[4];
	GLfloat glDiffuse[4];
	GLfloat glSpecular[4];
	GLfloat glEmission[4];
	GLfloat glShininess;

	unsigned char luminosity;
	unsigned char specular;
	unsigned char shininess;
	unsigned char emission;
	unsigned char trasparency;
		

private:
	

};

#endif /* _INC_MATERIALINFO_3CC9964F01F4_INCLUDED */

