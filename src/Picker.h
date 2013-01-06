/*******************************************************************\
*  Bezier Patch Editor  ---  Picker                                 *
*                                                                   *
*  Questa classe racchiude le operazioni Opengl necessarie per      *
*  effettuare il picking. Comunica direttamente con il patch        *
*  manager per selezionare e deselezionare i punti.                 *
\*******************************************************************/

#if !defined(PICKER_H__INCLUDED_)
#define PICKER_H__INCLUDED_

#include "common.h"
#include "PatchMgr.h"

/////////////////////////////////////////////////////////////////////////

class Picker  
{
public:

	// Dimensione del buffer per il picking
	enum
	{
		BUFFERSIZE = 8196
	};

	// Informazioni per ogni hit
	struct HitRecord
	{
		GLuint nNames;
		GLuint ZMin;
		GLuint ZMax;
		GLuint nPatch;
		GLuint nPoint;
	};

	// Dimensione per il click di picking (dimensione del dito)
	const int FingerSize;


  /////////////////////// Costuctor / Destructor ////////////////////////
	Picker();
	virtual ~Picker();

	// I punti di controllo saranno disegnati come punti
	void Drawing2d();

	// I punti di controllo saranno disegnati come sfere
	void Drawing3d();

  ///////////////////////////////////////////////////////////////////////

	// Ritorna vero se esiste un punto di controllo nella posizione indicata
	bool Verify(int x, int y);

	// Ritorna vero se esiste un punto di controllo selezionato nella posizione indicata
	bool VerifySelected(int x, int y);

	// Esegue un picking 'a click'
	bool Click(int x, int y, bool ctrl);

	// Esegue un picking con un rettangolo di selezione
	bool Sweep(int x1, int y1, int x2, int y2, bool ctrl);

	// Informa il picker di un cambio di dimensione della finestra
	void Reshape() { glGetIntegerv(GL_VIEWPORT, Viewport); }

private:

	GLint Viewport[4];
	GLuint SelectBuffer[BUFFERSIZE];
	float ProjMatrix[16];
	float ModelMatrix[16];

	// Analizza il buffer degli hit effettuando una selezione singola
	void SingleSelect(int nHits);

	// Analizza il buffer degli hit effettuando una selezione multipla
	void MultiSelect (int nHits, bool togglingMode);

	// Esegue il picking
	GLint DoPick(int x, int y, int w, int h);

  ///////////////////////////////////////////////////////////////////////

  Logger* log;
  PatchMgr* PM;

  bool DrawSpheres;
};

#endif // !defined(PICKER_H__INCLUDED_)
