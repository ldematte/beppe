/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport3d                             *
*                                                                   *
*  Estende Viewport e si specializza nella visualizzazione in       *
*  prospettiva. E' possibile "muoversi" nel mondo utilizzando a     *
*  scelta una trackball o una telecamera. Viene disegnata una       *
*  griglia sul piano XZ e nell'angolo superiore destro del viewport *
*  visualizza gli assi cartesiani per far capire la rotazione       *
*  attuale della trackball/telecamera.                              *
\*******************************************************************/

#if !defined(VIEWPORT3D_H_INCLUDED)
#define VIEWPORT3D_H_INCLUDED

#include "Viewport.h"
#include "Trackball.h"
#include "FogMgr.h"
#include "Camera.h"


class Viewport3d : public Viewport  
{
public:
	Viewport3d(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar);
	Viewport3d(FXComposite* p, FXGLVisual *vis, Viewport *prev, const char* title, bool bottomToolbar);
	virtual ~Viewport3d();

	void resetView();

protected:
	long onMotion(FXObject* , FXSelector, void* );

	void checkOperation(FXEvent *ev);
	
	// modifica la matrice di rotazione utilizzando il movimento del mouse sulla trackball
	void RotateWithTrackball(int lastx, int lasty, int x, int y);

	void prepareProjection();
	void prepareModelView();

	void drawGrid();
	void drawWorld();

	// disegna gli ultimi particolari
	void finalizeViewport();

	int blackFog;
	float curQuat[4];
	float lastQuat[4];
	
	float rotMatrix[4][4];
	
  Camera camera;

	FogMgr *fogMgr;

private:
	void init();

	GLubyte *LetteraX;
	GLubyte *LetteraY;
	GLubyte *LetteraZ;
	
};

#endif // !defined(VIEWPORT3D_H_INCLUDED)
