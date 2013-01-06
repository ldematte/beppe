/*******************************************************************\
*  Bezier Patch Editor  ---  FogInfo                                *
*                                                                   *
*  Utilizzata internamente dal Fog Manager, questa classe e'        *
*  in grado di memorizzare i parametri di una nebbia OpenGl         *
*  e di attivarla quando richiesto.                                 *
\*******************************************************************/

#if !defined(FOGINFO_H_INCLUDED)
#define FOGINFO_H_INCLUDED

#ifdef	WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

class FogInfo  
{
public:
    
  ////////////////////////// Construst / Destructor ////////////////////////
	FogInfo();
	virtual ~FogInfo();

	// colore della nebbia (tipicamente lo stesso dello sfondo)
	GLfloat color[4];

	// fog mode
	int mode;

  // fog parameters
	GLfloat density;
	GLfloat start;
	GLfloat end;

  // Setta il colore della nebbia
	void setColor(unsigned char r, unsigned char g, unsigned char b);
  
  // Setta i parametri della nebbia al default
	void setDefaultParameters();

  // Dice a OpenGL che la nebbia sara' cosi'
	void applyAllParameters();
};

#endif // !defined(FOGINFO_H_INCLUDED)
