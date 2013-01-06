/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport                               *
*                                                                   *
*  Fornisce funzionalita' generiche di un Viewport.                 *
*  Crea una finestra opengl, gestisce l'input del mouse e crea      *
*  una toolbar con alcuni pulsanti di controllo per regolare la     *
*  modalita' di visualizzazione. Inoltre visualizza una griglia     *
*  per facilitare l'editing.                                        *
\*******************************************************************/

#if !defined(VIEWPORT_H_INCLUDED)
#define VIEWPORT_H_INCLUDED

#include <fx.h>
#include <fx3d.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "PatchMgr.h"
#include "Picker.h"

enum ToolType
{
	TOOL_SELECTOR = 0,
	TOOL_CREATOR,
	TOOL_MIRROR,
	TOOL_ROTATE
};

class Viewport : public FXGLCanvas
{
	FXDECLARE(Viewport)
public:
	enum 
	{
		ID_CONTROL_POINTS = FXMainWindow::ID_LAST,
		ID_CONTROL_POLYGON,
    ID_CULL,
    ID_PATCH,
    ID_SNAP,
    ID_GRID_PLUS,
    ID_GRID_MINUS,
    ID_SHOW_GRID,
    ID_SOLID,
	 ID_TEXTURE,
    ID_CAMERA_TRACKBALL

	};

	// Tipo di operazione effettuata sul viewport
	enum OpType
	{
		OP_IDLE			= 0,	// niente
		OP_PANNING,				// spostamento
		OP_ROTATING,			// rotazione
		OP_SCALING,				// zoom
		OP_PICK_MOVE,			// trascina
		OP_SWEEPING,			// selezione a rettangolo
		OP_DRAWLINE				// utilizzato nel mirror e nel rotate
	};

	// Parametri di visualizzazione del viewport
	struct RenderType
	{
		bool Solid;			// se vero LIGHTING e SOLID, altrimenti WIREFRAME
		bool Cullface;
		bool DepthTest;
		float ClearColor[4];

		bool ShowCtrlPoints;
		bool ShowCtrlPolygon;
		bool ShowPatch;

		bool SnapToGrid;
		bool ShowGrid;
		bool UseCamera;
		bool Texture;
	};

	// parent, GLVisual, titolo, la toolbar in fondo?
	Viewport(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar);

	// con questo costruttore possiamo specificare un altro viewport 
	// che CONDIVIDERA' le stesse displaylist!!!
	Viewport(FXComposite* p, FXGLVisual *vis, Viewport *prev,  const char* title, bool bottomToolbar);
	virtual ~Viewport();

	// Ridisegna il viewport
	void redraw();

	// Cambia il tool corrente
	void setTool(ToolType t);

	// Ripristina la vista iniziale
	virtual void resetView() {}
	
	//-------- message handlers ---------------
	long onPaint(FXObject*,FXSelector,void*);
	long onConfigure(FXObject*,FXSelector,void*);
	long onLeftBtnPress(FXObject* , FXSelector, void* );
	long onLeftBtnRelease(FXObject* , FXSelector, void* );
	long onRightBtnPress(FXObject* , FXSelector, void* );
	long onRightBtnRelease(FXObject* , FXSelector, void* );
	virtual long onMotion(FXObject* , FXSelector, void* ) {return 0;}
	
	long onCmdControlPoints(FXObject* , FXSelector, void* );
	long onCmdControlPolygon(FXObject* , FXSelector, void* );
	long onCmdPatch(FXObject* , FXSelector, void* );
  long onCmdCullFace(FXObject* , FXSelector, void* );
  long onCmdSnap(FXObject* , FXSelector, void* );
  long onCmdGridPlus(FXObject* , FXSelector, void* );
  long onCmdGridMinus(FXObject* , FXSelector, void* );
  long onCmdShowHideGrid(FXObject* , FXSelector, void* );
  long onCmdSolid(FXObject* , FXSelector, void* );
  long onCmdTexture(FXObject* , FXSelector, void* );
  long onCmdCameraTrackball(FXObject* , FXSelector, void* );
  
protected:

	Viewport() {}

	// prepara la matrice di proiezione
	virtual void prepareProjection();

	// prepara la matrice di trasformazione
	virtual void prepareModelView();

	void glInit();	
	void modifyZoom(float factor);

	// disegna la scena
	virtual void drawWorld() {}	

	// verifica l'operazione da effettuare dopo un evento (es. pressione di un tasto del mouse)
	virtual void checkOperation(FXEvent *ev) {Operation = OP_IDLE;}

	// aggiurna lo stato dei bottoni
	virtual void updateButtons();

	// Applica il cursore corretto
	virtual void	setCorrectCursor();

	PatchMgr	*PM;
	Picker	Pick;

	int w,h;	// widht & height del viewport

	// l'operazione che stiamo effettuando
	OpType Operation;

	// il tool attivo (seleziona/sposta o crea);
	ToolType Tool;

	// mouse flags
	bool LeftBtn;
	bool RightBtn;
	
	// un flag che mi dice se devo calcolare la matrice di proiezione
	bool reproject;	

	RenderType	RenderMode;
		
	float zoom;
	float xpos, ypos;
  float gridSize;

	// pulsanti nella toolbar
	FXHorizontalFrame *sideToolbar;

	FXToggleButton *butCPoints;
	FXToggleButton *butCPolygon;
	FXToggleButton *butPatch;
	FXToggleButton *butSnapToGrid;
  FXToggleButton *butCullFace;
  FXToggleButton *butGridPlus;
  FXToggleButton *butGridMinus;
	FXToggleButton *butShowGrid;
  FXToggleButton *butSolid;
  FXToggleButton *butTexture;
  FXToggleButton *butCameraTrackball;  

private:

	void init(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar);	
};

#endif // !defined(VIEWPORT_H_INCLUDED)
