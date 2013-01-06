/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport2d                             *
*                                                                   *
*  Estende Viewport e si specializza nella visualizzazione in       *
*  modalita' ortografica. Si puo' creare un Viewport2d con vista    *
*  dall'alto, laterale e frontale.                                  *
\*******************************************************************/


#if !defined(VIEWPORT2D_H_INCLUDED)
#define VIEWPORT2D_H_INCLUDED

#include "Viewport.h"

class Viewport2d : public Viewport  
{
public:
	Viewport2d(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar);
	Viewport2d(FXComposite* p, FXGLVisual *vis, Viewport *prev, const char* title, bool bottomToolbar);
	virtual ~Viewport2d();

	// Applica una vista dall'alto
	void setTopView();

	// Applica una vista laterale
	void setSideView();

	// Applica una vista frontale
	void setFrontView();
	
	virtual void resetView();

protected:

	// Direzione di vista
	enum DirectionType
	{
		TopView = 0,
		SideView,
		FrontView
	};

	DirectionType dir;

	// coordinate per la selezione a rettangolo (sweep) 
	// nel sistema di coordinate del mondo
	point StartSweepWorld;
	point EndSweepWorld;

	// coordinate per la selezione a rettangolo (sweep) 
	// nel sistema di coordinate della finestra
	int StartSweepWindowX;
	int StartSweepWindowY;
	int EndSweepWindowX;
	int EndSweepWindowY;

	void checkOperation(FXEvent *ev);
	long onMotion(FXObject* , FXSelector, void* );

	void prepareProjection();
	void prepareModelView();

	// prepara una nuova operazione di sweep
	void initSweep(int sx, int sy);

	void drawGrid();
	void drawSweep();
	void drawWorld();

	// converte le coordinate da finestra a mondo
	void ScreenToWorld(int sx, int sy, point &p);
	void FixSweepCoordinates(point &start, point &end);

private:
	void init();
};

#endif // !defined(VIEWPORT2D_H_INCLUDED)
