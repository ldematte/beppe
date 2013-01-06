/*******************************************************************\
*  Bezier Patch Editor  ---  FogInfo                                *
*                                                                   *
*  Utilizzata internamente dal Fog Manager, questa classe e'        *
*  in grado di memorizzare i parametri di una nebbia OpenGl         *
*  e di attivarla quando richiesto.                                 *
\*******************************************************************/

#include "FogInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FogInfo::FogInfo()
{
	color[3] = 1.0f;
	setDefaultParameters();
}

FogInfo::~FogInfo()
{

}

// Setta il colore della nebbia
void FogInfo::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	color[0] = r / 255.0f;
	color[1] = g / 255.0f;
	color[2] = b / 255.0f;
}

// Setta i parametri della nebbia al default
void FogInfo::setDefaultParameters()
{
	setColor(127, 127, 127);
	mode = GL_EXP;

	density = 1.0f;
	start = 0.0f;
	end = 10.0f;
}

// Dice a OpenGL che la nebbia sara' cosi'
void FogInfo::applyAllParameters()
{
	glFogi(GL_FOG_MODE, mode);
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
}