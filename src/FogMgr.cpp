/*******************************************************************\
*  Bezier Patch Editor  ---  FogMgr                                 *
*                                                                   *
*  Il fog manager viene utilizzato per gestire in maniera semplice  *
*  le nebbie di Opengl.                                             *
\*******************************************************************/

#include <assert.h>

#include "FogMgr.h"
FogMgr* FogMgr::instance = NULL;

// Macro per controllare nella compilazioni di DEBUG che l'id
// della nebbia sia nel range corretto
#define FOG_ID_OK(n)	assert((n>0 && n <= numFogs) && "fogId non valido!");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FogMgr::FogMgr()
{
	log = Logger::getInstance();
	numFogs = 0;
	currentFog = 0;
	log->add("FogMgr :: inizializzato.");
}

FogMgr::~FogMgr()
{

}

//////////////////////////////////////////////////////////////////////
// Il nostro bel manager e' un singleton

FogMgr* FogMgr::getInstance()
{
	if (!instance)
		instance = new FogMgr();

	return instance;
}

//////////////////////////////////////////////////////////////////////
// Crea una nnuova nebbia...

unsigned int FogMgr::addFog()
{
	log->add("FogMgr :: creata una nuova nebbia..."); // ma va?
	++numFogs;
	fog[numFogs] = new FogInfo();
	return numFogs;
}

//////////////////////////////////////////////////////////////////////
// Usa la nebbia identificata dall'id 'fid'

void FogMgr::useFog(unsigned int fid)
{
	FOG_ID_OK(fid);
	if (currentFog == fid)
		return;
	currentFog = fid;
	fog[fid]->applyAllParameters();
	glHint(GL_FOG_HINT, GL_NICEST);
}

//////////////////////////////////////////////////////////////////////
// Setta il colore della nebbia

void FogMgr::setColor(unsigned int fid, unsigned char r, unsigned char g, unsigned char b)
{
	FOG_ID_OK(fid);
	fog[fid]->setColor(r,g,b);
	currentFog = 0;
}

//////////////////////////////////////////////////////////////////////
// Setta la modalita' di aumento di intensita' della nebbia in 
// funzione della distanza

void FogMgr::setMode(unsigned int fid, int mode)
{
	FOG_ID_OK(fid);
	fog[fid]->mode = mode;
	currentFog = 0;
}

//////////////////////////////////////////////////////////////////////
// Setta la densita' della nebbia

void FogMgr::setDensity(unsigned int fid, float d)
{
	FOG_ID_OK(fid);
	if (d<=0.0f)
		d = 0.01f;
	fog[fid]->density = d;
	currentFog = 0;
}

//////////////////////////////////////////////////////////////////////
// Setta il punto di inzio della nebbia

void FogMgr::setStart(unsigned int fid, float start)
{
	FOG_ID_OK(fid);
	if (start<0.0f)
		start = 0.0f;

	fog[fid]->start = start;
	currentFog = 0;
}

//////////////////////////////////////////////////////////////////////
// Setta il punto di fine della nebbia

void FogMgr::setEnd(unsigned int fid, float end)
{
	FOG_ID_OK(fid);
	if (end<0.0f)
		end = 0.0f;
	
	fog[fid]->end = end;
	currentFog = 0;
}


