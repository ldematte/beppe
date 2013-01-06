/*******************************************************************\
*  Bezier Patch Editor  ---  FogMgr                                 *
*                                                                   *
*  Il fog manager viene utilizzato per gestire in maniera semplice  *
*  le nebbie di Opengl.                                             *
\*******************************************************************/

#if !defined(FOGMGR_H_INCLUDED)
#define FOGMGR_H_INCLUDED

#include "FogInfo.h"
#include "Logger.h"

#define	MAX_FOGS		16

#define	FOG_LINEAR		GL_LINEAR
#define	FOG_EXP			GL_EXP
#define	FOG_EXP2			GL_EXP2

class FogMgr  
{
public:	
    
  // Il nostro Fog Manager e' un singleton  
	static FogMgr* getInstance();
	virtual ~FogMgr();

  // Crea una nnuova nebbia...
	unsigned int addFog();
  
  // Usa la nebbia identificata dall'id 'fid'
	void useFog(unsigned int fid);

  // Setta il colore della nebbia
	void setColor(unsigned int fid, unsigned char r, unsigned char g, unsigned char b);
  
  // Setta la modalita' di aumento di intensita' della nebbia in 
  // funzione della distanza
	void setMode(unsigned int fid, int mode);
  
  // Setta la densita' della nebbia
	void setDensity(unsigned int fid, float d);
  
  // Setta il punto di inzio della nebbia
	void setStart(unsigned int fid, float start);
  
  // Setta il punto di fine della nebbia
	void setEnd(unsigned int fid, float end);

private:
	FogMgr();

	static FogMgr *instance;
	Logger *log;

  // Tutte le nostre nebbie
	FogInfo* fog[MAX_FOGS];
	unsigned int numFogs;
	unsigned int currentFog;
};

#endif // !defined(FOGMGR_H_INCLUDED)
